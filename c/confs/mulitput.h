/*
 * Copyright Merlijn Wajer 2012
 *
 * This is the uinput-mapper configuration file.
 *
 * We are still working out all the details, but it basically boils down to the
 * following: You write the configuration entirely in the C preprocessor by
 * adding the right commands (macros) to the right sections.
 *
 *
 * As of now, there are three sections:
 *
 * - GLOBAL_MAP;
 *   This is where you set the amount of devices to be emulated (currently
 *   called JOYCOUNT) and the INPUT_PATH.
 *
 * - CONFIGURE_JOYSTICKS:
 *   This is where you tell uinput-mapper what buttons your new joysticks (or
 *   other devices) should expose. Any button not exposed here will never be
 *   send.
 *
 *   Macros that make sense to use here:
 *   - JOYSTICK_SET_OPT(<opt>, <bit>, <device>)
 *   - JOYSTICK_ADD_KEY(<key>, <bit to set>, <device>)
 *   - JOYSTICK_SET_LIM(<absmin|absmax>, <value>, <key>)
 *
 *   JOYSTICK_SET_LIM is mostly used for ABS_HATs.
 *
 * - JOYMAP:
 *
 *   Set the key mappings here.
 *
 *   Macros that make sense here:
 *   - KEYMAP(in_type, in_key, out_key, out_type, device, in_device, val) \
 *      - in_type: Key type of input key
 *      - in_key: Keycode of input key
 *      - out_key: Key code of output key
 *      - device: output device
 *      - in_device: Device to accept keys from. -1 for all devices.
 *      - val: function or statement applied to value.
 *
 * Notes:
 * - To expose a joystick device, expose the BTN_JOYSTICK ``button'' with
 *   JOYSTICK_ADD_KEY
 * - To expose a mouse device, expose (at least) the BTN_LEFT button with
 *   JOYSTICK_ADD_KEY
 *
 * ----------------------------------------------------------------------------
 */

#include "config_functions.h"

/* -------------------------------------------------------------------------- */
/* ----------------------------- FIRST SECTION ----------------------------- */
/* -------------------------------------------------------------------------- */

#ifndef H_GLOBAL_MAP
#define H_GLOBAL_MAP

/* Set up amount of joysticks here */
#define JOYCOUNT 2

/* Set up amount of input devices here */
#define INPUT_DEVICE_COUNT 2

#define INPUT_DEVICE_ANY -1

#endif

/* -------------------------------------------------------------------------- */
/* ------------------- FIRST AND A HALF SECTION ----------------------------- */
/* -------------------------------------------------------------------------- */

#ifdef H_CONFIGURE_EVENTS
#ifndef H_CONFIGURE_EVENTS_SEEN
#define H_CONFIGURE_EVENTS_SEEN

ADD_INPUT_PATH("/dev/input/event4", 0)
ADD_INPUT_PATH("/dev/input/event5", 1)

#endif
#endif

/* -------------------------------------------------------------------------- */
/* ----------------------------- SECOND SECTION ----------------------------- */
/* -------------------------------------------------------------------------- */

#ifdef H_CONFIGURE_JOYSTICKS
#ifndef H_CONFIGURE_JOYSTICKS_SEEN
#define H_CONFIGURE_JOYSTICKS_SEEN


/* Configure first joystick.
 *
 * Here we just tell the program what keys event we will expose and what
 * keys we want to use.
 *
 * If a key is not enabled here, it will never be passed.
 */

/* We want to send ABS and KEY events */
MAPPER_SET_OPT(EV_ABS, UI_SET_EVBIT, 0)
MAPPER_SET_OPT(EV_KEY, UI_SET_EVBIT, 0)

/* Hats:
 * We set the absmax and absmin; otherwise the hats make no sense.
 */
MAPPER_ADD_KEY(ABS_HAT0X, UI_SET_ABSBIT, 0)
JOYSTICK_SET_LIM(absmax, 1, ABS_HAT0X)
JOYSTICK_SET_LIM(absmin, -1, ABS_HAT0X)
MAPPER_ADD_KEY(ABS_HAT0Y, UI_SET_ABSBIT, 0)
JOYSTICK_SET_LIM(absmax, 1, ABS_HAT0Y)
JOYSTICK_SET_LIM(absmin, -1, ABS_HAT0Y)

/* XXX: ALWAYS SET BTN_JOYSTICK TO EXPOSE A JOYSTICK DEVICE */
MAPPER_ADD_KEY(BTN_JOYSTICK, UI_SET_KEYBIT, 0)

