$(function() {
	var h = 0;
	var knobA = $(".teamA"); 
	$(".teamA").knob();
	var knobB = $(".teamB"); 
	$(".teamB").knob();
	var myFB = -1;

	var io = require('socket.io-client');
	var socketURL = 'http://faable.com:8888';
	var options ={
		transports: ['websocket'],
		'force new connection': true
	};

	/* WEBSOCKET config using socket.io */
	var socket = io.connect(socketURL,options);

	function updateKnob(k,val){
	   k.val(val);
	   k.change();
	}
	window.addEventListener('shake', shakiEvent, false);

	function shakiEvent() {
	   updateKnob(knobA,h);
	   updateKnob(knobB,h);
	   if(h > 9){
	   		socket.emit('shake-update',myFB.id);
	   }
	   h = ( h+1 ) % 100;
	}


	/*socket.on('handshake', function (data) {
		console.log(data);
		socket.emit('handshake-ok', { user: 'boyander' });
	});*/
	/*socket.on('get-shakes', function (data) {
		socket.emit('shake-data', { user: 'boyander', accumulated:h });
	});*/

	socket.on('reload-users', function (users) {
		//console.log(users);
		// Remove all previous setted users
		var grA = "";
		var grB = "";
		
		//team A
		for(var key in users.teamA) {
			grA += setgroup(users.teamA[key].fbdata);
		}
		//team B
		for(var key in users.teamB) {
			grB += setgroup(users.teamB[key].fbdata);
		}

		//Refresh lists
		$('.groupA .list').replaceWith(grA);
		$('.groupB .list').replaceWith(grB);
		FB.XFBML.parse();
	});

	/* FACEBOOK ADD USERS VIA WEBSOCKET */
	function setgroup(fbdata){
	    var picture = '<span class="punch-user" alt="'+fbdata.name+'"><fb:profile-pic uid="'+fbdata.id+'" size="square" width="32" height="32"></fb:profile-pic></span>';
	    return picture;
	    /*if(group == 'A'){
	        $('.groupA .list').append(picture);
	    }else{
	        $('.groupB .list').append(picture);
	    }
	    console.log("New user add to group "+group+" with id "+fbdata.id);
	    //FB.XFBML.parse();*/
	}

	function pushUserToServer(fbdata){
	    socket.emit('fbuser-add',fbdata);
	}


	/* FACEBOOK API */
	window.fbAsyncInit = function() {

		FB.init({
			appId      : '309679859116570',
			status     : true, 
			cookie     : true,
			xfbml      : true,
			oauth      : true,
		});

	FB.Event.subscribe('auth.statusChange', function(response) {
		//console.log(response);
        var fb_id = -1;
       // var effect = 'explode';
		if (response.authResponse) {
		// user has auth'd your app and is logged into Facebook
			FB.api('/me', function(me){
				//console.log(me);
				myFB = me;
                pushUserToServer(me);
                $('#fb-auth-status #out').hide();
                $('#fb-auth-status').show();
			})
		} else {
			// user has not auth'd your app, or is not logged into Facebook
			$('#fb-auth-status #in').hide();
			$('#fb-auth-status').show();
		}
	});

	};


});


// Load the SDK Asynchronously
(function(d){
 var js, id = 'facebook-jssdk', ref = d.getElementsByTagName('script')[0];
 if (d.getElementById(id)) {return;}
 js = d.createElement('script'); js.id = id; js.async = true;
 js.src = "//connect.facebook.net/en_US/all.js";
 ref.parentNode.insertBefore(js, ref);
}(document));

