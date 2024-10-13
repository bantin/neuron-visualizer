import numpy as np
from run_simulation import run_simulation
from run_morphology import run_morphology
from delete_neuron import delete_all_sections
from plot_results import plot_results
from plot_results import kymograph_stim
from plot_results import kymograph
from run_simulation import reshape_v
from run_simulation import generate_waveform
import matplotlib.pyplot as plt
from matplotlib.colors import to_rgba
import json
import os
import webbrowser
from shutil import which
from itertools import chain
from swc_utils import count_compartments

# File to load morphology from
# morphology_file = "Jarsky"
# morphology_file = "ri04.swc"
morphology_file = "./cell4_2p_registered_cc_cleaned.swc"

duration = 500  # Total duration in ms
dt = 0.025  # Time step in ms

# Channels parameters
Nav_params = [0.050, 0.047, 300, 100]
Nav_inactivation = [1, 0.6, 300, 100]
Kad_params = [-0.2, 0.3, 0, 150]
Kdr_params = [0.04, 0.04, 5, 10]

Kap_params = [0, 0, 0, 1]
Nap_params = [0, 0.0, 250, 200]
Kca_params = [0, 0, 550, 100]
Mykca_params = [0, 0, 0, 1]
CalH_params = [0, 0, 550, 100]
Cal_params = [0, 0, 550, 100]
Car_params = [0, 0, 550, 100]
Car_mag_params = [0, 0, 550, 100]
Somacar_params = [0, 0, 0, 1]
Cat_params = [0, 0, 550, 100]
# Channel Parameters: [g0, g1, d, z] 
# where g = g0 + (g1 - g0) / (1 + exp((d - x) / z))

# Channelrhodopsin parameters
stim_start = 0  # ms
stim_end = 400  # ms
Chr_params = [1, 0, 50, 20, stim_start, stim_end, 0, 0]
# Chr_params = [1, 0, 5, 2, stim_start, stim_end, 0, 0]
# Chr_params = []
# [gmax, gmax_std, d, z, stim_start, stim_end, location (section nr., vararg, default 0), Reversal Potential (vararg, default 0)]
# To stimulate e.g. a region 50um around dendrite nr. 105 use location = 105. First 4 parameters as other channels above.

channel_params = [Nav_params, Nav_inactivation, Kap_params, Kad_params, Kdr_params, Chr_params, Kca_params, CalH_params, Cal_params, Somacar_params, Nap_params, Mykca_params, Car_params, Car_mag_params, Cat_params]

# Synapse parameters
input_duration = 1000 # Duration of input in ms
# synapse_params = [[100, 5, input_duration, 100, 0.001, "theta", "all", "ampa"], [100, 5, input_duration, 20, 0.01, "poisson", "basal", "gabaa"], [100, 105, input_duration, 100, 0.01, "gamma", "apical", "gabab"]]
synapse_params = [[100, 5, input_duration, 100, 0.001, "theta", "apical", "ampa"]]
# E.g. [200, 5, 195, 100, 0.001, "theta", "all", "ampa"] means: 200 AMPA synapses with Theta frequency input (100Hz avg. freq.) on all sections, with 5 ms delay, 190 ms duration, and 0.001 weight
synapse_params = []

# Parameters to output from the simulation. 
outputs = ["v", "g_chr", "gexp_chr", "gkabar_kad"]


# Channelrhodopsin light intensity parameters
stimmax = 8e-4  # Set Channelrhodopsin light intensity 
sim_params = [duration, dt, 2, 10, 190, 0]
waveform = generate_waveform(sim_params, stimmax, stim_start, stim_end, "full")
# waveform = np.zeros(len(waveform)) # Comment this line to use the waveform generated above
# Options: pulses, step, full, ramp

# save opto waveform to a file
np.save('opto_waveform.npy', waveform)

# Patch clamp parameters
sim_params[2] = 2 # ms pulse duration for patch clamp pulses
sim_params[3] = 20 # ms tims between pulses for patch clamp pulses
sim_params[4] = 100 # number of pulses for patch clamp pulses
sim_params[5] = 0 # baseline current/voltage for patch clamp pulses (nA or mV)
waveform_patch = generate_waveform(sim_params, 0, 0, 300, "pulses")
patch_params = [0, waveform_patch, "i"]

