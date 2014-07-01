rsx_steering_remote
===================

code behind steering wheel mounted stereo controls - re purposes the original cruise control buttons

###Background Info

I pulled the radio out of my 2002 Acura RSX Type-S and replaced it with a touchscreen computer running some custom mp3 player software. Touch screens are nice but sometimes its good to have physical buttons, espcially when you are moving at 65+mph in a tiny metal shell (a car). This code is part of a project that added physical volume control and play/pause buttons to control my mp3 player. It runs on a sparkfun arduino pro micro (5V) mounted in my dashboard. Full project write up available on my blog - [Electric Campfire](http://electriccampfire.info/2014/06/12/steering-wheel-stereo-controls/)

Info on MP3 Player project [is on my blog](http://electriccampfire.info/2013/12/08/rsx-in-dash-computer/)
Git Repo for MP3 Player software is on [GitHub](https://github.com/nickmyku/RSX_MP3_Player)

###Function

This code reads the ADC (A3) and compares the value to 6 potential voltage values, if the value is within a error range (+/-15) it outputs the corresponding signal. The output signal is either a 2-bit binary code simulating the original cruise control buttons or a keyboard press which controls the mp3 player/radio.

###Expected Analog Voltages

Several resistor voltage divider were transplanted into the original cruise control switch, this is a chart of the voltages each button can ouput and the codes behavior when that voltage is detected. When no switches are pressed a voltage of 5.00 is read on the ADC and no additional code is executed

| Voltage | Function Name | Behavior |
|:-------:|:-------------:|:---------|
| 0.57 | End Program | Sends ESC key to computer, terminates mp3 player program |
| 0.40 | Volume Up | Sends UP key to computer |
| 0.78 | Skip/Play | Short press sends RIGHT key to computer, long press sends SPACE key to computer |
| 1.46 | Volume Down | Sends DOWN key to computer |
| 2.08 | Resume/Accel | Sends 01 code to cruise control module (arduino output inverted by level shifter) |
| 3.00 | Cancel | Sends 11 code to cruise control module (arduino output inverted by level shifter) |
| 3.85 | Set/Decel | Sends 10 code to cruise control module (arduino ouput inverted by level shifter) |

###Wiring Info

This is a list of all connections made to the arduino and a basic description of thier purpose

| Arduino Pin | Description |
|:-----------:|:------------|
| A3 | Analog input from modified cruise control switch mounted on steering wheel |
| 8 | bit0 of 2-bit cruise control module input (pin 6 on cruise control connector) |
| 16 | bit1 of 2-bit cruise control module input (pin 7 on cruise control connector) |
| USB | Arduino emulates a USB keyboard and sends key presses back to the computer through the USB connection |
