from uinputmapper.cinput import *


"""
Configuration for a simple Microsoft SideWinter Game Pad Pro USB version 1.0
... as ABS input pointer device
"""

config = {
        (0, EV_KEY): {
            BTN_DPAD_LEFT: {
                'type' : (0, EV_KEY),
                'code' : KEY_A,
		'value': None
            },
	    BTN_DPAD_RIGHT: {
		'type' : (0, EV_KEY),
		'code' : KEY_D,
		'value' : None
	    },
	    BTN_DPAD_UP: {
		'type' : (0, EV_KEY),
		'code' : KEY_W,
		'value' : None
	    },
	    BTN_DPAD_DOWN: {
		'type' : (0, EV_KEY),
		'code' : KEY_S,
		'value' : None
	    },
	    BTN_SOUTH: {
		'type' : (0, EV_KEY),
		'code' : 29,
		'value' : None
	    },
	    BTN_X: {
		'type' : (0, EV_KEY),
		'code' : 42,
		'value' : None
	    },
	    BTN_START: {
		'type' : (0, EV_KEY),
		'code' : 1,
		'value' : None
	    },
	    BTN_SELECT: {
		'type' : (0, EV_KEY),
		'code' : 59,
		'value' : None
	    },
	    BTN_MODE: {
		'type' : (0, EV_KEY),
		'code' : 60,
		'value' : None
	    }
        },
        (1, EV_KEY): {
            BTN_DPAD_LEFT: {
                'type' : (0, EV_KEY),
                'code' : 105,
		'value': None
            },
	    BTN_DPAD_RIGHT: {
		'type' : (0, EV_KEY),
		'code' : 106,
		'value' : None
	    },
	    BTN_DPAD_UP: {
		'type' : (0, EV_KEY),
		'code' : 103,
		'value' : None
	    },
	    BTN_DPAD_DOWN: {
		'type' : (0, EV_KEY),
		'code' : 108,
		'value' : None
	    },
	    BTN_SOUTH: {
		'type' : (0, EV_KEY),
		'code' : 100,
		'value' : None
	    },
	    BTN_X: {
		'type' : (0, EV_KEY),
		'code' : 57,
		'value' : None
	    },
	    BTN_START: {
		'type' : (0, EV_KEY),
		'code' : 1,
		'value' : None
	    },
	    BTN_SELECT: {
		'type' : (0, EV_KEY),
		'code' : 59,
		'value' : None
	    },
	    BTN_MODE: {
		'type' : (0, EV_KEY),
		'code' : 60,
		'value' : None
	    }
        }
}

names = {
    0 : 'MakeCode_Arcade'

}

def config_merge(c, n):
    c.clear()
    c.update(config)
    n.update(names)
