import random

def generate_input_file(filename, d, n):
    """
    Generates an input file for the randomized LP solver.
    Format:
    <c1> <c2> ... <cd> (Objective Vector)
    <blank line>
    <a1> <a2> ... <ad> <b> (Constraint 1)
    ...
    <a1> <a2> ... <ad> <b> (Constraint n)
    """
    with open(filename, 'w') as f:
        # 1. Write Objective Vector (c) FIRST
        # Logic: Minimize c^T x
        # The parser detects dimension 'd' based on the count of numbers here.
        c_coeffs = [random.uniform(-10.0, 10.0) for _ in range(d)]
        c_line = " ".join([f"{x:.4f}" for x in c_coeffs]) + "\n"
        f.write(c_line)
        
        # 2. Write Blank Line Separator
        f.write("\n")
        
        # 3. Write Constraints (A matrix and b vector)
        # Format: a1 ... ad b
        # Logic: Ax <= b
        for i in range(n):
            coeffs = []
            # Generate random coefficients for A
            for _ in range(d):
                coeffs.append(random.uniform(-10.0, 10.0))
            
            # Generate b (RHS)
            # CRITICAL: We generate a positive 'b' (1 to 20) to ensure 
            # the origin (0,0...) is always inside the feasible region.
            # If b were negative, the origin might not be feasible, making
            # the problem harder to initialize without a "Phase 1" solver.
            b = random.uniform(1.0, 20.0) 
            
            # Write line: a[0] ... a[d-1] b
            line = " ".join([f"{x:.4f}" for x in coeffs]) + f" {b:.4f}\n"
            f.write(line)

    print(f"Successfully created '{filename}'")
    print(f"Format: {d} dimensions (detected from line 1), {n} constraints.")

if __name__ == "__main__":
    # --- Configuration ---
    FILENAME = "input.in" 
    DIMENSIONS = 2        # d: The new parser will auto-detect this from the first line
    CONSTRAINTS = 15      # n: The parser will read until EOF

    generate_input_file(FILENAME, DIMENSIONS, CONSTRAINTS)