#ifndef _MYBIB_H_
#define _MYBIB_H_
#include<SDL.h>
#include<stdio.h>
#include<stdlib.h>



void SDL_DestroyResources(SDL_Window *window,SDL_Renderer *render,SDL_Texture *texture);
void sdl_error_quit(const char *msg);
void SDL_Draw_Kab(SDL_Renderer *renderer,int x, int y, int size);
SDL_bool sdl_PointInRect(int x,int y,SDL_Rect rect);
void sdl_scrolling(int *xvel,int *yvel,int w, int h,SDL_Rect *rect);
void sdl_FollowMouse(int *speed,int w, int h,SDL_Rect *rect);
void sdl_RunAway(int *speed,int w, int h,int feild,SDL_Rect *rect);



#endif