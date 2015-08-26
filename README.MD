Quirkbot firmware upload in production
============

This script generates and uploads a self-test firmware for the Quirkbots. Each firmware contains a unique id that will be stored in the Quirkbot's eeprom.

If the the upload is sucessful, the id will also be apended to a ```UUIDs.txt```, stored in the root of this repository. After the production cycle, please send us back the ```UUIDs.txt``` so we can have control of every Quirkbot produced.

Requirements
============

Before doing anything else, make sure you comply to this list of requirements:

- Download the latest version of [this repository](https://github.com/Quirkbot/QuirkbotSelfTest).
- Install the [Quirkbot Windows Drivers](https://github.com/Quirkbot/QuirkbotWindowsDriverInstaller/releases) (Windows XP, Vista, 7 or 8 - Windows 10 doesn't require drivers).
- Install version 1.6.5 of [Arduino](https://www.arduino.cc/en/Main/Software).
- Install the latest version of  [Node.js](https://nodejs.org/download).
- Install the latest version of the [QuirkbotArduinoLibrary](https://github.com/Quirkbot/QuirkbotArduinoLibrary/releases).
- Install **manually** the latest version of the [QuirkbotArduinoHardware](https://github.com/Quirkbot/QuirkbotArduinoHardware/releases).

**Attention**: *It is very important that you do the manual installation of QuirkbotArduinoHardware. If your directory structure is not like the example below, it will fail!*

```
[sketchbook directory]
	\__ hardware
	|		\__ QuirkbotArduinoHardware
	\__ libraries
			\__ Quirkbot
```

Upload Instructions
============

#### 01 - Clone or download this repository
It's important to keep these files updated, so always before a production cycle, make sure re-download, or re-clone this this repository
```
$ git clone https://github.com/Quirkbot/QuirkbotSelfTest.git
```
---
#### 02 - Make note of where Arduino binary is installed (1)

On a standard Windows 7 (english) installation, it should be:
```
\Program Files\Arduino\arduino_debug.exe
```
---
#### 03 - Make note of the Quirkbots COM port (2)

Make sure there is a Quirkbot turned ON and connected to the computer. Use the "Device Manager" to discover what is the port it is connected to. On Windows, it should be something like ```COM3```,```COM4```,```COM5```, etc.

---
#### 04 - Make note of where Arduino binary is installed
On a standard Windows 7 (english) installation, it should be:
```
\Program Files\Arduino\arduino_debug.exe
```
---
#### 05 - Run the upload script
First, enter in the directory of this repository, eg:
```
$ cd QuirkbotSelfTest
```
Then run the script, passing the Arduino path(1) as the first argument, and the COM port(2) as the second, eg:
```
$ node script.js "\Program Files\Arduino\arduino_debug.exe" "COM3"
```

If the script ix executed correctly, you should see an out put like this.
```
Using Arduino: \Program Files\Arduino\arduino_debug.exe
Using COM port: COM3
Using UUID: 8d19c840e2284315
Compiling and uploading, please wait...
```
The upload process may take a few seconds, so please be patient. The Quirkbot leds should indicate when it goes into bootloader mode (mouth leds pulsating), and when the upload is completed (all leds will blink quickly counter clockwise).

---
#### 06 - Verify for SUCCESS or FAILED
If there were any errors during the process, you will see the word FAILED! in the end of the output, followed byt the actual error message:
```
Using Arduino: \Program Files\Arduino\arduino_debug.exe
Using COM port: COM3
Using UUID: 8d19c840e2284315
Compiling and uploading, please wait...
FAILED! /bin/sh: \Program Files\Arduino\arduino_debug.exe: command not found
```

If the upload is successful, you should see the word SUCCESS in the end of the output:
```
Using Arduino: \Program Files\Arduino\arduino_debug.exe
Using COM port: COM3
Using UUID: 8d19c840e2284315
Compiling and uploading, please wait...
SUCCESS!
```

---
#### 07 - Execute self test

If the upload was SUCCESSful, the Quirkbot is ready to be placed in the test jig.

**If the test fails**: all the 4 Leds will blink, for 4 seconds, with a 200 milliseconds interval between the blinks.

**If the test succeds**: only the eyes Leds will pulsate (smoothly, like it is breathing)
