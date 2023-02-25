#!/usr/bin/env python2
##################################################
# GNU Radio Python Flow Graph
# Title: Total Power Radiometer - AD9361
# Author: Matthew E Nelson
# Description: Total power radiometer connecting to a AD9361 SDR
# Generated: Thu Jan  1 00:20:51 1970
##################################################

import os
import sys
sys.path.append(os.environ.get('GRC_HIER_PATH', os.path.expanduser('~/.grc_gnuradio')))

from TPR import TPR  # grc-generated hier_block
from datetime import datetime
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import iio
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from grc_gnuradio import blks2 as grc_blks2
from optparse import OptionParser


class AD9361_TPR_nogui(gr.top_block):

    def __init__(self, RF_bandwidth=15000000, calib_1=4.0755e3, calib_2=-342.774, dc_gain=1, fftsize=8192, frequency=1.4125e9, integ=2, prefix="/home/analog/Documents/sdr_dev/data/", sample_rate=10e6, spavg=1):
        gr.top_block.__init__(self, "Total Power Radiometer - AD9361")

        ##################################################
        # Parameters
        ##################################################
        self.RF_bandwidth = RF_bandwidth
        self.calib_1 = calib_1
        self.calib_2 = calib_2
        self.dc_gain = dc_gain
        self.fftsize = fftsize
        self.frequency = frequency
        self.integ = integ
        self.prefix = prefix
        self.sample_rate = sample_rate
        self.spavg = spavg

        ##################################################
        # Variables
        ##################################################
        self.tpr_kelvin_file = tpr_kelvin_file = prefix +  "tpr_kelvin.dat"
        self.tpr_file = tpr_file = prefix +  "tpr.dat"
        self.samp_rate = samp_rate = int(sample_rate)
        self.freq = freq = frequency
        self.file_rate = file_rate = 2.0
        self.det_rate = det_rate = int(20.0)

        ##################################################
        # Blocks
        ##################################################
        self.iio_fmcomms2_source_0 = iio.fmcomms2_source_f32c("localhost", int(freq), int(sample_rate), RF_bandwidth, True, False, 0x8000, True, True, True, "manual", 64.0, "manual", 64.0, "A_BALANCED", "", True)
        self.blocks_multiply_const_vxx_1 = blocks.multiply_const_vff((calib_1, ))
        self.blocks_multiply_const_vxx_0 = blocks.multiply_const_vff((dc_gain, ))
        self.blocks_keep_one_in_n_1 = blocks.keep_one_in_n(gr.sizeof_float*1, int(det_rate/file_rate))
        self.blocks_file_sink_1 = blocks.file_sink(gr.sizeof_float*1, tpr_file, False)
        self.blocks_file_sink_1.set_unbuffered(True)
        self.blocks_file_sink_0 = blocks.file_sink(gr.sizeof_float*1, tpr_kelvin_file, False)
        self.blocks_file_sink_0.set_unbuffered(True)
        self.blocks_add_const_vxx_1 = blocks.add_const_vff((calib_2, ))
        self.blks2_valve_1 = grc_blks2.valve(item_size=gr.sizeof_float*1, open=bool(0))
        self.blks2_valve_0 = grc_blks2.valve(item_size=gr.sizeof_float*1, open=bool(0))
        self.TPR_0 = TPR(
            det_rate=det_rate,
            integ=integ,
            samp_rate=samp_rate,
        )

        ##################################################
        # Connections
        ##################################################
        self.connect((self.TPR_0, 0), (self.blocks_multiply_const_vxx_0, 0))    
        self.connect((self.blks2_valve_0, 0), (self.blocks_file_sink_1, 0))    
        self.connect((self.blks2_valve_1, 0), (self.blocks_file_sink_0, 0))    
        self.connect((self.blocks_add_const_vxx_1, 0), (self.blks2_valve_1, 0))    
        self.connect((self.blocks_keep_one_in_n_1, 0), (self.blks2_valve_0, 0))    
        self.connect((self.blocks_keep_one_in_n_1, 0), (self.blocks_multiply_const_vxx_1, 0))    
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.blocks_keep_one_in_n_1, 0))    
        self.connect((self.blocks_multiply_const_vxx_1, 0), (self.blocks_add_const_vxx_1, 0))    
        self.connect((self.iio_fmcomms2_source_0, 0), (self.TPR_0, 0))    


    def get_RF_bandwidth(self):
        return self.RF_bandwidth

    def set_RF_bandwidth(self, RF_bandwidth):
        self.RF_bandwidth = RF_bandwidth
        self.iio_fmcomms2_source_0.set_params(int(self.freq), int(self.sample_rate), self.RF_bandwidth, True, True, True, "manual", 64.0, "manual", 64.0, "A_BALANCED", "", True)

    def get_calib_1(self):
        return self.calib_1

    def set_calib_1(self, calib_1):
        self.calib_1 = calib_1
        self.blocks_multiply_const_vxx_1.set_k((self.calib_1, ))

    def get_calib_2(self):
        return self.calib_2

    def set_calib_2(self, calib_2):
        self.calib_2 = calib_2
        self.blocks_add_const_vxx_1.set_k((self.calib_2, ))

    def get_dc_gain(self):
        return self.dc_gain

    def set_dc_gain(self, dc_gain):
        self.dc_gain = dc_gain
        self.blocks_multiply_const_vxx_0.set_k((self.dc_gain, ))

    def get_fftsize(self):
        return self.fftsize

    def set_fftsize(self, fftsize):
        self.fftsize = fftsize

    def get_frequency(self):
        return self.frequency

    def set_frequency(self, frequency):
        self.frequency = frequency
        self.set_freq(self.frequency)

    def get_integ(self):
        return self.integ

    def set_integ(self, integ):
        self.integ = integ
        self.TPR_0.set_integ(self.integ)

    def get_prefix(self):
        return self.prefix

    def set_prefix(self, prefix):
        self.prefix = prefix
        self.set_tpr_file(self.prefix +  "tpr " + datetime.now().strftime("%Y.%m.%d.%H.%M.%S") + ".dat")
        self.set_tpr_kelvin_file(self.prefix +  "tpr_kelvin " + datetime.now().strftime("%Y.%m.%d.%H.%M.%S") + ".dat")

    def get_sample_rate(self):
        return self.sample_rate

    def set_sample_rate(self, sample_rate):
        self.sample_rate = sample_rate
        self.set_samp_rate(int(self.sample_rate))
        self.iio_fmcomms2_source_0.set_params(int(self.freq), int(self.sample_rate), self.RF_bandwidth, True, True, True, "manual", 64.0, "manual", 64.0, "A_BALANCED", "", True)

    def get_spavg(self):
        return self.spavg

    def set_spavg(self, spavg):
        self.spavg = spavg

    def get_tpr_kelvin_file(self):
        return self.tpr_kelvin_file

    def set_tpr_kelvin_file(self, tpr_kelvin_file):
        self.tpr_kelvin_file = tpr_kelvin_file
        self.blocks_file_sink_0.open(self.tpr_kelvin_file)

    def get_tpr_file(self):
        return self.tpr_file

    def set_tpr_file(self, tpr_file):
        self.tpr_file = tpr_file
        self.blocks_file_sink_1.open(self.tpr_file)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.TPR_0.set_samp_rate(self.samp_rate)

    def get_freq(self):
        return self.freq

    def set_freq(self, freq):
        self.freq = freq
        self.iio_fmcomms2_source_0.set_params(int(self.freq), int(self.sample_rate), self.RF_bandwidth, True, True, True, "manual", 64.0, "manual", 64.0, "A_BALANCED", "", True)

    def get_file_rate(self):
        return self.file_rate

    def set_file_rate(self, file_rate):
        self.file_rate = file_rate
        self.blocks_keep_one_in_n_1.set_n(int(self.det_rate/self.file_rate))

    def get_det_rate(self):
        return self.det_rate

    def set_det_rate(self, det_rate):
        self.det_rate = det_rate
        self.TPR_0.set_det_rate(self.det_rate)
        self.blocks_keep_one_in_n_1.set_n(int(self.det_rate/self.file_rate))


