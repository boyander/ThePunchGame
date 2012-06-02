'''
Created on 30/05/2012

@author: boyander
'''

import web
import facebook

class fbLogin:
	def __init__(self):
		self.api_key = '309679859116570'
		self.secret_key = '65b979f808328df94c2b3e0f65351772'

	def GET(self):

		fbapi = facebook.Facebook(self.api_key, self.secret_key)
		user = "None"
		if fbapi.check_session(req):
			user = fbapi.users.getInfo([fbapi.uid], DEFAULT_FIELDS)[0]


		return web.ctx.render.main(
		    title='The Punch Game!',
		    content="Login user " + user,
		)