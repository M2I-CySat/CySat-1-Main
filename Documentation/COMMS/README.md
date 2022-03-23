Antenna deployment tester Rev 1.2pdf
--User manual designed to detail the antenna deployment tester. 
--Can be useful for developing and testing I2C communications for the UHF antenna.

ES_UHF_Module_TestReport_0318TU211104.docx
-- Shows the tested values at certain temperature, humidity and power supply voltage.

UHF type_II_ESTTC_Protocol_Rev_4.pdf
-- This lists the commands we can supply to the uhf to receive the wanted values.
-- Critical importance as they will be used for sending information out the antenna.

UHF_Antenna_II_User_Manual_data_interface_Rev_2.3.pdf
-- User manual for the antenna specifically I2C communications


UHF_Antenna_II_User_Manual_Rev1.4_23112018.pdf
-- General overview of the antenna. This lists its features and hardware layout
-- in clear precise ways. (UHF band for communications is 435-438 MHz)

UHF_Antenna_III_Datasheet_Rev1.pdf
-- very similar to the user manual listed above. however this is for the Antenna III instead of Antenna II.

UHF_type_II_Datasheet_Rev_1.6.pdf
-- Lists the features, Morse code is among them, as well as how to test this component
-- on the earth and how it connects to the OBC and antenna.

UHF_type_II_UserManual_Rev_1.5_21112018.pdf
--Explains the connections and the features exactly like the previous document.
-- this may just be an older copy of the datasheet above.

PuttyLogs
-- This is the folder I have listed tests for. Working commands displayed in 
-- log files. 

# Answered Questions (According to EnduroSat on 3/16/2020)
* "When transmitting on the Transceiver in transparent mode, you send to the UART line and the module automatically wraps it in the Radio Packet Structure (with preamble, etc.)." - **Correct**
* "When receiving on the Transceiver in transparent mode, the packet must already be wrapped in this same Radio Packet Structure." - **Correct**
* "When transmitting a beacon, it will use the AX.25 protocol." - **Yes the data you send on the UART line via the AX.25 beacon set command will be encoded in AX.25.**
* "Question 1: Will this also be automatically wrapped in the Radio Packet Structure?" - **Correct, this is also how it is depicted in the updated AX.25 image that Ivan sent to you.**
* "Question 2: Is there anyway to receive on the Transceiver other than in transparent mode?" - **The transceiver will receive only valid AX.25 frames or valid ESTTC commands when not in transparent mode. Other packets will be received but since they are not valid AX.25 frames or ESTTC commands they will be discarded. Apart from these you will have to use transparent mode to pass data back and forth through the transceiver.**
* "Question 3: Is there any time that the Radio Packet Structure is not used?" - **Yes, when the audio beacon (morse code + melody) is being transmitted.** 