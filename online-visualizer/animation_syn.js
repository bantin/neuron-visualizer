import * as THREE from "./three.module.js";
import { OrbitControls } from "./OrbitControls.js";

fetch("/data.json")
  .then((response) => response.json())
  .then((data) => {
    fetch("/data_syn.json")
      .then((response2) => response2.json())
      .then((data_syn) => {
        console.log(data_syn.synapse_pos);

        const scene = new THREE.Scene();
        let varMin = Infinity;
        let varMax = -Infinity;

        // Loop through each array in data.var_plot to find min and max
        for (let i = 0; i < data.var_plot.length; i++) {
          for (let j = 0; j < data.var_plot[i].length; j++) {
            if (data.var_plot[i][j] < varMin) {
              varMin = data.var_plot[i][j];
            }
            if (data.var_plot[i][j] > varMax) {
              varMax = data.var_plot[i][j];
            }
          }
        }
        console.log(varMin, varMax);
        let pow = 0;
        if (varMin != 0) {
          let sign = Math.sign(varMin);
          pow = Math.floor(Math.log10(Math.abs(varMin)));
          varMin =
            (sign * Math.round(Math.abs(varMin) * Math.pow(10, -pow))) /
            Math.pow(10, -pow);
        }

        if (varMax != 0) {
          let sign = Math.sign(varMax);
          pow = Math.floor(Math.log10(Math.abs(varMax)));
          varMax =
            (sign * Math.round(Math.abs(varMax) * Math.pow(10, -pow))) /
            Math.pow(10, -pow);
        }

        console.log(varMin, varMax);

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
          else red = 1 - (0.5 * (i - 224)) / (257 - 224);
          if (i < 32) blue = 0.5 + i / (257 - 225);
          else if (i < 95) blue = 1;
          else if (i < 160) blue = 1 - (i - 95) / (160 - 95);
          else blue = 0;
          if (i < 31) green = 0;
          else if (i < 65) green = (i - 31) / 34;
          else if (i < 160) green = 1;
          else if (i < 224) green = 1 - (i - 160) / (160 - 95);
          else green = 0;
          colorMap.push(new THREE.Color(red, green, blue));
        }

        function getColor(varNormalized) {
          let index = Math.round(varNormalized * 255);
          index = Math.max(0, Math.min(255, index));
          return colorMap[index];
        }

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
        slider.min = -1;
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
          // Toggle scalebar visibility flag
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
        waveformImg.style.display = showWaveform ? "block" : "none"; // Display based on checkbox initial value
        waveformImg.style.width = "100%";
        waveformImg.style.height = "100%";

        waveformContainer.appendChild(waveformImg);

        const redLine = document.createElement("div");
        redLine.style.position = "absolute";
        redLine.style.top = "4px";
        redLine.style.left = "35px";
        redLine.style.width = "1px";
        redLine.style.height = "164px";
        redLine.style.backgroundColor = "red";
        redLine.style.display = showWaveform ? "block" : "none";

        waveformContainer.appendChild(redLine);
        legendContainer.appendChild(waveformContainer);
        document.body.appendChild(legendContainer);

        toggleWaveform.addEventListener("change", function () {
          // Toggle waveform visibility
          const isVisible = toggleWaveform.checked;
          if (isVisible) {
            waveformImg.style.display = "block";
            redLine.style.display = "block";
            legendContainer.style.height = "auto"; // Reset to default value
          } else {
            waveformImg.style.display = "none";
            redLine.style.display = "none";
            legendContainer.style.height = "275px"; // Set a specific height
          }
        });

        const axesHelper = new THREE.AxesHelper(1);
        scene.add(axesHelper);

        function updateScaleBar() {
          if (showScaleBar) {
            // Move AxesHelper off to the side by 100 units
            axesHelper.position.set(150, -150, 0);

            // Keep the size of the AxesHelper constant at 100 units
            axesHelper.scale.set(100, 100, 100);

            axesHelper.visible = true; // Make sure AxesHelper is visible
          } else {
            axesHelper.visible = false; // Hide AxesHelper
          }
        }

        let cylinders = [];
        let somaPosition;

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
          const diameter = Math.max(data.positions[i][3], 1.5);
          const section_index = data.positions[i][4];
          const next_section_index = data.positions[i + 1][4];

          if (i === 0) {
            somaPosition = position;
          }

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

            scene.add(cylinder);

            cylinders.push(cylinder);
          } else {
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

        // Array to store synapses
        let synapses = [];

        // Array to store synapse intensity
        let synapseIntensity = new Array(data_syn.synapse_pos.length).fill(0);

        for (let i = 0; i < data_syn.synapse_pos.length; i++) {
          const position = new THREE.Vector3(
            data_syn.synapse_pos[i][0],
            data_syn.synapse_pos[i][1],
            data_syn.synapse_pos[i][2]
          );

          let synapse = new THREE.Mesh(
            new THREE.SphereGeometry(1.5, 32, 32),
            new THREE.MeshBasicMaterial({ color: 0xffffff })
          ); // White color
          synapse.position.set(position.x, position.y, position.z);

          scene.add(synapse);

          synapses.push(synapse);
        }

        function animate() {
          controls.update();
          requestAnimationFrame(animate);

          frame = (frame + time_factor) % data.var_plot[0].length;

          let simulationTime = frame * 0.025;
          simulationTimeElement.textContent =
            "Time: " + simulationTime.toFixed(1) + " ms";

          for (let i = 0; i < cylinders.length; i++) {
            const cylinder = cylinders[i];
            const section_index_uncorrected = data.positions[i][4];
            const section_index =
              data.positions[i + section_index_uncorrected][4]; // Correction for the fact that the numbers of points and cylinders are different
            const var_plot = data.var_plot[section_index][frame];
            const color = getColor((var_plot - varMin) / (varMax - varMin));
            cylinder.material.color = color;
          }

          // Update each synapse
          for (let i = 0; i < synapses.length; i++) {
            const synapse = synapses[i];
            const spikeTimes = data_syn.spike_times[i];

            // Check if spikeTimes is an array before proceeding
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
            (simulationTime * (262 - 70)) / data.var_plot[0].length / 0.025;
          redLine.style.left = 35 + newLinePosition + "px";
          console.log(newLinePosition);

          renderer.render(scene, camera);
        }

        document.getElementById("loadingScreen").style.display = "none";
        animate();
      });
  });
