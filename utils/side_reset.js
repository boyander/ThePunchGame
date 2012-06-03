/*Send Start signal to server*/

var io = require('socket.io-client');

var socketURL = 'http://faable.com:8888';

/* WEBSOCKET config using socket.io */
var socket = io.connect(socketURL);

console.log("Connecting to server \" "+ socketURL +"\"....");
var args = process.argv.slice(2);

socket.on('connect', function() {
	if(args[0] == 'reset'){
		console.log("Sending reset...");
		socket.emit('reset-game');
	}else if(args[0] == 'start'){
		console.log("Sending start...");
		socket.emit('start-game');
	}else{
		console.log("Bad Arguments, use reset or start");
	}

	socket.disconnect();
	process.exit(code=0);
});
