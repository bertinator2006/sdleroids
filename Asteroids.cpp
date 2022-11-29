#include <iostream>
#include <cmath>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

typedef struct position {
    double x;
    double y;
} position;

typedef struct bullet {
    double x, y;
    double angle;
    double vel_x, vel_y;
} bullet;

typedef struct entity {
    double x;
    double y;
    double angle;
    int size;
    double vel_x;
    double vel_y;
} entity;

bool init();
bool run_game();
void close();
double degrees_to_radians(double degrees);
position angle_to_position(position origin, double angle);
void render_character(entity character);


const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;
const int FRAMETIME = 1000 / 4;
const int GLOBAL_SPEED = 200;
int* bullets;
int bullet_count;


SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Renderer* renderer = NULL;
SDL_Rect player_rect;

entity player;


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
    SDL_Event event;

    player.x = SCREEN_WIDTH / 2;
    player.y = SCREEN_HEIGHT / 2;
    player.angle = 0;
    player.size = 10;
    player.vel_x = 0;
    player.vel_y = 0;

    bullets = {};
    bullet_count = 0;
    

    while (!quit)
    {
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        deltaTime = (double)((NOW - LAST) / (double)SDL_GetPerformanceFrequency());

        while (SDL_PollEvent(&event)) //input
        {
            if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE)
            {
                quit = true;
            }
        }

        //input
        const Uint8* key_state= SDL_GetKeyboardState(NULL);

        //player turning
        if (key_state[SDL_SCANCODE_LEFT])
            player.angle -= (double)GLOBAL_SPEED*deltaTime;

        if (key_state[SDL_SCANCODE_RIGHT])
            player.angle += (double)GLOBAL_SPEED*deltaTime;
        
        if (player.angle < 0)
            player.angle += 360;
        if(player.angle > 360)
            player.angle -= 360;
        
        if (key_state[SDL_SCANCODE_UP])
        {
            player.vel_x += sin(degrees_to_radians(player.angle))*GLOBAL_SPEED*deltaTime;
            player.vel_y -= cos(degrees_to_radians(player.angle))*GLOBAL_SPEED*deltaTime;
        }
        if (key_state[SDL_SCANCODE_SPACE])
        {
            //create bullet
        }
        //int SDL_RenderDrawPoint(SDL_Renderer * renderer, int x, int y);
        

        //game logic
        player.x += player.vel_x*deltaTime;
        player.y += player.vel_y*deltaTime;

        if (player.x - player.size> SCREEN_WIDTH)
            player.x -= SCREEN_WIDTH;
        if(player.y - player.size > SCREEN_HEIGHT)
            player.y -=SCREEN_HEIGHT;
        if (player.x + player.size < 0)
            player.x += SCREEN_WIDTH;
        if(player.y + player.size < 0 )
            player.y +=SCREEN_HEIGHT;


        //rendering
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        render_character(player);
        SDL_RenderPresent(renderer);
    }

    //runs if quit == true
    close();
    return 0;
}


void render_character(entity character)
{
    position pos;
    pos.x = character.x;
    pos.y = character.y;
    position d1 = angle_to_position(pos, character.angle);
    position d2 = angle_to_position(pos, character.angle + 150);
    position d3 = angle_to_position(pos, character.angle + 210);
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
    point.x = origin.x + sin(degrees_to_radians(angle)) * player.size;
    point.y = origin.y - cos(degrees_to_radians(angle)) * player.size;
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