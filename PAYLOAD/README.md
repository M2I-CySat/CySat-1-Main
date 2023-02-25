SDR Board contains files related to the SDR chip and AD9361
SDR Carrier Board contians files related to the dev carrier board

To run program on boot-up, I followed the following steps:
1. sudo nano /etc/rc.local
2. added the line "sudo python /home/analog/Documents/sdr_dev/startup_radiometer_app.py &" to the end of the file before "exit 0"
3. sudo reboot
