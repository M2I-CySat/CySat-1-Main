# Empty Dictionaries for CMDs
cmd_dictionary = {}
sys_dictionary = {}
sys_list = []

# Global Packet Variables
OBC = 0x0A
ADCS = 0x14
EPS = 0x1E
SDR = 0x28
EOL = 0x5A

# Beacon constant
BEACON = "cysat>W0ISU:Hello, Earth! I am ISU's CySat-I"

# Debug status
DEBUG = 1

# Related to Command_Manifest
def populate_global_variables():
    grab_command_manifest()
    for key, value in sys_dictionary.items():
        sys_list.append(value)

def grab_command_manifest():
    current_id = 0
    # Opens text document where commands are stored
    with open("command_manifest.txt") as f:
        # Create Command Type List
        for line in f:
            # "#" establishes command type apart of the packet protocol
            if(line.startswith('#')):
                # Add new type to sys_dictionary
                line = line[line.startswith('#'):]
                (id, desc) = line.split(':')
                sys_dictionary[int(id)] = desc.strip('\n')
                # Seed new system type dictionary in cmd_dictionary
                current_id = id
                cmd_dictionary[int(current_id)] = {}
                
            elif(not line.startswith('\n') and not line.startswith('-')):
                # Create new command object with file parameters
                line_list = line.split(':')
                cmd = Command(line_list[0], str(line_list[1]), int(line_list[2]), int(line_list[3].strip('\n')))
                cmd_dictionary[int(current_id)][int(line_list[0])] = cmd

class Command:
    def __init__(self, id, desc, has_payload, sendable):
        self.cmd_id = id
        self.cmd_description = desc
        self.cmd_has_payload = has_payload
        self.cmd_sendable = sendable

    def __str__(self):
        return str(self.cmd_description) + " " + str(self.cmd_has_payload)

    #def __str__(self):
    #    return "Command: ID: %s, Description: %s, Has Payload: %s, Send-able: %s" % (self.cmd_id, self.cmd_description, self.cmd_has_payload, self.cmd_sendable)