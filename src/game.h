#pragma once
#include <iostream>
#include <cmath>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

typedef struct position {
    double x;
    double y;
} position;


typedef struct entity {
    double x;
    double y;
    double angle;
    int size;
    double vel_x;
    double vel_y;
} entity;

//assigned in game.cpp
bool init();
void close();
double deg_to_radian(double degrees);
position angle_to_position(position origin, double angle);
void render_character(entity character);

//assigned in asteroids.cpp
bool run_game();


extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int FRAMETIME;
extern const int SPEED;

extern SDL_Window* gWindow;
extern SDL_Surface* gScreenSurface;
extern SDL_Renderer* renderer;
extern SDL_Rect player_rect;

extern entity player;
