from neuron import h, gui

class SprustonModel:

    def __init__(self, channel_params):
        # Load the morphology
        self.load_morphology()

        self.add_passive()

        self.add_rxd_calcium()
        
        self.add_Nav_channels(*channel_params[0])
        self.add_Nav_inactivation(*channel_params[1])
        self.add_Kap_channels(*channel_params[2])
        self.add_Kad_channels(*channel_params[3])
        self.add_Kdr_channels(*channel_params[4])
        self.add_Chr_channels(*channel_params[5])

        self.add_kca_channel(*channel_params[6])
        self.add_calH_channel(*channel_params[7])
        self.add_cal_channel(*channel_params[8])
        self.add_somacar_channel(*channel_params[9])
        self.add_nap_channel(*channel_params[10])
        self.add_mykca_channel(*channel_params[11])
        self.add_car_channel(*channel_params[12])
        self.add_car_mag_channel(*channel_params[13])
        self.add_cat_channel(*channel_params[14])

        self.set_axon_conductance()

    def load_morphology(self):
        # Start a new section list, this will contain all the sections of your model
        self.all = h.SectionList()

        # Load soma and dendrites
        h.load_file('Jarsky_Gating_Morphology.nrn')
        for sec in h.allsec():
            self.all.append(sec=sec)

        # Load axon and AIS
        h.load_file('Jarsky_Gating_Axon.nrn')
        for sec in h.allsec():
            if sec not in self.all:
                self.all.append(sec=sec)

    def add_Nav_channels(self, x1, x2, density_pre, density_start, density_end, density_post):
        self.add_channel('na3', 'gbar_na3', x1, x2, density_pre, density_start, density_end, density_post)

    def add_Nav_inactivation(self, x1, x2, density_pre, density_start, density_end, density_post):
        self.set_channel('na3', 'ar2_na3', x1, x2, density_pre, density_start, density_end, density_post)

    def add_Kap_channels(self, x1, x2, density_pre, density_start, density_end, density_post):
        self.add_channel('kap', 'gkabar_kap', x1, x2, density_pre, density_start, density_end, density_post)

    def add_Kad_channels(self, x1, x2, density_pre, density_start, density_end, density_post):
        self.add_channel('kad', 'gkabar_kad', x1, x2, density_pre, density_start, density_end, density_post)

    def add_Kdr_channels(self, x1, x2, density_pre, density_start, density_end, density_post):
        self.add_channel('kdr', 'gkdrbar_kdr', x1, x2, density_pre, density_start, density_end, density_post)

    def add_Chr_channels(self, x1, x2, density_pre, density_start, density_end, density_post, stim_start, stim_end):
        self.add_channel('chr', 'g_chr', x1, x2, density_pre, density_start, density_end, density_post)
        self.set_channel('chr', 'del_chr', 0, 0, stim_start, stim_start, stim_start, stim_start)
        self.set_channel('chr', 'dur_chr', 0, 0, stim_end-stim_start, stim_end-stim_start, stim_end-stim_start, stim_end-stim_start)

    def add_kca_channel(self, x1, x2, density_pre, density_start, density_end, density_post):
        self.add_channel('kca', 'gbar_kca', x1, x2, density_pre, density_start, density_end, density_post)

    def add_calH_channel(self, x1, x2, density_pre, density_start, density_end, density_post):
        self.add_channel('calH', 'gcalbar_calH', x1, x2, density_pre, density_start, density_end, density_post)

    def add_cal_channel(self, x1, x2, density_pre, density_start, density_end, density_post):
        self.add_channel('cal', 'gcalbar_cal', x1, x2, density_pre, density_start, density_end, density_post)

    def add_somacar_channel(self, x1, x2, density_pre, density_start, density_end, density_post):
        self.add_channel('somacar', 'gcabar_somacar', x1, x2, density_pre, density_start, density_end, density_post)

    def add_nap_channel(self, x1, x2, density_pre, density_start, density_end, density_post):
        self.add_channel('nap', 'gnabar_nap', x1, x2, density_pre, density_start, density_end, density_post)

    def add_mykca_channel(self, x1, x2, density_pre, density_start, density_end, density_post):
        self.add_channel('mykca', 'gkbar_mykca', x1, x2, density_pre, density_start, density_end, density_post)

    def add_car_channel(self, x1, x2, density_pre, density_start, density_end, density_post):
        self.add_channel('car', 'gcabar_car', x1, x2, density_pre, density_start, density_end, density_post)

    def add_car_mag_channel(self, x1, x2, density_pre, density_start, density_end, density_post):
        self.add_channel('car_mag', 'gmax_car_mag', x1, x2, density_pre, density_start, density_end, density_post)

    def add_cat_channel(self, x1, x2, density_pre, density_start, density_end, density_post):
        self.add_channel('cat', 'gcatbar_cat', x1, x2, density_pre, density_start, density_end, density_post)

    def add_channel(self, channel_name, gvar, x1, x2, density_pre, density_start, density_end, density_post):
        soma_sec = next(h.allsec())
        for sec in self.all:
            sec.insert(channel_name)
            # Use setattr to set the conductance attribute dynamically
            x = h.distance(sec(0.5),soma_sec(0.5))

            # Set the conductance based on the distance
            if x < x1:
                g = density_pre
            elif x1 <= x < x2:
                g = density_start + (density_end - density_start) * (x - x1) / (x2 - x1)
            else:
                g = density_post            
 
            setattr(sec, gvar, g)

    def distance_soma(self, sec):
        soma_sec = next(h.allsec())
        return h.distance(sec(0.5), soma_sec(0.5))

    def set_channel(self, channel_name, gvar, x1, x2, density_pre, density_start, density_end, density_post):
        for sec in self.all:
            x = self.distance_soma(sec)
            # Set the conductance based on the distance
            if x < x1:
                g = density_pre
            elif x1 <= x < x2:
                g = density_start + (density_end - density_start) * (x - x1) / (x2 - x1)
            else:
                g = density_post            
 
            setattr(sec, gvar, g)

    def add_passive(self):
        Vleak = -66  # Leak reversal in Cs+
        Rm = 40000   # Specific membrane resistivity in ohm-cm^2
        global_ra = 200.00  # Internal resistivity in ohm-cm
        Cm = 0.75  # Specific membrane capacitance in uF/cm^2
        spinefactor = 2.0
        spinelimit = 300

        for sec in h.allsec():
            sec.insert('pas')
            sec.e_pas = Vleak
            if self.distance_soma(sec) <= spinelimit:
                sec.g_pas = 1 / Rm
                sec.cm = Cm
            else:
                sec.g_pas = spinefactor / Rm
                sec.cm = spinefactor * Cm
            sec.Ra = global_ra

    def add_rxd_calcium(self):
        for sec in h.allsec():
          #  sec.insert('cad')
            sec.insert('cacum')


    def set_axon_conductance(self):
        gnabar = 0.040  # Sodium conductance
        gnode = 30.0  # Sodium conductance at a node
        gkdr = 0.040  # Delayed rectifier potassium conductance
        gkap = 0.048  # A-type potassium conductance
        Rn = 50  # Nodal resistivity
        Rm = 40000  # specific membrane resistivity in ohm-cm^2 
        Cmy = 0.04  # Capacitance in myelin

        for sec in h.allsec():
            if 'hill' in sec.name() or 'iseg' in sec.name():
                sec.gbar_na3 = gnabar
                sec.gkdrbar_kdr = gkdr
                sec.gkabar_kap = gkap

            if 'node' in sec.name(): 
                sec.gbar_na3 = gnode
                sec.gkdrbar_kdr = gkdr
                sec.gkabar_kap = gkap * 0.2
                sec.g_pas = 1 / Rn

            if 'inode' in sec.name():            
                sec.gbar_na3 = gnabar
                sec.gkdrbar_kdr = gkdr
                sec.gkabar_kap = gkap * 0.2
                sec.cm = Cmy
                sec.g_pas = 1 / Rm

