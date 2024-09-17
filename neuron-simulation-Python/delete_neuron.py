from neuron import h

def delete_all_sections():
    for sec in h.allsec():
        h.delete_section(sec=sec)