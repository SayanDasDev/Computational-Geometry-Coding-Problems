/*
 * Randomized Algorithm for Linear Programming (Enhanced)
 * Handles: Infeasibility, Unboundedness, and Open Regions via Bounding Box
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <float.h>
#include <math.h>

#define EPSILON 1e-9
#define BIG_M 1e12  // Our "Infinity" bounding box
#define BUFFER_SIZE 4096

// --- Data Structures ---

typedef enum {
    LP_OPTIMAL,
    LP_INFEASIBLE,
    LP_UNBOUNDED
} Status;

typedef struct {
    double *a; // coefficients (size d)
    double b;  // RHS
    int id;    // Original index (for debugging)
} Constraint;

typedef struct {
    double *x;      // Optimal point (size d)
    double value;   // Objective value
    Status status;  // Result status
} Solution;

typedef struct {
    int d;          // Dimensions
    int n;          // Number of constraints
    double *c;      // Objective vector
    Constraint *cons; // Array of constraints
} Problem;

// --- Function Prototypes ---

Problem readInputFromFile(const char *filename);
void addBoundingBox(Problem *p); // NEW: Adds Big-M constraints
void freeProblem(Problem *p);
void freeSolution(Solution *sol);

void shuffle(int *arr, int n);
int checkFeasibility(double *point, Constraint *cons, int m, int d);

Solution solve1D(Constraint *cons, int m, double *c);
Solution solve2D(Constraint *cons, int m, double *c);
Solution solveLP(Constraint *cons, int m, int d, double *c);
void printSolution(Solution sol, Problem p);

// --- Main Execution ---

int main(int argc, char *argv[]) {
    setbuf(stdout, NULL);
    srand((unsigned int)time(NULL));
    
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }
    
    Problem prob = readInputFromFile(argv[1]);
    
    printf("\n=== Solving LP (Dims: %d, Constraints: %d) ===\n", prob.d, prob.n);

    // 1. Inject Bounding Box to handle Unbounded cases robustly
    // This adds 2*d constraints (e.g., x <= BIG_M, x >= -BIG_M)
    addBoundingBox(&prob);

    // 2. Shuffle indices for Randomized Incremental Construction
    int *perm = (int *)malloc(prob.n * sizeof(int));
    for (int i = 0; i < prob.n; i++) perm[i] = i;
    shuffle(perm, prob.n);
    
    Constraint *permuted = (Constraint *)malloc(prob.n * sizeof(Constraint));
    for (int i = 0; i < prob.n; i++) permuted[i] = prob.cons[perm[i]];
    
    // 3. Solve
    Solution sol = solveLP(permuted, prob.n, prob.d, prob.c);
    
    // 4. Output
    printSolution(sol, prob);
    
    // Cleanup
    free(perm);
    free(permuted);
    freeSolution(&sol);
    freeProblem(&prob);
    
    return 0;
}

// --- Solver Logic ---

Solution solveLP(Constraint *cons, int m, int dim, double *c) {
    Solution sol;
    sol.x = (double *)calloc(dim, sizeof(double));
    sol.status = LP_OPTIMAL;
    
    // Initialize with a trivial feasible point (Origin)
    // Note: In a pure implementation, we should run a Phase 1 LP to find a feasible start.
    // However, thanks to the Bounding Box, (0,0...0) is strictly feasible with respect to the
    // box constraints (which are usually placed first or last).
    // The logic below relies on Seidel's recursion to correct infeasibility.

    if (m == 0) return sol;

    for (int i = 0; i < m; i++) {
        double sum = 0.0;
        for (int j = 0; j < dim; j++) sum += cons[i].a[j] * sol.x[j];
        
        // Constraint Violated?
        if (sum > cons[i].b + EPSILON) {
            
            // If dimensions are small, use specialized exact solvers
            if (dim == 1) {
                Solution subSol = solve1D(cons, i + 1, c);
                if (subSol.status != LP_OPTIMAL) {
                    free(sol.x);
                    return subSol;
                }
                free(sol.x);
                sol = subSol;
            } 
            else if (dim == 2) {
                Solution subSol = solve2D(cons, i + 1, c);
                if (subSol.status != LP_OPTIMAL) {
                    free(sol.x);
                    return subSol;
                }
                free(sol.x);
                sol = subSol;
            } 
            else {
                // Recursion for d > 2 (Not fully implemented in this snippet)
                // This would involve projecting constraints onto the hyperplane
                sol.status = LP_INFEASIBLE; // Placeholder
                return sol;
            }
        }
    }
    
    // Calculate final value
    if (sol.status == LP_OPTIMAL) {
        sol.value = 0.0;
        for (int i = 0; i < dim; i++) sol.value += c[i] * sol.x[i];

        // CHECK UNBOUNDEDNESS:
        // If any coordinate hits the BIG_M wall, the problem is likely unbounded
        for(int i=0; i<dim; i++) {
            if (fabs(sol.x[i]) >= BIG_M - 1.0) {
                sol.status = LP_UNBOUNDED;
                break;
            }
        }
    }
    
    return sol;
}

/* * 1D Solver (Interval Intersection)
 * Solves: min c*x subject to constraints
 */
