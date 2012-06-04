'''
THE PUNCH GAME
UPC, EET - Emitters and Receivers
@author: MPomar,JDomenech,VHidalgo,AOlive,HGallego
@repo: https://github.com/boyander/ThePunchGame
'''

import web

class MainPage:

    def GET(self):
        return web.ctx.render.main(
            title='The Punch Game!', 
        )