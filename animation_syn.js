import * as THREE from "./three.module.js";
import { OrbitControls } from "./OrbitControls.js";

fetch('/data.json')
    .then(response => response.json())
    .then(data => {
        fetch('/data_syn.json')
            .then(response2 => response2.json())
            .then(data_syn => {
                console.log(data_syn.synapse_pos)

                const scene = new THREE.Scene();
                const varMin = -80;
                const varMax = 40;
                let time_factor = 3;
                let frame = 0;

                const camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 5000);
                camera.position.z = 900;
                camera.maxDistance = 10000;

                const renderer = new THREE.WebGLRenderer();
                renderer.setSize(window.innerWidth, window.innerHeight);
                document.body.appendChild(renderer.domElement);

                window.addEventListener('resize', function () {
                    // Update camera aspect ratio and renderer size
                    camera.aspect = window.innerWidth / window.innerHeight;
                    camera.updateProjectionMatrix();

                    renderer.setSize(window.innerWidth, window.innerHeight);
                }, false);

                const controls = new OrbitControls(camera, renderer.domElement);
                // controls.autoRotate = true;

                const geometry = new THREE.BufferGeometry();
                const vertices = [];
                const colors = [];
                const sizes = [];

                let colorMap = [];
                for (let i = 0; i < 256; i++) {
                    let red, green, blue;
                    if (i < 95) red = 0;
                    else if (i < 160) red = (i - 95) / 65;
                    else if (i < 224) red = 1;
                    else red = 1 - 0.5 * (i - 224) / (257 - 224);
                    if (i < 32) blue = 0.5 + i / (257 - 225);
                    else if (i < 224) blue = 1;
                    else if (i < 160) blue = 1 - (i - 224) / (160 - 95);
                    else blue = 0;
                    if (i < 31) green = 0;
                    else if (i < 65) green = (i - 31) / 34;
                    else if (i < 224) green = 1;
                    else if (i < 160) green = 1 - (i - 224) / (160 - 95);
                    else green = 0;
                    colorMap.push(new THREE.Color(red, green, blue));
                }

                function getColor(varNormalized) {
                    let index = Math.round(varNormalized * 255);
                    index = Math.max(0, Math.min(255, index));
                    return colorMap[index];
                }

                let cylinders = [];

                for (let i = 0; i < data.positions.length - 1; i++) {
                    const position = new THREE.Vector3(data.positions[i][0], data.positions[i][1], data.positions[i][2]);
                    const nextPosition = new THREE.Vector3(data.positions[i + 1][0], data.positions[i + 1][1], data.positions[i + 1][2]);
                    const diameter = Math.max(data.positions[i][3], 3);
                    const section_index = data.positions[i][4];
                    const next_section_index = data.positions[i + 1][4];

                    vertices.push(position.x, position.y, position.z);
                    const var_plot = data.var_plot[section_index][0];
                    const color = getColor((var_plot - varMin) / (varMax - varMin));
                    colors.push(color.r, color.g, color.b);

                    if (section_index === next_section_index) {
                        let cylinderGeometry = new THREE.CylinderGeometry(diameter / 2, diameter / 2, position.distanceTo(nextPosition), 32);
                        let cylinderMaterial = new THREE.MeshBasicMaterial({ color: color });

                        let cylinder = new THREE.Mesh(cylinderGeometry, cylinderMaterial);
                        cylinder.position.lerpVectors(position, nextPosition, 0.5);
                        cylinder.lookAt(nextPosition);
                        cylinder.quaternion.setFromUnitVectors(new THREE.Vector3(0, 1, 0), new THREE.Vector3().subVectors(nextPosition, position).normalize());

                        scene.add(cylinder);

                        cylinders.push(cylinder);
                        }
                    }

                    geometry.setAttribute('position', new THREE.Float32BufferAttribute(vertices, 3));
                    geometry.setAttribute('size', new THREE.Float32BufferAttribute(sizes, 1));

                    const material = new THREE.PointsMaterial({ vertexColors: true, sizeAttenuation: true });

                    const points = new THREE.Points(geometry, material);
                    scene.add(points);

                    // Array to store synapses
                    let synapses = [];

                    // Array to store synapse intensity
                    let synapseIntensity = new Array(data_syn.synapse_pos.length).fill(0);

                    for (let i = 0; i < data_syn.synapse_pos.length; i++) {
                        const position = new THREE.Vector3(data_syn.synapse_pos[i][0], data_syn.synapse_pos[i][1], data_syn.synapse_pos[i][2]);

                        let synapse = new THREE.Mesh(new THREE.SphereGeometry(3, 32, 32), new THREE.MeshBasicMaterial({ color: 0xFFFFFF }));  // White color
                        synapse.position.set(position.x, position.y, position.z);

                        scene.add(synapse);

                        synapses.push(synapse);
                    }

                    function animate() {
                        controls.update();
                        requestAnimationFrame(animate);

                        for (let i = 0; i < cylinders.length; i++) {
                            const cylinder = cylinders[i];
                            const section_index = data.positions[i][4];
                            const var_plot = data.var_plot[section_index][frame];
                            const color = getColor((var_plot - varMin) / (varMax - varMin));
                            cylinder.material.color = color;
                        }

                        // Update each synapse
                        for (let i = 0; i < synapses.length; i++) {
                            const synapse = synapses[i];
                            const spikeTimes = data_syn.spike_times[i];

                            if (spikeTimes.some(time => time >= frame && time < frame + time_factor - 1)) {
                                synapseIntensity[i] = 1;  // set the intensity to 1 if there is a spike
                            } else {
                                synapseIntensity[i] *= Math.exp(-1 / 10);  // apply exponential decay otherwise
                            }

                            // Update synapse color based on its intensity
                            let color = new THREE.Color(1, 1 - synapseIntensity[i], 1 - synapseIntensity[i]);
                            synapse.material.color = color;
                        }

                        frame = (frame + time_factor) % data.var_plot[0].length;
                        console.log(frame);
                        renderer.render(scene, camera);
                    }

                    animate();

            });
    });