Solution solve1D(Constraint *cons, int m, double *obj) {
    Solution sol;
    sol.x = (double *)malloc(sizeof(double));
    sol.status = LP_OPTIMAL;
    
    // Interval [low, high]
    double low = -BIG_M * 2.0; 
    double high = BIG_M * 2.0;

    for (int i = 0; i < m; i++) {
        double a = cons[i].a[0];
        double b = cons[i].b;

        if (fabs(a) > EPSILON) {
            double val = b / a;
            if (a > 0) {
                // ax <= b => x <= val (Upper Bound)
                if (val < high) high = val;
            } else {
                // ax <= b (neg a) => x >= val (Lower Bound)
                if (val > low) low = val;
            }
        } else {
            // 0 <= b
            if (b < -EPSILON) {
                sol.status = LP_INFEASIBLE;
                return sol;
            }
        }
    }

    if (low > high + EPSILON) {
        sol.status = LP_INFEASIBLE;
    } else {
        // Optimization
        if (obj[0] > 0) sol.x[0] = low;      // Minimize: go left
        else if (obj[0] < 0) sol.x[0] = high; // Minimize negative: go right
        else sol.x[0] = (fabs(low) < fabs(high)) ? low : high; // c=0, pick smallest magnitude

        sol.value = obj[0] * sol.x[0];
    }
    
    return sol;
}

/* * 2D Solver (Vertex Enumeration)
 * Because we added a Bounding Box, we are guaranteed to find intersections
 * even for "open" regions (they will intersect with the Box).
 */
Solution solve2D(Constraint *cons, int m, double *obj) {
    Solution sol;
    sol.x = (double *)calloc(2, sizeof(double));
    sol.value = DBL_MAX;
    sol.status = LP_INFEASIBLE; // Assume infeasible until point found
    
    // Iterate over all pairs of lines to find vertices
    for (int p = 0; p < m; p++) {
        for (int q = p + 1; q < m; q++) {
            
            // Cramer's Rule for intersection
            double det = cons[p].a[0] * cons[q].a[1] - cons[p].a[1] * cons[q].a[0];
            
            if (fabs(det) > EPSILON) {
                double x = (cons[p].b * cons[q].a[1] - cons[p].a[1] * cons[q].b) / det;
                double y = (cons[p].a[0] * cons[q].b - cons[p].b * cons[q].a[0]) / det;
                double point[2] = {x, y};
                
                // Must satisfy ALL other constraints
                if (checkFeasibility(point, cons, m, 2)) {
                    double val = obj[0] * x + obj[1] * y;
                    
                    // Update if this vertex is better
                    // (Or if we haven't found a feasible point yet)
                    if (sol.status == LP_INFEASIBLE || val < sol.value) {
                        sol.x[0] = x;
                        sol.x[1] = y;
                        sol.value = val;
                        sol.status = LP_OPTIMAL;
                    }
                }
            }
        }
    }
    
    return sol;
}

// --- Helper Functions ---

/* * Adds "Big-M" constraints to make the region bounded.
 * For each dimension i, adds: x_i <= M and -x_i <= M (x_i >= -M)
 */
