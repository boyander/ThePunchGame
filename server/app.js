/*
* THE PUNCH GAME
* UPC, EET - Emitters and Receivers
* author: MPomar, JDomenech, VHidalgo, AOlive, HGallego
* @repo: https://github.com/boyander/ThePunchGame
*/

var SOCKET_IO_PORT = 8888,
	EXPRESS_WS_PORT = 3000;

var io = require('socket.io').listen(SOCKET_IO_PORT),
    express = require('express'),
    http = require('http')
    app = express.createServer(express.logger());


/*
 * Master app var
 */
var app = express(),
	clients = {};

/* 
 * Default configuration hook on production and development enviroments 
 */
app.configure(function(){
  app.set('localrun', process.env.LOCAL || false);
  app.set('port', process.env.PORT || EXPRESS_WS_PORT);
  app.set('clients', clients);
  app.set('title', "The Punch Game")
  app.set('domain', 'http://thepunchgame.faable.com');
  app.set('views', __dirname + '/templates');
  app.set('view engine', 'jade');
  app.set('view options', {
    layout: false
  });
  app.use(express.favicon(__dirname + "/public/images/favicon.ico"));
  app.use(express.logger('dev'));
  app.use(express.limit('20mb'));
  //app.use(express.bodyParser());
  app.use(express.methodOverride());
  app.use(app.router);
  app.use(express.errorHandler());
  app.use(require('connect-assets')({'src':'coffee-less'}));
  app.use(express.static(__dirname + '/public'));
});

/*
 * HTTP Server
 */
server = http.createServer(app).listen(app.get('port'), function(){
  console.log("Express server listening on port " + app.get('port'));
});

/* 
 * Routing 
 */
app.get('/', game_page);


function game_page(req, res) {
	var params = {
		port: req.app.get("port"),
		title: req.app.get("title"),
	};
	res.render("game", params);
};


/* 
 * Game Logic
 */
var tA = new Object();
var tB = new Object();

var gameON = false;

var users = {'teamA':new Array(),'teamB':new Array()};

function prepareUsers(){
	var a = new Array();
	var b = new Array();
	for(var key in tA){
		var dA = {'id':tA[key].fbdata.id,'name':tA[key].fbdata.name};
		a.push({'fbdata':dA,'shakes':tA[key].shakes});
	}
	for(var key in tB){
		var dB = {'id':tB[key].fbdata.id,'name':tB[key].fbdata.name};
		b.push({'fbdata':dB,'shakes':tB[key].shakes});
	}
	return {'teamA':a,'teamB':b}
}

function accumulateShakes(userID, shakesToAdd){
	if(tA.hasOwnProperty(userID)){
		tA[userID].shakes += shakesToAdd;
	}else if(tB.hasOwnProperty(userID)){
		tB[userID].shakes += shakesToAdd;
	}else{
		return false;
	}
	return true;
}

function getShakes(userID){
	if(tA.hasOwnProperty(userID)){
		return tA[userID].shakes;
	}else if(tB.hasOwnProperty(userID)){
		return tB[userID].shakes;
	}else{
		return -1;
	}
}

function calculateShakingWindow(){

	var shakesA = 0;
	var clientsA = 0;
	var shakesB = 0;
	var clientsB = 0;

	for(var key in tA){
		shakesA += tA[key].shakes;
		clientsA += 1;
	}
	for(var key in tB){
		shakesB += tB[key].shakes;
		clientsB += 1;
	}

	return {'teamA': shakesA/clientsA, 'teamB': shakesB/clientsB};
}

function resetCalculus(){
	for(var key in tA){
		tA[key].shakes = 0;
	}
	for(var key in tB){
		tB[key].shakes = 0;
	}
}


var timer = null; 
var displayRefresh = 1000;
var winTreshold = 100.0;
var current = {'teamA': 0, 'teamB': 0 ,'winner':false };

var updatetimer = function () {
	console.log('DISPLAY UPDATE BROADCAST');
	current = calculateShakingWindow();
	if(current.teamA > winTreshold || current.teamB > winTreshold){
		current.winner = true;
		gameON = false;
		if(current.teamA > winTreshold){
			current.teamA = winTreshold;
		}else{
			current.teamB = winTreshold;
		}
	}
	io.sockets.emit('shake-refresh', current );
    timer = setTimeout(updatetimer, displayRefresh);
};

timer = setTimeout(updatetimer, displayRefresh);

io.sockets.on('connection', function (socket) {

	/* Welcome to new users, so let's send data to client */
	socket.on('start-game', function (data) {
		resetCalculus();
		gameON = true;
		current.winner = false;
		io.sockets.emit('game-status',{'gameON':true});
	});

	socket.on('reset-game', function (data) {
		resetCalculus();
		gameON = false;
		current.winner = false;
		console.log("RESETING GAME!!!!");
	});


	socket.on('handshake', function (data) {
		socket.emit('reload-users',users);
		socket.emit('game-status',{'gameON':gameON});
		if(gameON){
			socket.emit('shake-refresh', current );
		}
	});

	/* Add user to system from facebook */
	socket.on('fbuser-add', function (data) {
		console.log('Login user -> '+ data.name);

		var selectedTeam = 'B';

		//var user_rooms = io.sockets.manager.roomClients[socket.id]
		if(tA.hasOwnProperty(data.id)){
			//User already on team A
			console.log('Already on teamA!');
			socket.join('teamA');
			selectedTeam = 'A';
		}else if(tB.hasOwnProperty(data.id)){
			//User already on team B
			console.log('Already on teamB!');
			socket.join('teamB');
		}else{
			if (Object.keys(tA).length <= Object.keys(tB).length ){
				//Join to Team A
				selectedTeam = 'A';
				tA[data.id] = {'fbdata':data,'shakes':0};
			}else{
				tB[data.id] = {'fbdata':data,'shakes':0};
			}
			console.log(data.id + " Joined to "+selectedTeam);
			//socket.join(selectedTeam);
		}

		socket.set('fbdata', data);

		socket.emit('game-status',{'team':selectedTeam,'gameON':gameON});

		users = prepareUsers();
		//Broadcast reload clients
		socket.emit('reload-users',users);
		socket.broadcast.emit('reload-users',users);
	});

	/* Perform shake update */
	socket.on('shake-update', function (data) {
		if(!current.winner && gameON){
			if(accumulateShakes(data.userID,data.shakes)){
				console.log("Current shakes for " + data.userID + " are " + getShakes(data.userID));
			}
		}else{
			console.log("Shake ignored due to no current game!!");
		}
	});


});
