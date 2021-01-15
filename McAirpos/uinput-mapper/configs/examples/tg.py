from uinputmapper.cinput import *

config = {
        (0, EV_KEY) : {
            BTN_MIDDLE : {
                'type' : (0, EV_KEY),
                'code' : KEY_RIGHTMETA,
                'value' : None
            }
        }
}

names = {
    0 : 'Example mouse'
}

def config_merge(c, n):
    c.clear()
    c.update(config)
    n.update(names)
