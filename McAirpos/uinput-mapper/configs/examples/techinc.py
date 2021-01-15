from uinputmapper.cinput import *
from uinputmapper.mapper import pretty_conf_print


"""
Configuration for a simple Microsoft SideWinter Game Pad Pro USB version 1.0
... as ABS input pointer device
"""

config = {
    (0, EV_KEY) : {
        KEY_UP : {
            'type' : (0, EV_ABS), 'code' : ABS_HAT0Y, 'value' : lambda _: -_,
            'prop' : { 'min' : -1, 'max' : 1, 'flat' : 0, 'fuzz' : 0 }
        },
        KEY_DOWN : {
            'type' : (0, EV_ABS), 'code' : ABS_HAT0Y, 'value' : lambda _: +_,
            'prop' : { 'min' : -1, 'max' : 1, 'flat' : 0, 'fuzz' : 0 }
        },
        KEY_LEFT : {
            'type' : (0, EV_ABS), 'code' : ABS_HAT0X, 'value' : lambda _: -_,
            'prop' : { 'min' : -1, 'max' : 1, 'flat' : 0, 'fuzz' : 0 }
        },
        KEY_RIGHT : {
            'type' : (0, EV_ABS), 'code' : ABS_HAT0X, 'value' : lambda _: +_,
            'prop' : { 'min' : -1, 'max' : 1, 'flat' : 0, 'fuzz' : 0 }
        },
        KEY_LEFTCTRL : {
            'type' : (0, EV_KEY), 'code' : BTN_0, 'value' : None
        },
        KEY_LEFTALT: {
            'type' : (0, EV_KEY), 'code' : BTN_1, 'value' : None
        },
        KEY_SPACE : {
            'type' : (0, EV_KEY), 'code' : BTN_2, 'value' : None
        },
        KEY_1 : {
            'type' : (0, EV_KEY), 'code' : BTN_3, 'value' : None
        },
        KEY_4 : {
            'type' : (0, EV_KEY), 'code' : BTN_JOYSTICK, 'value' : None
        },

        KEY_R : {
            'type' : (1, EV_ABS), 'code' : ABS_HAT0Y, 'value' : lambda _: -_,
            'prop' : { 'min' : -1, 'max' : 1, 'flat' : 0, 'fuzz' : 0 }
        },
        KEY_F: {
            'type' : (1, EV_ABS), 'code' : ABS_HAT0Y, 'value' : lambda _: +_,
            'prop' : { 'min' : -1, 'max' : 1, 'flat' : 0, 'fuzz' : 0 }
        },
        KEY_D : {
            'type' : (1, EV_ABS), 'code' : ABS_HAT0X, 'value' : lambda _: -_,
            'prop' : { 'min' : -1, 'max' : 1, 'flat' : 0, 'fuzz' : 0 }
        },
        KEY_G: {
            'type' : (1, EV_ABS), 'code' : ABS_HAT0X, 'value' : lambda _: +_,
            'prop' : { 'min' : -1, 'max' : 1, 'flat' : 0, 'fuzz' : 0 }
        },
        KEY_A : {
            'type' : (1, EV_KEY), 'code' : BTN_0, 'value' : None
        },
        KEY_S: {
            'type' : (1, EV_KEY), 'code' : BTN_1, 'value' : None
        },
        KEY_Q : {
            'type' : (1, EV_KEY), 'code' : BTN_2, 'value' : None
        },
        KEY_2 : {
            'type' : (1, EV_KEY), 'code' : BTN_3, 'value' : None
        },
        KEY_3 : {
            'type' : (1, EV_KEY), 'code' : BTN_JOYSTICK, 'value' : None
        },

    }
}

names = {
    0 : 'Joystick 0',
    1 : 'Joystick 1',
}

def config_merge(c, n):
    pretty_conf_print(c)
    print '-' * 80
    c.clear()
    print '-' * 80
    pretty_conf_print(c)
    c.update(config)
    print '-' * 80
    pretty_conf_print(c)

    n.update(names)
