/*Send Start signal to server*/

var io = require('socket.io-client');

var socketURL = 'http://faable.com:8888';

/* WEBSOCKET config using socket.io */
var socket = io.connect(socketURL);

console.log("Connecting to server \" "+ socketURL +"\"....");

socket.on('connect', function() {
	console.log("Sending reset...");
	socket.emit('start-game');
	socket.disconnect();
	process.exit(code=0);
});
