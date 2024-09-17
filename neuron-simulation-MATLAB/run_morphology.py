import numpy as np
from neuron import h, gui
from SprustonModel import SprustonModel

def run_morphology(channel_params):
    # Create a model using the provided channel parameters
    model = SprustonModel(channel_params)

    # Generate distance matrix
    distance_matrix = [
        np.array([h.distance(section(0.5), second_section(0.5)) for second_section in h.allsec()])
        for section in h.allsec()
    ]

    # Collect 3D position information
    section_count = sum(1 for _ in h.allsec())
    section_nums = [0] * section_count
    positions = []
    for index, section in enumerate(h.allsec()):
        section_nums[index] = section.n3d()
        for k in range(section_nums[index]):
            position_info = [
                section.x3d(k),
                section.y3d(k),
                section.z3d(k),
                section.diam3d(k),
                index,
            ]
            positions.append(position_info)

    # Collect section labels
    section_names = []
    for sec in h.allsec():
        section_names.append(sec.name())
    
    return section_names, distance_matrix, section_nums, positions

