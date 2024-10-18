

def cleanup_swc_file(input_path, output_path, soma_nodes=None,
    xy_microns_per_pixel=None, z_microns_per_pixel=None):
    
    with open(input_path, 'r') as f:
        # read each line of the file.
        # If it is a comment, write it to the output file.
        # If it is a node which is not a soma node, write it to the output file
        # as apical node (#4). If it is a soma node, write it to the output file
        # as a soma node (#1).
        lines = f.readlines()

    # Each line is a string formatted as follows:
    # node_id, node_type, x, y, z, radius, parent_id
    with open(output_path, 'w') as f:
        for line in lines:
            if line[0] == '#':
                f.write(line)
            else:
                node_id, node_type, x, y, z, radius, parent_id = line.split()

                # If the xy_microns_per_pixel and z_microns_per_pixel are provided,
                # convert the x, y, and z coordinates to microns.
                if xy_microns_per_pixel is not None:
                    x = float(x) * xy_microns_per_pixel
                    y = float(y) * xy_microns_per_pixel
                if z_microns_per_pixel is not None:
                    z = float(z) * z_microns_per_pixel
                
                # Convert radius to microns using the xy_microns_per_pixel
                radius = float(radius) * xy_microns_per_pixel

                if soma_nodes is not None and int(node_id) in soma_nodes:
                    f.write(f'{node_id} 1 {x} {y} {z} {radius} {parent_id}\n')
                else:
                    f.write(f'{node_id} 4 {x} {y} {z} {radius} {parent_id}\n')

def count_compartments(swc_path):
    with open(swc_path, 'r') as f:
        lines = f.readlines()
    
    return len([line for line in lines if line[0] != '#'])