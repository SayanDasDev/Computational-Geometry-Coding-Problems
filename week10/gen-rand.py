import random
import argparse

"""
Random Half-Plane Generator
Outputs lines of:
    a  b  c
meaning:
    a*x + b*y + c <= 0
"""

def rand_coeff(rangeA):
    # avoid all-zero line
    while True:
        a = random.uniform(-rangeA, rangeA)
        b = random.uniform(-rangeA, rangeA)
        if abs(a) > 1e-9 or abs(b) > 1e-9:
            break
    c = random.uniform(-rangeA, rangeA)
    return a, b, c

def generate_random(n, coeff_range):
    hp = []
    for _ in range(n):
        hp.append(rand_coeff(coeff_range))
    return hp


def write_to_file(hp, outfile):
    with open(outfile, "w") as f:
        for (a, b, c) in hp:
            f.write(f"{a} {b} {c}\n")


def main():
    parser = argparse.ArgumentParser(description="Generate random half-plane constraints")
    parser.add_argument("--n", type=int, default=5, help="number of half-planes")
    parser.add_argument("--range", type=float, default=10.0, help="coefficient range")
    parser.add_argument("--out", type=str, default="input.in", help="output filename")
    parser.add_argument("--seed", type=int, default=None, help="seed for reproducibility")

    args = parser.parse_args()

    if args.seed is not None:
        random.seed(args.seed)

    hp = generate_random(args.n, args.range)

    write_to_file(hp, args.out)

    print(f"Generated {args.n} half-planes → {args.out}")


if __name__ == "__main__":
    main()
