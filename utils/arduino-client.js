var serialport = require("serialport");
var io = require('socket.io-client');
var socketURL = 'http://faable.com:8888';
var acc_threshold = 3000.0;
var time_threshold = 10;
var d = new Date();
var c_sk = 0;
var last_shake = d.getTime();

function printACC(acc){
	console.log("X-> "+ acc.X + " \tY-> " + acc.Y + "\tZ-> " + acc.Z);
}

function isShake(lastacc){
	var d = new Date();
	var current = d.getTime();
	if(lastacc.Y > acc_threshold && ((current-last_shake) > time_threshold)){
		last_shake = current;
		return true;
	}
	return false;
}


/* WEBSOCKET config using socket.io */
console.log("Connecting to server \""+ socketURL +"\"....");
var socket = io.connect(socketURL);

var SerialPort = serialport.SerialPort; // localize object constructor

var sp = new SerialPort("/dev/tty.usbserial-A600agms", { 
	parser: serialport.parsers.readline("\n"),
	baudrate: 115200
});

socket.on('connect', function() {
	console.log("Connected!!");
	socket.emit('fbuser-add',{'id':'arduino','name':'Arduino'});
	sp.on("data", function (data) {
		var acc_s = data.split(';');

		var acc = {'X':0.0,'Y':0.0,'Z':0.0};
		if (acc_s.length >= 3){
			acc.X = parseFloat(acc_s[0]);
			acc.Y = parseFloat(acc_s[1]);
			acc.Z = parseFloat(acc_s[2]);
		}
		if(isShake(acc)){
			c_sk = (c_sk + 1) % 11;
			console.log("SHAKE " + c_sk);
			if(c_sk == 10){
				socket.emit('shake-update',{'userID':'arduino','shakes':10});
			}
		}
		//printACC(acc);
	});
});

