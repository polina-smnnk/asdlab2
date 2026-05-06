#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define WIDTH 1000
#define HEIGHT 800
#define PI 3.141592653589793

int n;
int x[50], y[50];

typedef struct EdgeNode {
    int dest;
    int weight;
    struct EdgeNode* next;
} EdgeNode;

EdgeNode* adjList[50] = {NULL};

void addEdgeList(int src, int dest, int weight) {
    EdgeNode* newNode = (EdgeNode*)malloc(sizeof(EdgeNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = adjList[src];
    adjList[src] = newNode;
}

typedef struct {
    int u;
    int v;
    int weight;
} Step;

Step steps[500];
int step_count = 0;
int current_step = 0;
int inMST[50] = {0};
int mst_started = 0;
int total_mst_weight = 0;

const int font[10][5] = {
    {0x7, 0x5, 0x5, 0x5, 0x7},
    {0x2, 0x2, 0x2, 0x2, 0x2},
    {0x7, 0x1, 0x7, 0x4, 0x7},
    {0x7, 0x1, 0x7, 0x1, 0x7},
    {0x5, 0x5, 0x7, 0x1, 0x1},
    {0x7, 0x4, 0x7, 0x1, 0x7},
    {0x7, 0x4, 0x7, 0x5, 0x7},
    {0x7, 0x1, 0x1, 0x1, 0x1},
    {0x7, 0x5, 0x7, 0x5, 0x7},
    {0x7, 0x5, 0x7, 0x1, 0x7} 
};

void drawNum(SDL_Renderer* renderer, int dx, int dy, int num, int scale) {
    char buf[16];
    sprintf(buf, "%d", num);
    int len = strlen(buf);
    
    int totalWidth = len * 4 * scale;
    dx -= totalWidth / 2;
    dy -= 2 * scale;

    for(int i = 0; i < len; i++) {
        int d = buf[i] - '0';
        for(int row = 0; row < 5; row++) {
            for(int col = 0; col < 3; col++) {
                if(font[d][row] & (1 << (2 - col))) {
                    SDL_Rect rect = {dx + i*4*scale + col*scale, dy + row*scale, scale, scale};
                    SDL_RenderFillRect(renderer, &rect);
                }
            }
        }
    }
}

void drawCircle(SDL_Renderer* renderer, int cx, int cy, int r) {
    for (int w = 0; w < r * 2; w++) {
        for (int h = 0; h < r * 2; h++) {
            int dx = r - w;
            int dy = r - h;
            if (dx * dx + dy * dy <= r * r)
                SDL_RenderDrawPoint(renderer, cx + dx, cy + dy);
        }
    }
}

void drawLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2) {
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void printMatrix(int mat[50][50], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%4d ", mat[i][j]);
        }
        printf("\n");
    }
}

void prepare_prim() {
    memset(inMST, 0, sizeof(inMST));
    step_count = 0;
    current_step = 0;
    total_mst_weight = 0;
    
    inMST[0] = 1;
    
    for (int i = 0; i < n - 1; i++) {
        int min_weight = 1e9;
        int min_u = -1, min_v = -1;
        
        for (int u = 0; u < n; u++) {
            if (inMST[u]) {
                EdgeNode* curr = adjList[u];
                while (curr != NULL) {
                    int v = curr->dest;
                    if (!inMST[v] && curr->weight < min_weight) {
                        min_weight = curr->weight;
                        min_u = u;
                        min_v = v;
                    }
                    curr = curr->next;
                }
            }
        }
        
        if (min_u != -1 && min_v != -1) {
            inMST[min_v] = 1;
            steps[step_count].u = min_u;
            steps[step_count].v = min_v;
            steps[step_count].weight = min_weight;
            total_mst_weight += min_weight;
            step_count++;
        }
    }
    mst_started = 1;
    printf("\n--- Алгоритм Пріма готовий. Натискайте SPACE для покрокової візуалізації ---\n");
}

