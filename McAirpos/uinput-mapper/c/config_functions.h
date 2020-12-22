#ifndef H_CONFIG_FUNCTIONS
#define H_CONFIG_FUNCTIONS

#define ADD_INPUT_PATH(INPUT_PATH, NUM) \
    /* Open input and uinput */ \
    in[NUM] = open(INPUT_PATH, O_RDONLY); \
    if(in[NUM] < 0) { \
        perror("Could not open: " INPUT_PATH); \
        return 1; \
    }


#define MAPPER_SET_OPT(opt, bit, device) \
    if (device == j) { \
        printf("SET_OPT: %d, %ld, %d\n", opt, bit, device); \
        if (ioctl(js[device], bit, opt) < 0) { \
            perror("Error in JOYSTICK_SET_OPT"); \
            fprintf(stderr, "ERROR: JOYSTICK_SET_OPT for device %d, opt %s, bit: %s\n", device, #opt, #bit); \
        } else { \
            printf("JOYSTICK_SET_OPT for device %d, opt %s, bit: %s\n", device, #opt, #bit); \
        } \
    }

#define MAPPER_ADD_KEY(key, bit, device) \
    if (device == j) { \
        if (ioctl(js[device], bit, key) < 0) { \
            perror("Error in JOYSTICK_ADD_KEY"); \
            fprintf(stderr, "ERROR: JOYSTICK_ADD_KEY for device %d, key %s, bit: %s\n", device, #key, #bit); \
            return 1; \
        } else { \
            printf("JOYSTICK_ADD_KEY for device %d, key %s, bit: %s\n", device, #key, #bit); \
        } \
    }


#define JOYSTICK_SET_LIM(lim, val, key) \
    uidev.lim[key] = val;

#define LEGAL_VALUE(statement, macro) \
    if (statement) { \
        macro \
    }

#define KEYMAP(in_type, in_key, out_key, out_type, device, in_device, val) \
    if(e.type == in_type && e.code == in_key && (in_device == -1 || in_device == fdrr)) {\
        je.type = out_type; \
        je.code = out_key; \
        je.value = val(e.value); \
        j = device; \
        nowrite = 0;\
    }

#endif
