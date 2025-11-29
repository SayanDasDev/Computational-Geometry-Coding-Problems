/*
 * Randomized Algorithm for Linear Programming (Fixed)
 * Issue Resolved: Correct initialization for Maximization
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <float.h>
#include <math.h>

#define EPSILON 1e-9
#define BIG_M 1e12  
#define BUFFER_SIZE 4096

// --- Data Structures ---

typedef enum {
    LP_OPTIMAL,
    LP_INFEASIBLE,
    LP_UNBOUNDED
} Status;

typedef struct {
    double *a; 
    double b;  
    int id;    
} Constraint;

typedef struct {
    double *x;      
    double value;   
    Status status;  
} Solution;

typedef struct {
    int d;          
    int n;          
    double *c;      
    Constraint *cons; 
} Problem;

// --- Function Prototypes ---

Problem readInputFromFile(const char *filename);
void addBoundingBox(Problem *p); 
void moveToFront(Constraint *cons, int n, int count); // Helper to fix box position
void freeProblem(Problem *p);
void freeSolution(Solution *sol);

void shuffle(Constraint *arr, int start, int n); // Modified shuffle
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
    printf("=== MODE: MAXIMIZE ===\n");

    // 1. Flip Objective for Maximization
    for(int i = 0; i < prob.d; i++) prob.c[i] = -prob.c[i];
    
    // 2. Add Bounding Box
    int original_n = prob.n;
    addBoundingBox(&prob);
    
    // 3. Move Bounding Box constraints to the FRONT (indices 0 to 2d-1)
    // The 'addBoundingBox' function added them to the end. We swap them.
    int box_count = 2 * prob.d;
    moveToFront(prob.cons, prob.n, box_count);

    // 4. Shuffle ONLY the user constraints (indices box_count to n-1)
    // We keep the box constraints fixed at the start so recursion always includes them.
    shuffle(prob.cons, box_count, prob.n);
    
    // 5. Solve
    Solution sol = solveLP(prob.cons, prob.n, prob.d, prob.c);
    
    printSolution(sol, prob);
    
    freeSolution(&sol);
    freeProblem(&prob);
    
    return 0;
}

// --- Solver Logic ---

Solution solveLP(Constraint *cons, int m, int dim, double *c) {
    Solution sol;
    sol.x = (double *)malloc(dim * sizeof(double));
    sol.status = LP_OPTIMAL;
    sol.value = 0.0;
    
    // --- FIX START ---
    // Initialize solution to the Bounding Box Optimal Corner
    // Do NOT initialize to (0,0).
    // Logic: If c[i] > 0, we want x[i] small (-M). If c[i] < 0, we want x[i] large (M).
    // (This minimizes c*x)
    for (int j = 0; j < dim; j++) {
        if (c[j] > 0) sol.x[j] = -BIG_M;
        else sol.x[j] = BIG_M;
        
        sol.value += c[j] * sol.x[j];
    }
    // --- FIX END ---

    if (m == 0) return sol;

    // Iterate through constraints
    // We can safely start loop at 0. The first 2*d constraints (Bounding Box) 
    // will definitely be satisfied by the point we just calculated above.
    for (int i = 0; i < m; i++) {
        double sum = 0.0;
        for (int j = 0; j < dim; j++) sum += cons[i].a[j] * sol.x[j];
        
        if (sum > cons[i].b + EPSILON) {
            if (dim == 1) {
                Solution subSol = solve1D(cons, i + 1, c);
                if (subSol.status != LP_OPTIMAL) { free(sol.x); return subSol; }
                free(sol.x); sol = subSol;
            } 
            else if (dim == 2) {
                Solution subSol = solve2D(cons, i + 1, c);
                if (subSol.status != LP_OPTIMAL) { free(sol.x); return subSol; }
                free(sol.x); sol = subSol;
            } 
            else {
                // Recursion for d > 2 placeholder
                sol.status = LP_INFEASIBLE; 
                return sol;
            }
        }
    }
    
    // Update value one last time to be sure
    if (sol.status == LP_OPTIMAL) {
        sol.value = 0.0;
        for (int i = 0; i < dim; i++) sol.value += c[i] * sol.x[i];

        for(int i=0; i<dim; i++) {
            if (fabs(sol.x[i]) >= BIG_M - 1.0) {
                sol.status = LP_UNBOUNDED;
                break;
            }
        }
    }
    
    return sol;
}

Solution solve1D(Constraint *cons, int m, double *obj) {
    Solution sol;
    sol.x = (double *)malloc(sizeof(double));
    sol.status = LP_OPTIMAL;
    double low = -BIG_M * 2.0, high = BIG_M * 2.0;

    for (int i = 0; i < m; i++) {
        double a = cons[i].a[0], b = cons[i].b;
        if (fabs(a) > EPSILON) {
            double val = b / a;
            if (a > 0) { if (val < high) high = val; } 
            else { if (val > low) low = val; }
        } else if (b < -EPSILON) { sol.status = LP_INFEASIBLE; return sol; }
    }

    if (low > high + EPSILON) sol.status = LP_INFEASIBLE;
    else {
        if (obj[0] > 0) sol.x[0] = low;
        else if (obj[0] < 0) sol.x[0] = high;
        else sol.x[0] = (fabs(low) < fabs(high)) ? low : high;
        sol.value = obj[0] * sol.x[0];
    }
    return sol;
}

Solution solve2D(Constraint *cons, int m, double *obj) {
    Solution sol;
    sol.x = (double *)calloc(2, sizeof(double));
    sol.value = DBL_MAX;
    sol.status = LP_INFEASIBLE; 
    
    for (int p = 0; p < m; p++) {
        for (int q = p + 1; q < m; q++) {
            double det = cons[p].a[0] * cons[q].a[1] - cons[p].a[1] * cons[q].a[0];
            if (fabs(det) > EPSILON) {
                double x = (cons[p].b * cons[q].a[1] - cons[p].a[1] * cons[q].b) / det;
                double y = (cons[p].a[0] * cons[q].b - cons[p].b * cons[q].a[0]) / det;
                double point[2] = {x, y};
                
                if (checkFeasibility(point, cons, m, 2)) {
                    double val = obj[0] * x + obj[1] * y;
                    if (sol.status == LP_INFEASIBLE || val < sol.value) {
                        sol.x[0] = x; sol.x[1] = y;
                        sol.value = val; sol.status = LP_OPTIMAL;
                    }
                }
            }
        }
    }
    return sol;
}

// --- Helper Functions ---

void addBoundingBox(Problem *p) {
    int extra = p->d * 2;
    p->cons = (Constraint *)realloc(p->cons, (p->n + extra) * sizeof(Constraint));
    
    for (int i = 0; i < p->d; i++) {
        int idx1 = p->n + (2 * i);
        p->cons[idx1].a = (double *)calloc(p->d, sizeof(double));
        p->cons[idx1].a[i] = 1.0; p->cons[idx1].b = BIG_M; p->cons[idx1].id = -1; 

        int idx2 = p->n + (2 * i) + 1;
        p->cons[idx2].a = (double *)calloc(p->d, sizeof(double));
        p->cons[idx2].a[i] = -1.0; p->cons[idx2].b = BIG_M; p->cons[idx2].id = -1;
    }
    p->n += extra;
}

// Moves the last 'count' elements to the front of the array
void moveToFront(Constraint *cons, int n, int count) {
    if (count <= 0 || count >= n) return;
    Constraint *temp = (Constraint *)malloc(count * sizeof(Constraint));
    // Copy last 'count' elements
    for(int i=0; i<count; i++) temp[i] = cons[n - count + i];
    // Shift the rest right
    for(int i=n-1; i>=count; i--) cons[i] = cons[i - count];
    // Place temp at front
    for(int i=0; i<count; i++) cons[i] = temp[i];
    free(temp);
}

// Shuffle range [start, n-1]
void shuffle(Constraint *arr, int start, int n) {
    for (int i = n - 1; i > start; i--) {
        int range = i - start + 1;
        int j = start + (rand() % range);
        Constraint t = arr[i]; arr[i] = arr[j]; arr[j] = t;
    }
}

Problem readInputFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) { fprintf(stderr, "Error opening %s\n", filename); exit(1); }
    Problem p = {0, 0, NULL, NULL};
    char line[BUFFER_SIZE];
    int capacity = 16;
    p.cons = (Constraint *)malloc(capacity * sizeof(Constraint));

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\n' || line[0] == '#') continue;
        char *temp = strdup(line), *tok = strtok(temp, " \t\n");
        while(tok) { p.d++; tok = strtok(NULL, " \t\n"); }
        free(temp);
        p.c = (double *)malloc(p.d * sizeof(double));
        tok = strtok(line, " \t\n");
        for(int i=0; i<p.d; i++) { p.c[i] = atof(tok); tok = strtok(NULL, " \t\n"); }
        break;
    }

    while (fgets(line, sizeof(line), file)) {
        int empty=1; for(int i=0; line[i]; i++) if(line[i]>' ') empty=0;
        if(empty) continue;
        if (p.n >= capacity) { capacity *= 2; p.cons = (Constraint *)realloc(p.cons, capacity * sizeof(Constraint)); }
        p.cons[p.n].a = (double *)malloc(p.d * sizeof(double));
        p.cons[p.n].id = p.n;
        char *tok = strtok(line, " \t\n");
        for(int i=0; i<p.d; i++) { p.cons[p.n].a[i] = atof(tok); tok = strtok(NULL, " \t\n"); }
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
        printf("Status: UNBOUNDED (+infinity)\n");
    } else if (sol.status == LP_INFEASIBLE) {
        printf("Status: INFEASIBLE\n");
    } else {
        printf("Status: OPTIMAL\n");
        printf("x = (");
        for (int i = 0; i < p.d; i++) printf("%.4f%s", sol.x[i], (i<p.d-1)?", ":"");
        printf(")\nObjective Value: %.4f\n", -sol.value);
    }
}

void freeProblem(Problem *p) {
    if (p->cons) { for (int i = 0; i < p->n; i++) free(p->cons[i].a); free(p->cons); }
    if (p->c) free(p->c);
}
void freeSolution(Solution *sol) { if (sol->x) free(sol->x); }