import ctypes

import struct

from uinput_gen import input_constants_dict as icd

for k, v in icd.iteritems():
    locals()[k] = v

rdict = lambda x: dict(map(lambda (k, v): (v, k), x.iteritems()))

events = dict(filter(lambda (k, v): k in ["EV_SYN", "EV_KEY", "EV_REL",
    "EV_ABS", "EV_MSC", "EV_SW", "EV_LED", "EV_SND", "EV_REP",
    "EV_FF", "EV_PWR", "EV_FF_STATUS"], icd.iteritems()))
rev_events = rdict(events)

filter_event = lambda c: dict(filter(lambda (k, v): c(k), icd.iteritems()))

keys = filter_event(lambda x: x.startswith("KEY_") or x.startswith("BTN_"))
rev_keys = rdict(keys)

absaxes = filter_event(lambda x: x.startswith("ABS_"))
rev_absaxes = rdict(absaxes)

rel = filter_event(lambda x: x.startswith("REL_"))
rev_rel = rdict(rel)

syn = filter_event(lambda x: x.startswith("SYN_"))
rev_syn = rdict(syn)

misc = filter_event(lambda x: x.startswith("MSC_"))
rev_misc = rdict(misc)

leds = filter_event(lambda x: x.startswith("LED_"))
rev_leds = rdict(leds)

sounds = filter_event(lambda x: x.startswith("SND_"))
rev_sounds = rdict(sounds)

repeats = filter_event(lambda x: x.startswith("REP_"))
rev_repeats = rdict(repeats)

switches = filter_event(lambda x: x.startswith("SW_"))
rev_switches = rdict(switches)


force, forcestatus = {}, {}
rev_force = rdict(force)
rev_forcestatus = rdict(forcestatus)

del rdict

event_keys = {
        EV_SYN: syn,
        EV_KEY: keys,
        EV_REL: rel,
        EV_ABS: absaxes,
        EV_MSC: misc,
        EV_LED: leds,
        EV_SND: sounds,
        EV_REP: repeats,
        EV_SW:  switches,
        EV_FF:  force,
        EV_FF_STATUS: forcestatus
}

rev_event_keys = {
        EV_SYN: rev_syn,
        EV_KEY: rev_keys,
        EV_REL: rev_rel,
        EV_ABS: rev_absaxes,
        EV_MSC: rev_misc,
        EV_LED: rev_leds,
        EV_SND: rev_sounds,
        EV_REP: rev_repeats,
        EV_SW:  rev_switches,
        EV_FF:  rev_force,
        EV_FF_STATUS: rev_forcestatus
}

class timeval(ctypes.Structure):
    _fields_ = [("tv_sec", ctypes.c_long), ("tv_usec", ctypes.c_long)]

class input_event(ctypes.Structure):
    _fields_ = [
        ("time", timeval),
        ("type", ctypes.c_uint16),
        ("code", ctypes.c_uint16),
        ("value", ctypes.c_int32)
    ]

class input_id(ctypes.Structure):
    _fields_ = [
        ("bustype", ctypes.c_uint16),
        ("vendor", ctypes.c_uint16),
        ("product", ctypes.c_uint16),
        ("version", ctypes.c_uint16),
    ]

class input_absinfo(ctypes.Structure):
    _fields_ = [
        ("value", ctypes.c_int32),
        ("minimum", ctypes.c_int32),
        ("maximum", ctypes.c_int32),
        ("fuzz", ctypes.c_int32),
        ("flat", ctypes.c_int32),
        ("resolution", ctypes.c_int32)
    ]


from ioctlhelp import IOR, IOW, IOC, IO, _IOC_READ

# Get driver version
EVIOCGVERSION = IOR(ord('E'), 0x01, '@i')

# Get device ID
#EVIOCGID = IOR(ord('E'), 0x02, struct input_id)

# Get repeat settings
EVIOCGREP = IOR(ord('E'), 0x03, '@ii')
# Set repeat settings
EVIOCSREP = IOW(ord('E'), 0x03, '@ii')

# Get keycode
EVIOCGKEYCODE = IOR(ord('E'), 0x04, '@ii')
# EVIOCGKEYCODE_V2 _IOR('E', 0x04, struct input_keymap_entry)

# Set keycode

EVIOCSKEYCODE = IOW(ord('E'), 0x04, '@ii')
#EVIOCSKEYCODE_V2 _IOW('E', 0x04, struct input_keymap_entry)

# Get device name
EVIOCGNAME = lambda _len: IOC(_IOC_READ, ord('E'), 0x06,
        struct.calcsize('%ds' % _len))

# Get physical location
EVIOCGPHYS= lambda _len: IOC(_IOC_READ, ord('E'), 0x07,
        struct.calcsize('%ds' % _len))

# Get unique identifier
EVIOCGUNIQ = lambda _len: IOC(_IOC_READ, ord('E'), 0x08,
        struct.calcsize('%ds' % _len))

# Get device properties
EVIOCGPROP = lambda _len: IOC(_IOC_READ, ord('E'), 0x09,
        struct.calcsize('%ds' % _len))

#EVIOCGMTSLOTS(len)	_IOC(_IOC_READ, 'E', 0x0a, len)
#
#EVIOCGKEY(len)		_IOC(_IOC_READ, 'E', 0x18, len)		/* get global key state */
#EVIOCGLED(len)		_IOC(_IOC_READ, 'E', 0x19, len)		/* get all LEDs */
#EVIOCGSND(len)		_IOC(_IOC_READ, 'E', 0x1a, len)		/* get all sounds status */
#EVIOCGSW(len)		_IOC(_IOC_READ, 'E', 0x1b, len)		/* get all switch states */


# Get event bits
EVIOCGBIT = lambda ev, _len: IOC(_IOC_READ, ord('E'), 0x20 + ev, _len)

EVIOCGABS = lambda _abs: IOR(ord('E'), 0x40 + _abs, ctypes.sizeof(input_absinfo))
#EVIOCGABS(abs)		_IOR('E', 0x40 + (abs), struct input_absinfo)	/* get abs value/limits */
#EVIOCSABS(abs)		_IOW('E', 0xc0 + (abs), struct input_absinfo)	/* set abs value/limits */
#
#EVIOCSFF		_IOC(_IOC_WRITE, 'E', 0x80, sizeof(struct ff_effect))	/* send a force effect to a force feedback device */
#EVIOCRMFF		_IOW('E', 0x81, int)			/* Erase a force effect */
#EVIOCGEFFECTS		_IOR('E', 0x84, int)			/* Report number of effects playable at the same time */


# Grab/Release device
EVIOCGRAB = IOW(ord('E'), 0x90, 'i')

#
#EVIOCSCLOCKID		_IOW('E', 0xa0, int)			/* Set clockid to be used for timestamps */
