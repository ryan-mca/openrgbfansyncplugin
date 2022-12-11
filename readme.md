# THIS PROJECT IS NOW PART OF THE OPENRGBDEVELOPERS GROUP AND HAS A NEW URL: [OpenRGBDevelopers/OpenRGBFanSyncPlugin](https://gitlab.com/OpenRGBDevelopers/OpenRGBFanSyncPlugin)


# OpenRGBFanSyncPlugin

[![pipeline status](https://gitlab.com/ShadyNawara/openrgbfansyncplugin/badges/master/pipeline.svg)](https://gitlab.com/ShadyNawara/openrgbfansyncplugin/-/commits/master)

Fan sync control is an [OpenRGB](https://gitlab.com/CalcProgrammer1/OpenRGB) plugin that allows you to set fan speed curves for onboard fans using custom functions and hardware sensor input (cpu temp, gpu clock, etc.)

This plugin was inspired by [OpenRGBHardwareSyncPlugin](https://gitlab.com/OpenRGBDevelopers/OpenRGBHardwareSyncPlugin) and similarly it uses [LibreHardwareMonitor](https://github.com/LibreHardwareMonitor/LibreHardwareMonitor) to read hardware sensors and control onboard fans.

## Downloads

* [Windows 64](https://gitlab.com/ShadyNawara/openrgbfansyncplugin/-/jobs/artifacts/master/download?job=Windows%2064)

## Install

Download and place [lhwm-wrapper.dll](https://gitlab.com/OpenRGBDevelopers/lhwm-wrapper/-/jobs/artifacts/master/download?job=main_build)  ([source](https://gitlab.com/OpenRGBDevelopers/lhwm-wrapper)) in the directory of `OpenRGB.exe`.

Notice: some of the hardware measures may require that you run OpenRGB as admin.

This version of the of the lhwm-wrapper is compatible with [OpenRGBHardwareSyncPlugin](https://gitlab.com/OpenRGBDevelopers/OpenRGBHardwareSyncPlugin) so only this file is needed if using both plugins.

## Linux Support
Linux support has been added using lm-sensors but I wasn't able to fully test it so I disabled the pipeline build, if you want to try it you can download and compile the code manually.

## Usage & Features
* After installing the plugin go to the Hardware Fan Sync tab in OpenRGB
* List of available fans/controls is visible on the left, right-click tab to hide unused fans
* Sensor dropdown shows all available hardware sensors, select sensor then click "Add" to insert the sensor identifier into the expression box
* The function expression box accepts most mathmatical operations. see screenshot below and [ExprTk](http://www.partow.net/programming/exprtk/index.html#simpleexample01) for refrence
* Set the desired Min/Max value for the expression
* "Result" shows the result of the expression, "Normalized Result" shows the result normalized between 0-100 using the min/max values. The Normalized result is what is used to interpolate the final fan speed
* Fan Curve allows you to set a custom speed curve based on the normalized result of the expression function, double click anywhere to add new points, drag points to change the curve, and double click existing points to remove them
* 5 Fan Presets can be used as shortcuts to set predefined curves (Presets are similar to Asus's Q-Fan Control utility)
* Save button saves the current configuration for the selected control to make it presistant across app launces
* Reset button restores original default (Does not save automatically)

## Screenshot
![Screenshot](https://gitlab.com/ShadyNawara/openrgbfansyncplugin/-/raw/master/Screenshot.png)
