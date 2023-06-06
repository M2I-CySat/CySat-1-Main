import time
import serial

msg = "Hello World"
print(msg)

# This program, Middleman, acts as a middleman between the OBC and the GNU Radio SDR program, prompting it to run and trasnferring data.

# This program is designed to run upon Payload Linux startup, probably once compiled as its own .exe file or whatever Linux uses

# By the CySat-1 Team

# Contact Steven Scheuermann for help, ultimatesteve99@gmail.com




# Waits a certain amount of time for system to warm up, ask Matt

# Checks if take measurement at startup is true or false

# If true, take measurement, if false, skip and go to additional commands. This might change to needing OBC prompt to take measurement.

# Waits for measurment to be taken, until OBC responds with send me the measurement

# ---> Sends first file

# Waits for OBC response

# If error, try resending, but if 2 errors, send abort message and delete measurement probably

# If success, delete measurement, step not needed if GNU radio does this automatically, unsure if overwrite deletes or overwrites

# ---> Sends second file

# Waits for OBC response

# If error, try resending, but if 2 errors, send abort message and delete measurement probably

# If success, delete measurement, step not needed if GNU radio does this automatically, unsure if overwrite deletes or overwrites

# Send "All done" message to the OBC

# If take measurement at startup is disabled, skip to here

# Wait for OBC to send message for changing SDR settings

# After settings change, send "All done" message to OBC

# After any All Done, the OBC may decide to turn the Payload off, so the program should be in a stable configuration at that point