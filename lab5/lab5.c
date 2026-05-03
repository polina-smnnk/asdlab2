#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define WIDTH 1000
#define HEIGHT 800
#define PI 3.141592653589793

int n;
int matrix[50][50];
int tree_matrix[50][50];
int new_order[50];
int order_counter = 1;
int x[50], y[50];

typedef struct {
    int type;
    int u;
    int v;
} Step;

Step steps[500];
int step_count = 0;
int current_step = 0;
int current_mode = 0; 
int visited[50];

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

int get_start_vertex() {
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            int out_deg = 0;
            for (int j = 0; j < n; j++) {
                if (matrix[i][j]) out_deg++;
            }
            if (out_deg > 0) return i;
        }
    }
    for (int i = 0; i < n; i++) {
        if (!visited[i]) return i;
    }
    return -1;
}

void prepare_bfs() {
    memset(visited, 0, sizeof(visited));
    memset(tree_matrix, 0, sizeof(tree_matrix));
    memset(new_order, 0, sizeof(new_order));
    step_count = 0;
    current_step = 0;
    current_mode = 1;
    order_counter = 1;

    while (1) {
        int start_v = get_start_vertex();
        if (start_v == -1) break;

        int q[100], head = 0, tail = 0;
        q[tail++] = start_v;
        visited[start_v] = 1;
        new_order[start_v] = order_counter++;
        
        steps[step_count].type = 1;
        steps[step_count].u = start_v;
        step_count++;

        while (head < tail) {
            int u = q[head++];
            for (int v = 0; v < n; v++) {
                if (matrix[u][v] && !visited[v]) {
                    visited[v] = 1;
                    tree_matrix[u][v] = 1;
                    new_order[v] = order_counter++;
                    q[tail++] = v;
                    
                    steps[step_count].type = 2;
                    steps[step_count].u = u;
                    steps[step_count].v = v;
                    step_count++;
                    
                    steps[step_count].type = 1;
                    steps[step_count].u = v;
                    step_count++;
                }
            }
        }
    }

    printf("\n=== BFS RESULTS FOR REPORT ===\n");
    printf("5. Traversal Tree Adjacency Matrix:\n");
    printMatrix(tree_matrix, n);
    printf("\n6. Vector of new numbering (Original v_i -> New number):\n");
    for (int i = 0; i < n; i++) {
        printf("v%d -> %d\n", i + 1, new_order[i]);
    }
    printf("\n--- BFS READY. Press SPACE to step forward ---\n");
}

void dfs_recursive(int u) {
    for (int v = 0; v < n; v++) {
        if (matrix[u][v] && !visited[v]) {
            visited[v] = 1;
            tree_matrix[u][v] = 1;
            new_order[v] = order_counter++;
            
            steps[step_count].type = 2;
            steps[step_count].u = u;
            steps[step_count].v = v;
            step_count++;
            
            steps[step_count].type = 1;
            steps[step_count].u = v;
            step_count++;
            
            dfs_recursive(v);
        }
    }
}

void prepare_dfs() {
    memset(visited, 0, sizeof(visited));
    memset(tree_matrix, 0, sizeof(tree_matrix));
    memset(new_order, 0, sizeof(new_order));
    step_count = 0;
    current_step = 0;
    current_mode = 2;
    order_counter = 1;

    while (1) {
        int start_v = get_start_vertex();
        if (start_v == -1) break;

        visited[start_v] = 1;
        new_order[start_v] = order_counter++;

        steps[step_count].type = 1;
        steps[step_count].u = start_v;
        step_count++;
        
        dfs_recursive(start_v);
    }

    printf("\n=== DFS RESULTS FOR REPORT ===\n");
    printf("5. Traversal Tree Adjacency Matrix:\n");
    printMatrix(tree_matrix, n);
    printf("\n6. Vector of new numbering (Original v_i -> New number):\n");
    for (int i = 0; i < n; i++) {
        printf("v%d -> %d\n", i + 1, new_order[i]);
    }
    printf("\n--- DFS READY. Press SPACE to step forward ---\n");
}

int main()
{
    int n1 = 5, n2 = 4, n3 = 2, n4 = 7;
    n = 10 + n3;

    double k = 1.0 - n3 * 0.01 - n4 * 0.005 - 0.15;

    srand(5427);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double r = (double)rand() / RAND_MAX * 2.0;
            matrix[i][j] = (r * k >= 1.0) ? 1 : 0;
        }
    }

    printf("4. Directed Graph Adjacency Matrix:\n");
    printMatrix(matrix, n);
    
    printf("\nInstructions:\n");
    printf("Press '1' to start BFS\n");
    printf("Press '2' to start DFS\n");
    printf("Press 'SPACE' to perform the next step\n");

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow(
        "Lab 5 - BFS & DFS",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT, 0
    );
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    int centerX = WIDTH / 2;
    int centerY = HEIGHT / 2;
    int radius = 300;

    for (int i = 0; i < n; i++)
    {
        double angle = 2 * PI * i / n;
        x[i] = centerX + radius * cos(angle);
        y[i] = centerY + radius * sin(angle);
    }

    int running = 1;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = 0;
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_1) {
                    prepare_bfs();
                }
                else if (event.key.keysym.sym == SDLK_2) {
                    prepare_dfs();
                }
                else if (event.key.keysym.sym == SDLK_SPACE) {
                    if (current_mode == 0) {
                        printf("Please select a mode first (Press 1 or 2)\n");
                    }
                    else if (current_step < step_count) {
                        Step s = steps[current_step];
                        if (s.type == 1) {
                            printf("Protocol: Visited vertex v%d\n", s.u + 1);
                        } else if (s.type == 2) {
                            printf("Protocol: Traversed edge v%d -> v%d\n", s.u + 1, s.v + 1);
                        }
                        current_step++;
                    } else {
                        printf("Traversal completed!\n");
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (matrix[i][j] == 1)
                    drawArrow(renderer, x[i], y[i], x[j], y[j], 20);

        SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
        for (int i = 0; i < n; i++)
            drawCircle(renderer, x[i], y[i], 20);

        for (int i = 0; i < current_step; i++) {
            if (steps[i].type == 2) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                drawArrow(renderer, x[steps[i].u], y[steps[i].u], x[steps[i].v], y[steps[i].v], 20);
            }
        }

        for (int i = 0; i < current_step; i++) {
            if (steps[i].type == 1) {
                SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
                drawCircle(renderer, x[steps[i].u], y[steps[i].u], 20);
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}