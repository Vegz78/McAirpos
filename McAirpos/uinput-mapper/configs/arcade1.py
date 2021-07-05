from uinputmapper.cinput import *

"""
Configuration for many EV_ABS(axis) and EV_KEY(digital buttons) directional controllers
... as EV_KEY MakeCode Arcade keyboard device

To optimize for speed on slower devices like the RPi Zero, please:
    1. Remove redundant mappings which do no apply to your controller
    2. Set min and max variables for your controller's axes manually below
"""

# Global variables
# Attempts to dettermine  min and max for EV_ABS events more quickly and automatically if True.
# If False, a full stroke in both direction of one axis it required to determine min and max.
# Min and max values below can be set manually according to evtest to skip auto-calibration altogether.
autoCalibrateOn = True
# Deadzone in percentage before uinput-mapper reacts to EV_ABS events.
# Used to dampen reactions to axis movements around center values
deadzoneFactor = 0.25


# Variables for EV_ABS controller no. 1
invertYUP1 = False  # For inverting Y axis if True, e.g. Nimbus SteelSeries controller
invertXLEFT1 = False  # For inverting X axis if True
min1 = 0  # Seed value = 0 for auto-calibration. If set manual, find min and max using evtest or ../input-read -v -p /dev/input/eventX
max1 = 0  # Seed value = 0 for auto-calibration
mid1 = (min1 + max1)/2
deadzonePos1 = mid1 + (max1 - mid1)*deadzoneFactor
deadzoneNeg1 = mid1 - (mid1 - min1)*deadzoneFactor
# Variables for EV_ABS HAT controller no. 1
hmin1 = 0
hmax1 = 0
hmid1 = (hmin1 + hmax1)/2


# Auto-calibration function
# Determines min, mid and max value intervals and calculates deadzones
def AutoCalibrate(z, min, mid, max, deadzonePos, deadzoneNeg):
    global autoCalibrateOn
    if autoCalibrateOn:
        # Finds most EV_ABS value intervals in 2^x increments in the int16 range from -32767 to 32768
        for i in range(1, 17):
            if abs(z) > (pow(2, i) - 1) - pow(2, i-2):
                continue
            else:
                if i <= 2:
                    min = -1
                    max = 1
                elif min >= 0:
                    min = 0
                    max = pow(2, i) - 1
                elif max <= 0:
                    min = -pow(2, i) + 1
                    max = 0
                else:
                    min = -pow(2, i-1) + 1
                    max = pow(2, i-1) - 1
                break

    mid = (min + max)/2
    deadzonePos = mid + (max - mid)*deadzoneFactor
    deadzoneNeg = mid - (mid - min)*deadzoneFactor

    return min, mid, max, deadzonePos, deadzoneNeg


# Directional EV_ABS to EV_KEY conversion functions for controller no. 1
# For EV_ABS values less than center
def digitizeNeg1(n):
    global min1, mid1, max1, deadzonePos1, deadzoneNeg1

    # Calls calibration function until value interval is found
    if n < min1:
        min1 = n
        min1, mid1, max1, deadzonePos1, deadzoneNeg1 = AutoCalibrate(\
	    n, min1, mid1, max1, deadzonePos1, deadzoneNeg1)

    # Triggers corresponding EV_Key 'code' in config below for wanted EV_ABS range
    if n < deadzoneNeg1:
        return 1

    return 0

# For EV_ABS values greater than center
def digitizePos1(p):
    global min1, mid1, max1, deadzonePos1, deadzoneNeg1

    # Calls calibration function until value interval is found
    if p > max1:
        max1 = p
        min1, mid1, max1, deadzonePos1, deadzoneNeg1 = AutoCalibrate(\
            p, min1, mid1, max1, deadzonePos1, deadzoneNeg1)

    # Triggers corresponding EV_Key 'code' in config below for wanted EV_ABS range
    if p > deadzonePos1:
        return 1

    return 0


# Directional EV_ABS HAT to EV_KEY conversion functions for controller no. 1
# For EV_ABS HAT values less than center
def hat0Neg1(n):
    global hmin1, hmid1, hmax1

    # Calls calibration function until value interval is found
    if n > hmax1:
        hmax1 = n
        dummy = 0
        hmin1, hmid1, hmax1, dummy, dummy = AutoCalibrate(\
            n, hmin1, hmid1, hmax1, dummy, dummy)

    # Triggers corresponding EV_Key 'code' in config below for wanted EV_ABS HAT range
    if n < hmid1:
        return 1

    return 0

# For EV_ABS HAT values greater than center
def hat0Pos1(p):
    global hmin1, hmid1, hmax1

    # Calls calibration function until value interval is found
    if p < hmin1:
        hmin1 = p
        dummy = 0
        hmin1, hmid1, hmax1, dummy, dummy = AutoCalibrate(\
            p, hmin1, hmid1, hmax1, dummy, dummy)

    # Triggers corresponding EV_Key 'code' in config below for wanted EV_ABS HAT range
    if p > hmid1:
        return 1

    return 0


