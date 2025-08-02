#!/usr/bin/python3

import random
import sys

def left_turn ( p1, p2, p3 ):
    (x1 , y1) = p1
    (x2 , y2) = p2
    (x3 , y3) = p3
    return x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2) > 0

def right_turn ( p1, p2, p3 ):
    (x1 , y1) = p1
    (x2 , y2) = p2
    (x3 , y3) = p3
    return x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2) < 0

S = []
for i in range (0,16) :
    x = float('%.1f'%(random.random()*10))
    y = float('%.1f'%(random.random()*10))
    S.append( (x,y) )
S.sort()

n = len(S)
if n < 3 :
   sys.exit(1)

UH = [S[0], S[1]]
LH = [S[0], S[1]]

nu = 2
nl = 2
i = 2
while i < n:
    while (nl > 1) and not left_turn ( LH[nl-2], LH[nl-1], S[i] ) :
        nl -= 1
        LH.pop()
    LH.append(S[i])
    nl += 1
    while (nu > 1) and not right_turn ( UH[nu-2], UH[nu-1], S[i] ):
        nu -= 1
        UH.pop()
    UH.append(S[i])
    nu += 1
    i += 1

#for p in S:
#    print('(%4.2f,' % p[0] + '%4.2f)' % p[1],)
#print()

UH.reverse()
for p in UH:
    print('%3.1f ' % p[0] + '%3.1f' % p[1],)

for p in LH[1:-1]:
    print('%3.1f ' % p[0] + '%3.1f' % p[1],)