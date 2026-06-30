# HomeTempReader
Instructions and scrips for home temperature reader, for fermentation

# Hardware
- 3 Female jumping wires
- 1 DS18B29 temperature sensor
- 1 AML-S905X-CC (Le Potato); or any microcomputer
- 1 4.7K Ohm resistor
- 1 MicroSD card (adaptor if needed)
- Wi-Fi capabilities for the Le Potato if needed 

## Soldering Steps
1. Strip the temperature reader wire and expose the black (ground), red (power), and yellow (data) wires
2. strip one end of the three female jumping wires and expose 1/2 inch of wire
3. Solder at 525 degrees F the red wire to the jumping wire and one end of the resistor 
4. solder the yellow, jumping wire, and other end of the resistor together
5. solder a jumping wire to the black ground wire 
6. tape all exposed wire 


## Le Potato OS set up 
1. Open a terminal and use this command to find the DS card identifier
```shell 
diskutil list
```
2. unmount the disk with the following command
```shell
diskutil unmountDisk /dev/{identifier}
```
- My example was called disk4s1, s1 means it only grabbed a single partition, so I used disk4
3. Flash the OS image to the external SD card with this command
```shell
sudo dd if=/path/to/image.img of=/dev/rdisk2 bs=4m status=progress
```
- dd -> copies raw data byte to byte, a built in MacOS tool
- if = input file
- of = output file
- bs = block size, makes it read 4MB at a time not byte by byte to speed up process
- status = progress - > shows live progress in the terminal

