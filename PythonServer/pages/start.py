'''
THE PUNCH GAME
UPC, EET - Emitters and Receivers
@author: MPomar,JDomenech,VHidalgo,AOlive,HGallego
@repo: https://github.com/boyander/ThePunchGame
'''

import web
from web import form
from websocket import create_connection

def encode_for_socketio(message):
    """
    Encode 'message' string or dictionary to be able
    to be transported via a Python WebSocket client to 
    a Socket.IO server (which is capable of receiving 
    WebSocket communications). This method taken from 
    gevent-socketio.
    """
    MSG_FRAME = "~m~"
    HEARTBEAT_FRAME = "~h~"
    JSON_FRAME = "~j~"

    if isinstance(message, basestring):
            encoded_msg = message
    elif isinstance(message, (object, dict)):
            return encode_for_socketio(JSON_FRAME + json.dumps(message))
    else:
            raise ValueError("Can't encode message.")

    return MSG_FRAME + str(len(encoded_msg)) + MSG_FRAME + encoded_msg

class StartGame:
	def __init__(self):
		self.initcode = 'marcus'; 
		self.f = form.Form(
		    form.Password('Password'),
		    form.Button(name="submit-bt",html="START GAME", type="submit"),
		)
		SOCKET_IO_HOST = "faable.com"
		SOCKET_IO_PORT = 8888
		self.socket_io_url = 'ws://' + SOCKET_IO_HOST + ':' + str(SOCKET_IO_PORT) + '/socket.io/websocket'

	def GET(self):
		ws = w.create_connection(self.socket_io_url)
		msg = "reset"
		msg = encode_for_socketio(msg)
		ws.send(msg)
		return web.ctx.render.start(
		    title='Start game...',
		    form=self.f,
		)

	def POST(self):
		if not self.f.validates():
			return web.ctx.render.start(
				title='Start game...',
				message='ERROR!!',
			)		
		else:
			post_data = web.input()
			print post_data
			m = 'YOU SHALL NOT PASS!'
			if post_data.Password == self.initcode:
				m = "Reseting game!!"
			return web.ctx.render.start(
				title='Start game...',
				message=m,
			)
