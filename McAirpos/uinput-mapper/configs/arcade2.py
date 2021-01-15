from uinputmapper.cinput import *


"""
Configuration for many EV_ABS(axis) and EV_KEY(digital buttons) directional controllers
... as EV_KEY MakeCode Arcade keyboard device
"""

# Global variables
autoconf = 1	#Determines min and max for EV_ABS events automatically if 1, min and max must be set manually below if 0
deadzone = 0.25	#Deadzone in percentage before EV_ABS events react, used to dampen reactions to axis movements


# Variables for EV_ABS controller no. 1
invertUp = 0	#For inverting Y axis if 1, e.g. Nimbus SteelSeries controller
invertLeft = 0	#For inverting X axis if 1
max = 1		#Seed value = 1 for autoconf, if manual find properties using ./input-read -v -p /dev/input/eventX
min = 0		#Seed value = 0 for autoconf
mid = (min + max)/2

# Directional functions for EV_ABS controller no. 1
def digitizeUp(x):
    global min, mid, max, deadzone
    if x < min:
	min = x
	mid = (min + max)/2

    if invertUp:
	if x > (mid + (max - mid) * deadzone):
	    x = 1
	else:
	    x = 0
    else:
	if x < (mid - (max - mid) * deadzone):
	    x = 1
	else:
	    x = 0

    return int(x)

def digitizeDown(x):
    global min, mid, max, deadzone
    if x > max:
	max = x
	mid = (min + max)/2

    if invertUp:
	if x < (mid - (max - mid) * deadzone):
	    x = 1
	else:
	    x = 0
    else:
	if x > (mid + (max - mid) * deadzone):
	    x = 1
	else:
	    x = 0

    return int(x)

def digitizeLeft(x):
    global min, mid, max, deadzone
    if x < min:
	min = x
	mid = (min + max)/2

    if invertLeft:
	if x > (mid + (max - mid) * deadzone):
	    x = 1
	else:
	    x = 0
    else:
	if x < (mid - (max - mid) * deadzone):
	    x = 1
	else:
	    x = 0

    return int(x)

def digitizeRight(x):
    global min, mid, max, deadzone
    if x > max:
	max = x
	mid = (min + max)/2

    if invertLeft:
	if x < (mid - (max - mid) * deadzone):
	    x = 1
	else:
	    x = 0
    else:
	if x > (mid + (max - mid) * deadzone):
	    x = 1
	else:
	    x = 0

    return int(x)


# Variables for EV_ABS controller no. 2
invertUp2 = 0	#For inverting Y axis if 1, e.g. Nimbus SteelSeries controller
invertLeft2 = 0	#For inverting X axis if 1
max2 = 1	#Seed value = 1 for autoconf, if manual find properties using ./input-read -v -p /dev/input/eventX
min2 = 0	#Seed value = 0 for autoconf
mid2 = (min + max)/2

# Directional functions for EV_ABS controller no. 2
def digitizeUp2(x):
    global min2, mid2, max2, deadzone
    if x < min2:
	min2 = x
	mid2 = (min2 + max2)/2

    if invertUp2:
	if x > (mid2 + (max2 - mid2) * deadzone):
	    x = 1
	else:
	    x = 0
    else:
	if x < (mid2 - (max2 - mid2) * deadzone):
	    x = 1
	else:
	    x = 0

    return int(x)

def digitizeDown2(x):
    global min2, mid2, max2, deadzone
    if x > max2:
	max2 = x
	mid2 = (min2 + max2)/2

    if invertUp2:
	if x < (mid2 - (max2 - mid2) * deadzone):
	    x = 1
	else:
	    x = 0
    else:
	if x > (mid2 + (max2 - mid2) * deadzone):
	    x = 1
	else:
	    x = 0

    return int(x)

def digitizeLeft2(x):
    global min2, mid2, max2, deadzone
    if x < min2:
	min2 = x
	mid2 = (min2 + max2)/2

    if invertLeft2:
	if x > (mid2 + (max2 - mid2) * deadzone):
	    x = 1
	else:
	    x = 0
    else:
	if x < (mid2 - (max2 - mid2) * deadzone):
	    x = 1
	else:
	    x = 0

    return int(x)

def digitizeRight2(x):
    global min2, mid2, max2, deadzone
    if x > max2:
	max2 = x
	mid2 = (min2 + max2)/2

    if invertLeft2:
	if x < (mid2 - (max2 - mid2) * deadzone):
	    x = 1
	else:
	    x = 0
    else:
	if x > (mid2 + (max2 - mid2) * deadzone):
	    x = 1
	else:
	    x = 0

    return int(x)


# Variables for EV_ABS HAT controllers
hmin = -1
hmax = 1
hmid = 0

# Directional functions for EV_ABS HAT controllers
def hat0Pos(x):
    global hmin, hmid, hmax

    if x > hmid:
	x = 1
    else:
	x = 0

    return int(x)

def hat0Neg(x):
    global hmin, hmid, hmax

    if x < hmid:
	x = 1
    else:
	x = 0

    return int(x)


