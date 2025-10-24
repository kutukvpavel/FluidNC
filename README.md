This fork adds the following functionality to FluidNC:
 - Backlash compensation (ported from RabbidGRBL and pull-request #24) - does not work yet;
 - "Hybrid" jogging, i.e. jogging enables only the motors required for motion. This is useful for "hybrid" operation of the machine, when FluidNC jogging is used to achieve automatic feed on a subset of axes, while an operator is manually controlling the left over axis/axes;
 - A setting to enable hard stop on feed hold and jog cancel (reverses commit 218cb7f). This is useful for "hybrid" operation of the machine, when FluidNC jogging is used to achieve automatic feed. When this setting is on, jog can be stopped precisely.

Corresponding new configuration parameters are:
 - TBD
 - Axis/hybrid_jogging (bool)
 - Axis/feed_hold_hard_stop (bool) and Axis/jog_cancel_hard_stop (bool)

<img src="https://github.com/bdring/FluidNC/wiki/images/logos/FluidNC.svg" width="600">

## Introduction

**FluidNC** is a CNC firmware optimized for the ESP32 controller. It is the next generation of firmware from the creators of Grbl_ESP32. It includes a web based UI and the flexibility to operate a wide variety of machine types. This includes the ability to control machines with multiple tool types such as laser plus spindle or a tool changer.  

## Firmware Architecture

- Object-Oriented hierarchical design
- Hardware abstraction for machine features like spindles, motors, and stepper drivers
- Extensible - Adding new features is much easier for the firmware as well as gcode senders.

## Machine Definition Method

There is no need to compile the firmware. You use an installation script to upload the latest release of the firmware and then create [config file](http://wiki.fluidnc.com/en/config/overview) text file that describes your machine.  That file is uploaded to the FLASH on the ESP32 using the USB/Serial port or WIFI.

You can have multiple config files stored on the ESP32. The default is config.yaml, but you can change that with [**$Config/Filename=<myOtherConfig.yaml>**](http://wiki.fluidnc.com/en/features/commands_and_settings#config_filename)

## Basic Grbl Compatibility

The intent is to maintain as much Grbl compatibility as possible. It is 100% compatible with the day to day operations of running gcode with a sender, so there is no change to the Grbl gcode send/response protocol, and all Grbl gcode are supported. Most of the $ settings have been replaced with easily readable items in the config file.


## WebUI

FluidNC includes a built-in browser-based Web UI (Esp32_WebUI) so you control the machine from a PC, phone, or tablet on the same Wifi network.

## Wiki

[Check out the wiki](http://wiki.fluidnc.com) if you want the learn more about the feature or how to use it.

## Credits

The original [Grbl](https://github.com/gnea/grbl) is an awesome project by Sungeon (Sonny) Jeon. I have known him for many years and he is always very helpful. I have used Grbl on many projects.

The Wifi and WebUI is based on [this project.](https://github.com/luc-github/ESP3D-WEBUI)  

## Discussion

<img src="http://wiki.fluidnc.com/discord-logo_trans.png" width="180">

We have a Discord server for the development this project. Ask for an invite


## Donations

This project requires a lot of work and often expensive items for testing. Please consider a safe, secure and highly appreciated donation via the PayPal link below or via the GitHub sponsor link at the top of the page.

[![](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/donate/?hosted_button_id=8DYLB6ZYYDG7Y)