int main(int argc, char* argv[])
{
    int n1 = 5, n2 = 4, n3 = 2, n4 = 7;
    n = 10 + n3;

    double k = 1.0 - n3 * 0.01 - n4 * 0.005 - 0.05;

    int A_dir[50][50] = {0};
    int A_undir[50][50] = {0};
    double B[50][50] = {0};
    int C[50][50] = {0};
    int D[50][50] = {0};
    int H[50][50] = {0};
    int Tr[50][50] = {0};
    int W[50][50] = {0};

    srand(5427);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double r = (double)rand() / RAND_MAX * 2.0;
            A_dir[i][j] = (r * k >= 1.0) ? 1 : 0;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A_undir[i][j] = A_dir[i][j] || A_dir[j][i];
        }
    }

    srand(5427);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            B[i][j] = (double)rand() / RAND_MAX * 2.0;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = ceil(B[i][j] * 100 * A_undir[i][j]);
            D[i][j] = (C[i][j] > 0) ? 1 : 0;
            Tr[i][j] = (i < j) ? 1 : 0;
        }
    }
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            H[i][j] = (D[i][j] != D[j][i]) ? 1 : 0;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            W[i][j] = (D[i][j] + H[i][j] * Tr[i][j]) * C[i][j];
            W[j][i] = W[i][j];
        }
    }

    printf("4. Згенерована матриця суміжності ненапрямленого графа:\n");
    printMatrix(A_undir, n);

    printf("\n5. Згенерована матриця ваг графа:\n");
    printMatrix(W, n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (W[i][j] > 0) {
                addEdgeList(i, j, W[i][j]);
            }
        }
    }

    printf("\nІнструкція:\n");
    printf("Натисніть '1', щоб розпочати алгоритм Пріма (Оскільки n4=7 непарне)\n");
    printf("Натискайте 'SPACE' для покрокового виконання\n");

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow(
        "Lab 6 - Minimum Spanning Tree (Prim)",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT, 0
    );
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    int centerX = WIDTH / 2;
    int centerY = HEIGHT / 2;
    int radius = 300;

    for (int i = 0; i < n; i++) {
        double angle = 2 * PI * i / n;
        x[i] = centerX + radius * cos(angle);
        y[i] = centerY + radius * sin(angle);
    }

    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = 0;
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_1) {
                    prepare_prim();
                }
                else if (event.key.keysym.sym == SDLK_SPACE) {
                    if (!mst_started) {
                        printf("Спочатку оберіть алгоритм (Натисніть 1)\n");
                    }
                    else if (current_step < step_count) {
                        Step s = steps[current_step];
                        printf("Крок: додано ребро v%d - v%d (вага %d)\n", s.u + 1, s.v + 1, s.weight);
                        current_step++;
                        
                        if (current_step == step_count) {
                            printf("\nПобудову кістяка завершено!\n");
                            printf("7. Сума ваг ребер знайденого мінімального кістяка: %d\n", total_mst_weight);
                        }
                    } 
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        for (int i = 0; i < n; i++) {
            EdgeNode* curr = adjList[i];
            while(curr) {
                if (i < curr->dest) {
                    drawLine(renderer, x[i], y[i], x[curr->dest], y[curr->dest]);
                }
                curr = curr->next;
            }
        }

        for (int i = 0; i < n; i++) {
            EdgeNode* curr = adjList[i];
            while(curr) {
                if (i < curr->dest) {
                    int midX = (x[i] + x[curr->dest]) / 2;
                    int midY = (y[i] + y[curr->dest]) / 2;
                    
                    SDL_Rect bg = {midX - 10, midY - 6, 20, 12};
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderFillRect(renderer, &bg);
                    
                    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
                    drawNum(renderer, midX, midY, curr->weight, 1);
                }
                curr = curr->next;
            }
        }

        for (int i = 0; i < current_step; i++) {
            int u = steps[i].u;
            int v = steps[i].v;
            
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            drawLine(renderer, x[u], y[u], x[v], y[v]);
            drawLine(renderer, x[u]+1, y[u]+1, x[v]+1, y[v]+1);
            drawLine(renderer, x[u]-1, y[u]-1, x[v]-1, y[v]-1);
            
            int midX = (x[u] + x[v]) / 2;
            int midY = (y[u] + y[v]) / 2;
            
            SDL_Rect bg = {midX - 10, midY - 6, 20, 12};
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &bg);
            
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            drawNum(renderer, midX, midY, steps[i].weight, 1);
        }

        SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
        for (int i = 0; i < n; i++) {
            int is_in_mst = 0;
            if (current_step > 0 && i == 0) is_in_mst = 1;
            for(int k = 0; k < current_step; k++) {
                if (steps[k].u == i || steps[k].v == i) is_in_mst = 1;
            }
            
            if (is_in_mst) {
                SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
            }
            drawCircle(renderer, x[i], y[i], 20);
            
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            drawNum(renderer, x[i], y[i], i + 1, 2);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}