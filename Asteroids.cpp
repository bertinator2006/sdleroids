#include <iostream>
#include <cmath>
#define SDL_MAIN_HANDLED
#include <SDL.h>

typedef struct position {
    double x;
    double y;
} position;

bool init();
bool run_game();
void close();
double degrees_to_radians(double degrees);
position angle_to_position(position origin, double angle);
void render_character(position char_pos, double char_angle);


const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;
const int FRAMETIME = 1000 / 4;
const int GLOBAL_SPEED = 100;
const int PLAYER_SIZE = 10;


SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Renderer* renderer = NULL;
SDL_Rect player_rect;


double player_angle;
position player_pos;


int main()
{
    run_game();
	return 0;
}


bool run_game()
{
    init();
    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;
    bool quit = false;
    SDL_Event e;
    player_pos.x = SCREEN_WIDTH / 2;
    player_pos.y = SCREEN_HEIGHT / 2;
    player_angle = 0;

    while (!quit)
    {
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        deltaTime = (double)((NOW - LAST) / (double)SDL_GetPerformanceFrequency());

        while (SDL_PollEvent(&e)) //input
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }

            const Uint8* key_state= SDL_GetKeyboardState(NULL);

            if (key_state[SDL_SCANCODE_LEFT])
            {
                player_angle -= 10;
            }
            if (key_state[SDL_SCANCODE_RIGHT])
            {
                player_angle += 10;
            }
            //THERE IS A PROBLEM HERE WITH HOLDING THE KEYDOWN
            //AND THIS PROBLEM NEEDS TO BE RESOLVED BEFORE ANYTHING ELSE
            //BELOW IS A PROBLEM THAT I HAVE PURPOSELY ADDED TO REMIND MYSELF TO SOLVE THIS ISSUE

        }
        

        //rendering
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        render_character(player_pos, player_angle);
        SDL_RenderPresent(renderer);
    }

    //runs if quit == true
    close();
    return 0;
}


void render_character(position char_pos, double char_angle)
{
    position d1 = angle_to_position(char_pos, char_angle);
    position d2 = angle_to_position(char_pos, char_angle + 150);
    position d3 = angle_to_position(char_pos, char_angle + 210);
    SDL_RenderDrawLine(renderer, d1.x, d1.y, d2.x, d2.y);
    SDL_RenderDrawLine(renderer, d1.x, d1.y, d3.x, d3.y);
    SDL_RenderDrawLine(renderer, d2.x, d2.y, d3.x, d3.y);
}
    
double degrees_to_radians(double degrees)
{
    return degrees * (M_PI / 180);
}

position angle_to_position(position origin, double angle)
{
    position point;
    point.x = origin.x + sin(degrees_to_radians(angle)) * PLAYER_SIZE;
    point.y = origin.y - cos(degrees_to_radians(angle)) * PLAYER_SIZE;
    return point;
}

bool init()
{
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            std::cout << "Window could not be created! SDL_Error: %s\n" << SDL_GetError() << std::endl;
            success = false;
        }
        else
        {

            renderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (renderer == NULL)
            {
                printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
                success = false;
            }
        }
    }

    return success;
}

void close()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
}