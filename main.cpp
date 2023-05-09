#include <iostream>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define PI 3.1415926535

using namespace std;

SDL_Event event;
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Surface* surface;
float pdX, pdY, pA;
bool RunState;
int vel = 10;
int mapX = 8, mapY = 9, mapS = 64;
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

class Player{
    public:
        SDL_Rect dest = {100, 100, 10, 10};
};

Player player;

void DrawRay3d()
{
    int r, mx, my, mp, dof;
    float rx, ry, ra, xo, yo;
    ra = pA;
    for(r = 0; r > -1; r--)
    {
        dof = 0;
        float aTan = -1/tan(ra);
        if(ra>PI)
        {
            ry = (((int)pdY/64)*64)-0.0001;
            rx = (pdY-ry)*aTan+pdX;
            yo = -64;
            xo= -yo*aTan;
        }
        if(ra<PI)
        {
            ry = (((int)pdY/64)*64)+64;
            rx = (pdY-ry)*aTan+pdX;
            yo = 64;
            xo= -yo*aTan;
        }
        if(ra==0 || ra==PI)
        {
            rx = pdX;
            ry = pdY;
            dof = 8;
        }
        while(dof<8)
        {
            mx = (int)(rx)/64;
            my = (int)(ry)/64;
            mp = my*mapX+mx;
            if(mp<mapX*mapY && map[mp]==1)
            {
                dof = 8;
            }
            else{
                rx += xo;
                ry += yo;
                dof +=1;
            }
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderDrawLine(renderer, player.dest.x+player.dest.w/2+pdX*5, player.dest.y+player.dest.h/2+pdY*5, rx, ry);
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
                    player.dest.y+=pdY;
                    player.dest.x+=pdX;
                    break;
                case SDLK_DOWN:
                    player.dest.y-=pdY;
                    player.dest.x-=pdX;
                    break;
                case SDLK_LEFT:
                    pA-=0.1;
                    if(pA<0) pA+=2*PI;
                    pdX=cos(pA)*vel; pdY = sin(pA)*vel;
                    break;
                case SDLK_RIGHT:
                    pA+=0.1;
                    if(pA>2*PI) pA-=2*PI;
                    pdX=cos(pA)*vel; pdY = sin(pA)*vel;
                    break;
            }
        }

        //Render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        Drawmap();
        DrawRay3d();

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &player.dest);
        SDL_RenderDrawLine(renderer, player.dest.x+player.dest.w/2, player.dest.y+player.dest.h/2, player.dest.x+player.dest.w/2+pdX*5, player.dest.y+player.dest.h/2+pdY*5);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;

    return 0;
}