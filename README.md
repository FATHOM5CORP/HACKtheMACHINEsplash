# SPLASH

This repository contains source code for operating a series of LED lights attached to a single board computer. Also attached to the computer is a set of resistor pads that, when pressed, will change the pattern or color of the LED strip. 

The binary called simply 'splash' is a compiled copy of splash.c compiled alongside gpio.c, which is a library of commands for operating the pins on the computer.

The binaries called 'splash2' through 'splash6' are diversified copies of 'splash'.

The goal of the game is to use a return oriented attack against one of the binaries to get a command shell. Once a shell has been popped, issue the command to turn the rotating light on:
```
echo 1 > /sys/class/gpio/gpio27/value
```
Turning the light on through a shell returned from a ROP attack is the only way to earn points in the splash game. 