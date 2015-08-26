Quirkbot firmware upload
============

This script generates and uploads a self-test firmware for the Quirkbots. Each firmware contains a unique id that will be stored in the Quirkbot's eeprom.

If the the upload is sucessful, the id will also be apended to a ```UUIDs.txt```, stored in the root of this repository. After the production cycle, please send us back the ```UUIDs.txt``` so we can have control of every Quirkbot produced.

Requirements
============

 Before doing anything else, make sure you comply to this list of requirements:

 - Download the latest version of [this repository](https://github.com/Quirkbot/QuirkbotSelfTest).
 - Install version 1.6.5 of [Arduino](https://www.arduino.cc/en/Main/Software).
 - Install the latest version of  [Node.js](https://nodejs.org/download).
 - Install the latest version of the [QuirkbotArduinoLibrary](https://github.com/Quirkbot/QuirkbotArduinoLibrary/releases).
 - Install **manually** the latest version of the [QuirkbotArduinoHardware](https://github.com/Quirkbot/QuirkbotArduinoHardware/releases).
 

 Attention: *It is very important that you do the manual installation of QuirkbotArduinoHardware. If your directory structure is not like the example below, it will fail!*

 ```
 	sketchbook directory]
    	\---hardware
        	\---QuirkbotArduinoHardware
 ```



 Instructions
 ============