# Run simulation
result = run_simulation(sim_params, channel_params, synapse_params, waveform, outputs, morphology_file, patch_params)
delete_all_sections()

# Run morphology analysis
morpho = run_morphology(channel_params, morphology_file)
delete_all_sections()

# Parse results
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

varray = np.array(v)
plt.imshow(varray)

# Plot results
#plot_results(time, v, distance_matrix, duration, False)
[distances, voltages] = kymograph(time,v,distance_matrix)
distances_norm = distances / np.max(distances)

# Make a compartments x time matrix of voltages, where each
# row is a compartment in the SWC file and each column is a time step.
# For a given compartment c, we can find the section it was assigned to as:
# section_index = positions[c][-1]

# Build map from compartment to section
num_compartments = count_compartments(morphology_file)
cmpt_map = {c : int(positions[c][-1]) for c in range(num_compartments)}
voltages = np.zeros((num_compartments, len(time)))
for c in range(num_compartments):
    voltages[c, :] = varray[cmpt_map[c], :]

# save voltages and compartment map to a file
np.save('voltages.npy', voltages)
np.save('compartment_map.npy', cmpt_map)

# Plot waveform
plt.plot(time, waveform_patch*1000)
plt.xlabel('Time (ms)')
plt.ylabel('Amplitude (pA)')
plt.title('Patch Waveform')
plt.grid(True)
plt.show()


# Plot kymograph of optogenetic stimulation
stim_dist = [[a * b for a, b in zip(sublist1, sublist2)] for sublist1, sublist2 in zip(gexp_chr, g_chr)]
[distances, stims] = kymograph(time,stim_dist,distance_matrix)



# Animate results
data_file = "data.json"
data_syn_file = "data_syn.json"
#animation_path = "C:\\Users\\danie\\Documents\\GitHub\\neuron-visualizer\\online-visualizer" #Set path to save results to.
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

# Create the figure
fig, ax1 = plt.subplots()
fig.patch.set_facecolor('none')
ax1.set_xlabel('Time (ms)')
ax1.set_ylabel('Net Synaptic Stimulation (Hz)', color=(240/255, 240/255, 240/255))
if len(allSpikeTimes) == 0:
    ax1.plot(0, 0)
    ax1.set_ylim([0, 10])
else:
    _, bins, _ = ax1.hist(allSpikeTimes, bins=range(0, int(duration), 1), facecolor=(240/255, 240/255, 240/255), edgecolor=(240/255, 240/255, 240/255))
    ax1.set_ylim([0, max(np.histogram(allSpikeTimes, bins=bins)[0])])

ax2 = ax1.twinx()
if max(waveform) == 0:
    waveform_norm = waveform
else:
    waveform_norm = waveform / max(waveform)
ax2.plot(time, waveform_norm, linewidth=2, color=(0, 0.705, 0.896))
ax2.set_ylabel('Optogenetic Stimulation', rotation=-90, labelpad=15)
if max(waveform) == 0:
    ax2.set_ylim([0, 1.05])
else:
    ax2.set_ylim([0, max(waveform_norm)*1.05])
for ax in [ax1, ax2]:
    ax.spines['bottom'].set_color((240/255, 240/255, 240/255))
    ax.spines['top'].set_color((240/255, 240/255, 240/255))
    ax.xaxis.label.set_color((240/255, 240/255, 240/255))
    ax.yaxis.label.set_color((240/255, 240/255, 240/255))
    ax.tick_params(axis='x', colors=(240/255, 240/255, 240/255))
    ax.tick_params(axis='y', colors=(240/255, 240/255, 240/255))
    ax.xaxis.label.set_size(14)
    ax.yaxis.label.set_size(14)
    ax.tick_params(axis='both', which='major', labelsize=14)
plt.xlim([0, duration])
plt.tight_layout()

if animation_path != "none":
    plt.savefig(f"{animation_path}/waveform.png", transparent=True)
plt.show()
