#!/usr/bin/python3

import random

n = random.randint(5,20)

eps = 0.1

for i in range (1,n) :
    y1 = random.random() * 9.9
    while (True) :
        y2 = random.random() * 9.9
        if ( abs(y1 -y2) > eps ) :
            break
    x1 = random.random() * 9.9
    x2 = random.random() * 9.9
    print ('%4.1f' % (x1) , '%4.1f' % (y1), '%4.1f' % (x2) , '%4.1f' % (y2))