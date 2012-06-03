var io = require('socket.io').listen(8888);

var tA = new Object();
var tB = new Object();

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

	return {'teamA': shakesA/clientsA, 'teamB': shakesB/clientsB} 
}

var timer = null; 
var displayRefresh = 5000;

var updatetimer = function () {
	console.log('DISPLAY UPDATE BROADCAST');
	var current = calculateShakingWindow();
	io.sockets.emit('shake-refresh', current );
    timer = setTimeout(updatetimer, displayRefresh);
};

timer = setTimeout(updatetimer, displayRefresh);

io.sockets.on('connection', function (socket) {

	socket.on('fbuser-add', function (data) {
		console.log('Login user -> '+ data.name);
		//var user_rooms = io.sockets.manager.roomClients[socket.id]
		if(tA.hasOwnProperty(data.id)){
			//User already on team A
			console.log('Already on teamA!');
			socket.join('teamA');
		}else if(tB.hasOwnProperty(data.id)){
			//User already on team B
			console.log('Already on teamB!');
			socket.join('teamB');
		}else{
			var selectedTeam = 'teamB';
			if (Object.keys(tA).length <= Object.keys(tB).length ){
				//Join to Team A
				selectedTeam = 'teamA';
				tA[data.id] = {'fbdata':data,'shakes':0};
			}else{
				tB[data.id] = {'fbdata':data,'shakes':0};
			}
			console.log(data.id + " Joined to "+selectedTeam);
			//socket.join(selectedTeam);
		}
		socket.set('fbdata', data);

		var users = prepareUsers();

		//Broadcast reload clients
		//io.sockets.to('teamA').emit('reload-users',users);
		//io.sockets.to('teamB').emit('reload-users',users);
		socket.emit('reload-users',users);
		socket.broadcast.emit('reload-users',users);
	});

	socket.on('shake-update', function (data) {
		if(accumulateShakes(data.userID,data.shakes)){
			console.log("Current shakes for " + data.userID + " are " + getShakes(data.userID));
		}
	});

	/*socket.on('fbuser-add', function (data) {
		console.log('New User "' + data.name + '" with ID -> ' + data.id);
		socket.emit('reload-users',new Array(data));
	});*/
});
