import logging
import importlib

packet_protocol_module = importlib.import_module('CySatPacket')
global_module = importlib.import_module('CySatGlobal')

BEACON = global_module.BEACON
sys_dictionary = global_module.sys_dictionary
cmd_dictionary = global_module.cmd_dictionary

class QueueHandler(logging.Handler):
    """Class to send logging records to a queue
    It can be used from different threads
    The ConsoleUi class polls this queue to display records in a ScrolledText widget
    """

    # Example: https://gist.github.com/moshekaplan/c425f861de7bbf28ef06
    # (https://stackoverflow.com/questions/13318742/python-logging-to-tkinter-text-widget) is not thread safe!
    # See https://stackoverflow.com/questions/43909849/tkinter-python-crashes-on-new-thread-trying-to-log-on-main-thread

    def __init__(self, log_queue):
        super().__init__()
        self.log_queue = log_queue

    def emit(self, record):
        self.log_queue.put(record)

# A class to extend logging functionality
class CySatLogger(logging.Logger):
    def __init__(self, logger_name):
        super().__init__(logger_name)

    # Log properly from received packet
    def log_packet_received(self, packet, payload_str, sys_dictionary, cmd_dictionary):
        receive_str = "RX Packet - Type ID:" + hex(packet.type).upper() + "(" + sys_dictionary[packet.type] + ") "
        receive_str += "CMD ID:" + hex(packet.cmd).upper() + "(" + cmd_dictionary[packet.type][packet.cmd].cmd_description + ")"

    # Check if data should be printed
        if(len(payload_str) > 0):
            self.log(logging.INFO, receive_str +  "  Data: " + payload_str)
        else:
            self.log(logging.INFO,  receive_str)
        
    # Log properly from sent packet
    def log_packet_sent(self, packet, payload, sys_dictionary, cmd_dictionary):
        send_str = "TX Packet - Type ID:" + hex(packet.type).upper() + "(" + sys_dictionary[packet.type] + ") "
        send_str += "CMD ID:" + hex(packet.cmd).upper() + "(" + cmd_dictionary[packet.type][packet.cmd].cmd_description + ")"

        # check if data should be printed
        if(len(payload) > 0):
            self.log(logging.INFO, send_str +  "  Data: " + payload.decode('utf-8'))
        else:
            self.log(logging.INFO,  send_str)