#!/usr/bin/env python3
"""
gen-multi-faces.py

Generates:
  - point.in
  - segment.in
  - region.in

Format matches your parser (Option A):
For each face:
  <face-id>
  <outer vertex ids (space separated)>
  <hole vertex ids OR blank line>

Behavior:
 - Produces multiple faces.
 - Each face is a simple polygon (vertices placed around a center, sorted by angle).
 - Each face may optionally have at most one hole (smaller polygon inside).
 - All vertices have unique global IDs (no sharing between faces).
 - All segments (edges) are written, no duplicates, no self-loops.
"""

import random
import math
from pathlib import Path

# ----------------- CONFIG -----------------
OUT_DIR = Path(".")
POINT_FILE = OUT_DIR / "point.in"
SEGMENT_FILE = OUT_DIR / "segment.in"
REGION_FILE = OUT_DIR / "region.in"

NUM_FACES = 4                  # number of faces to generate
MIN_VERTS_PER_FACE = 3
MAX_VERTS_PER_FACE = 8
HOLE_PROBABILITY = 0.3        # probability a face has one hole
MIN_HOLE_VERTS = 3
MAX_HOLE_VERTS = 6
SPREAD = 100.0                # coordinate spread
RADIUS_MEAN = 6.0
RADIUS_VARIATION = 3.0
SEED = None                   # set to int e.g. 42 for reproducible results
# ------------------------------------------

if SEED is not None:
    random.seed(SEED)

next_vid = 1
points = {}          # vid -> (x, y)
segments_added = []  # list of (u, v) preserving insertion direction
segments_set = set() # set of frozenset({u,v}) to avoid duplicates
regions = []         # list of dict{id, outer: [vids], inner: [vids] or []}


def add_segment(u, v):
    """Add an undirected segment (u,v) if not already present.
       We store direction as (u,v) for segment.in but avoid dupes using set key."""
    if u == v:
        return
    key = (u, v) if u < v else (v, u)
    if key in segments_set:
        return
    segments_set.add(key)
    segments_added.append((u, v))


def make_polygon_around(center_x, center_y, n_vertices, spread_radius):
    """
    Create n_vertices points around center, with slight randomness in radius and angle spacing.
    Returns list of (vid, x, y) in CCW order.
    """
    global next_vid, points
    angles = []
    base = 2 * math.pi / n_vertices
    for i in range(n_vertices):
        # jitter angle a little
        jitter = random.uniform(-0.3, 0.3) * base
        angles.append(i * base + jitter)
    angles.sort()
    polygon_vids = []
    for ang in angles:
        r = random.gauss(spread_radius, spread_radius * 0.2)
        r = max(0.5, r)
        x = round(center_x + r * math.cos(ang), 6)
        y = round(center_y + r * math.sin(ang), 6)
        vid = next_vid
        next_vid += 1
        points[vid] = (x, y)
        polygon_vids.append(vid)
    return polygon_vids


def generate():
    global next_vid
    # Spread the faces' centers across the plane to avoid overlap
    centers = []
    grid = int(math.ceil(math.sqrt(NUM_FACES)))
    margin = 2 * RADIUS_MEAN + RADIUS_VARIATION
    for i in range(NUM_FACES):
        gx = (i % grid)
        gy = (i // grid)
        cx = (gx + 0.5) * (margin * 3) + random.uniform(-margin, margin)
        cy = (gy + 0.5) * (margin * 3) + random.uniform(-margin, margin)
        centers.append((cx, cy))

    for fid, center in enumerate(centers[:NUM_FACES], start=1):
        cx, cy = center
        n_outer = random.randint(MIN_VERTS_PER_FACE, MAX_VERTS_PER_FACE)
        outer_vids = make_polygon_around(cx, cy, n_outer, RADIUS_MEAN)

        # Add edges for outer boundary
        for i in range(len(outer_vids)):
            u = outer_vids[i]
            v = outer_vids[(i + 1) % len(outer_vids)]
            add_segment(u, v)

        # Optionally add a single hole (smaller polygon near center)
        inner_vids = []
        if random.random() < HOLE_PROBABILITY and n_outer >= 4:
            n_hole = random.randint(MIN_HOLE_VERTS, min(MAX_HOLE_VERTS, n_outer - 1))
            # hole center is slightly jittered near face center
            hx = cx + random.uniform(-1.0, 1.0)
            hy = cy + random.uniform(-1.0, 1.0)
            hole_radius = max(0.8, RADIUS_MEAN * 0.35)
            inner_vids = make_polygon_around(hx, hy, n_hole, hole_radius)

            # Add edges for hole boundary (hole edges are oriented too)
            for i in range(len(inner_vids)):
                u = inner_vids[i]
                v = inner_vids[(i + 1) % len(inner_vids)]
                add_segment(u, v)

        regions.append({
            "id": fid,
            "outer": outer_vids,
            "inner": inner_vids  # may be empty list
        })


def write_files():
    # Write point.in
    with open(POINT_FILE, "w", encoding="utf-8") as pf:
        for vid in sorted(points.keys()):
            x, y = points[vid]
            pf.write(f"{vid} {x} {y}\n")

    # Assign numeric IDs to segments for segment.in
    with open(SEGMENT_FILE, "w", encoding="utf-8") as sf:
        for sid, (u, v) in enumerate(segments_added, start=1):
            sf.write(f"{sid} {u} {v}\n")

    # Write region.in using Option A: 3 lines per face
    with open(REGION_FILE, "w", encoding="utf-8") as rf:
        for reg in regions:
            rf.write(f"{reg['id']}\n")
            rf.write(" ".join(str(v) for v in reg["outer"]) + "\n")
            # if there are inner vertices, write them; else write blank line
            if reg["inner"]:
                rf.write(" ".join(str(v) for v in reg["inner"]) + "\n")
            else:
                rf.write("\n")  # blank line as required by your parser


def main():
    generate()
    write_files()
    print("Generated files:")
    print(f"  {POINT_FILE}")
    print(f"  {SEGMENT_FILE}")
    print(f"  {REGION_FILE}")
    print("")
    print("Summary:")
    print(f"  Faces generated: {len(regions)}")
    print(f"  Total vertices: {len(points)}")
    print(f"  Total segments: {len(segments_added)}")
    faces_with_holes = sum(1 for r in regions if r["inner"])
    print(f"  Faces with holes: {faces_with_holes}")


if __name__ == "__main__":
    main()