# Button mapping config
config = {
	# Controller no. 1
        (0, EV_KEY): {
	    BTN_DPAD_UP: {
		'type' : (0, EV_KEY),
		'code' : 17,
		'value' : None
	    },
	    BTN_DPAD_DOWN: {
		'type' : (0, EV_KEY),
		'code' : 31,
		'value' : None
	    },
	    BTN_DPAD_LEFT: {
		'type' : (0, EV_KEY),
		'code' : 30,
		'value' : None
	    },
	    BTN_DPAD_RIGHT: {
		'type' : (0, EV_KEY),
		'code' : 32,
		'value' : None
	    },
	    BTN_SOUTH: {
		'type' : (0, EV_KEY),
		'code' : 29,
		'value' : None
	    },
	    BTN_B: {
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
	    },
        },
	(0, EV_ABS): {
	    ABS_X: {
		'type' : (0, EV_KEY),
		'code' : 30,
		'value' : digitizeLeft
	    },
	    ABS_Y: {
		'type' : (0, EV_KEY),
		'code' : 17,
		'value' : digitizeUp
	    },
	    ABS_HAT0X: {
		'type' : (0, EV_KEY),
		'code' : 32,
		'value' : hat0Pos
	    },
	    ABS_HAT0Y: {
		'type' : (0, EV_KEY),
		'code' : 31,
		'value' : hat0Pos
	    }
	},
        (1, EV_KEY): {
	    BTN_THUMB: {
		'type' : (0, EV_KEY),
		'code' : 29,
		'value' : None
	    },
	    BTN_THUMB2: {
		'type' : (0, EV_KEY),
		'code' : 42,
		'value' : None
	    },
	    BTN_BASE4: {
		'type' : (0, EV_KEY),
		'code' : 1,
		'value' : None
	    },
	    BTN_BASE3: {
		'type' : (0, EV_KEY),
		'code' : 59,
		'value' : None
	    },
	    KEY_HOMEPAGE: {
		'type' : (0, EV_KEY),
		'code' : 60,
		'value' : None
	    }
        },
	(1, EV_ABS): {
	    ABS_X: {
		'type' : (0, EV_KEY),
		'code' : 32,
		'value' : digitizeRight
	    },
	    ABS_Y: {
		'type' : (0, EV_KEY),
		'code' : 31,
		'value' : digitizeDown
	    },
	    ABS_Z: {
		'type' : (0, EV_KEY),
		'code' : 1,
		'value' : hat0Pos
	    },
	    ABS_RZ: {
		'type' : (0, EV_KEY),
		'code' : 59,
		'value' : hat0Pos
	    },
	    ABS_HAT0X: {
		'type' : (0, EV_KEY),
		'code' : 30,
		'value' : hat0Neg
	    },
	    ABS_HAT0Y: {
		'type' : (0, EV_KEY),
		'code' : 17,
		'value' : hat0Neg
	    }
	},
	# Controller no. 2
        (2, EV_KEY): {
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
	    BTN_DPAD_LEFT: {
		'type' : (0, EV_KEY),
		'code' : 105,
		'value' : None
	    },
	    BTN_DPAD_RIGHT: {
		'type' : (0, EV_KEY),
		'code' : 106,
		'value' : None
	    },
	    BTN_SOUTH: {
		'type' : (0, EV_KEY),
		'code' : 100,
		'value' : None
	    },
	    BTN_B: {
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
	    },
        },
	(2, EV_ABS): {
	    ABS_X: {
		'type' : (0, EV_KEY),
		'code' : 105,
		'value' : digitizeLeft2
	    },
	    ABS_Y: {
		'type' : (0, EV_KEY),
		'code' : 103,
		'value' : digitizeUp2
	    },
	    ABS_HAT0X: {
		'type' : (0, EV_KEY),
		'code' : 106,
		'value' : hat0Pos
	    },
	    ABS_HAT0Y: {
		'type' : (0, EV_KEY),
		'code' : 108,
		'value' : hat0Pos
	    }
	},
        (3, EV_KEY): {
	    BTN_THUMB: {
		'type' : (0, EV_KEY),
		'code' : 100,
		'value' : None
	    },
	    BTN_THUMB2: {
		'type' : (0, EV_KEY),
		'code' : 57,
		'value' : None
	    },
	    BTN_BASE4: {
		'type' : (0, EV_KEY),
		'code' : 1,
		'value' : None
	    },
	    BTN_BASE3: {
		'type' : (0, EV_KEY),
		'code' : 59,
		'value' : None
	    },
	    KEY_HOMEPAGE: {
		'type' : (0, EV_KEY),
		'code' : 60,
		'value' : None
	    }
        },
	(3, EV_ABS): {
	    ABS_X: {
		'type' : (0, EV_KEY),
		'code' : 106,
		'value' : digitizeRight2
	    },
	    ABS_Y: {
		'type' : (0, EV_KEY),
		'code' : 108,
		'value' : digitizeDown2
	    },
	    ABS_Z: {
		'type' : (0, EV_KEY),
		'code' : 1,
		'value' : hat0Pos
	    },
	    ABS_RZ: {
		'type' : (0, EV_KEY),
		'code' : 59,
		'value' : hat0Pos
	    },
	    ABS_HAT0X: {
		'type' : (0, EV_KEY),
		'code' : 105,
		'value' : hat0Neg
	    },
	    ABS_HAT0Y: {
		'type' : (0, EV_KEY),
		'code' : 103,
		'value' : hat0Neg
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
