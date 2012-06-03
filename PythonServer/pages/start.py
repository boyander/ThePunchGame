'''
Created on 30/05/2012

@author: boyander
'''

import web

class StartGame:
	def __init__(self):
		self.initcode = 'marcus'; 
		self.f = form.Form(
            form.Password('pass'),
            form.Button(name="submit-bt",html="START GAME", type="submit"),
        )

	def POST():
		data = web.data()
		m = "YOU SHALL NOT PASS!"
		print data
		if data.pass == self.initcode:
			m = "STARTING GAME!!"

		return web.ctx.render.start(
			title='Start game...',
			message=m,
		)

	def GET(self):

		return web.ctx.render.start(
		    title='Start game...',
		    form=self.f,
		)