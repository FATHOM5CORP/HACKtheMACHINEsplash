# SPLASH

This repository contains source code for operating a series of LED lights attached to a single board computer. Also attached to the computer is a set of resistor pads that, when pressed, will change the pattern or color of the LED strip. 

The binary called simply 'splash' is a compiled copy of splash.c compiled alongside gpio.c, which is a library of commands for operating the pins on the computer.

The binaries called 'splash2' through 'splash6' are hardened copies of 'splash'.

The goal of the game is to use a return oriented attack against one or more of the binaries to get a command shell. Once a shell has been popped, issue the command to turn the rotating light on:
```
echo 1 > /sys/class/gpio/gpio27/value
```
Turning the light on through a shell returned from a ROP attack is the only way to earn points in the splash game. 

To start one of the binaires, ssh into the chosen board using the appropriate login credentials for that board. Once logged in start the binary by running:
```
sudo ./splash3
```
for example to start splash3. You will not be prompted for a password. Player accounts have root permissions to start and stop the chosen binaries, but no other root acces. Once started the binary will not put anything to the terminal, but will listen on stdin. If you pass a 2 you will see the name of a variable echoed to the terminal. 
```
$ 2
$ splash
```
You can set this variable to whatever string value you choose by passing in a 1 followed by a space and the value that you want to set the variable to. 
```
$ 2
$ splash
$ 1 newvalue
$ 2
$ newvalue
````