# Button mapping config
# Uses two inputs per controller, to map and convert from EV_ABS to EV_KEY in both directions
config = {
    # EV_ABS to EV_KEY mappings for 1st direction of controller 1
    (0, EV_ABS): {
        # X-axis initially towards less than/negative of center(normally left)
        ABS_X: {
            'type': (0, EV_KEY),
            'code': 30,  # Output key 'a'
            'value': lambda x: digitizeNeg1(x) if not invertXLEFT1 else digitizePos1(x)
            # lambda calls function with joystick value dependent on if axis is inverted or not
        },
        # Y-axis initially towards less than/negative of center(normally up)
        ABS_Y: {
            'type': (0, EV_KEY),
            'code': 17,  # Output key 'w'
            'value': lambda y: digitizeNeg1(y) if not invertYUP1 else digitizePos1(y)
            # lambda calls function with joystick value dependent on if axis is inverted or not
        },
        ABS_HAT0X: {
            # HAT0X-axis initially towards greater than/positive of center(normally right)
            'type': (0, EV_KEY),
            'code': 32,  # etc.
            'value': hat0Pos1  # Joystick value variable x is implied for function hat0Pos
        },
        # HAT0Y-axis initially towards greater than/positive of center(normally down)
        ABS_HAT0Y: {
            'type': (0, EV_KEY),
            'code': 31,
            'value': hat0Pos1
        }
    },
    # EV_ABS to EV_KEY mappings for 2nd direction of controller 1
    (1, EV_ABS): {
        # X-axis initially towards greater than/positive of center(normally right)
        ABS_X: {
            'type': (0, EV_KEY),
            'code': 32,
            'value': lambda x: digitizePos1(x) if not invertXLEFT1 else digitizeNeg1(x)

        },
        # Y-axis initially towards greater than/positive of center(normally down)
        ABS_Y: {
            'type': (0, EV_KEY),
            'code': 31,
            'value': lambda y: digitizePos1(y) if not invertYUP1 else digitizeNeg1(y)
        },
        # HAT0X-axis initially towards less than/negative of center(normally left)
        ABS_HAT0X: {
            'type': (0, EV_KEY),
            'code': 30,
            'value': hat0Neg1
        },
        # HAT0Y-axis initially towards less than/negative of center(normally up)
        ABS_HAT0Y: {
            'type': (0, EV_KEY),
            'code': 17,
            'value': hat0Neg1
        },
        # Start button conversion for some controllers without start button
        ABS_Z: {
            'type': (0, EV_KEY),
            'code': 1,
            'value': hat0Pos1
        },
        # Select button conversion for some controllers without select button
        ABS_RZ: {
            'type': (0, EV_KEY),
            'code': 59,
            'value': hat0Pos1
        }
    },
    # First EV_KEY to EV_KEY mappings for controller 1
    (0, EV_KEY): {
        BTN_DPAD_UP: {
            'type': (0, EV_KEY),
            'code': 17,
            'value': None
        },
        BTN_DPAD_DOWN: {
            'type': (0, EV_KEY),
            'code': 31,
            'value': None
        },
        BTN_DPAD_LEFT: {
            'type': (0, EV_KEY),
            'code': 30,
            'value': None
        },
        BTN_DPAD_RIGHT: {
            'type': (0, EV_KEY),
            'code': 32,
            'value': None
        },
        BTN_SOUTH: {  # (BTN_A synonym)
            'type': (0, EV_KEY),
            'code': 29,
            'value': None
        },
        BTN_B: {
            'type': (0, EV_KEY),
            'code': 42,
            'value': None
        },
        BTN_START: {
            'type': (0, EV_KEY),
            'code': 1,
            'value': None
        },
        BTN_SELECT: {
            'type': (0, EV_KEY),
            'code': 59,
            'value': None
        },
        BTN_MODE: {
            'type': (0, EV_KEY),
            'code': 60,
            'value': None
        }
    },
    # Second EV_KEY to EV_KEY mappings for controller 1
    (1, EV_KEY): {
        BTN_THUMB: {
            'type': (0, EV_KEY),
            'code': 29,
            'value': None
        },
        BTN_THUMB2: {
            'type': (0, EV_KEY),
            'code': 42,
            'value': None
        },
        BTN_BASE4: {
            'type': (0, EV_KEY),
            'code': 1,
            'value': None
        },
        BTN_BASE3: {
            'type': (0, EV_KEY),
            'code': 59,
            'value': None
        },
        KEY_HOMEPAGE: {
            'type': (0, EV_KEY),
            'code': 60,
            'value': None
        }
    },
    # Maps keyboard player 2 to WASD keys
    (2, EV_KEY): {
        KEY_A: {
            'type': (0, EV_KEY),
            'code': 105,
            'value': None
        },
        KEY_D: {
            'type': (0, EV_KEY),
            'code': 106,
            'value': None
        },
        KEY_W: {
            'type': (0, EV_KEY),
            'code': 103,
            'value': None
        },
        KEY_S: {
            'type': (0, EV_KEY),
            'code': 108,
            'value': None
        },
        KEY_LEFTCTRL: {
            'type': (0, EV_KEY),
            'code': 100,
            'value': None
        },
        KEY_LEFTSHIFT: {
            'type': (0, EV_KEY),
            'code': 57,
            'value': None
        },
        KEY_ESC: {
            'type': (0, EV_KEY),
            'code': 1,
            'value': None
        },
        KEY_F1: {
            'type': (0, EV_KEY),
            'code': 59,
            'value': None
        },
        KEY_F2: {
            'type': (0, EV_KEY),
            'code': 60,
            'value': None
        }
    }
}


# Name for created output /dev/input/eventX
names = {
    0: 'MakeCode_Arcade'
}


# Start uinput-mapper on configuration and with the output event name above
def config_merge(c, n):
    c.clear()		# Clear all initial incoming controller mappings
    c.update(config)    # Replace with above mapping configuration
    n.update(names)  # Update output event with the above name
