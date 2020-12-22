/* Terminal fixer, based on 
 * https://gist.github.com/joerick/9e2d244f456c2431619e7063eda62e1d
 *
 * Restore a terminal messed up by a semi-graphical application such as a
 * SDL framebuffer app not cleaning up after itself.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/vt.h>
#include <linux/kd.h>
#include <linux/keyboard.h>

int main(int argc, char** argv) {
    char* path = "/dev/tty";
    if (argc == 2) {
        path = argv[1];
    } else if (argc > 2) {
        printf("usage: termfix [/path/to/tty]\n");
        return 1;
    }

    int fd = open(path, O_RDWR, 0);
    if (fd < 0) {
        perror("unable to open tty");
        return 1;
    }

    if (ioctl(fd, VT_UNLOCKSWITCH, 1) < 0) {
        perror("warn: ioctl VT_UNLOCKSWITCH failed");
    }

    if (ioctl(fd, KDSETMODE, KD_TEXT) < 0) {
        perror("warn: ioctl KDSETMODE failed");
    }

    if (ioctl(fd, KDSKBMODE, K_XLATE) < 0) {
        perror("warn: ioctl KBSKBMODE failed");
    }

    return 0;
}
