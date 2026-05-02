#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH 1200
#define HEIGHT 600
#define PI 3.141592653589793

int n;
int matrix1_dir[50][50];
int matrix1_un[50][50];
int matrix[50][50];
int reachability[50][50];
int strong_conn[50][50];
int component[50];
int cond_matrix[50][50];
int num_components = 0;

int x[50], y[50];
int cond_x[50], cond_y[50];

void drawCircle(SDL_Renderer* renderer, int cx, int cy, int r)
{
    for (int w = 0; w < r * 2; w++)
    {
        for (int h = 0; h < r * 2; h++)
        {
            int dx = r - w;
            int dy = r - h;
            if (dx * dx + dy * dy <= r * r)
                SDL_RenderDrawPoint(renderer, cx + dx, cy + dy);
        }
    }
}

void drawArrow(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int node_radius) {
    if (x1 == x2 && y1 == y2) {
        drawCircle(renderer, x1 - node_radius, y1 - node_radius, node_radius);
        return;
    }

    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    
    double angle = atan2(y2 - y1, x2 - x1);
    int arrow_len = 15;
    double arrow_angle = PI / 6;

    int ex = x2 - node_radius * cos(angle);
    int ey = y2 - node_radius * sin(angle);

    int ax1 = ex - arrow_len * cos(angle - arrow_angle);
    int ay1 = ey - arrow_len * sin(angle - arrow_angle);
    int ax2 = ex - arrow_len * cos(angle + arrow_angle);
    int ay2 = ey - arrow_len * sin(angle + arrow_angle);

    SDL_RenderDrawLine(renderer, ex, ey, ax1, ay1);
    SDL_RenderDrawLine(renderer, ex, ey, ax2, ay2);
}

void printMatrix(int mat[50][50], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
}

