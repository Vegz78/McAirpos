#include "config_functions.h"

#ifndef H_GLOBAL_MAP
#define H_GLOBAL_MAP

/* Set up amount of joysticks here */
#define JOYCOUNT 1

/* Set up amount of input devices here */
#define INPUT_DEVICE_COUNT 1

#endif

/* -------------------------------------------------------------------------- */
/* ------------------- FIRST AND A HALF SECTION ----------------------------- */
/* -------------------------------------------------------------------------- */

#ifdef H_CONFIGURE_EVENTS
#ifndef H_CONFIGURE_EVENTS_SEEN
#define H_CONFIGURE_EVENTS_SEEN

ADD_INPUT_PATH("/dev/input/event9", 0)

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

/* Mouse */
MAPPER_SET_OPT(EV_KEY, UI_SET_EVBIT, 0)
MAPPER_ADD_KEY(KEY_RIGHTMETA, UI_SET_KEYBIT, 0)
#endif
#endif

/* -------------------------------------------------------------------------- */
/* ----------------------------- THIRD SECTION ----------------------------- */
/* -------------------------------------------------------------------------- */

#ifdef H_JOYMAP
#ifndef H_JOYMAP_SEEN
#define H_JOYMAP_SEEN

KEYMAP(EV_KEY, BTN_MIDDLE, KEY_RIGHTMETA, EV_KEY, 0, -1, +)

#endif
#endif
