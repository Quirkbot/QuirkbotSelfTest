var uuid = require('node-uuid');
var fs = require('fs');
var execSync = require('child_process').execSync;
var path = require('path');
var utils = require('./utils');
var endOfLine = require('os').EOL;

var arduino = process.argv[2] || '/Applications/Arduino.app/Contents/MacOS/Arduino';
var comPort = process.argv[3];

if(!arduino){
	console.log('ERROR: You need to inform the path to the arduino binary!');
	return;
}

if(!comPort){
	console.log('ERROR: You need to inform the COM port!');
	return;
}

var UUID = uuid.v4().replace(/-/g, '').substring(0, 16);

console.log('Using Arduino: '+arduino);
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
.then(utils.execute(
	'"' + arduino + '" '
	+ '--verbose-upload '
	+ '--port '+ comPort +' '
	+ '--board QuirkbotArduinoHardware:avr:quirkbot '
	+ '--upload '
	+ '"' + path.resolve('firmware', 'firmware.ino') + '" '
))
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
.then(utils.appendFile('UUIDs.txt', UUID+endOfLine))
.then(function(){
	console.log('SUCCESS!');
})
.catch(function(error){
	console.log('FAILED! '+ error);
})
