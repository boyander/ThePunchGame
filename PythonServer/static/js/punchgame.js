$(function() {
	var h = 0;
	var knobA = $(".teamA"); knobA.knob();
	var knobB = $(".teamB"); knobB.knob();
	var myFB = -1;

	/* WEBSOCKET config using socket.io */
	var socket = io.connect('ws://faable.com:8888');

	function updateKnob(k,val){
	   k.val(val);
	   k.change();
	}
	window.addEventListener('shake', shakiEvent, false);
	function shakiEvent() {
	   updateKnob(knobA,h);
	   updateKnob(knobB,h);
	   if(h > 9){
	   		socket.emit('shake-update',myFB.id,h);
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
		console.log(users)
		//team A
		for(var key in users.teamA) {
			setgroup('A',users.teamA[key].fbdata);
		}
		for(var key in users.teamB) {
			setgroup('B',users.teamB[key].fbdata);
		}
	});

	/* FACEBOOK ADD USERS VIA WEBSOCKET */
	function setgroup(group,fbdata){
	    var picture = '<span class="punch-user" alt="'+fbdata.name+'"><fb:profile-pic uid="'+fbdata.id+'" size="square" width="32" height="32"></fb:profile-pic></span>';
	    if(group == 'A'){
	        $('.groupA .list').append(picture);
	    }else{
	        $('.groupB .list').append(picture);
	    }
	    console.log("New user add to group "+group+" with id "+fbdata.id+"!");
	    FB.XFBML.parse();
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
			$('#fb-auth-status #out').show();
			$('#fb-auth-status').show();
		}
	});

	};


});


(function(d, s, id) {
  var js, fjs = d.getElementsByTagName(s)[0];
  if (d.getElementById(id)) return;
  js = d.createElement(s); js.id = id;
  js.src = "//connect.facebook.net/es_ES/all.js";
  fjs.parentNode.insertBefore(js, fjs);

}(document, 'script', 'facebook-jssdk'));