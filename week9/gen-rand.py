import random

def generate_y_monotone_polygon(n=8, spread=10, filename="input.in"):
    """
    Generates n points forming a y-monotone polygon
    and writes them to input.txt, one 'x y' per line.
    """
    # Step 1: generate random points
    pts = [(random.uniform(0, spread), random.uniform(0, spread)) for _ in range(n)]
    
    # Step 2: sort by y (primary), x (secondary)
    pts.sort(key=lambda p: (p[1], p[0]))
    
    # Step 3: split into two chains by x wrt median or random divider
    mid_index = n // 2
    
    left_chain  = sorted(pts[:mid_index],  key=lambda p: p[1])   # bottom → top
    right_chain = sorted(pts[mid_index:], key=lambda p: -p[1])   # top → bottom

    polygon = left_chain + right_chain

    # Step 4: write to file
    with open(filename, "w") as f:
        for x, y in polygon:
            f.write(f"{round(x,3)} {round(y,3)}\n")

    print(f"Generated y-monotone polygon with {n} vertices at {filename}.")

if __name__ == "__main__":
    generate_y_monotone_polygon()
