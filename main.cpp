#include <iostream>
#include <math.h>
#include <iomanip>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define PI 3.1415926535f

using namespace std;

SDL_Event event;
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Surface* surface;
double pdX = 0, pdY = 0, pA = 0, px = 100, py = 100;
SDL_Rect player = {100, 100, 10, 10};
bool RunState;
int vel = 10;
int mapX = 8, mapY = 8, mapS = 64;
SDL_Rect tileRect = {0, 0, 64, 64};
int map[] = 
{
    1,1,1,1,1,1,1,1,
    1,0,0,0,1,0,0,1,
    1,0,1,1,0,0,0,1,
    1,0,1,0,0,0,0,1,
    1,0,0,0,0,1,0,1,
    1,0,0,0,1,1,0,1,
    1,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,
};

void DrawRay3d()
{
    int r, mx, my, mp, dof;
    double rx, ry, ra, xo, yo;
    ra = pA;
    for (r = 0; r < 1; r++){
        // Check horizontal lines
        dof = 0;
        double aTan = -1.0f/tan(ra);
        if (ra == 0 || ra == PI) {
            rx = px;
            ry = py;
            dof = 8;
        }
        //Look up
        else if (ra > PI) {
            ry = (int)py/64*64 - 0.0001f;
            rx = (py - ry)*aTan + px;
            yo = -64;
            xo = -yo*aTan;
        }
        //Look down
        else if (ra < PI) {
            ry = (int)py/64*64 + 64;
            rx = (py - ry)*aTan + px;
            yo = 64;
            xo = -yo*aTan;
        }
        while (dof < 8){
            mx = (int)rx / 64;
            my = (int)ry / 64;
            mp = my * mapX + mx;
            if (mp >= 0 && mp < mapX*mapY && map[mp] == 1) dof = 8;
            else {
                rx += xo;
                ry += yo;
                dof++;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderDrawLine(renderer, px, py, rx, ry);
    }
}

void Drawmap()
{
    for(int y = 0; y < mapY; y++)
    {
        for(int x = 0; x < mapX; x++)
        {
            if(map[y*mapX+x]==1) 
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                tileRect.x = x*64;
                tileRect.y = y*64;
                SDL_RenderFillRect(renderer, &tileRect);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderDrawLine(renderer, x*64, y*64, x*64 + 64, y*64);
                SDL_RenderDrawLine(renderer, x*64+64, y*64, x*64 + 64, y*64 + 64);
                SDL_RenderDrawLine(renderer, x*64 + 64, y*64 + 64, x*64, y*64 + 64);
                SDL_RenderDrawLine(renderer, x*64, y*64 + 64, x*64, y*64);
            }
            else if(map[y*mapX+x]==0) 
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                tileRect.x = x*64;
                tileRect.y = y*64;
                SDL_RenderFillRect(renderer, &tileRect);

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawLine(renderer, x*64, y*64, x*64 + 64, y*64);
                SDL_RenderDrawLine(renderer, x*64+64, y*64, x*64 + 64, y*64 + 64);
                SDL_RenderDrawLine(renderer, x*64 + 64, y*64 + 64, x*64, y*64 + 64);
                SDL_RenderDrawLine(renderer, x*64, y*64 + 64, x*64, y*64);
            }
        }
    }
}

int main(int argc, char* args[]){

    //Initialize window
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("RayCastin'", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 512, SDL_WINDOW_ALLOW_HIGHDPI);
    if(window)
    {
        renderer = SDL_CreateRenderer(window, -1, 0);
        if(renderer)
        {
            RunState = true;
            surface = SDL_GetWindowSurface(window);
        }
    }

    //Looping
    while(RunState)
    {
        //Handel event
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT)
        {
            RunState = false;
        }
        if(event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym){
                case SDLK_UP:
                    py+=pdY;
                    px+=pdX;
                    break;
                case SDLK_DOWN:
                    py-=pdY;
                    px-=pdX;
                    break;
                case SDLK_LEFT:
                    pA -= 0.1f;
                    if(pA < 0) pA += 2*PI;
                    pdX = cos(pA) * vel;
                    pdY = sin(pA) * vel;
                    break;
                case SDLK_RIGHT:
                    pA += 0.1f;
                    if(pA >= 2.0f*PI) pA -= 2.0f*PI;
                    pdX = cos(pA)*vel;
                    pdY = sin(pA)*vel;
                    break;
            }
        }
        player.x = (int)px;
        player.y = (int)py;
        //Render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        Drawmap();
        DrawRay3d();
        SDL_Rect temp = player;
        temp.x -= player.w/2;
        temp.y -= player.h/2;

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &temp);
        SDL_RenderDrawLine(renderer, px, py, px + 30*cos(pA), py + 30*sin(pA));

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;

    return 0;
}