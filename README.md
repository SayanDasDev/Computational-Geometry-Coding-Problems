## 📑 Table of Contents

- [Week 1: Collinear/Coincident Check](#📂-week-1-collinear/coincident-check)
- [Week 2: Simple/Convex Polygon Check](#📂-week-2-simple/convex-polygon-check)
- [Week 3: Inside/Outside Polygon Check in Linear Time](#📂-week-3-inside/outside-polygon-check-in-linear-time)
- [Week 4: Convex Hull](#📂-week-4-convex-hull)
- [Week 5: Inside/Outside/On boundary Polygon Check in Logarithmic Time](#📂-week-5-inside/outside/on-boundary-polygon-check-in-logerithmic-time)
- [Week 6: Segment Intersections](#📂-week-6-segment-intersections)
- [Week 7: DCEL](#📂-week-7-dcel)
- [Week 8: Triangulation by Ear Removal](#📂-week-8-triangulation-by-ear-removal)


# Weekly Coding Problems 📚


## 📂 Week 1: Collinear/Coincident Check

### 📜 Problem Statement
> Given n points in R^2 check if they are collinear/coincident.

---

### 📁 Folder
[🔗 Go to Week 1 folder](./week1/)

---

### 📄 Demo Input (`input.txt`)
<details>
<summary>Click to view sample input</summary>

```
0.03771 0.71596
0.03773 0.19013
0.03759 0.91330
0.03725 0.99234
0.03706 0.18751
0.03710 0.86564
0.03765 0.73903
0.03709 0.82133
0.03771 0.19167
```

</details>

---

## 📂 Week 2: Simple/Convex Polygon Check

### 📜 Problem Statement
> Let S be a sequence of n points.
Does S define a convex polygon or simple polygon?

---

### 📁 Folder
[🔗 Go to Week 2 folder](./week2/)

---

### 📄 Demo Input
<details>
<summary>Click to view sample input</summary>

```
9.4 4.1
9.2 5.6
6.5 7.4
2.2 7.4
0.4 1.5
7.9 0.0
```

</details>

---

## 📂 Week 3: Inside/Outside Polygon Check in Linear Time

### 📜 Problem Statement
> Let P be a sequence of n points denoting a polygon and S be a set of m points.
Check if the points in S are inside or outside the polygon.

---

### 📁 Folder
[🔗 Go to Week 3 folder](./week3/)

---

### 📄 Demo Input
<details>
<summary>Click to view sample input</summary>

```
0 0
5 0
5 5
0 5

1 1
2.5 2.5
4.9 4.9
0 2.5
-1 1
```

</details>

---

## 📂 Week 4: Convex Hull

### 📜 Problem Statement
> Given a set of points S compute the convex hull.

---

### 📁 Folder
[🔗 Go to Week 4 folder](./week4/)

---

### 📄 Demo Input
<details>
<summary>Click to view sample input</summary>

```
0 0
1 0 
0 1
1 1
0.5 0.5
```

</details>

---

## 📂 Week 5: Inside/Outside/On boundary Polygon Check in Logerithmic Time

### 📜 Problem Statement
> Let S be a set of n points in R^2.
Let P be a convex polygon.
Check for each query point q ∈ S, if it is inside, outside, or on hte boundary of polygon P.

---

### 📁 Folder
[🔗 Go to Week 5 folder](./week5/)

---

### 📄 Demo Input
<details>
<summary>Click to view sample input</summary>

```
1.0 5.7
1.0 3.6
1.2 2.1
1.4 1.6
2.4 1.1
3.3 0.7
6.1 0.4
9.0 2.3
9.7 3.6
9.6 4.9
9.0 7.9
6.8 9.4
3.5 9.7
3.4 9.6
2.1 8.7
1.1 7.3
```

</details>

---

## 📂 Week 6: Segment Intersections

### 📜 Problem Statement
> Given a number of line segments find its intersections.

---

### 📁 Folder
[🔗 Go to Week 6 folder](./week6/)

---

### 📄 Demo Input
<details>
<summary>Click to view sample input</summary>

```
0 0 6 0
2 -2 2 2
4 -1 4 1
1 1 5 1
```

</details>

---

## 📂 Week 7: DCEL

### 📜 Problem Statement
> Let S be a subdivision. Input S form a a file and store it in DCEL.
Then answer some queries about faces, vertices and edges.

Input :

struct point {
  Ident id,
  float x,
  float y
}

struct segment {
  Ident id,
  Ident start
  Ident end
}

struct region {
  Ident id
  List<Ident> outer (anti-clock-wise)
  List<Ident> inner (clock-wise)
}

class DCEL {

}


Query: 
Search a Face ID. Find the number of edges in that face.
  Check for dangling edge/bridge (do it in O(n))

---

### 📁 Folder
[🔗 Go to Week 7 folder](./week7/)

---

### 📄 Demo Input
<details>
<summary>Click to view sample input</summary>

```
1 0.0 0.0
2 1.0 0.0
3 0.5 1.0
```

</details>

---

## 📂 Week 8: Triangulation by Ear Removal

### 📜 Problem Statement
> Triangulate using removal of ears.
Input: Simple polygon as a list of vertices in anti-clockwise order.

---

### 📁 Folder
[🔗 Go to Week 8 folder](./week8/)

---

### 📄 Demo Input
<details>
<summary>Click to view sample input</summary>

```
0 2
2 0
4 1
3 3
1 3
```

</details>

---
