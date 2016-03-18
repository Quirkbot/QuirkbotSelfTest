var uuid = require('node-uuid');
var fs = require('fs');
var execSync = require('child_process').execSync;
var path = require('path');
var utils = require('./utils');
var endOfLine = require('os').EOL;

var comPort = process.argv[2];

if(!comPort){
	console.log('ERROR: You need to inform the COM port!');
	console.log('\nEXAMPLE WINDOWS:')
	console.log('npm start "COM3"')
	console.log('\nEXAMPLE MAC:')
	console.log('npm start "/dev/cu.usbmodem1411"')
	return;
}

var UUID = '0000' + uuid.v4().replace(/-/g, '').substring(4, 16);

console.log('Using COM port: '+comPort);
console.log('Using UUID: '+UUID);


utils.pass()
.then(utils.readFile(path.resolve('./', 'base.ino')))
.then(function (data) {
	var codeString = '';
	codeString += 'Bot::forceSaveUuid=true;'+endOfLine;
	for (var i = 0; i < UUID.length; i++) {
		codeString += 'Bot::uuid['+i+']=\''+UUID[i]+'\';'+endOfLine;
	}
	var code = data.replace('/** GENERATED UUID **/', codeString);
	console.log('Compiling and uploading, please wait...')
	return utils.pass()
	.then(utils.writeFile(path.resolve('firmware', 'firmware.ino'), code))
})
// Create (if needed) temp folder
.then(function () {
	return new Promise(function(resolve, reject) {
		utils.pass()
		.then(utils.readDir('.tmp-build'))
		.then(resolve)
		.catch(function() {
			utils.pass()
			.then(utils.mkdir('.tmp-build'))
			.then(resolve)
			.catch(reject)
		})
	});

})
// Compile sketch
.then(utils.execute(
	path.resolve('node_modules', 'npm-arduino-builder', 'arduino-builder', 'arduino-builder') + ' ' +
	'-hardware="' + path.resolve('node_modules') + '" ' +
	'-hardware="' + path.resolve('node_modules', 'npm-arduino-builder', 'arduino-builder', 'hardware') + '" ' +
	'-libraries="' + path.resolve('node_modules') + '" ' +
	'-tools="' + path.resolve('node_modules', 'npm-arduino-avr-gcc', 'tools') + '" ' +
	'-tools="' + path.resolve('node_modules', 'npm-arduino-builder', 'arduino-builder', 'tools') + '" ' +
	'-fqbn="quirkbot-arduino-hardware:avr:quirkbot" ' +
	'-ide-version=10607 ' +
	'-build-path="' + path.resolve('.tmp-build') + '" ' +
	'-verbose ' +
	path.resolve('firmware', 'firmware.ino')
))
// Try to enable bootloader Mode
.then(function () {
	return new Promise(function(resolve, reject) {
		var SerialPort = require("serialport").SerialPort;
		var serialPort = new SerialPort(comPort, {
			baudrate: 57600
		});
		serialPort.on("open", function () {
			serialPort.write([0xb], function(err, results) {
				if(err){
					return reject(err);
				}

				serialPort.close(function () {
					if(err){
						return reject(err);
					}
					setTimeout(resolve, 2000);
				});

			});
		});
		serialPort.on("error", function () {
			reject(arguments);
		});


	});

})
// Upload
.then(function () {
	return new Promise(function(resolve, reject) {
		var command = path.resolve('node_modules', 'npm-arduino-avrdude', 'tools', 'avrdude-6.0.1', 'bin', 'avrdude') + ' ' +
		'-C' + path.resolve('node_modules', 'npm-arduino-avrdude', 'tools', 'avrdude-6.0.1', 'etc', 'avrdude.conf') + ' ' +
		'-v ' +
		'-patmega32u4 ' +
		'-cavr109 ' +
		'-b57600 ' +
		'-D ' +
		'-P' + comPort + ' ' +
		'-Uflash:w:' + path.resolve('.tmp-build', 'firmware.ino.hex')  + ':i ';

		utils.pass()
		.then(utils.execute(command))
		.then(function (output) {
			clearTimeout(timer)
			resolve(output)
		})
		.catch(function (error) {
			clearTimeout(timer)
			reject(error)
		})

		var timer = setTimeout(function () {
			reject('Upload timed out, please try again.')
		},20000);
	});
})
.then(function(result){
	return new Promise(function(resolve, reject){
		var out = result.stdout + result.stderr;
		if(out.indexOf('Thank you') === -1){
			reject(result);
		}
		else{
			resolve();
		}
	})
})
.then(utils.appendFile('UUIDs.txt', new Date() + ',' +UUID.substring(0, 16)+endOfLine))
.then(function(){
	console.log('SUCCESS!');
})
.catch(function(error){
	console.log('FAILED!');
	if(typeof error === 'object'){
		for (var variable in error) {
			if (error.hasOwnProperty(variable)) {
				console.log(variable);
				console.log('	' + error[variable]);
			}
		}
	}
	else{
		console.log(error);
	}
	process.exit();
})
