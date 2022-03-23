#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Total Power Radiometer
# Author: Matthew Nelson
# Description: Blocks for power detection, integration and LPF for a total power radiometer
# Generated: Sun Apr 12 23:03:59 2015
##################################################

from gnuradio import blocks
from gnuradio import filter
from gnuradio import gr
from gnuradio.filter import firdes

class TPR(gr.hier_block2):

    def __init__(self, integ=1, samp_rate=1, det_rate=1):
        gr.hier_block2.__init__(
            self, "Total Power Radiometer",
            gr.io_signature(1, 1, gr.sizeof_gr_complex*1),
            gr.io_signature(1, 1, gr.sizeof_float*1),
        )

        ##################################################
        # Parameters
        ##################################################
        self.integ = integ
        self.samp_rate = samp_rate
        self.det_rate = det_rate

        ##################################################
        # Blocks
        ##################################################
        self.single_pole_iir_filter_xx_0 = filter.single_pole_iir_filter_ff(1.0/((samp_rate*integ)/2.0), 1)
        (self.single_pole_iir_filter_xx_0).set_processor_affinity([1])
        self.blocks_keep_one_in_n_4 = blocks.keep_one_in_n(gr.sizeof_float*1, samp_rate/det_rate)
        self.blocks_complex_to_mag_squared_1 = blocks.complex_to_mag_squared(1)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_complex_to_mag_squared_1, 0), (self.single_pole_iir_filter_xx_0, 0))    
        self.connect((self.blocks_keep_one_in_n_4, 0), (self, 0))    
        self.connect((self, 0), (self.blocks_complex_to_mag_squared_1, 0))    
        self.connect((self.single_pole_iir_filter_xx_0, 0), (self.blocks_keep_one_in_n_4, 0))    


    def get_integ(self):
        return self.integ

    def set_integ(self, integ):
        self.integ = integ
        self.single_pole_iir_filter_xx_0.set_taps(1.0/((self.samp_rate*self.integ)/2.0))

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.single_pole_iir_filter_xx_0.set_taps(1.0/((self.samp_rate*self.integ)/2.0))
        self.blocks_keep_one_in_n_4.set_n(self.samp_rate/self.det_rate)

    def get_det_rate(self):
        return self.det_rate

    def set_det_rate(self, det_rate):
        self.det_rate = det_rate
        self.blocks_keep_one_in_n_4.set_n(self.samp_rate/self.det_rate)