if __name__ == '__main__':
    parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
    parser.add_option("", "--RF-bandwidth", dest="RF_bandwidth", type="intx", default=15000000,
        help="Set RF bandwidth (Hz) [default=%default]")
    parser.add_option("", "--calib-1", dest="calib_1", type="eng_float", default=eng_notation.num_to_str(4.0755e3),
        help="Set Calibration value 1 [default=%default]")
    parser.add_option("", "--calib-2", dest="calib_2", type="eng_float", default=eng_notation.num_to_str(-342.774),
        help="Set Calibration value 2 [default=%default]")
    parser.add_option("", "--fftsize", dest="fftsize", type="intx", default=8192,
        help="Set fftsize [default=%default]")
    parser.add_option("", "--frequency", dest="frequency", type="eng_float", default=eng_notation.num_to_str(1.4125e9),
        help="Set Center Frequency [default=%default]")
    parser.add_option("", "--integ", dest="integ", type="eng_float", default=eng_notation.num_to_str(2),
        help="Set Integration Time (seconds) [default=%default]")
    parser.add_option("", "--prefix", dest="prefix", type="string", default="/home/analog/Documents/sdr_dev/data/",
        help="Set prefix [default=%default]")
    parser.add_option("", "--spavg", dest="spavg", type="intx", default=1,
        help="Set Spectral Averaging (Seconds) [default=%default]")
    (options, args) = parser.parse_args()
    tb = AD9361_TPR_nogui(RF_bandwidth=options.RF_bandwidth, calib_1=options.calib_1, calib_2=options.calib_2, fftsize=options.fftsize, frequency=options.frequency, integ=options.integ, prefix=options.prefix, spavg=options.spavg)
    tb.start()
    try:
        raw_input('Press Enter to quit: ')
    except EOFError:
        pass
    tb.stop()
    tb.wait()
