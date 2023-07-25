fetch('/data.json')
    .then(response => response.json())
    .then(data => {
        // Create the scene
        const scene = new THREE.Scene();
        
        // Assuming that data.var_plot is a 2D array
        let varMin = Infinity;
        let varMax = -Infinity;
        
        // Iterating over data.var_plot based on its length
        for(let i = 0; i < data.var_plot.length; i++) {
            for(let j = 0; j < data.var_plot[i].length; j++) {
                // Updating varMin if current element is smaller
                if(data.var_plot[i][j] < varMin) {
                    varMin = data.var_plot[i][j];
                }
        
                // Updating varMax if current element is larger
                if(data.var_plot[i][j] > varMax) {
                    varMax = data.var_plot[i][j];
                }
            }
        }
        
        console.log('Min:', varMin);
        console.log('Max:', varMax);
        
        let time_factor = 3;


        let frame = 0;

        // Create the camera
        const camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);
        camera.position.z = 900;

        // Create the renderer
        const renderer = new THREE.WebGLRenderer();
        renderer.setSize(window.innerWidth, window.innerHeight);
        document.body.appendChild(renderer.domElement);

        // Add OrbitControls 
        //const controls = new THREE.OrbitControls(camera, renderer.domElement);
        //controls.autoRotate = true;

        // Create the geometry
        const geometry = new THREE.BufferGeometry();

        const vertices = [];
        const colors = [];
        const sizes = [];

        // Define colormap
        let colorMap = [];
        for(let i=0; i<256; i++) {
            let red, green, blue;
        
            // Define red channel
            if(i<95) red = 0;
            else if(i<160) red = (i-95)/65;
            else if(i<224) red = 1;
            else red = 1 - 0.5*(i-224)/(257-224);
        
            // Define blue channel
            if(i<32) blue = 0.5 + i/(257-225);
            else if(i<224) blue = 1;
            else if(i<160) blue = 1 - (i-224)/(160-95);
            else blue = 0;
        
            // Define green channel
            if(i<31) green = 0;
            else if(i<65) green = (i-31)/34;
            else if(i<224) green = 1;
            else if(i<160) green = 1 - (i-224)/(160-95);
            else green = 0;
        
            colorMap.push(new THREE.Color(red, green, blue));
        }

        function getColor(varNormalized) {
            // Convert var to index in the colormap
            let index = Math.round(varNormalized * 255);
            
            // Clamp index to valid range
            index = Math.max(0, Math.min(255, index));
            
            // Return color
            return colorMap[index];
        }


        // Create an array to store the spheres
        let spheres = [];

        // Process the positions and vars
        for (let i = 0; i < data.positions.length; i++) {
            const position = data.positions[i];
            const diameter = Math.max(data.positions[i][3],3);
            const section_index = data.positions[i][4];
            
            // Add vertex (point position)
            vertices.push(position[0], position[1], position[2]);

            // Color the point based on the var_plot
            const var_plot = data.var_plot[section_index][0];
            const color = getColor((var_plot - varMin) / (varMax - varMin));
            colors.push(color.r, color.g, color.b);

            // Create the geometry and material for the sphere
            let sphereGeometry = new THREE.SphereGeometry(diameter/2, 32, 32);
            let sphereMaterial = new THREE.MeshBasicMaterial({color: color});

            // Create the sphere, set its position, and add it to the scene
            let sphere = new THREE.Mesh(sphereGeometry, sphereMaterial);
            sphere.position.set(position[0], position[1], position[2]);
            scene.add(sphere);

            // Store the sphere
            spheres.push(sphere);
        }

        geometry.setAttribute('position', new THREE.Float32BufferAttribute(vertices, 3));
        geometry.setAttribute('size', new THREE.Float32BufferAttribute(sizes, 1));

        // Create the material
        const material = new THREE.PointsMaterial({ vertexColors: true, sizeAttenuation: true });

        // Create the points object and add it to the scene
        const points = new THREE.Points(geometry, material);
        scene.add(points);
                        
        // Render the scene
        function animate() {
            //controls.update();
            requestAnimationFrame(animate);
            
            // Update each sphere
            for (let i = 0; i < spheres.length; i++) {
                const sphere = spheres[i];
                const section_index = data.positions[i][4];
        
                // Color the point based on the var_plot
                const var_plot = data.var_plot[section_index][frame];
                const color = getColor((var_plot - varMin) / (varMax - varMin));
                sphere.material.color = color;
            }
            
            // Proceed to next frame or go back to first frame if we've reached the end
            frame = (frame + time_factor) % data.var_plot[0].length;
            console.log(frame);
            renderer.render(scene, camera);
        }        

        animate();
    });

