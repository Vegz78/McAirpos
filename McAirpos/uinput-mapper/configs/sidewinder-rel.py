from uinputmapper.cinput import *

"""
Configuration for a simple Microsoft SideWinter Game Pad Pro USB version 1.0
... as REL input pointer device
"""


config = {
        (0, EV_KEY): {
            BTN_A: {
                'type' : (0, EV_KEY),
                'code' : BTN_MOUSE,
                'value' : lambda x: 0 if x == 0 else 1
            }
        },
        (0, EV_ABS) : {
            ABS_X: {
                'type' : (0, EV_REL),
                'code' : REL_X,
                'value' : lambda x: x / 7,
            },
            ABS_Y: {
                'type' : (0, EV_REL),
                'code' : REL_Y,
                'value' : lambda x: x / 7,
            }
        }
}

names = {
    0 : 'Example mouse',
}

def config_merge(c, n):
    del c[(0, EV_KEY)]
    del c[(0, EV_ABS)]
    c.update(config)
    n.update(names)
