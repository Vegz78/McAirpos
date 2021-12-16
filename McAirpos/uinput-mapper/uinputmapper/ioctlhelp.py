# See linux/include/uapi/asm-generic/ioctl.h

import struct

_IOC_NRBITS = 8
_IOC_TYPEBITS = 8

# According to linux these *may* be architecture specific
_IOC_SIZEBITS = 14
_IOC_DIRBITS = 2

_IOC_NRMASK = (1 << _IOC_NRBITS) - 1
_IOC_TYPEMASK = (1 << _IOC_TYPEBITS) - 1
_IOC_SIZEMASK = (1 << _IOC_SIZEBITS) - 1
_IOC_DIRMASK = (1 << _IOC_DIRBITS) - 1

_IOC_NRSHIFT = 0
_IOC_TYPESHIFT = _IOC_NRSHIFT + _IOC_NRBITS
_IOC_SIZESHIFT = _IOC_TYPESHIFT + _IOC_TYPEBITS
_IOC_DIRSHIFT = _IOC_SIZESHIFT + _IOC_SIZEBITS

_IOC_NONE = 0
_IOC_WRITE = 1
_IOC_READ = 2


def IOC(_dir, _type, nr, size):
#    if type(size) in (str, unicode):
    if type(size) in (str, u''.__class__):
        size = struct.calcsize(size)
    return _dir << _IOC_DIRSHIFT | _type << _IOC_TYPESHIFT | \
            nr << _IOC_NRSHIFT | size << _IOC_SIZESHIFT


IO = lambda _type, nr: IOC(_IOC_NONE, _type, nr, 0)
IOR = lambda _type, nr, size: IOC(_IOC_READ, _type, nr, size)
IOW = lambda _type, nr, size: IOC(_IOC_WRITE, _type, nr, size)
IORW = lambda _type, nr, size: IOC(_IOC_READ | _IOC_WRITE, _type, nr, size)
