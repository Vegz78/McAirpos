from uinputmapper.cinput import *


"""
Configuration for a simple Microsoft SideWinter Game Pad Pro USB version 1.0
... as ABS input pointer device
"""

config = {
        (0, EV_KEY): {
            BTN_A: {
                'type' : (0, EV_KEY),
                'code' : BTN_MOUSE,
                'value' : lambda x: 0 if x == 0 else 1
            }
        }
}

names = {
    0 : 'Example mouse'
}

def config_merge(c):
    del c[(0, EV_KEY)]
    c.update(config)