/* Buttons. */
MAPPER_ADD_KEY(BTN_0, UI_SET_KEYBIT, 0)
MAPPER_ADD_KEY(BTN_1, UI_SET_KEYBIT, 0)
MAPPER_ADD_KEY(BTN_2, UI_SET_KEYBIT, 0)
MAPPER_ADD_KEY(BTN_3, UI_SET_KEYBIT, 0)

/* Second joystick ; same comments as first one */
MAPPER_SET_OPT(EV_ABS, UI_SET_EVBIT, 1)
MAPPER_SET_OPT(EV_KEY, UI_SET_EVBIT, 1)

MAPPER_ADD_KEY(ABS_HAT0X, UI_SET_ABSBIT, 1)
JOYSTICK_SET_LIM(absmax, 1, ABS_HAT0X)
JOYSTICK_SET_LIM(absmin, -1, ABS_HAT0X)

MAPPER_ADD_KEY(ABS_HAT0Y, UI_SET_ABSBIT, 1)
JOYSTICK_SET_LIM(absmax, 1, ABS_HAT0X)
JOYSTICK_SET_LIM(absmin, -1, ABS_HAT0X)

MAPPER_ADD_KEY(BTN_JOYSTICK, UI_SET_KEYBIT, 1)

MAPPER_ADD_KEY(BTN_0, UI_SET_KEYBIT, 1)
MAPPER_ADD_KEY(BTN_1, UI_SET_KEYBIT, 1)
MAPPER_ADD_KEY(BTN_2, UI_SET_KEYBIT, 1)
MAPPER_ADD_KEY(BTN_3, UI_SET_KEYBIT, 1)

#endif
#endif

/* -------------------------------------------------------------------------- */
/* ----------------------------- THIRD SECTION ----------------------------- */
/* -------------------------------------------------------------------------- */

#ifdef H_JOYMAP
#ifndef H_JOYMAP_SEEN
#define H_JOYMAP_SEEN

/* First joystick */

LEGAL_VALUE(e.value == 1 || e.value == 0,
    /* HAT */
    KEYMAP(EV_KEY, KEY_UP, ABS_HAT0Y, EV_ABS, 0, INPUT_DEVICE_ANY, -)
    KEYMAP(EV_KEY, KEY_DOWN, ABS_HAT0Y, EV_ABS, 0, INPUT_DEVICE_ANY, +)
    KEYMAP(EV_KEY, KEY_LEFT, ABS_HAT0X, EV_ABS, 0, INPUT_DEVICE_ANY, -)
    KEYMAP(EV_KEY, KEY_RIGHT, ABS_HAT0X, EV_ABS, 0, INPUT_DEVICE_ANY, +)

    /* Red buttons */
    KEYMAP(EV_KEY, KEY_LEFTCTRL, BTN_0, EV_KEY, 0, INPUT_DEVICE_ANY, +)
    KEYMAP(EV_KEY, KEY_LEFTALT, BTN_1, EV_KEY, 0, INPUT_DEVICE_ANY, +)
    KEYMAP(EV_KEY, KEY_SPACE, BTN_2, EV_KEY, 0, INPUT_DEVICE_ANY, +)

    /* Yellow button */
    KEYMAP(EV_KEY, KEY_1, BTN_3, EV_KEY, 0, INPUT_DEVICE_ANY, +)
)

/* Second joystick */

LEGAL_VALUE(e.value == 1 || e.value == 0,
    /* HAT */
    KEYMAP(EV_KEY, KEY_R, ABS_HAT0Y, EV_ABS, 1, INPUT_DEVICE_ANY, -)
    KEYMAP(EV_KEY, KEY_F, ABS_HAT0Y, EV_ABS, 1, INPUT_DEVICE_ANY, +)
    KEYMAP(EV_KEY, KEY_D, ABS_HAT0X, EV_ABS, 1, INPUT_DEVICE_ANY, -)
    KEYMAP(EV_KEY, KEY_G, ABS_HAT0X, EV_ABS, 1, INPUT_DEVICE_ANY, +)

    /* Red buttons */
    KEYMAP(EV_KEY, KEY_A, BTN_0, EV_KEY, 1, INPUT_DEVICE_ANY, +)
    KEYMAP(EV_KEY, KEY_S, BTN_1, EV_KEY, 1, INPUT_DEVICE_ANY, +)
    KEYMAP(EV_KEY, KEY_Q, BTN_2, EV_KEY, 1, INPUT_DEVICE_ANY, +)

    /* Yellow button */
    KEYMAP(EV_KEY, KEY_2, BTN_3, EV_KEY, 1, INPUT_DEVICE_ANY, +)
)

#endif
#endif
