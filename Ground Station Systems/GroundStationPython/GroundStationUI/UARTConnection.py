from threading import Thread
import threading
import ctypes
import logging
import importlib
import binascii

packet_protocol_module = importlib.import_module('CySatPacket')
global_module = importlib.import_module('CySatGlobal')

BEACON = global_module.BEACON
sys_dictionary = global_module.sys_dictionary
cmd_dictionary = global_module.cmd_dictionary

# Writing to UART Port
def gs_uart_write(uart, byte_array):
    try:
        num_bytes = uart.write(byte_array)
        print("TX(" + str(num_bytes) + "): " + str(binascii.hexlify(bytearray(byte_array))))
    except Exception as e:
        print(e)
        print("UART Write Error (Port probably not connected or configured properly)")

class uart_reader(Thread):
    def __init__(self, uart_source, logger):
        Thread.__init__(self)
        self.uart_source = uart_source
        self.logger = logger

    def run(self):

        # Maybe have it always look for beacon and once seen starts a 10(?)min window where it then looks
        # for packets only and after the window ends will go back to only look for beacon.
        
        while True:

            # sample 1 byte from the buffer
            byte = self.uart_source.read(1)

            # check if byte is empty or not
            if(len(byte) > 0):

                # check if byte matches start of packet value
                if(ord(byte) == 0xFF):

                    # capture packet bytes
                    packet_bytes = packet_protocol_module.capture_packet_bytes(self.uart_source)
                    print(repr(packet_bytes))
                    
                    # parse packet bytes
                    packet_data = packet_protocol_module.parse_packet(packet_bytes)

                    # handle the packet information
                    packet_protocol_module.handle_packet(packet_data, self.logger, sys_dictionary, cmd_dictionary)
                    
                elif(ord(byte) == ord(BEACON[0])):  # packets start with "cysat>W0ISU:"

                    possible_beacon = bytearray(byte)

                    # check if CySat Beacon
                    bytes = self.uart_source.read(len(BEACON) - 1)  # one less for the character we already read
                    # bytes = self.uart_source.read(12 - 1 + 13)  # 12 for the header, minus the one we already read, plus 13 for "Hello, World!"
                    
                    for i in bytes:
                        possible_beacon.append(i)

                    # check if beacon received and reply accordingly
                    if(possible_beacon.decode('utf-8') == global_module.BEACON):
                        self.logger.log(logging.WARNING, "CySat-1 Beacon Detected: " + possible_beacon.decode('utf-8'))
                        payload_bytearray = bytearray()
                        packet = packet_protocol_module.create_packet(global_module.OBC, 0x03, payload_bytearray)
                        packet_bytes = packet_protocol_module.package_packet(packet)
                        gs_uart_write(self.uart_source, packet_bytes)
                        self.logger.log_packet_sent(packet, payload_bytearray)

    def get_id(self): 
        # returns id of the respective thread 
        if hasattr(self, '_thread_id'): 
            return self._thread_id 
        for id, thread in threading._active.items(): 
            if thread is self: 
                return id

    def raise_exception(self): 
        thread_id = self.get_id() 
        res = ctypes.pythonapi.PyThreadState_SetAsyncExc(thread_id, ctypes.py_object(SystemExit)) 
        if res > 1: 
            ctypes.pythonapi.PyThreadState_SetAsyncExc(thread_id, 0) 
            print('Exception raise failure')