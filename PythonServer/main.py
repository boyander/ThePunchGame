#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
@author: boyander
Created on 25/05/2012
'''
import os
import web

##Import render for templating
from web.contrib.template import render_mako


urls = (
	'/', 'pages.main.MainPage',
)

#Context load
def load_ctx(handler):
	
	web.ctx.marc = "hola mundo!"
	web.ctx.render = render_mako(
				directories = [os.path.join(os.path.dirname(__file__), 'templates')],
	            input_encoding='utf-8',
	            output_encoding='utf-8')
	
	return handler()


#Main execution
def main():
	app = web.application(urls, globals())
	app.add_processor(load_ctx)
	app.run()

if __name__ == "__main__":main()


