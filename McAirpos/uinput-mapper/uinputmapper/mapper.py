# encoding: utf-8
import cinput

"""
Module to help out with config parsing and input mapping
"""

def parse_conf(f, devname):
    """
    Reads in input devices and returns a config that contains all the events
    exported by the device.

    devname specificies the idx of this input device
    """
    conf = {}
    e = f.get_exposed_events()
    for k, v in e.iteritems():
        t = cinput.events[k]
        if t == cinput.EV_SYN:
            continue

        conf[(devname, t)] = {}

        for key in v:
            tt = cinput.event_keys[t][key]
            conf[(devname, t)][tt] = {
                'type' : (devname, t),
                'code' : tt,
                'value' : None
                #'value' : lambda x: x
            }

            if t == cinput.EV_ABS:
                p = f.get_absprop(tt)
                conf[(devname, t)][tt]['prop'] = {
                        'max' : p.maximum,
                        'min' : p.minimum,
                        'fuzz' : p.fuzz,
                        'flat' : p.flat
                }


    return conf


def pretty_conf_print(c):
    """
    Function to print an entire configuration
    """
    for k, v in c.iteritems():
        print 'Input:', k[0], 'Type:', cinput.rev_events[k[1]]
        for kk, vv in v.iteritems():
            n_ev_d, n_ev_t = vv['type']
            print ' ' * 4,
            print cinput.rev_event_keys[k[1]][kk],
            print ' → ([%d, %s], %s)' % (n_ev_d,
                cinput.rev_events[n_ev_t],
                cinput.rev_event_keys[n_ev_t][vv['code']])

            if n_ev_t == cinput.EV_ABS:
                print 'Properties: Max: %d Min: %d Fuzz: %d Flat: %d' % (
                        vv['prop']['max'], vv['prop']['min'],
                        vv['prop']['fuzz'], vv['prop']['flat'])

def get_exported_device_count(c):
    """
    Iterate dictionary to find out how many devices are exported.
    (Rather simple at the moment)
    """
    m = 0
    for _, v in c.iteritems():
        for _, o in v.iteritems():
            m = max(m, o['type'][0])

    return m + 1


class KeyMapper(object):
    def __init__(self, config):
        self._config = config

    def map_event(self, ev, fd):
        """
        Maps an event *ev* from fd *fd* to a possibly different event and output
        fd *ofd*.
        """
        _type = ev.type
        ofd = fd

        if (fd, _type) in self._config:
            typemaps = self._config[(fd, _type)]
            if ev.code in typemaps:
                info = typemaps[ev.code]
                ofd, ev.type = info['type']
                ev.code = info['code']
                if info['value'] is not None:
                    ev.value = info['value'](ev.value)
                else:
                    ev.value = ev.value

        return ofd, ev

    def expose(self, d, fd):
        """
        Expose exposes events to a uinput-device *d* with index *fd* from the
        config passed to __init__.
        """
        for (n, evt), v in self._config.iteritems():
            for code, dat in v.iteritems():
                ofd, t = dat['type']
                if ofd != fd:
                    continue
                d.expose_event_type(t)
                d.expose_event(t, dat['code'])

                if t == cinput.EV_ABS:
                    p = dat['prop']
                    d.set_absprop(dat['code'], _max=p['max'], _min=p['min'],
                            fuzz=p['fuzz'], flat=p['flat'])
