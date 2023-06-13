from neuron import h, gui
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import ipywidgets as widgets
from IPython.display import display

# Create a ball-and-stick neuron model
soma = h.Section(name='soma')
dend = h.Section(name='dend')
dend.connect(soma(1))

# ... rest of neuron setup ...

# Add a synapse
syn = h.ExpSyn(dend(0.5))
syn.tau = 2 # ms

# Add a NetStim and NetCon for delivering stimuli
stimulus = h.NetStim()
stimulus.number = 1
connection = h.NetCon(stimulus, syn)
connection.weight[0] = 0.01 # Strength of stimulus

# Add a current clamp to the soma
stim = h.IClamp(soma(0.5))
stim.delay = 0  # Start the current injection immediately
stim.dur = 1e9  # Keep it going indefinitely

# Set up recording variables
v_soma = h.Vector().record(soma(0.5)._ref_v)
v_dend = h.Vector().record(dend(0.5)._ref_v)
t = h.Vector().record(h._ref_t)

# Set up the plot
fig, ax = plt.subplots()
line1, = ax.plot([], [], label='soma')
line2, = ax.plot([], [], label='dend')
ax.legend()

# This function updates the plot
def update_plot(frame):
    line1.set_data(t, v_soma)
    line2.set_data(t, v_dend)
    ax.set_xlim((h.t-30, h.t))

    # Remove old data to keep the plot from slowing down
    if len(t) > 3000:
        v_soma.remove(0)
        v_dend.remove(0)
        t.remove(0)

# Set up the slider
current_slider = widgets.FloatSlider(min=-100, max=1000, description='Current:')

# Set up the button
def stimulate_synapse(b):
    stimulus.start = h.t
stimulus_button = widgets.Button(description='Stimulate Synapse')
stimulus_button.on_click(stimulate_synapse)

# Display the slider and button
display(current_slider)
display(stimulus_button)

# Start the plot animation
ani = FuncAnimation(fig, update_plot, interval=30)

# Run the simulation
while True:
    stim.amp = current_slider.value
    h.fadvance()

