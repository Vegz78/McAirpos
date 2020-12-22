from ioctlhelp import *

import linux_input

# For uinput version 3

UINPUT_IOCTL_BASE = ord('U')

UI_DEV_CREATE = IO(UINPUT_IOCTL_BASE, 1)
UI_DEV_DESTROY = IO(UINPUT_IOCTL_BASE, 2)

#define UI_DEV_CREATE       _IO(UINPUT_IOCTL_BASE, 1)
#define UI_DEV_DESTROY      _IO(UINPUT_IOCTL_BASE, 2)

UI_SET_EVBIT = IOW(UINPUT_IOCTL_BASE, 100, '@i')
UI_SET_KEYBIT = IOW(UINPUT_IOCTL_BASE, 101, '@i')
UI_SET_RELBIT = IOW(UINPUT_IOCTL_BASE, 102, '@i')
UI_SET_ABSBIT = IOW(UINPUT_IOCTL_BASE, 103, '@i')
UI_SET_MSCBIT = IOW(UINPUT_IOCTL_BASE, 104, '@i')
UI_SET_LEDBIT = IOW(UINPUT_IOCTL_BASE, 105, '@i')
UI_SET_SNDBIT = IOW(UINPUT_IOCTL_BASE, 106, '@i')
UI_SET_FFBIT = IOW(UINPUT_IOCTL_BASE, 107, '@i')
UI_SET_PHYS = IOW(UINPUT_IOCTL_BASE, 108, '@i')
UI_SET_SWBIT = IOW(UINPUT_IOCTL_BASE, 109, '@i')
UI_SET_PROPBIT = IOW(UINPUT_IOCTL_BASE, 110, '@i')

# TODO: Lacking PHYS and PROP
evbits = {
    linux_input.EV_KEY : UI_SET_KEYBIT,
    linux_input.EV_REL : UI_SET_RELBIT,
    linux_input.EV_ABS : UI_SET_ABSBIT,
    linux_input.EV_MSC : UI_SET_MSCBIT,
    linux_input.EV_LED : UI_SET_LEDBIT,
    linux_input.EV_SND : UI_SET_SNDBIT,
    linux_input.EV_FF  : UI_SET_FFBIT,
    linux_input.EV_SW  : UI_SET_SWBIT
}

# TODO
#define UI_BEGIN_FF_UPLOAD  _IOWR(UINPUT_IOCTL_BASE, 200, struct uinput_ff_upload)
#define UI_END_FF_UPLOAD    _IOW(UINPUT_IOCTL_BASE, 201, struct uinput_ff_upload)
#define UI_BEGIN_FF_ERASE   _IOWR(UINPUT_IOCTL_BASE, 202, struct uinput_ff_erase)
#define UI_END_FF_ERASE     _IOW(UINPUT_IOCTL_BASE, 203, struct uinput_ff_erase)

EV_UINPUT = 0x0101

UI_FF_UPLOAD = 1
UI_FF_ERASE = 2

import ctypes
import linux_input

UINPUT_MAX_NAME_SIZE = 80
class uinput_user_dev(ctypes.Structure):
    _fields_ = [
        ("name", ctypes.c_char * UINPUT_MAX_NAME_SIZE),
        ("_id", linux_input.input_id),
        ("ff_effects_max", ctypes.c_uint32),
        ("absmax", ctypes.c_int32 * linux_input.ABS_CNT),
        ("absmin", ctypes.c_int32 * linux_input.ABS_CNT),
        ("absfuzz", ctypes.c_int32 * linux_input.ABS_CNT),
        ("absflat", ctypes.c_int32 * linux_input.ABS_CNT)
    ]
