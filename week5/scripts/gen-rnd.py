#!/usr/bin/python3

import random

n = random.randint(10,40)

for i in range (1,n) :
    x = random.random() * 11.9 - 1
    y = random.random() * 11.9 - 1
    print ('%4.1f' % (x) , '%4.1f' % (y))
