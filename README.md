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

## Wi-Fi and config set up (Ubuntu)
1. Go onto a Linux VM, make sure you allow it to read an external drive
2. run this command to see the card 
```shell
lsblk
```
3. Mount the Linux partition
```shell
mkdir /mnt/ubuntu
sudo mount /dev/sda2 /mnt/ubuntu
```
4. Go to the network config folder for the Ubuntu cloud here
```shell
/mnt/ubuntu/etc/netplan
```
- I only had an eth0.yaml, so I created a wireless.yaml
5. create a wireless.yaml and fill it with your wifi information
```shell
network:
  version: 2
  renderer: networkd
  wifis:
    {interface}: 
      dhcp4: true
      optional: true
      access-points:
        "YourNetworkName":
          password: "YourPassword"
```
- wlan0 is default usually it can be wrong, boot the Le Potato for a few minutes and mount it to your VM again and check the journal file to get the interface name
6. Now you need to install and enable SSH for the device, default user and password is both "root"
```shell
sudo chroot /mnt/ubuntu /bin/bash
```
-  the above command will change the root, treating the folder it's moving to as a root directory, meaning the Le Potato's Ubuntu system you mounted is the new root
- also opens a bash shell allowing you to run commands inside the Ubuntu file system while on your Kali VM, then able to boot
```shell
echo "nameserver 8.8.8.8" > /etc/resolv.conf
apt install openssh-server
systemctl enable ssh
ssh-keygen -A # generate hostkeys
exit
```
- installs ssh and enables it
7. after giving the device time to boot, on your mac run
```shell
arp -a
```
- find the IP and ssh into it

8. install Git and connect your GitHub with a PAT


# Notes/commands for future

```shell
ldto list
```
- list represents every pre-built device tree with the wiring of the board, corresponds with specific hardware or pin config
- w1-gpio configures a GPIO pin as a 1-Wire bus. 

```shell
sudo ldto enable w1-gpio
```
- enable to test


```shell
/sys/bus/w1/devices/28-000000ba3148# ls
alarms     eeprom_cmd  hwmon  power       temperature
conv_time  ext_power   id     resolution  uevent
driver     features    name   subsystem   w1_slave
```
- the filepath, w1_slave is a virtual file that will produce a temperature reading everytime it's opened
- temperature file gives just a number, same as the w1_slave file giving a t=temp to the millidegree, no CEC check to validate