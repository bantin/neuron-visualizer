from neuron import h, gui
from SprustonModel import SprustonModel
import numpy as np

def run_simulation(duration, channel_params):
    # Create a neuron model
    model = SprustonModel(channel_params)
    
    # Simulation parameters
    h.tstop = duration
    h.dt = 0.025
    celsius = 35; # Temperature 

    # Record time
    rec_t = h.Vector()
    rec_t.record(h._ref_t)

    # Record voltage from each section
    rec_v = []
    for sec in h.allsec():
        vec = h.Vector()
        vec.record(sec(0.5)._ref_v)
        rec_v.append(vec)

    # Initialize and run the simulation
    h.finitialize(-70)
    h.continuerun(duration)

    # Convert recorded data to NumPy arrays
    time = np.array(rec_t)
    voltage = [np.array(vec) for vec in rec_v]

    return time, voltage
