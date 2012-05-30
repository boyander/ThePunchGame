$(function() {
	var h = 0;
	var knobA = $(".teamA"); knobA.knob();
	var knobB = $(".teamB"); knobB.knob();
	function updateKnob(k,val){
	   k.val(val);
	   k.change();
	}
	window.addEventListener('shake', shakiEvent, false);
	function shakiEvent() {
	   updateKnob(knobA,h);
	   updateKnob(knobB,h);
	   h = ( h+1 ) % 100;
	}
	var socket = io.connect('ws://localhost:8888');
	socket.on('handshake', function (data) {
		console.log(data);
		socket.emit('handshake-ok', { user: 'boyander' });
	});
	socket.on('get-shakes', function (data) {
		socket.emit('shake-data', { user: 'boyander', accumulated:h });
	});
});