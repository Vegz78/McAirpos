from uinputmapper.cinput import *

def keymouse(k):
    pass


config = {
        (0, EV_KEY) : {
            KEY_UP : {
                'type' : (0, EV_REL),
                'code' : REL_Y,
                'value' : lambda _: -_*5
            },
            KEY_DOWN : {
                'type' : (0, EV_REL),
                'code' : REL_Y,
                'value' : lambda _: _*5
            },
            KEY_LEFT : {
                'type' : (0, EV_REL),
                'code' : REL_X,
                'value' : lambda _: -_*5
            },
            KEY_RIGHT : {
                'type' : (0, EV_REL),
                'code' : REL_X,
                'value' : lambda _: _*5
            },
            KEY_LEFTCTRL : {
                'type' : (0, EV_KEY),
                'code' : BTN_LEFT,
                'value' : None
            },
            KEY_LEFTALT : {
                'type' : (0, EV_KEY),
                'code' : BTN_RIGHT,
                'value' : None
            }
        }
}

names = {
    0 : 'Example mouse'
}

def config_merge(c, n):
    c.clear()

    n.update(names)

    for k, v in config.iteritems():
        if k in c:
            c[k].update(v)
        else:
            c[k] = v
