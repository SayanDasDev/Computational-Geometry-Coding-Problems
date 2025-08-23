#!/usr/bin/python3

import random

input_type = random.randrange(0,6)
eps = 0.001

match input_type:
    case 0:
        x = random.random()
        y = random.random()
        for i in range (0,5) :
            eps_x = eps * random.random()
            eps_y = eps * random.random()
            print('%7.5f' % (x + eps_x), '%7.5f' % (y + eps_y))
        for i in range (0,5) :
            eps_x = eps * random.random()
            eps_y = eps * random.random()
            print('%7.5f' % (random.random() + eps_x), '%7.5f' % (y + eps_y))
        for i in range (0,5) :
            eps_x = eps * random.random()
            eps_y = eps * random.random()
            print('%7.5f' % (x + eps_x), '%7.5f' % (random.random() + eps_y))
        for i in range (0,5) :
            eps_x = eps * random.random()
            eps_y = eps * random.random()
            print('%7.5f' % (random.random() + eps_x), '%7.5f' % (random.random() + eps_y))
    case 1:
        x = random.random()
        for i in range (0,10) :
            eps_x = eps * random.random()
            eps_y = eps * random.random()
            print('%7.5f' % (x + eps_x), '%7.5f' % (random.random() + eps_y))
    case 2:
        y = random.random()
        for i in range (0,10) :
            eps_x = eps * random.random()
            eps_y = eps * random.random()
            print('%7.5f' % (random.random() + eps_x), '%7.5f' % (y + eps_y))
    case 3:
        x = random.random()
        y = random.random()
        for i in range (0,10) :
            eps_x = eps * random.random()
            eps_y = eps * random.random()
            print('%7.5f' % (x + eps_x), '%7.5f' % (y + eps_y))
    case 4:
        n = random.randrange(0,5)
        for i in range (0,n) :
            print('%7.5f' % random.random(), '%7.5f' % random.random())
    case 5:
        m = random.random()
        c = random.random()
        for i in range (0,10) :
            eps_x = eps * random.random()
            eps_y = eps * random.random()
            x = random.random()
            y = m * x + c
            print('%7.5f' % (x + eps_x), '%7.5f' % (y + eps_y))


