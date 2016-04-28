Quirkbot firmware upload in production (v1.2.0)
============

This script generates and uploads a self-test firmware for the Quirkbots. Each firmware contains a unique id that will be stored in the Quirkbot's eeprom.

If the the upload is successful, the id will also be appended to a ```UUIDs.txt```, stored in the root of this repository. After the production cycle, please send us back the ```UUIDs.txt``` so we can have control of every Quirkbot produced.

Upload Instructions
============

#### 01 - Install NodeJS 5.11.0
- [Download NodeJS 5.11.0 (Windows 32)](https://nodejs.org/dist/v5.11.0/node-v5.11.0-x86.msi).
- [Download NodeJS 5.11.0 (Windows 64)](https://nodejs.org/dist/v5.11.0/node-v5.11.0-x64.msi).
- [Download NodeJS 5.11.0 (OSX)](https://nodejs.org/dist/v5.11.0/node-v5.11.0.pkg).

---
#### 02 - Install the Quirkbot Drivers (only required by Windows 7 and 8)
[Download Quirkbot Windows Drivers here](https://github.com/Quirkbot/QuirkbotWindowsDriverInstaller/raw/master/Quirkbot-Windows-Drivers-Installer.exe).

---
#### 03 - Install the Upload script
This may take a few minutes.

```
$ npm install quirkbot-self-test
```
---
#### 04 - Make note of the Quirkbots COM port

Make sure there is a Quirkbot turned ON and connected to the computer. Use the "Device Manager" to discover what is the port it is connected to. On Windows, it should be something like ```COM3```,```COM4```,```COM5```, etc.

---
#### 05 - Run the script
Make sure you are on the same directory where the upload script was installed (see 03 - Install the Upload script).

Run the script, passing the COM port as an argument

Example on Windows:
```
$ node node_modules\quirkbot-self-test\script.js "COM3"
```

Example on Mac:
```
$ node node_modules/quirkbot-self-test/script.js "/dev/cu.usbmodem1411"
```

If the script is executed correctly, you should see an output like this.
```
Using COM port: COM3
Using UUID: 8d19c840e2284315
Compiling and uploading, please wait...
```
The upload process may take a few seconds, so please be patient.

---
#### 06 - Verify for SUCCESS or FAILED
If the upload is successful, you should see the word SUCCESS in the end of the output:
```
Using COM port: COM3
Using UUID: 8d19c840e2284315
Compiling and uploading, please wait...
No device refresh
SUCCESS!
```

If there were any errors during the process, you will see the word FAILED! in the end of the output, followed by the actual error message:
```
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
4 LEDS ON.

### If the test fails:
LEDs blinking.

# UUIDs.txt (very important)

Before starting a production cycle, please make sure to **delete** the ``UUIDs.txt`` file in the root of this repository, if it exists.

After the production cycle, please **send us** the  ``UUIDs.txt`` that was generated. This file contains all the generated ids, and will be used by us to verify original Quirkbots.
