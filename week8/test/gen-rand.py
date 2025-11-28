import random
import math

def generate_random_polygon():
    n = random.randint(4, 10)
    
    angles = sorted([random.random() * 2 * math.pi for _ in range(n)])
    
    radii = [random.uniform(1, 10) for _ in range(n)]
    
    points = [(r * math.cos(a), r * math.sin(a)) for r, a in zip(radii, angles)]
    
    points = [(round(x, 2), round(y, 2)) for x, y in points]
    
    for x, y in points:
        print(x, y)

generate_random_polygon()
