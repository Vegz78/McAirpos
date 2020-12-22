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

ADD_INPUT_PATH("/dev/input/event8", 0)

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
MAPPER_SET_OPT(EV_REL, UI_SET_EVBIT, 0)

MAPPER_ADD_KEY(BTN_LEFT, UI_SET_KEYBIT, 0)
MAPPER_ADD_KEY(BTN_RIGHT, UI_SET_KEYBIT, 0)
MAPPER_ADD_KEY(BTN_MIDDLE, UI_SET_KEYBIT, 0)
MAPPER_ADD_KEY(REL_X, UI_SET_RELBIT, 0)
MAPPER_ADD_KEY(REL_Y, UI_SET_RELBIT, 0)
MAPPER_ADD_KEY(REL_HWHEEL, UI_SET_RELBIT, 0)
MAPPER_ADD_KEY(REL_WHEEL, UI_SET_RELBIT, 0)
#endif
#endif

/* -------------------------------------------------------------------------- */
/* ----------------------------- THIRD SECTION ----------------------------- */
/* -------------------------------------------------------------------------- */

#ifdef H_JOYMAP
#ifndef H_JOYMAP_SEEN
#define H_JOYMAP_SEEN

KEYMAP(EV_KEY, BTN_LEFT, BTN_LEFT, EV_KEY, 0, -1, +)
KEYMAP(EV_KEY, BTN_RIGHT, BTN_RIGHT, EV_KEY, 0, -1, +)

KEYMAP(EV_REL, REL_X, REL_X, EV_REL, 0, -1, +)
KEYMAP(EV_REL, REL_Y, REL_Y, EV_REL, 0, -1, +)
    /*
KEYMAP(EV_KEY, KEY_3, BTN_LEFT, EV_KEY, 0, -1, +)
KEYMAP(EV_KEY, KEY_M, REL_X, EV_REL, 0, -1, 10*)
KEYMAP(EV_KEY, KEY_N, REL_X, EV_REL, 0, -1, -10*)
KEYMAP(EV_KEY, KEY_J, REL_Y, EV_REL, 0, -1, 10*)
KEYMAP(EV_KEY, KEY_K, REL_Y, EV_REL, 0, -1, -10*)
*/

#endif
#endif
