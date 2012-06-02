'''
Created on 25/05/2012

@author: boyander
'''

import web

class MainPage:

    def GET(self):
        return web.ctx.render.main(
            title='The Punch Game!', 
        )