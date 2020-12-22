API
===

Documentation is incomplete; for now, see
http://hetgrotebos.org/wiki/uinput-mapper

Introduction to Input and UInput
--------------------------------



Events
------



.. code-block:: python

    class input_event(ctypes.Structure):
        _fields_ = [
            ("time", timeval),
            ("type", ctypes.c_uint16),
            ("code", ctypes.c_uint16),
            ("value", ctypes.c_int32)
        ]

CInput
------

.. automodule:: cinput
    :members:

ioctl helper
------------

.. automodule:: ioctlhelp
    :members:


Mapper
------

.. automodule:: mapper
    :members:


