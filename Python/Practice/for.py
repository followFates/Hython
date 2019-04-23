#!/usr/bin/env python
# coding=utf-8
bat = {'b': 'baidu', 't': 'tencent', 'a': 'alibaba'}
for value in bat.values():
    print value

for key in bat:
    print key

for k, v in bat.items():
    print k, '>', v
