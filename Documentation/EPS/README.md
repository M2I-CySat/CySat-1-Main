# EPS documentation
* In this folder is all known versions of EnduroSat EPS documentation.
* Some of the documents contain inaccuracies, but the ones at the root level are mostly correct
* The files in the Flash Drive folder are the ones that came directly from EnduroSat when the EPS was purchased, but there appear to be some inaccuracies here too, especially with the I2C manual
* The folder labelled inaccurate documents is kept for the sake of posterity

# Testing Folders
* Kept in these folders are various screenshots and logs taken during the testing phases of development
* They are named as best as we could describe them

# Inaccurate Documents
* These are inaccurate due to discrepancies in the diagrams and tables

# Root Level Files
* These files are assumed to be mostly accurate, although some issues are known
* For example, in the EPS Datasheet, there are some extra pins such as the V_RTC and EPS_Reset which we do not believe we have for our hardware
* In the Manual version 1.1, the LUP 5v says it is on pin H1-48, but we have measured it as swapped with LUP 3.3v. To clarify, LUP 5v is on pin H1-51 and LUP 3.3v is on H1-48
* In the I2C manual version 2, there are diagrams showing hardware we do not have on our device.