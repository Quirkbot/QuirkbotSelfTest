"use strict";

var fs = require('fs');
var exec = require('child_process').exec;
var path = require('path');
//var Promise = require('promise');

var modulePath = function(module){
	return path.resolve(require.resolve(module), '..');
}
exports.modulePath = modulePath;

var pass = function(){
	var payload = arguments;
	var promise = function(resolve, reject){
		resolve.apply(null, payload);
	}
	return new Promise(promise);
}
exports.pass = pass;

var execute = function(command, resolveError){
	//console.log('execute:', command);
	return function(){
		var payload = arguments;

		var promise = function(resolve, reject){
			var process = exec(command, function (error, stdout, stderr) {
				if (!resolveError && error !== null) reject(stderr);
				else resolve({stdout:stdout, stderr:stderr});
			});
		}
		return new Promise(promise);
	}
}
exports.execute = execute;

var writeFile = function(path, content){
	//console.log('writeFile:', path);
	return function(){
		var payload = arguments;

		var promise = function(resolve, reject){
			fs.writeFile(path, content, function (error) {
				if (error) {
					reject(error)
					return;
				}
				resolve.apply(null, payload)
			});
		}
		return new Promise(promise);
	}
}
exports.writeFile = writeFile;

var readFile = function(path){
	//console.log('readFile:', path);
	return function(){
		var payload = arguments;

		var promise = function(resolve, reject){
			fs.readFile(path, "utf-8", function read(error, data) {
				if (error) {
					reject(error)
					return;
				}
				resolve(data)
			});
		}
		return new Promise(promise);
	}
}
exports.readFile = readFile;

var appendFile = function(path, content){
	//console.log('appendFile:', path);
	return function(){
		var payload = arguments;

		var promise = function(resolve, reject){
			fs.appendFile(path, content, function (error) {
				if (error) {
					reject(error)
					return;
				}
				resolve.apply(null, payload)
			});
		}
		return new Promise(promise);
	}
}
exports.appendFile = appendFile;

var readDir = function(path){
	//console.log('readDir:', path);
	return function(){
		var payload = arguments;

		var promise = function(resolve, reject){
			fs.readdir(path, function read(error, files) {
				if (error) {
					reject(error)
					return;
				}
				resolve(files)
			});
		}
		return new Promise(promise);
	}
}
exports.readDir = readDir;

var mkdir = function(path){
	//console.log('mkdir:', path);
	return function(){
		var payload = arguments;

		var promise = function(resolve, reject){
			fs.mkdir(path, function(error) {
				if (error) {
					reject(error)
					return;
				}
				resolve()
			});
		}
		return new Promise(promise);
	}
}
exports.mkdir = mkdir;

var log = function(){
	var payload = arguments;
	var promise = function(resolve, reject){
		for (var i = 0; i < payload.length; i++) {
			console.log(payload[i])
		};
		resolve.apply(null, payload);
	}
	return new Promise(promise);
}
exports.log = log;

var delay = function(millis){
	return function(){
		var payload = arguments;
		var promise = function(resolve, reject){
			setTimeout(function(){
				resolve.apply(null, payload);
			}, millis)

		}
		return new Promise(promise);
	}
}
exports.delay = delay;
