import * as THREE from "./three.module.js"; 
import { OrbitControls } from "./OrbitControls.js";

fetch('/data.json')
    .then(response => response.json())
    .then(data => {
        // Create the scene
        const scene = new THREE.Scene();

        // Initialize voltage range
        const voltageMin = -80; // minimum expected voltage
        const voltageMax = 40;  // maximum expected voltage
        let time_factor = 4;

        let frame = 0;

        // Create the camera
        const camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 5000);
        camera.position.z = 900;
        camera.maxDistance = 10000;

        // Create the renderer
        const renderer = new THREE.WebGLRenderer();
        renderer.setSize(window.innerWidth, window.innerHeight);
        document.body.appendChild(renderer.domElement);

        // Add OrbitControls 
        const controls = new OrbitControls(camera, renderer.domElement);
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

        function getColor(voltageNormalized) {
            // Convert voltage to index in the colormap
            let index = Math.round(voltageNormalized * 255);
            
            // Clamp index to valid range
            index = Math.max(0, Math.min(255, index));
            
            // Return color
            return colorMap[index];
        }


        // Create an array to store the cylinders
        let cylinders = [];

        // Process the positions and voltages
        for (let i = 0; i < data.positions.length - 1; i++) {
            const position = new THREE.Vector3(data.positions[i][0], data.positions[i][1], data.positions[i][2]);
            const nextPosition = new THREE.Vector3(data.positions[i+1][0], data.positions[i+1][1], data.positions[i+1][2]);
            const diameter = Math.max(data.positions[i][3],3);
            const section_index = data.positions[i][4];
            const next_section_index = data.positions[i+1][4];
            
            // Add vertex (point position)
            vertices.push(position.x, position.y, position.z);

            // Color the point based on the voltage
            const voltage = data.voltage[section_index][0];
            const color = getColor((voltage - voltageMin) / (voltageMax - voltageMin));
            colors.push(color.r, color.g, color.b);

            // Only create the cylinder if the section index is the same
            if (section_index === next_section_index) {
                // Create the geometry and material for the cylinder
                let cylinderGeometry = new THREE.CylinderGeometry(diameter/2, diameter/2, position.distanceTo(nextPosition), 32);
                let cylinderMaterial = new THREE.MeshBasicMaterial({color: color});

                // Create the cylinder, set its position and rotation
                let cylinder = new THREE.Mesh(cylinderGeometry, cylinderMaterial);
                cylinder.position.lerpVectors(position, nextPosition, 0.5);
                cylinder.lookAt(nextPosition);
                cylinder.quaternion.setFromUnitVectors(new THREE.Vector3(0, 1, 0), new THREE.Vector3().subVectors(nextPosition, position).normalize());

                // Add it to the scene
                scene.add(cylinder);

                // Store the cylinder
                cylinders.push(cylinder);
            }
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
            controls.update();
            requestAnimationFrame(animate);
                    
            // Update each cylinder
            for (let i = 0; i < cylinders.length; i++) {
                const cylinder = cylinders[i];
                const section_index = data.positions[i][4];
                
                // Color the point based on the voltage
                const voltage = data.voltage[section_index][frame];
                const color = getColor((voltage - voltageMin) / (voltageMax - voltageMin));
                cylinder.material.color = color;
            }

            // Proceed to next frame or go back to first frame if we've reached the end
            frame = (frame + time_factor) % data.voltage[0].length;
            console.log(frame);
            renderer.render(scene, camera);
        }        

        animate();

    });

