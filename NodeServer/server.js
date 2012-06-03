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
	if(tA.hasOwnProperty(data.id)){
		tA[userID].shakes += shakesToAdd;
	}else{
		tB[userID].shakes += shakesToAdd;
	}
}

function getShakes(userID){
	if(tA.hasOwnProperty(data.id)){
		return tA[userID].shakes;
	}else{
		return tB[userID].shakes += shakesToAdd;
	}
}

io.sockets.on('connection', function (socket) {

	socket.emit('handshake');

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
			socket.join(selectedTeam);
		}
		socket.set('fbdata', data);

		var users = prepareUsers();

		//Broadcast reload clients
		io.broadcast.emit('reload-users',users);
	});

	socket.on('shake-update', function (userID,shakes) {
		accumulateShakes(userID,shakes);
		console.log("Current shakes for " + userID + " are " + getShakes(userID));
	});

	/*socket.on('fbuser-add', function (data) {
		console.log('New User "' + data.name + '" with ID -> ' + data.id);
		socket.emit('reload-users',new Array(data));
	});*/
});
