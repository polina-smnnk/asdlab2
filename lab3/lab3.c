#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH 1000
#define HEIGHT 800
#define PI 3.141592653589793

int n;
int matrix[50][50];
int x[50], y[50];

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

int main()
{
    int n1 = 5, n2 = 4, n3 = 2, n4 = 7;

    n = 10 + n3;

    double k = 1.0 - n3 * 0.02 - n4 * 0.005 - 0.25;

    srand(5427);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
        {
            double r = (double)rand() / RAND_MAX * 2.0;
            matrix[i][j] = (r * k >= 1.0) ? 1 : 0;
        }

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "Graph Variant 5427",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        0
    );

    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

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

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        int centerX = WIDTH / 2;
        int centerY = HEIGHT / 2;
        int radius = 280;

        for (int i = 0; i < n; i++)
        {
            double angle = 2 * PI * i / n;

            x[i] = centerX + radius * cos(angle);
            y[i] = centerY + radius * sin(angle);
        }

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (matrix[i][j] == 1)
                    SDL_RenderDrawLine(renderer, x[i], y[i], x[j], y[j]);

        for (int i = 0; i < n; i++)
            drawCircle(renderer, x[i], y[i], 20);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}