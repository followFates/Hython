#!/usr/bin/env python
# coding=utf-8
n = raw_input()
first = 0
second = 1
i = 1
while i < int(n):
    first, second = second, first + second
    i = i + 1
print second

