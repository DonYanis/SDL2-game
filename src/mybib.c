#include<SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"mybib.h"



void SDL_DestroyResources(SDL_Window *window,SDL_Renderer *render,SDL_Texture *texture){
    if(texture!=NULL)
        SDL_DestroyTexture(texture);
    if(render!=NULL)
        SDL_DestroyRenderer(render);
    if(window!=NULL)
        SDL_DestroyWindow(window);
}



void sdl_error_quit(const char *msg){
    SDL_Log("ERROR : %s %s\n",msg,SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}
void SDL_Draw_Kab(SDL_Renderer *renderer,int x, int y, int size){

        SDL_Rect kab[7];
        for(int i=0;i<4;i++){
            kab[i].w=size;
            kab[i].h=2*size;
        }

    kab[0].x=x;          kab[0].y=y;
    kab[1].x=x+4*size;   kab[1].y=y;
    kab[2].x=x;    kab[2].y=y+5*size;
    kab[3].x=x+4*size;   kab[3].y=y+5*size;

    kab[4].x=x+size;   kab[4].y=y+size;   kab[4].h=size;   kab[4].w=3*size;
    kab[5].x=x+size;   kab[5].y=y+5*size;   kab[5].h=size;   kab[5].w=3*size;
    kab[6].x=x+2*size;   kab[6].y=y;   kab[6].h=7*size;   kab[6].w=size;
    kab[7].x=x+size;   kab[7].y=y+3*size;   kab[7].h=size;   kab[7].w=3*size;

    if(SDL_RenderFillRects(renderer,kab,8)!=0)
        sdl_error_quit("rects not drawn");
    }
    SDL_bool sdl_PointInRect(int x,int y,SDL_Rect rect){
        if((x>rect.x)&&(x<rect.x+rect.w)&&(y>rect.y)&&(y<rect.y+rect.h))
            return SDL_TRUE;
        else
            return SDL_FALSE;
    }
    void sdl_scrolling(int *xvel,int *yvel,int w, int h,SDL_Rect *rect){
        
        if(rect->x<=0){
            rect->x=0;
            *xvel=-(*xvel);
        }
        if(rect->x>=w-rect->w){
            rect->x=w-rect->w;
            *xvel=-(*xvel);
        }
        if(rect->y<=0){
            rect->y=0;
            *yvel=-(*yvel);
        }
        if(rect->y>=h-rect->h){
            rect->y=h-rect->h;
            *yvel=-(*yvel);
        }   

        rect->x+=(*xvel);
        rect->y+=(*yvel);
       
    }

    ////
    void sdl_FollowMouse(int *speed,int w, int h,SDL_Rect *rect){
        
        int mouse_x,mouse_y,target_x,target_y,distance,xvel,yvel;

        SDL_GetMouseState(&mouse_x,&mouse_y);
        target_x=mouse_x-rect->w/2;
        target_y=mouse_y-rect->h/2;
        distance=sqrt(pow(mouse_y-target_y,2)+pow(mouse_x-target_x,2));

        xvel=(target_x-rect->x)*(*speed)/distance;
        yvel=(target_y-rect->y)*(*speed)/distance;

        if(rect->x<=0){
            rect->x=0;
        }
        if(rect->x>=w-rect->w){
            rect->x=w-rect->w;  
        }
        if(rect->y<=0){
            rect->y=0;  
        }
        if(rect->y>=h-rect->h){
            rect->y=h-rect->h;
        }
    rect->x+=xvel;
    rect->y+=yvel;
    }

    /////
    void sdl_RunAway(int *speed,int w, int h,int field,SDL_Rect *rect){
        
        int mouse_x,mouse_y,target_x,target_y,distance,xvel,yvel;
        SDL_Rect f;
        f.x=rect->x+(rect->w-field*rect->w)/2;
        f.y=rect->y+(rect->h-field*rect->h)/2;
        f.w=field*rect->w;
        f.h=field*rect->h;
        SDL_GetMouseState(&mouse_x,&mouse_y);
        target_x=mouse_x-rect->w/2;
        target_y=mouse_y-rect->h/2;
        distance=sqrt(pow(mouse_y-target_y,2)+pow(mouse_x-target_x,2));

    if(sdl_PointInRect(mouse_x,mouse_y,f)){
        xvel=(target_x-rect->x)*(-(*speed))/distance;
        yvel=(target_y-rect->y)*(-(*speed))/distance;

        if(rect->x<=0){
            rect->x=0;
        }
        if(rect->x>=w-rect->w){
            rect->x=w-rect->w;  
        }
        if(rect->y<=0){
            rect->y=0;  
        }
        if(rect->y>=h-rect->h){
            rect->y=h-rect->h;
        }
    rect->x+=xvel;
    rect->y+=yvel;
    }
    }