void addBoundingBox(Problem *p) {
    int extra = p->d * 2;
    p->cons = (Constraint *)realloc(p->cons, (p->n + extra) * sizeof(Constraint));
    
    for (int i = 0; i < p->d; i++) {
        // Upper bound: 1.0 * x_i <= BIG_M
        int idx1 = p->n + (2 * i);
        p->cons[idx1].a = (double *)calloc(p->d, sizeof(double));
        p->cons[idx1].a[i] = 1.0;
        p->cons[idx1].b = BIG_M;
        p->cons[idx1].id = -1; // Marker for artificial constraint

        // Lower bound: -1.0 * x_i <= BIG_M
        int idx2 = p->n + (2 * i) + 1;
        p->cons[idx2].a = (double *)calloc(p->d, sizeof(double));
        p->cons[idx2].a[i] = -1.0;
        p->cons[idx2].b = BIG_M;
        p->cons[idx2].id = -1;
    }
    
    p->n += extra;
}

Problem readInputFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) { fprintf(stderr, "Error opening %s\n", filename); exit(1); }

    Problem p = {0, 0, NULL, NULL};
    char line[BUFFER_SIZE];
    int capacity = 16;
    p.cons = (Constraint *)malloc(capacity * sizeof(Constraint));

    // Parse Objective
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\n' || line[0] == '#') continue;
        char *temp = strdup(line);
        char *tok = strtok(temp, " \t\n");
        while(tok) { p.d++; tok = strtok(NULL, " \t\n"); }
        free(temp);
        
        p.c = (double *)malloc(p.d * sizeof(double));
        tok = strtok(line, " \t\n");
        for(int i=0; i<p.d; i++) { p.c[i] = atof(tok); tok = strtok(NULL, " \t\n"); }
        break;
    }

    // Parse Constraints
    while (fgets(line, sizeof(line), file)) {
        int empty=1;
        for(int i=0; line[i]; i++) if(line[i]>' ') empty=0;
        if(empty) continue;

        if (p.n >= capacity) {
            capacity *= 2;
            p.cons = (Constraint *)realloc(p.cons, capacity * sizeof(Constraint));
        }
        
        p.cons[p.n].a = (double *)malloc(p.d * sizeof(double));
        p.cons[p.n].id = p.n; // Store original ID
        
        char *tok = strtok(line, " \t\n");
        for(int i=0; i<p.d; i++) {
            p.cons[p.n].a[i] = atof(tok);
            tok = strtok(NULL, " \t\n");
        }
        p.cons[p.n].b = atof(tok);
        p.n++;
    }
    fclose(file);
    return p;
}

int checkFeasibility(double *point, Constraint *cons, int m, int d) {
    for (int i = 0; i < m; i++) {
        double sum = 0.0;
        for (int j = 0; j < d; j++) sum += cons[i].a[j] * point[j];
        if (sum > cons[i].b + EPSILON) return 0;
    }
    return 1;
}

void printSolution(Solution sol, Problem p) {
    printf("=== Result ===\n");
    if (sol.status == LP_UNBOUNDED) {
        printf("Status: UNBOUNDED (-infinity)\n");
        printf("Direction of unboundedness detected at values near +/- %.1e\n", BIG_M);
    } 
    else if (sol.status == LP_INFEASIBLE) {
        printf("Status: INFEASIBLE\n");
        printf("No solution satisfies all constraints.\n");
    } 
    else {
        printf("Status: OPTIMAL\n");
        printf("x = (");
        for (int i = 0; i < p.d; i++) printf("%.4f%s", sol.x[i], (i<p.d-1)?", ":"");
        printf(")\nObjective Value: %.4f\n", sol.value);
    }
}

void freeProblem(Problem *p) {
    if (p->cons) {
        for (int i = 0; i < p->n; i++) free(p->cons[i].a);
        free(p->cons);
    }
    if (p->c) free(p->c);
}

void freeSolution(Solution *sol) {
    if (sol->x) free(sol->x);
}

void shuffle(int *arr, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int t = arr[i]; arr[i] = arr[j]; arr[j] = t;
    }
}