import numpy as np
from run_simulation import run_simulation
from run_morphology import run_morphology
from delete_neuron import delete_all_sections
from plot_results import plot_results
from run_simulation import reshape_v
import matplotlib.pyplot as plt
from matplotlib.colors import to_rgba
import json
import os
import webbrowser
from shutil import which

# File to load morphology from
morphology_file = "Jarsky"

duration = 250  # ms
dt = 0.025  # ms
stim_start = 50  # ms
stim_end = duration-5  # ms
protocol = "step"
stimmax = 0.0001  # Light intensity

sim_params = [duration, dt]

# Channels parameters
Nav_params = [0, 600, 0.048, 0.048, 0.048, 0.048]
Nav_inactivation = [340, 700, 1, 1, 0.3, 0.3]
Kap_params = [100, 101, 0.048, 0, 0, 0]
Kad_params = [100, 350, 0, 0.048, 0.048 * 6.5, 0.048 * 6.5]
Kdr_params = [0, 10, 0.04, 0.04, 0.04, 0.04]

# Nap_params = [300, 400, 0, 0.0, 0.0, 0]
# Kca_params = [500, 600, 0.0003, 0.0003, 0.0045, 0.0045]
# Mykca_params = [0, 0, 0, 0, 0, 0]
# CalH_params = [500, 600, 0.0003, 0.0003, 0.0045, 0.0045]
# Cal_params = [500, 600, 0.0003, 0.0003, 0.003, 0.003]
# Car_params = [500, 600, 0.0003, 0.0003, 0.0035, 0.0035]
# Car_mag_params = [500, 600, 0.0003, 0.0003, 0.0035, 0.0035]
# Somacar_params = [0, 0, 0, 0, 0, 0]
# Cat_params = [500, 600, 0.0003, 0.0003, 0.0035, 0.0035]
Nap_params = [300, 400, 0, 0.0, 0.0, 0]
Kca_params = [500, 600, 0.000, 0.000, 0.00, 0.00]
Mykca_params = [0, 0, 0, 0, 0, 0]
CalH_params = [500, 600, 0.000, 0.000, 0.00, 0.00]
Cal_params = [500, 600, 0.000, 0.000, 0.00, 0.00]
Car_params = [500, 600, 0.000, 0.000, 0.00, 0.00]
Car_mag_params = [500, 600, 0.000, 0.000, 0.00, 0.00]
Somacar_params = [0, 0, 0, 0, 0, 0]
Cat_params = [500, 600, 0.000, 0.000, 0.0, 0.00]

Chr_params = [15, 50, 1, 1, 1, 1, stim_start, stim_end]


channel_params = [Nav_params, Nav_inactivation, Kap_params, Kad_params, Kdr_params, Chr_params, Kca_params, CalH_params, Cal_params, Somacar_params, Nap_params, Mykca_params, Car_params, Car_mag_params, Cat_params]

# Synapse parameters
#synapse_params = [[1000, 5, 195, 100, 0.001, "poisson", "all", "ampa"], [100, 5, 195, 20, 0.01, "regular", "all", "gabaa"], [100, 105, 195, 100, 0.01, "regular", "all", "gabab"]]
synapse_params = []

outputs = ["v", "g_chr", "gexp_chr"]

if protocol == "full": # Like simple_waveform of FF
    # Stimulation parameters
    stim_duration = duration  # total duration of stimulation protocol
    scale = stim_duration / 200
    times = np.arange(0, sim_params[0] + dt, sim_params[1])
    t_down = 10 * scale  # ms
    t_up = 20 * scale  # ms
    t_ramp = 75 * scale  # ms

    n_start = int(stim_start / sim_params[1])
    n_down = int(t_down / sim_params[1])
    n_up = int(t_up / sim_params[1])
    n_ramp = int(t_ramp / sim_params[1])
    zeros_start = np.zeros(n_start)
    zeros_down = np.zeros(n_down)
    ramp_up_1 = np.ones(n_up) * stimmax / 3
    ramp_up_2 = np.ones(n_up) * stimmax * 2 / 3
    ramp_up_3 = np.ones(n_up) * stimmax
    ramp = np.linspace(0, stimmax, n_ramp)

    # Combine them to form the complete waveform
    waveform = np.concatenate([zeros_start, zeros_down, ramp_up_1, zeros_down, ramp_up_2, zeros_down, ramp_up_3, np.tile(zeros_down, 2), ramp, zeros_down])
elif protocol == "step":
    times = np.arange(0, sim_params[0] + dt, sim_params[1])
    waveform = np.ones(len(times)) * stimmax

if len(waveform) < len(times):
    waveform = np.concatenate([waveform, np.zeros(len(times) - len(waveform))])
elif len(waveform) > len(times):
    waveform = waveform[:len(times)]


# Run simulation
result = run_simulation(sim_params, channel_params, synapse_params, waveform, outputs, morphology_file)
delete_all_sections()
morpho = run_morphology(channel_params, morphology_file)
delete_all_sections()



# Get results
time = [float(x) for x in result[0]]
section_names = [x for x in morpho[0]]
section_nums = [float(x) for x in morpho[2]]
result_vars = result[1]

synapse_nr = 0
for param in synapse_params:
    synapse_nr += int(param[0])

synapse_pos = []
spike_times = []
spike_times_frames = []
for idx in range(synapse_nr):
    synapse_pos.append([float(x) for x in result[2][idx]])
    spike_times.append([float(x) for x in result[3][idx]])
    spike_times_frames.append([float(x) for x in result[4][idx]])

if 'synapse_pos' not in locals():
    synapse_pos = []
if 'spike_times' not in locals():
    spike_times = []
    spike_times_frames = []

varNames = outputs
for varName in varNames:
    varCell = result_vars[varName]
    varMatrix = []
    for item in varCell:
        varMatrix.extend([float(x) for x in item])
    locals()[varName] = varMatrix
p = morpho[3]
positions = [[float(x) for x in item] for item in p]
d = morpho[1]
distance_matrix = [[float(x) for x in item] for item in d]
allSpikeTimes = [item for sublist in spike_times for item in sublist]
waveform = [x if stim_start <= t <= stim_end else 0 for x, t in zip(waveform, time)]

for output in outputs:
    variable = locals()[output]
    reshaped_variable = reshape_v(variable, distance_matrix)
    locals()[output] = reshaped_variable


# Plot results
plot_results(time, v, distance_matrix, duration, False)


# Animate results
data_file = "data.json"
data_syn_file = "data_syn.json"
animation_path = "none"
if animation_path != "none":
    # Combine the strings for the file names and paths
    combined_data_file = os.path.join(animation_path, data_file)
    combined_data_syn_file = os.path.join(animation_path, data_syn_file)
    # If the files exist, delete them
    if os.path.isfile(combined_data_file):
        os.remove(combined_data_file)
    if os.path.isfile(combined_data_syn_file):
        os.remove(combined_data_syn_file)

    # Define data and write to file
    v_short = np.array(v).astype(int).tolist()  # Convert to list
    data = {'positions': positions, 'var_plot': v_short}  # No need to convert positions to list
    with open(combined_data_file, 'w') as file:
        json.dump(data, file)

    # Prepare spike times and synapse positions
    spike_times_cell = [list(spike_times_frame) for spike_times_frame in spike_times_frames] # Converting inner lists if needed
    data_syn = {'spike_times': spike_times_cell, 'synapse_pos': list(synapse_pos)}  # Convert synapse_pos to list if needed
    with open(combined_data_syn_file, 'w') as file_syn:
        json.dump(data_syn, file_syn)
