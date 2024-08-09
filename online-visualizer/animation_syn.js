import * as THREE from "./three.module.js";
import { OrbitControls } from "./OrbitControls.js";

fetch("/data.json")
  .then((response) => response.json())
  .then((data) => {
    fetch("/data_syn.json")
      .then((response2) => response2.json())
      .then((data_syn) => {
        //console.log(data_syn.synapse_pos);

        const scene = new THREE.Scene();
        let varMin = Infinity;
        let varMax = -Infinity;

        // Loop through each array in data.var_plot to find min and max excluding axon
        for (let i = 0; i < data.var_plot.length - 6; i++) {
          for (let j = 0; j < data.var_plot[i].length; j++) {
            if (data.var_plot[i][j] < varMin) {
              varMin = data.var_plot[i][j];
            }
            if (data.var_plot[i][j] > varMax) {
              varMax = data.var_plot[i][j];
            }
          }
        }
        varMax = 30;
        varMin = -80;

        function onMouseClick(event) {
          mouse.x = (event.clientX / window.innerWidth) * 2 - 1;
          mouse.y = -(event.clientY / window.innerHeight) * 2 + 1;
          raycaster.setFromCamera(mouse, camera);

          const intersects = raycaster.intersectObjects(cylinders);

          if (intersects.length > 0) {
            const selectedCylinder = intersects[0].object;
            const clickedIndex = selectedCylinder.sectionIndex;
            console.log(clickedIndex);
            showVoltagePlot(clickedIndex);
          }
        }

        function adaptiveRounding(varMin, varMax) {
          let difference = Math.abs(varMax - varMin);

          // Calculate the number of significant figures based on the difference
          let numSigFigs = Math.ceil(-Math.log10(difference)) + 1;
          numSigFigs = Math.max(numSigFigs, 0); // Make sure it's non-negative

          // Round the numbers to that many significant figures
          if (varMin !== 0) {
            let factor = Math.pow(10, numSigFigs);
            varMin = Math.round(varMin * factor) / factor;
          }
          if (varMax !== 0) {
            let factor = Math.pow(10, numSigFigs);
            varMax = Math.round(varMax * factor) / factor;
          }

          return { varMin, varMax };
        }

        const vars_out = adaptiveRounding(varMin, varMax);
        varMin = vars_out.varMin;
        varMax = vars_out.varMax;
        //console.log(varMin, varMax);

        let time_factor = 5;
        let frame = 0;

        const camera = new THREE.PerspectiveCamera(
          75,
          window.innerWidth / window.innerHeight,
          0.1,
          5000
        );
        camera.position.z = 900;
        camera.maxDistance = 10000;

        const renderer = new THREE.WebGLRenderer();
        renderer.setSize(window.innerWidth, window.innerHeight);
        renderer.domElement.addEventListener("click", onMouseClick, false);
        document.body.appendChild(renderer.domElement);

        window.addEventListener(
          "resize",
          function () {
            // Update camera aspect ratio and renderer size
            camera.aspect = window.innerWidth / window.innerHeight;
            camera.updateProjectionMatrix();

            renderer.setSize(window.innerWidth, window.innerHeight);
          },
          false
        );

        const controls = new OrbitControls(camera, renderer.domElement);
        // controls.autoRotate = true;

        const raycaster = new THREE.Raycaster();
        const mouse = new THREE.Vector2();

        controls.update();

        const geometry = new THREE.BufferGeometry();
        const vertices = [];
        const colors = [];
        const sizes = [];

        // let colorMap = [];
        // for (let i = 0; i < 256; i++) {
        //   let red, green, blue;
        //   if (i < 95) red = 0;
        //   else if (i < 160) red = (i - 95) / 65;
        //   else if (i < 224) red = 1;
        //   else red = 1 - (0.5 * (i - 224)) / (257 - 224);
        //   if (i < 32) blue = 0.5 + i / (257 - 225);
        //   else if (i < 95) blue = 1;
        //   else if (i < 160) blue = 1 - (i - 95) / (160 - 95);
        //   else blue = 0;
        //   if (i < 31) green = 0;
        //   else if (i < 65) green = (i - 31) / 34;
        //   else if (i < 160) green = 1;
        //   else if (i < 224) green = 1 - (i - 160) / (160 - 95);
        //   else green = 0;
        //   colorMap.push(new THREE.Color(red, green, blue));
        // }

        let colorMap = [];

        for (let i = 0; i < 256; i++) {
          let f = i / 255;

          var a = (1 - f) / 0.25; //invert and group
          var X = Math.floor(a); //this is the integer part
          var Y = Math.floor(255 * (a - X)); //fractional part from 0 to 255
          let red, green, blue;

          switch (X) {
            case 0:
              red = 255;
              green = Y;
              blue = 0;
              break;
            case 1:
              red = 255 - Y;
              green = 255;
              blue = 0;
              break;
            case 2:
              red = 0;
              green = 255;
              blue = Y;
              break;
            case 3:
              red = 0;
              green = 255 - Y;
              blue = 255;
              break;
            case 4:
              red = 0;
              green = 0;
              blue = 255;
              break;
          }

          // Push the color to the colormap
          colorMap.push(new THREE.Color(red / 255, green / 255, blue / 255)); //THREE.Color expects values between 0 and 1
        }

        function getColor(varNormalized) {
          let index = Math.round(varNormalized * 255);
          index = Math.max(0, Math.min(255, index));
          return colorMap[index];
        }

        const plottedSections = [];

        function generateRandomColor() {
          const r = Math.floor(Math.random() * 256);
          const g = Math.floor(Math.random() * 256);
          const b = Math.floor(Math.random() * 256);
          return `rgb(${r},${g},${b})`;
        }

        const activeSections = []; // An array to keep track of activated sections

        function addGlowToSection(sectionIndex, color) {
          for (let i = 0; i < data.positions.length - 1; i++) {
            if (data.positions[i][4] === sectionIndex) {
              const position = new THREE.Vector3(
                data.positions[i][0],
                data.positions[i][1],
                data.positions[i][2]
              );

              // Create a point light at the cylinder's position
              //const pointLight = new THREE.PointLight(color, 1, 50); // You can adjust intensity and decay distance as needed
              //pointLight.position.set(position.x, position.y, position.z);
              //scene.add(pointLight);
            }
          }
        }
        let chart;
        let canvas;
        const allDatasets = [];

        function showVoltagePlot(sectionIndex) {
          // Check if a dataset for the sectionIndex already exists
          const datasetLabel = `Voltage (${sectionIndex})`;
          const existingDataset = allDatasets.find(
            (dataset) => dataset.label === datasetLabel
          );
          if (existingDataset) {
            return; // If dataset already exists, exit the function early
          }

          const voltageData = data.var_plot[sectionIndex];
          const timeLabels = Array.from(
            { length: voltageData.length },
            (_, i) => Math.round(0.025 * i * 1000) / 1000
          );
          const existingSection = activeSections.find(
            (section) => section.index === sectionIndex
          );
          if (existingSection) {
            return; // If section already exists, exit the function early
          }

          const randomColor = generateRandomColor();
          activeSections.push({ index: sectionIndex, color: randomColor });
          //addGlowToSection(sectionIndex, randomColor);

          const newDataset = {
            label: datasetLabel,
            data: voltageData,
            borderColor: randomColor,
            fill: false,
            pointRadius: 0,
            borderWidth: 1,
          };

          allDatasets.push(newDataset);
          const maxTime = parseFloat(timeLabels[timeLabels.length - 1]);
          let tickStepSize;

          if (maxTime <= 10) {
            tickStepSize = 1;
          } else if (maxTime <= 100) {
            tickStepSize = 10;
          } else {
            tickStepSize = 100;
          }

          if (!chart) {
            canvas = document.createElement("canvas");
            document.body.appendChild(canvas);

            const ctx = canvas.getContext("2d");
            chart = new Chart(ctx, {
              type: "line",
              data: {
                labels: timeLabels,
                datasets: allDatasets,
              },
              options: {
                scales: {
                  x: {
                    display: true,
                    title: {
                      display: true,
                      text: "Time (ms)",
                    },
                    ticks: {
                      maxTicksLimit: 10,
                      stepSize: tickStepSize,
                      //suggestedMin: 0,
                      //suggestedMax: Math.round(maxTime),
                    },
                  },
                  y: {
                    display: true,
                    title: {
                      display: true,
                      text: "Voltage (mV)",
                    },
                  },
                },
              },
            });
          } else {
            chart.update();
          }

          if (activeSections.length > 0) {
            removeAllBtn.style.display = "block";
          }
        }

        // function removeDataset(sectionIndex) {
        //   const datasetLabel = `Voltage (${sectionIndex})`;
        //   const index = allDatasets.findIndex(
        //     (dataset) => dataset.label === datasetLabel
        //   );

        //   if (index !== -1) {
        //     allDatasets.splice(index, 1);
        //     chart.update();
        //   }
        // }

        document
          .getElementById("removeAllBtn")
          .addEventListener("click", function () {
            if (canvas) {
              document.body.removeChild(canvas); // Remove canvas from the document
              canvas = null; // Clear the reference
            }
            allDatasets.length = 0;
            chart = null;
            activeSections.length = 0;
            removeAllBtn.style.display = "none";
          });

        const removeAllBtn = document.getElementById("removeAllBtn");
        removeAllBtn.style.display = "none";

        const legendContainer = document.createElement("div");
        legendContainer.style.position = "absolute";
        legendContainer.style.left = "20px";
        legendContainer.style.top = "20px";
        legendContainer.style.color = "white";
        legendContainer.style.fontSize = "14px";
        legendContainer.style.backgroundColor = "rgba(128, 128, 128, 0.5)";
        legendContainer.style.padding = "10px";
        legendContainer.style.borderRadius = "5px";

        const title = document.createElement("div");
        title.textContent = "Neuron Visualizer";
        title.style.fontSize = "26px";
        legendContainer.appendChild(title);

        const name = document.createElement("div");
        name.textContent = "D. G. Itkis (2023)";
        name.style.marginTop = "5px";
        legendContainer.appendChild(name);

        const affiliation = document.createElement("div");
        affiliation.textContent = "Cohen Lab, Harvard";
        affiliation.style.marginBottom = "20px";
        legendContainer.appendChild(affiliation);

        const timeAndSpeedContainer = document.createElement("div");
        timeAndSpeedContainer.style.display = "flex";
        timeAndSpeedContainer.style.justifyContent = "space-between";
        timeAndSpeedContainer.style.alignItems = "center";

        const simulationTimeElement = document.createElement("div");
        simulationTimeElement.style.color = "white";
        simulationTimeElement.style.fontSize = "20px";
        //simulationTimeElement.style.marginLeft = "5px";
        simulationTimeElement.textContent = "t = 0 ms";
        timeAndSpeedContainer.appendChild(simulationTimeElement);

        const sliderContainer = document.createElement("div");
        // sliderContainer.style.color = "white";
        // sliderContainer.style.marginTop = "14px";
        // sliderContainer.textContent = "Speed: ";

        const slider = document.createElement("input");
        slider.type = "range";
        slider.min = -10;
        slider.max = 10;
        slider.value = time_factor;
        slider.step = 1;
        sliderContainer.style.marginTop = "3px";

        slider.addEventListener("input", function () {
          time_factor = parseInt(slider.value);
          //sliderContainer.textContent = "Speed: " + time_factor;
        });

        sliderContainer.appendChild(slider);
        timeAndSpeedContainer.appendChild(sliderContainer);

        legendContainer.appendChild(timeAndSpeedContainer);

        const legend = document.createElement("div");
        const excitatorySynapseQuiet = document.createElement("span");
        excitatorySynapseQuiet.style.display = "inline-block";
        excitatorySynapseQuiet.style.width = "8px";
        excitatorySynapseQuiet.style.height = "8px";
        excitatorySynapseQuiet.style.marginTop = "10px";
        excitatorySynapseQuiet.style.borderRadius = "50%";
        excitatorySynapseQuiet.style.backgroundColor = "white";
        excitatorySynapseQuiet.style.marginRight = "5px";
        legend.appendChild(excitatorySynapseQuiet);
        legend.innerHTML += "Synapse (quiet) <br>";
        const excitatorySynapseActive = document.createElement("span");
        excitatorySynapseActive.style.display = "inline-block";
        excitatorySynapseActive.style.width = "8px";
        excitatorySynapseActive.style.height = "8px";
        excitatorySynapseActive.style.borderRadius = "50%";
        excitatorySynapseActive.style.backgroundColor = "red";
        excitatorySynapseActive.style.marginRight = "5px";
        legend.appendChild(excitatorySynapseActive);
        legend.innerHTML += "Synapse (active)";
        legendContainer.appendChild(legend);

        let colorBar = document.createElement("div");
        colorBar.style.width = "256px";
        colorBar.style.height = "10px";
        colorBar.style.display = "flex";
        colorBar.style.flexDirection = "row";
        colorBar.style.marginTop = "20px";

        for (let i = 0; i < 256; i++) {
          let colorStripe = document.createElement("div");
          colorStripe.style.width = "1px";
          colorStripe.style.height = "10px";
          let color = getColor(i / 255);
          colorStripe.style.background = `rgb(${color.r * 255}, ${
            color.g * 255
          }, ${color.b * 255})`;
          colorBar.appendChild(colorStripe);
        }

        legendContainer.appendChild(colorBar);
        document.body.appendChild(legendContainer);

        // Creating labels for color bar
        let colorBarLabels = document.createElement("div");
        colorBarLabels.style.display = "flex";
        colorBarLabels.style.justifyContent = "space-between";
        colorBarLabels.style.width = "256px";
        colorBarLabels.style.fontSize = "14px";

        let colorBarLabelMin = document.createElement("div");
        colorBarLabelMin.style.color = "white";
        colorBarLabelMin.textContent = varMin + " mV";
        colorBarLabels.appendChild(colorBarLabelMin);

        let colorBarLabelMax = document.createElement("div");
        colorBarLabelMax.style.color = "white";
        colorBarLabelMax.textContent = varMax + " mV";
        colorBarLabels.appendChild(colorBarLabelMax);

        legendContainer.appendChild(colorBarLabels);

        const scaleBarLabel = document.createElement("div");
        scaleBarLabel.style.color = "white";
        scaleBarLabel.style.fontSize = "14px";
        scaleBarLabel.textContent = "100 μm Scale Bars ";
        scaleBarLabel.style.marginTop = "10px";

        let showScaleBar = true;
        const toggleScaleBar = document.createElement("input");
        toggleScaleBar.type = "checkbox";
        toggleScaleBar.checked = showScaleBar;
        toggleScaleBar.id = "toggleScaleBar";

        scaleBarLabel.appendChild(toggleScaleBar);

        legendContainer.appendChild(scaleBarLabel);
        document.body.appendChild(legendContainer);

        toggleScaleBar.addEventListener("change", function () {
          showScaleBar = toggleScaleBar.checked;
        });

        const waveformLabel = document.createElement("div");
        waveformLabel.style.color = "white";
        waveformLabel.style.fontSize = "14px";
        waveformLabel.textContent = "Stimulation waveform ";
        waveformLabel.style.marginTop = "5px";

        let showWaveform = true;
        const toggleWaveform = document.createElement("input");
        toggleWaveform.type = "checkbox";
        toggleWaveform.checked = showWaveform;
        toggleWaveform.id = "toggleWaveform";

        waveformLabel.appendChild(toggleWaveform);
        legendContainer.appendChild(waveformLabel);

        const waveformContainer = document.createElement("div");
        waveformContainer.style.position = "relative";
        waveformContainer.style.width = "262px";
        waveformContainer.style.height = "196px";
        waveformContainer.style.marginTop = "10px";

        const waveformImg = document.createElement("img");
        waveformImg.src = "waveform.png";
        waveformImg.style.display = showWaveform ? "block" : "none";
        waveformImg.style.width = "100%";
        waveformImg.style.height = "100%";

        waveformContainer.appendChild(waveformImg);

        const redLine = document.createElement("div");
        redLine.style.position = "absolute";
        redLine.style.top = "10px";
        redLine.style.left = "35px";
        redLine.style.width = "1px";
        redLine.style.height = "158px";
        redLine.style.backgroundColor = "red";
        redLine.style.display = showWaveform ? "block" : "none";

        waveformContainer.appendChild(redLine);
        legendContainer.appendChild(waveformContainer);
        document.body.appendChild(legendContainer);

        toggleWaveform.addEventListener("change", function () {
          const isVisible = toggleWaveform.checked;
          if (isVisible) {
            waveformImg.style.display = "block";
            redLine.style.display = "block";
            legendContainer.style.height = "auto";
          } else {
            waveformImg.style.display = "none";
            redLine.style.display = "none";
            legendContainer.style.height = "275px";
          }
        });

        const axesHelper = new THREE.AxesHelper(1);
        scene.add(axesHelper);

        function updateScaleBar() {
          if (showScaleBar) {
            axesHelper.position.set(150, -150, 0);
            axesHelper.scale.set(100, 100, 100);

            axesHelper.visible = true;
          } else {
            axesHelper.visible = false;
          }
        }

        let cylinders = [];
        let somaPosition = new THREE.Vector3(
          data.positions[0][0],
          data.positions[0][1],
          data.positions[0][2]
        );

        for (let i = 0; i < data.positions.length - 1; i++) {
          const position = new THREE.Vector3(
            data.positions[i][0],
            data.positions[i][1],
            data.positions[i][2]
          );
          const nextPosition = new THREE.Vector3(
            data.positions[i + 1][0],
            data.positions[i + 1][1],
            data.positions[i + 1][2]
          );
          const prevPosition = new THREE.Vector3(
            data.positions[Math.max(0, i - 1)][0],
            data.positions[Math.max(0, i - 1)][1],
            data.positions[Math.max(0, i - 1)][2]
          );
          const diameter = Math.max(data.positions[i][3], 1.5);
          const section_index = data.positions[i][4];
          const next_section_index = data.positions[i + 1][4];
          const previous_section_index = data.positions[Math.max(0, i - 1)][4];

          const currentToPrevDist = position.distanceTo(prevPosition);
          const currentToSomaDist = position.distanceTo(somaPosition);

          if (
            previous_section_index !== section_index &&
            currentToPrevDist > 30 &&
            currentToSomaDist < 30
          ) {
            //console.log("Found a branch point at index " + i);
            const cylinderGeometry = new THREE.CylinderGeometry(
              diameter / 2,
              diameter / 2,
              position.distanceTo(somaPosition),
              32
            );

            const cylinderMaterial = new THREE.MeshBasicMaterial({
              color: [0, 0, 0],
            });

            const cylinder = new THREE.Mesh(cylinderGeometry, cylinderMaterial);
            cylinder.sectionIndex = section_index;
            cylinder.position.lerpVectors(position, somaPosition, 0.5);
            cylinder.lookAt(somaPosition);
            cylinder.quaternion.setFromUnitVectors(
              new THREE.Vector3(0, 1, 0),
              new THREE.Vector3().subVectors(somaPosition, position).normalize()
            );
            scene.add(cylinder);
            cylinders.push(cylinder);
          }

          // Continue with the rest of the cylinder drawing
          vertices.push(position.x, position.y, position.z);
          const var_plot = data.var_plot[section_index][0];
          const color = getColor((var_plot - varMin) / (varMax - varMin));
          colors.push(color.r, color.g, color.b);

          if (section_index === next_section_index) {
            let cylinderGeometry = new THREE.CylinderGeometry(
              diameter / 2,
              diameter / 2,
              position.distanceTo(nextPosition),
              32
            );
            let cylinderMaterial = new THREE.MeshBasicMaterial({
              color: color,
            });

            let cylinder = new THREE.Mesh(cylinderGeometry, cylinderMaterial);
            cylinder.position.lerpVectors(position, nextPosition, 0.5);
            cylinder.lookAt(nextPosition);
            cylinder.quaternion.setFromUnitVectors(
              new THREE.Vector3(0, 1, 0),
              new THREE.Vector3().subVectors(nextPosition, position).normalize()
            );

            cylinder.sectionIndex = section_index;

            scene.add(cylinder);

            cylinders.push(cylinder);
          }
        }

        geometry.setAttribute(
          "position",
          new THREE.Float32BufferAttribute(vertices, 3)
        );
        geometry.setAttribute(
          "size",
          new THREE.Float32BufferAttribute(sizes, 1)
        );

        const material = new THREE.PointsMaterial({
          vertexColors: true,
          sizeAttenuation: true,
        });

        const points = new THREE.Points(geometry, material);
        scene.add(points);

        let synapses = [];

        let synapseIntensity = new Array(data_syn.synapse_pos.length).fill(0);

        function closestCylinderTo(position) {
          let minDist = Infinity;
          let closestCylinder;
          for (const cylinder of cylinders) {
            const dist = position.distanceTo(cylinder.position);
            if (dist < minDist) {
              minDist = dist;
              closestCylinder = cylinder;
            }
          }
          return closestCylinder;
        }

        for (let i = 0; i < data_syn.synapse_pos.length; i++) {
          const position = new THREE.Vector3(
            data_syn.synapse_pos[i][0],
            data_syn.synapse_pos[i][1],
            data_syn.synapse_pos[i][2]
          );

          const closestCyl = closestCylinderTo(position);

          const cylOrientation = new THREE.Vector3()
            .subVectors(closestCyl.position, position)
            .normalize();
          const perpVector = new THREE.Vector3(0, 1, 0)
            .cross(cylOrientation)
            .normalize();

          const seed = i;
          const rng = new Math.seedrandom(seed);
          const randomAngle = rng() * Math.PI * 2;

          // Apply radial displacement
          perpVector.applyAxisAngle(cylOrientation, randomAngle);
          perpVector.multiplyScalar(
            Math.max(closestCyl.geometry.parameters.radiusTop, 1.5)
          );
          position.add(perpVector);

          let synapse = new THREE.Mesh(
            new THREE.SphereGeometry(1, 32, 32),
            new THREE.MeshBasicMaterial({ color: 0xffffff })
          );
          synapse.position.set(position.x, position.y, position.z);

          scene.add(synapse);

          synapses.push(synapse);
        }

        function animate() {
          controls.update();
          requestAnimationFrame(animate);

          frame =
            (frame + time_factor + data.var_plot[0].length) %
            data.var_plot[0].length;

          let simulationTime = frame * 0.025;
          simulationTimeElement.textContent =
            "Time: " + simulationTime.toFixed(1) + " ms";

          for (let i = 0; i < cylinders.length; i++) {
            const cylinder = cylinders[i];
            const sectionIndex = cylinder.sectionIndex;

            const var_plot = data.var_plot[sectionIndex][frame];

            const color = getColor((var_plot - varMin) / (varMax - varMin));
            cylinder.material.color = color;
          }

          // Update each synapse
          for (let i = 0; i < synapses.length; i++) {
            const synapse = synapses[i];
            const spikeTimes = data_syn.spike_times[i];

            // Check if spikeTimes is an array before proceeding.
            //This is to avoid errors when there are no spikes or one spike in the synapse
            if (
              Array.isArray(spikeTimes) &&
              spikeTimes.some(
                (time) => time >= frame && time < frame + time_factor - 1
              )
            ) {
              synapseIntensity[i] = 1;
            } else {
              synapseIntensity[i] *= Math.exp(-1 / 20);
            }

            let color = new THREE.Color(
              1,
              1 - synapseIntensity[i],
              1 - synapseIntensity[i]
            );
            synapse.material.color = color;
          }

          updateScaleBar();

          const newLinePosition =
            (simulationTime * 200) / data.var_plot[0].length / 0.025;
          redLine.style.left = 30 + newLinePosition + "px";
          // console.log(newLinePosition);

          renderer.render(scene, camera);
        }

        document.getElementById("loadingScreen").style.display = "none";
        animate();
      });
  });