int main()
{
    int n1 = 5, n2 = 4, n3 = 2, n4 = 7;
    n = 10 + n3;

    double k1 = 1.0 - n3 * 0.01 - n4 * 0.01 - 0.3;
    double k2 = 1.0 - n3 * 0.005 - n4 * 0.005 - 0.27;

    srand(5427);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double r = (double)rand() / RAND_MAX * 2.0;
            matrix1_dir[i][j] = (r * k1 >= 1.0) ? 1 : 0;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix1_un[i][j] = matrix1_dir[i][j] || matrix1_dir[j][i];
        }
    }

    srand(5427);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double r = (double)rand() / RAND_MAX * 2.0;
            matrix[i][j] = (r * k2 >= 1.0) ? 1 : 0;
        }
    }
    
    printf("--- PART 1 ---\n\n");
    printf("Directed Graph 1 Matrix:\n");
    printMatrix(matrix1_dir, n);
    
    printf("\nUndirected Graph 1 Matrix:\n");
    printMatrix(matrix1_un, n);

    printf("\n--- PART 2 ---\n\n");
    
    int deg_un[50] = {0};
    printf("Degrees of Undirected Graph 1:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix1_un[i][j]) deg_un[i]++;
        }
        printf("v%d: %d\n", i+1, deg_un[i]);
    }

    printf("\nHalf-degrees of Directed Graph 1 (out / in):\n");
    for (int i = 0; i < n; i++) {
        int in_deg = 0, out_deg = 0;
        for (int j = 0; j < n; j++) {
            if (matrix1_dir[i][j]) out_deg++;
            if (matrix1_dir[j][i]) in_deg++;
        }
        printf("v%d: out = %d, in = %d\n", i+1, out_deg, in_deg);
    }

    int is_homogeneous = 1;
    for (int i = 1; i < n; i++) {
        if (deg_un[i] != deg_un[0]) {
            is_homogeneous = 0;
            break;
        }
    }
    printf("\nHomogeneity (Undirected Graph 1):\n");
    if (is_homogeneous) {
        printf("The graph is regular (homogeneous) with degree %d.\n", deg_un[0]);
    } else {
        printf("The graph is NOT regular (homogeneous).\n");
    }

    printf("\nIsolated vertices (Undirected Graph 1): ");
    int has_isolated = 0;
    for (int i = 0; i < n; i++) {
        if (deg_un[i] == 0) {
            printf("v%d ", i+1);
            has_isolated = 1;
        }
    }
    if (!has_isolated) printf("None");
    printf("\n");

    printf("Pendant vertices (Undirected Graph 1): ");
    int has_pendant = 0;
    for (int i = 0; i < n; i++) {
        if (deg_un[i] == 1) {
            printf("v%d ", i+1);
            has_pendant = 1;
        }
    }
    if (!has_pendant) printf("None");
    printf("\n");

    printf("\n--- PART 3 ---\n\n");
    printf("Modified Directed Graph 2 Matrix:\n");
    printMatrix(matrix, n);

    printf("\n--- PART 4 ---\n\n");
    printf("Half-degrees of Directed Graph 2 (out / in):\n");
    for (int i = 0; i < n; i++) {
        int in_deg = 0, out_deg = 0;
        for (int j = 0; j < n; j++) {
            if (matrix[i][j]) out_deg++;
            if (matrix[j][i]) in_deg++;
        }
        printf("v%d: out = %d, in = %d\n", i+1, out_deg, in_deg);
    }

    printf("\nPaths of length 2 (Graph 2):\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (matrix[i][j])
                for (int m = 0; m < n; m++)
                    if (matrix[j][m])
                        printf("%d - %d - %d\n", i+1, j+1, m+1);

    printf("\nPaths of length 3 (Graph 2):\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (matrix[i][j])
                for (int m = 0; m < n; m++)
                    if (matrix[j][m])
                        for (int l = 0; l < n; l++)
                            if (matrix[m][l])
                                printf("%d - %d - %d - %d\n", i+1, j+1, m+1, l+1);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            reachability[i][j] = matrix[i][j];
            
    for (int i = 0; i < n; i++) reachability[i][i] = 1;

    for (int m = 0; m < n; m++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                reachability[i][j] = reachability[i][j] || (reachability[i][m] && reachability[m][j]);

    printf("\nReachability matrix (Graph 2):\n");
    printMatrix(reachability, n);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            strong_conn[i][j] = reachability[i][j] && reachability[j][i];

    printf("\nStrong connectivity matrix (Graph 2):\n");
    printMatrix(strong_conn, n);

    int visited[50] = {0};
    printf("\nStrongly connected components (Graph 2):\n");
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            num_components++;
            printf("K%d: ", num_components);
            for (int j = 0; j < n; j++) {
                if (strong_conn[i][j]) {
                    component[j] = num_components;
                    visited[j] = 1;
                    printf("v%d ", j+1);
                }
            }
            printf("\n");
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] && component[i] != component[j]) {
                cond_matrix[component[i]-1][component[j]-1] = 1;
            }
        }
    }

    printf("\nCondensation graph matrix (%dx%d):\n", num_components, num_components);
    printMatrix(cond_matrix, num_components);

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow(
        "Lab 4 - Graph & Condensation",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT, 0
    );
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    int running = 1;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = 0;
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        int centerX = WIDTH / 4;
        int centerY = HEIGHT / 2;
        int radius = 220;

        for (int i = 0; i < n; i++)
        {
            double angle = 2 * PI * i / n;
            x[i] = centerX + radius * cos(angle);
            y[i] = centerY + radius * sin(angle);
        }

        int cond_centerX = WIDTH * 3 / 4;
        int cond_centerY = HEIGHT / 2;
        int cond_radius = 180;

        for (int i = 0; i < num_components; i++)
        {
            double angle = (num_components > 1) ? (2 * PI * i / num_components) : 0;
            cond_x[i] = cond_centerX + cond_radius * cos(angle);
            cond_y[i] = cond_centerY + cond_radius * sin(angle);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (matrix[i][j] == 1)
                    drawArrow(renderer, x[i], y[i], x[j], y[j], 15);

        for (int i = 0; i < n; i++)
            drawCircle(renderer, x[i], y[i], 15);

        for (int i = 0; i < num_components; i++)
            for (int j = 0; j < num_components; j++)
                if (cond_matrix[i][j] == 1)
                    drawArrow(renderer, cond_x[i], cond_y[i], cond_x[j], cond_y[j], 25);

        for (int i = 0; i < num_components; i++)
            drawCircle(renderer, cond_x[i], cond_y[i], 25);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}