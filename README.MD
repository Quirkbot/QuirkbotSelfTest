Quirkbot firmware upload in production
============

This script generates and uploads a self-test firmware for the Quirkbots. Each firmware contains a unique id that will be stored in the Quirkbot's eeprom.

If the the upload is successful, the id will also be appended to a ```UUIDs.txt```, stored in the root of this repository. After the production cycle, please send us back the ```UUIDs.txt``` so we can have control of every Quirkbot produced.

Requirements
============

Before doing anything else, make sure you comply to this list of requirements:

- Download version ```0.0.3```of [this repository](https://github.com/Quirkbot/QuirkbotSelfTest).
	- *You can verify version in the ```version.txt``` file*


- Install the [Quirkbot Windows Drivers](https://github.com/Quirkbot/QuirkbotWindowsDriverInstaller/releases)
	- *Windows XP, Vista, 7 or 8 - Windows 10 doesn't require drivers.*


- Install version ```1.6.5``` of [Arduino](https://www.arduino.cc/en/Main/Software).


- Install the latest version of  [Node.js](https://nodejs.org/download).


- Install version ```0.4.3``` [QuirkbotArduinoLibrary](https://github.com/Quirkbot/QuirkbotArduinoLibrary/releases).
	- *You can verify version in the ```library.properties``` file.*


- Install **manually** version ```0.0.13``` of the [QuirkbotArduinoHardware](https://github.com/Quirkbot/QuirkbotArduinoHardware/releases).
	- *You can verify version in the ```version.txt``` file*


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
#### 04 - Run the upload script
First, enter in the directory of this repository, eg:
```
$ cd QuirkbotSelfTest
```
Then run the script, passing the Arduino path(1) as the first argument, and the COM port(2) as the second, eg:
```
$ node script.js "\Program Files\Arduino\arduino_debug.exe" "COM3"
```

If the script is executed correctly, you should see an output like this.
```
Using Arduino: \Program Files\Arduino\arduino_debug.exe
Using COM port: COM3
Using UUID: 8d19c840e2284315
Compiling and uploading, please wait...
```
The upload process may take a few seconds, so please be patient.

---
#### 05 - Verify for SUCCESS or FAILED
If the upload is successful, you should see the word SUCCESS in the end of the output:
```
Using Arduino: \Program Files\Arduino\arduino_debug.exe
Using COM port: COM3
Using UUID: 8d19c840e2284315
Compiling and uploading, please wait...
SUCCESS!
```

If there were any errors during the process, you will see the word FAILED! in the end of the output, followed by the actual error message:
```
Using Arduino: \Program Files\Arduino\arduino_debug.exe
Using COM port: COM3
Using UUID: 8d19c840e2284315
Compiling and uploading, please wait...
FAILED! /bin/sh: \Program Files\Arduino\arduino_debug.exe: command not found
```



---
# Testing on the test jig

If the upload was SUCCESSful, the Quirkbot is ready to be tested in the test jig.

Place the Quirkbot in the jig, turn it on, and observe the feedback from the onboard LEDs.

### If the test passes:
Only the 2 "eyes" LEDs will pulsate smoothly (like it is "breathing").

![Success](/reference/success.gif?raw=true "Success")

### If the test fails:
All the 4 "eyes and mouth" LEDs will blink (not smoothly, just turning ON/OFF), for 4 seconds, with a 200 milliseconds interval between the blinks. After 4 seconds, only the "eyes" will pulsate, exactly like when the test passes, so **make sure to be looking to the Quirkbot during the first seconds**.

![Fail](/reference/fail.gif?raw=true "Fail")

**Attention:** *Before displaying the results of the test The Quirkbot LEDs will display the "start sequence". All LEDs will blink quickly, one by one, spinning in a clockwise fashion, finishing by the 2 eyes blinking quickly simultaneously. This is should not be confused with a failed test!*
