$(function() {
	       var kn = $(".teamA").knob();
	       var kn = $(".teamB").knob();
	       var h = 0;
	       var tid = setTimeout(mycode, 500);
	       var knobA = $(".teamA");
	       var knobB = $(".teamB");
	       function updateKnob(k,val){
	           k.val(val);
	           k.change();
	       }
	       function mycode() {
	         // do some stuff...
	           console.log(h);
	           updateKnob(knobA,h);
	           updateKnob(knobB,h);
	           h = ( h+1 ) % 102;
	           if( h <= 100){
	               tid = setTimeout(mycode, 100); // repeat myself
	           }
	       }
	       function abortTimer() { // to be called when you want to stop the timer
	         clearTimeout(tid);
	       }
});