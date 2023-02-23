README: Release 29 January 2018

Files from: https://wiki.analog.com/resources/tools-software/linux-software/zynq_images

Board: zynq-adrv9361-z7035-fmc

BOOT.BIN must be updated when a new bitstream is created for FPGA. Do this via build_boot_bin.sh, provide new .hdf file.

Extract zynq-adrv9361-z7035-fmc/bootgen_sysfiles from .tgz using tar to expose previous u-boot.elf.

See https://wiki.analog.com/resources/tools-software/linux-software/build-the-zynq-boot-image for build_boot_bin.sh.

BUILD ON 'linux-3.ece.iastate.edu'

run `source /remote/Xilinx/2018.2/Vivado/2018.2/settings64.sh'

run 'chmod +x build_boot_bin.sh' then `./build_boot_bin.sh <path to your design_1_wrapper.hdf here> zynq-adrv9361-z7035-fmc/u-boot-zed.elf new_bootgen_sysfiles`

Move output_boot_bin/BOOT.bin to root of directory.
