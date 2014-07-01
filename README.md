rsx_steering_remote
===================

code behind steering wheel mounted stereo controls - re purposes the original cruise control buttons



This code reads the ADC (A3) and compares the value to 6 potential voltage values, if the value is within a error range (+/-15) it outputs the corresponding signal. The output signal is either a 2-bit binary code simulating the original cruise control buttons or a keyboard press which controls the mp3 player/radio.
