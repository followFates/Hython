#!/usr/bin/env python
# coding=utf-8
dict = {'lily': 100, 'bob': 88, 'alice': 59}
dict['leijun'] = 99
print dict.items()
print dict['bob'] + 1
print 'The grades are lily: %(lily)d, bob: %(bob)d, alice: %(alice)d' % dict
print dict.keys()
print dict['bob'] + 1
