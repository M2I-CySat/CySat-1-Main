# CySat 1 - Make to Innovate at Iowa State University

**Onboard Computer (OBC) firmware, Satellite Systems, and Ground Station Software** - [CySat M2I](https://m2i.aere.iastate.edu/cysat/)

## Overview

The EnduroSat Onboard Computer Type II (Indicated as Type I on the OBC in the lab) includes the STM32F4x Microcontroller, various communication interfaces, peripheral I/O devices, a prototyping area, and a PC-104 stack connector. EnduroSat provides an Open Software SDK which is included in this repository in addition to the main CySat 1 mission program.

## Environment Setup

1. Dowloand [GitHub Desktop](https://desktop.github.com/) from the web and clone the CySat-1-Main repository within the app
2. Download and install STM32CubeIDE:

- Go to: [https://www.st.com/en/development-tools/stm32cubeide.html](https://www.st.com/en/development-tools/stm32cubeide.html)
- Select "Get Software" for the version that corresponds to your computer’s operating system
- Accept license agreement
- Enter first and last name, email, and check "I have read and understood the Sales Terms & Conditions, Terms of Use and Privacy Policy"
- Click "Download"
- Check email and click "Download now" on email, redirecting you to the website
- It might automatically start downloading. If it doesn’t, select "Get Software" again and it should start downloading

3. Open the installer and follow the installation steps
4. Launch STM32Cube and select a workspace folder that is NOT your GitHub repo location (the default location should be sufficient)
5. Open the cloned repo: `File > Open Project from File System > Directory > *navigate to repo location* > Make sure all folders are selected > Finish`
6. Update Software by going to `Help > Check for Updates`
7. Build the project by clicking the "Hammer" icon on the toolbar at the top
8. Happy Programming!

## Debug/Run Code on the OBC

### The following items are required to debug, run, and deploy a program on the OBC (see CySat inventory)

- Pumpkin Board (or equivalent test bed equipped with a PC-104 stack connector + interface)
- Power Supply + barrell power cable for PSU (the OBC does NOT recieve power via its onboard USB interface, this is used for other purposes)
- ST-LINK v2 In-circuit Debugger + SWD cable (OBC <-> ST-LINK) + Mini-USB cable (ST-LINK <-> Dev computer)
- Jumper wire (enable pin on OBC)
    - To enable, connect pins OBC-En and 3.3V as noted in the EnduroSat OBC User Manual (pages 20-21)
    - This sets the enable bit once so the wire can be removed once the LEDs illuminate
    - **NOTE:** If the OBC is unplugged from/replugged in the ST-LINK or PC, the OBC will need to be disabled/re-enabled. Otherwise the STM IDE will not detect it for some reason
![OBC-Pumpkin-Setup](https://user-images.githubusercontent.com/25646224/196340691-be1189e8-4458-4514-be5e-e68f9b84f2b4.jpg)

### Then, in the STM IDE
1. Ensure you have the OBC_SDK project selected and click the "Run" (play) button, that's it!
2. For troubleshooting purposes, in debug/run configurations -> Debugger tab, make sure the ST-LINK S/N is filled in, the interface is set to SWD, and the reset behavior type is "Connect under reset"

## Documentation

The following documents and datasheets can be referenced in the [CySat 1 box folder](https://iastate.app.box.com/folder/174137892065) under Subsystems -> [Subsystem]

### ADCS - Attitude Determination and Control System

- `CubeADCS - Commissioning Manual [V3.06]` - This document describes the activities related to the commissioning of a CubeSpace ADCS module in flight.
- `CubeADCS - Health Check [V3.15]` - This document will provide the instructions and results of the health check of the CubeADCS unit. The instructions provided must be followed exactly and the observed results must be indicated as is.
- `CubeADCS - ICD [V3.08]` - This document describes the characteristics of the CubeADCS unit as well as the mechanical and electrical interfaces to the bundle. CubeADCS units can be configured with different sensor and actuators for different mission requirements. This document covers all ADCS configurations and readers are to use their discretion in determining which parts are relevant to their various bundle configurations.
- `CubeADCS - Option Sheet (Y-Momentum) [V3.02]\_TGT` - This document include the complete ADCS solution for Y-momentum control. A momentum wheel (in various sizes) is included in the solution and can be mounted either on the bundle or separately from the bundle, depending on the size of the wheel. CubeSpace’s fine sun and nadir sensor module, CubeSense, can also be added to the solution if more accurate attitude control is required.
- `CubeADCS - Reference Manual [V3.10]` - This document serves as source of information to refer to when using the User Manual, Commissioning Manual, and other CubeADCS documentation. The document contains complete listings of TCs and TLMs, communications examples, and other reference examples to assist in designing a system which will interface with the ADCS.
- `CubeADCS - User Manual [V3.07]` - This document provide the user with instructions of completing the initial setup of the CubeADCS unit for the health check, instructions for connecting the unit to an OBC, a guide to mounting sensors inside a satellite and calculating mounting transforms, a functional description of the control modes that the CubeADCS unit is capable of executing, general usage instructions for all major functionality, and an overview of the hardware included in this bundle and their performance.

### COMMS/Antenna

- `Antenna deployment tester Rev 1.2pdf` - User manual designed to detail the antenna deployment tester. Can be useful for developing and testing I2C communications for the UHF antenna.
- `ES_UHF_Module_TestReport_0318TU211104.docx` - Shows the tested values at certain temperature, humidity and power supply voltage.
- `UHF type_II_ESTTC_Protocol_Rev_4.pdf` - This lists the commands we can supply to the uhf to receive the wanted values. Critical importance as they will be used for sending information out the antenna.
- `UHF_Antenna_II_User_Manual_data_interface_Rev_2.3.pdf` - User manual for the antenna specifically I2C communications
- `UHF_Antenna_II_User_Manual_Rev1.4_23112018.pdf` - General overview of the antenna. This lists its features and hardware layout in clear precise ways. (UHF band for communications is 435-438 MHz)
- `UHF_Antenna_III_Datasheet_Rev1.pdf` - Very similar to the user manual listed above. however this is for the Antenna III instead of Antenna II.
- `UHF_type_II_Datasheet_Rev_1.6.pdf` - Lists the features, Morse code is among them, as well as how to test this component on the earth and how it connects to the OBC and antenna.
- `UHF_type_II_UserManual_Rev_1.5_21112018.pdf` - Explains the connections and the features exactly like the previous document. This may just be an older copy of the datasheet above.
- `PuttyLogs` - This is the folder I have listed tests for. Working commands displayed in log files.

### EPS - Electrical Power System

- In this folder is all known versions of EnduroSat EPS documentation.
- Some of the documents contain inaccuracies: discrepancies in the diagrams and tables, but the ones at the root level are mostly correct
- The files in the Flash Drive folder are the ones that came directly from EnduroSat when the EPS was purchased, but there appear to be some inaccuracies here too, especially with the I2C manual
- The folder labelled inaccurate documents is kept for the sake of posterity

#### Testing Folders

- Kept in these folders are various screenshots and logs taken during the testing phases of development
- They are named as best as we could describe them

#### Root Level Files

- These files are assumed to be mostly accurate, although some issues are known
- For example, in the EPS Datasheet, there are some extra pins such as the V_RTC and EPS_Reset which we do not believe we have for our hardware
- In the Manual version 1.1, the LUP 5v says it is on pin H1-48, but we have measured it as swapped with LUP 3.3v. To clarify, LUP 5v is on pin H1-51 and LUP 3.3v is on H1-48
- In the I2C manual version 2, there are diagrams showing hardware we do not have on our device.

### Payload

- Analog Devices Guides contains the documentation for both the PicoZed Dev board and the Carrier Card
- SDR Carrier Board contains files related to the carrier board dev
- SDR Board contains files related to the SDR chip
- SDR Sequence contains developing operation sequence
