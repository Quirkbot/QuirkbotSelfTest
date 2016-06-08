#!/usr/bin/env node


var uuid = require('node-uuid');
var fs = require('fs');
var execSync = require('child_process').execSync;
var path = require('path');
var utils = require('./utils');
var endOfLine = require('os').EOL;

var comPort = process.argv[2];
var uploadComPort = comPort;
var portsBeforeReset = [];

var isWin = /^win/.test(process.platform);


if(!comPort){
	console.log('ERROR: You need to inform the COM port!');
	console.log('\nEXAMPLE WINDOWS:')
	console.log('node node_modules\\quirkbot-self-test\\script.js "COM3"')
	console.log('\nEXAMPLE MAC:')
	console.log('node node_modules/quirkbot-self-test/script.js "/dev/cu.usbmodem1411"')
	return;
}

var UUID = '0000' + uuid.v4().replace(/-/g, '').substring(4, 16);

console.log('Using COM port: '+comPort);
console.log('Using UUID: '+UUID);


utils.pass()
.then(utils.readFile(path.resolve(__dirname, 'base.ino')))
.then(function (data) {
	var codeString = '';
	codeString += 'Bot::forceSaveUuid=true;'+endOfLine;
	for (var i = 4; i < UUID.length; i++) {
		codeString += 'Bot::uuid['+i+']=\''+UUID[i]+'\';'+endOfLine;
	}
	var code = data.replace('/** GENERATED UUID **/', codeString);
	console.log('Compiling and uploading, please wait...')
	return utils.pass()
	.then(utils.writeFile(path.resolve(__dirname, 'firmware', 'firmware.ino'), code))
})
// Create (if needed) temp folder
.then(function () {
	return new Promise(function(resolve, reject) {
		console.log('Creating temp folder...');
		utils.pass()
		.then(utils.readDir(path.resolve(__dirname, '.tmp-build')))
		.then(resolve)
		.catch(function() {
			utils.pass()
			.then(utils.mkdir(path.resolve(__dirname, '.tmp-build')))
			.then(resolve)
			.catch(reject)
		})
	});

})
// Compile sketch
.then(function functionName() {
	console.log('Trying to compile...');
	return utils.pass()
	.then(utils.execute(
		path.resolve(utils.modulePath('npm-arduino-builder'), 'arduino-builder', 'arduino-builder') + ' ' +
		'-hardware="' + path.resolve('node_modules') + '" ' +
		'-hardware="' + path.resolve(utils.modulePath('npm-arduino-builder'), 'arduino-builder', 'hardware') + '" ' +
		'-libraries="' + path.resolve('node_modules') + '" ' +
		'-tools="' + path.resolve(utils.modulePath('npm-arduino-avr-gcc'), 'tools') + '" ' +
		'-tools="' + path.resolve(utils.modulePath('npm-arduino-builder'), 'arduino-builder', 'tools') + '" ' +
		'-fqbn="quirkbot-arduino-hardware:avr:quirkbot" ' +
		'-ide-version=10607 ' +
		'-build-path="' + path.resolve(__dirname, '.tmp-build') + '" ' +
		'-verbose ' +
		path.resolve(__dirname, 'firmware', 'firmware.ino')
	))
})
// Try to enable bootloader Mode
.then(function () {
	return new Promise(function(resolve, reject) {
		console.log('Trying to enable bootloader...');
		var sp = isWin ? require("serialport-dirty-win7-fix") : require("serialport");

		sp.list(function (err, ports) {
			if(err){
				console.log('Serial list error.');
				return reject(err);
			}
			portsBeforeReset = ports;

			var serialPort = new sp.SerialPort(comPort, {
				baudrate: 57600
			});
			serialPort.on("open", function (err) {
				if(err){
					console.log('Serial open error.');
					return reject(err);
				}
				serialPort.write([0xb], function(err, results) {
					if(err){
						console.log('Serial write error.');
						return reject(err);
					}

					serialPort.close(function (err) {
						if(err){
							console.log('Serial close error.');
							return reject(err);
						}
						setTimeout(resolve,200 );
					});

				});
			});
			serialPort.on("error", function () {
				console.log('Serial error event.');
				reject(arguments);
			});
		});
	});

})
.then(function () {
	return new Promise(function(resolve, reject) {
		var sp = isWin ? require("serialport-dirty-win7-fix") : require("serialport");
		var count = 0;
		var countAndScheduleCheck = function() {
			count++;
			if(count >= 10){
				console.log('No device refresh')
				uploadComPort = comPort;
				resolve()
			}
			else{
				setTimeout(check, 300);
			}
		}

		var check = function () {
			sp.list(function (err, ports) {
				if(err){
					return countAndScheduleCheck();
				}
				if(ports.length <= portsBeforeReset.length){
					var disappeared = objectArrayDiffByKey(portsBeforeReset,ports, 'comName');

					if(disappeared.length){
						disappeared = disappeared[0];
						console.log('A device disappeared:', disappeared);
						for (var i = portsBeforeReset.length-1; i >= 0; i--) {
							if(portsBeforeReset[i].comName == disappeared.comName){
								portsBeforeReset.splice(i, 1);
							}
						}
					}

				}
				if(ports.length >= portsBeforeReset.length){
					var appeared = objectArrayDiffByKey(ports, portsBeforeReset, 'comName');
					if(appeared.length){
						appeared = appeared[0];
						console.log('A device appeared:', appeared);

						uploadComPort = appeared.comName;
						resolve();
						return;
					}
				}
				countAndScheduleCheck();
			});
		}
		check();
	});
})
// Upload
.then(function () {
	return new Promise(function(resolve, reject) {
		console.log('Trying to upload...');
		var command = path.resolve(utils.modulePath('npm-arduino-avrdude'), 'tools', 'avrdude-6.0.1', 'bin', 'avrdude') + ' ' +
		'-C' + path.resolve(utils.modulePath('npm-arduino-avrdude'), 'tools', 'avrdude-6.0.1', 'etc', 'avrdude.conf') + ' ' +
		'-v ' +
		'-patmega32u4 ' +
		'-cavr109 ' +
		'-b57600 ' +
		'-D ' +
		'-P' + uploadComPort + ' ' +
		'-Uflash:w:' + path.resolve(__dirname, '.tmp-build', 'firmware.ino.hex')  + ':i ';

		utils.pass()
		.then(utils.execute(command))
		.then(function (output) {
			clearTimeout(timer)
			resolve(output)
		})
		.catch(function (error) {
			clearTimeout(timer)
			console.log('Error on upload.')
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
			console.log('No "thank you" message.');
			reject(result);
		}
		else{
			resolve();
		}
	})
})
.then(utils.appendFile(path.resolve( 'UUIDs.txt'), new Date() + ',' +UUID.substring(0, 16)+endOfLine))
.then(function(){
	console.log('SUCCESS!');
})
.catch(function(error){
	console.log('FAILED!', arguments);
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


var compareArrays = function(a,b){
	if(a.length != b.length) return false;

	for (var i = 0; i < a.length; i++) {
		if(a[i] != b[i])
			return false;
	};

	return true;
}

var objectArrayDiffByKey = function(A, B, key) {
	var map = {}, C = [];

	for(var i = B.length; i--; )
		map[B[i][key]] = true;

	for(var i = A.length; i--; ) {
		if(!map[A[i][key]]){
			C.push(A[i]);
		}

	}

	return C;
}
