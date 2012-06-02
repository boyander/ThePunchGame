#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
@author: boyander
Created on 25/05/2012
'''
import os
import sys
import web

from ws4py.client.threadedclient import WebSocketClient

##Import render for templating
from web.contrib.template import render_mako


urls = (
	'/', 'pages.main.MainPage',
	'/fb/login', 'pages.fbLogin.fbLogin',
)

#Context load
def load_ctx(handler):
	
	web.ctx.render = render_mako(
				directories = [os.path.join(os.path.dirname(__file__), 'templates')],
	            input_encoding='utf-8',
	            output_encoding='utf-8')
	
	return handler()

#Main execution
def main(mode = 'test'):

	print "The Punch Game - Starting server....."
	print "Please, report issues @ https://github.com/boyander/ThePunchGame"
	
	sys.stdout = sys.stderr
	app = web.application(urls, globals(),autoreload=True)
	app.add_processor(load_ctx)
	#app.notfound = notfound

	if mode == 'test':
		#Run app as local server (on webpy server)
		app.run()
	elif mode == 'wsgi':
		#Run wsgi (production enviroment, i.e: apache)
		application = app.wsgifunc()



#Main execution handling
if __name__ == "__main__": main()
if __name__.startswith('_mod_wsgi_'): main('wsgi')

