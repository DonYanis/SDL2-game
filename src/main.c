#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"mybib.h"
#define WIN_WID 1067
#define WIN_HIGH 600
#define speed 5
#define falling_speed 2
#define step 50
#define y_diff_down 10
#define diff_bar_vertc 150
#define diff_bar_horz 300
#define xpos_int 300
#define ypos_int -400
//gcc src/main.c -o bin/game -I include -L lib -lmingw32 -lSDL2main -lSDL2 -mwindows
//for the icon in cmd : windres *.rc -o *.o

int main(int argc, char **argv){

//init:      
        SDL_Window *window;
        SDL_Renderer *render;
        
        if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)!=0)
                sdl_error_quit("SDL not initialized");

        TTF_Init();
        Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048);
        window=SDL_CreateWindow("JUMPPING CAKE",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WIN_WID,WIN_HIGH,0);
        if(window==NULL)
                sdl_error_quit("window not created");
      
        render=SDL_CreateRenderer(window,-1,SDL_RENDERER_PRESENTVSYNC);
        if(render==NULL){
                SDL_DestroyResources(window,NULL,NULL);
                sdl_error_quit("render not ceated");
        }
/*------------------------------------------------------------*/
// sound:
        Mix_Music *background_music=NULL;
        Mix_Chunk *hit=NULL,*jump=NULL;

        background_music=Mix_LoadMUS("audio/1.mp3");
        hit=Mix_LoadWAV("audio/Bass.wav");
        jump=Mix_LoadWAV("audio/jump.wav");

        if(background_music==NULL){
                SDL_DestroyResources(window,render,NULL);
                sdl_error_quit("backgruond not loaded");
        }
        if(hit==NULL){
                SDL_DestroyResources(window,render,NULL);
                sdl_error_quit("backgruond not loaded");
        }
        if(jump==NULL){
                SDL_DestroyResources(window,render,NULL);
                sdl_error_quit("backgruond not loaded");
        }
/*------------------------------------------------------------*/

//world:
        SDL_Surface *img=NULL,*text=NULL;
        SDL_Texture *object=NULL,*background=NULL,*ground1=NULL,*ground2=NULL,*obst[10];
        SDL_Rect    obj,backg,grnd1,grnd2,obstacle[10],fixer;

//background :
        img=IMG_Load("img/background.jpg");
        if(img==NULL){
                SDL_DestroyResources(window,render,NULL);
                sdl_error_quit("backgruond not loaded");
        }
        background=SDL_CreateTextureFromSurface(render,img);
        if(background==NULL){
           SDL_DestroyResources(window,render,NULL);
                sdl_error_quit("backgruond not created");     
        }

        if(SDL_QueryTexture(background,NULL,NULL,&backg.w,&backg.h)!=0){
                SDL_DestroyResources(window,render,background);
                sdl_error_quit("error in query background");
        }
        backg.x=0;backg.y=0;
        if(SDL_RenderCopy(render,background,NULL,&backg)!=0){
                SDL_DestroyResources(window,render,background);
                sdl_error_quit("error in presenting background");
        }
        SDL_FreeSurface(img);
/*------------------------------------------------------------*/

//ground:
        img=NULL;
        img=IMG_Load("img/ground.jpg");
        if(img==NULL){
                SDL_DestroyResources(window,render,background);
                sdl_error_quit("gruond not loaded");
        }

        ground1=SDL_CreateTextureFromSurface(render,img);
        ground2=SDL_CreateTextureFromSurface(render,img);
        if((ground1==NULL)||(ground2==NULL)){
           SDL_DestroyResources(window,render,background);
                sdl_error_quit("gruond not created");     
        }

        if(SDL_QueryTexture(ground1,NULL,NULL,&grnd1.w,&grnd1.h)!=0){
                SDL_DestroyResources(window,render,background);
                sdl_error_quit("error in query ground");
        }
        grnd1.x=0;              grnd1.y=WIN_HIGH-grnd1.h;
        grnd2.w=grnd1.w;        grnd2.h=grnd1.h         ;grnd2.x=WIN_WID        ;grnd2.y=grnd1.y;


        SDL_FreeSurface(img);
/*------------------------------------------------------------*/

//object:
        img=NULL;
        img=IMG_Load("img/cake.png");
        SDL_SetColorKey(img,SDL_TRUE,SDL_MapRGB(img->format,0,0,0));
        if(img==NULL){
                SDL_DestroyResources(window,render,background);
                sdl_error_quit("cake not loaded");
        }
         
        object=SDL_CreateTextureFromSurface(render,img);
        if(object==NULL){
                SDL_DestroyResources(window,render,background);
                sdl_error_quit("cake not created");
        }
        if(SDL_QueryTexture(object,NULL,NULL,&obj.w,&obj.h)!=0){
                SDL_DestroyResources(window,render,background);
                sdl_error_quit("error in query cake");
        }

/*--------------------------------------------------------------*/
//window icon:
        SDL_SetWindowIcon(window,img);
        SDL_FreeSurface(img);              
/*--------------------------------------------------------------*/

//obstacles:
        img=NULL;
        img=IMG_Load("img/bar.png");
        if(img==NULL){
                SDL_DestroyResources(window,render,background);
                sdl_error_quit("bar not loaded");
        }

        for(int i=0;i<10;i++){
        obst[i]=SDL_CreateTextureFromSurface(render,img);
        if(obst[i]==NULL){
                SDL_DestroyResources(window,render,background);
                sdl_error_quit("bar not created");
        }
        if(SDL_QueryTexture(obst[i],NULL,NULL,&obstacle[i].w,&obstacle[i].h)!=0){
                SDL_DestroyResources(window,render,background);
                sdl_error_quit("error in query bar");
        }
        }
        SDL_FreeSurface(img);
/*--------------------------------------------------------------*/
//font:
        int distance=0;
        char dist[20];
        SDL_Surface *meters=NULL;
        SDL_Texture *txt,*txt2,*distance_tex;
        TTF_Font *police=NULL;
        SDL_Color myColor={255,0,255};
        SDL_Rect texr,score_rec={260,0,100,100};
//score:       
        police=TTF_OpenFont("font/JOKERMAN.ttf",80);
        if(police==NULL){
                SDL_DestroyResources(window,render,background);
                sdl_error_quit("error in loading police");
        }

        text=TTF_RenderText_Solid(police,"distance : ",myColor);
        if(text==NULL){
                SDL_DestroyResources(window,render,background);
                sdl_error_quit("error in loading text:");
        }
        
        distance_tex=SDL_CreateTextureFromSurface(render,text);
        if(distance_tex==NULL){
                SDL_DestroyResources(window,render,background);
                sdl_error_quit("error in loading txt");
        }

        texr.x=0;
        texr.y=0;
        texr.w=250;
        texr.h=100;
      
        sprintf(dist,"%dm",distance);  
        meters=TTF_RenderText_Solid(police,dist,myColor);
        if(meters==NULL){
                SDL_DestroyResources(window,render,background);
                sdl_error_quit("error in loading text:");
        }
        
        txt=SDL_CreateTextureFromSurface(render,meters);
        if(txt==NULL){
                SDL_DestroyResources(window,render,background);
                sdl_error_quit("error in loading txt");
        }
        if(SDL_RenderCopy(render,txt,NULL,&score_rec)!=0){
                         SDL_DestroyResources(window,render,background);
                         sdl_error_quit("error in presenting text");
                }

/*--------------------------------------------------------------------*/
//final score :
        SDL_Color blue={0,0,255};
        SDL_Surface *score_surface=NULL;
        SDL_Texture *final_score=NULL;
        SDL_Rect fscore={450,427,50,50};


/*---------------------------------------------------------------*/
//game over:
        SDL_Rect lost_rec;
        img=IMG_Load("img/lost.png");
          if(img==NULL){
                SDL_DestroyResources(window,render,background);
                sdl_error_quit("lost not loaded");
        }
        SDL_SetColorKey(img,SDL_TRUE,SDL_MapRGB(img->format,255,255,255));

        SDL_Texture *game_over=NULL;
        game_over=SDL_CreateTextureFromSurface(render,img);
        if(game_over==NULL){
                SDL_DestroyResources(window,render,background);
                sdl_error_quit("lost not created");
        }
        if(SDL_QueryTexture(game_over,NULL,NULL,&lost_rec.w,&lost_rec.h)!=0){
                SDL_DestroyResources(window,render,background);
                sdl_error_quit("error in query lost");
        }
        lost_rec.x=0;
        lost_rec.y=0;
       SDL_FreeSurface(img);
/*--------------------------------------------------------------*/
//starting a new game :
        new_game:
        Mix_PauseMusic();
        Mix_VolumeMusic(10);
        distance=0;

        obj.x=150;
        obj.y=200;

        obstacle[0].x=xpos_int;                 obstacle[0].y=ypos_int;
        obstacle[1].x=xpos_int;                 obstacle[1].y=ypos_int+obstacle[0].h+diff_bar_vertc;
        obstacle[2].x=xpos_int+diff_bar_horz;   obstacle[2].y=ypos_int+50;
        obstacle[3].x=xpos_int+diff_bar_horz;   obstacle[3].y=obstacle[2].y+obstacle[0].h+diff_bar_vertc;
        obstacle[4].x=xpos_int+2*diff_bar_horz; obstacle[4].y=ypos_int-50;
        obstacle[5].x=xpos_int+2*diff_bar_horz; obstacle[5].y=obstacle[4].y+obstacle[0].h+diff_bar_vertc;
        obstacle[6].x=xpos_int+3*diff_bar_horz; obstacle[6].y=ypos_int+100;
        obstacle[7].x=xpos_int+3*diff_bar_horz; obstacle[7].y=obstacle[6].y+obstacle[0].h+diff_bar_vertc;
        obstacle[8].x=xpos_int+4*diff_bar_horz; obstacle[8].y=ypos_int-100;
        obstacle[9].x=xpos_int+4*diff_bar_horz; obstacle[9].y=obstacle[8].y+obstacle[0].h+diff_bar_vertc;

        for(int i=0;i<10;i++){
                if(SDL_RenderCopy(render,obst[i],NULL,&obstacle[i])!=0){
                SDL_DestroyResources(window,render,background);
                sdl_error_quit("error in presenting bars");
        }

        }

        SDL_RenderPresent(render);
        
        
/*------------------------------------------------------------------*/
//main loop:
        int x=-5;
        int change=100,now,before=SDL_GetTicks();
        SDL_bool open=SDL_TRUE,loose=SDL_FALSE;
        while(open)
        {
                SDL_Event event;
                while(SDL_PollEvent(&event)){
                        switch(event.type)
                        {

                        case SDL_KEYDOWN:
                        case SDL_MOUSEBUTTONDOWN:
                                if((event.key.keysym.sym==SDLK_SPACE)||(event.button.button==SDL_BUTTON_LEFT)){
                                        Mix_PlayChannel(-1,jump,0);
                                        obj.y-=step;
                                }
                        continue;
                        case SDL_QUIT:
                                goto quit;
                                break;
                        
                        default:
                                continue;
                        }
                }

        grnd1.x-=2;
        grnd2.x-=2;
        if(grnd1.x<=-grnd1.w)
                grnd1.x=WIN_WID;
        if(grnd2.x<=-grnd2.w)
                grnd2.x=WIN_WID;
        
        if(SDL_RenderCopy(render,background,NULL,&backg)!=0){
                SDL_DestroyResources(window,render,background);
                sdl_error_quit("error in presenting background");
        }

        if(SDL_RenderCopy(render,ground1,NULL,&grnd1)!=0){
                SDL_DestroyResources(window,render,background);
                sdl_error_quit("error in presenting ground");
        }
        if(SDL_RenderCopy(render,ground2,NULL,&grnd2)!=0){
                SDL_DestroyResources(window,render,background);
                sdl_error_quit("error in presenting ground");
        }
        

        obj.y+=falling_speed;
        if(obj.y<=0)
                obj.y=0;
        
        SDL_Delay(16);

        if(SDL_RenderCopy(render,object,NULL,&obj)!=0){
                SDL_DestroyResources(window,render,background);
                sdl_error_quit("error in presenting cake");
        }
        
        for(int i=0;i<10;i++){
                obstacle[i].x--;
                if(obstacle[i].x<=-150){
                        obstacle[i].x=1350;
                        obstacle[i].y+=change;
                if((i+1)%2==0)
                        change=-change;
                }

                if(SDL_RenderCopy(render,obst[i],NULL,&obstacle[i])!=0){
                SDL_DestroyResources(window,render,background);
                sdl_error_quit("error in presenting bars");
        }
        }

        if(SDL_RenderCopy(render,distance_tex,NULL,&texr)!=0){
                SDL_DestroyResources(window,render,background);
                sdl_error_quit("error in presenting distance");
        }
/*------------------------------------------------------------------*/
//timer:        
        now=SDL_GetTicks();
        if(x<0){     
                txt=SDL_CreateTextureFromSurface(render,meters);
                if(SDL_RenderCopy(render,txt,NULL,&score_rec)!=0){
                         SDL_DestroyResources(window,render,background);
                         sdl_error_quit("error in presenting text");
                }}else {
                if(SDL_RenderCopy(render,txt2,NULL,&score_rec)!=0){
                         SDL_DestroyResources(window,render,background);
                          sdl_error_quit("error in presenting text");
                 }
                 }
        if(now-before>=1000){               
       
        distance++;  
        sprintf(dist,"%dm",distance);
        meters=TTF_RenderText_Solid(police,dist,myColor);

        if(meters==NULL){
                SDL_DestroyResources(window,render,background);
                sdl_error_quit("errror in loading text:");
        }

        txt2=SDL_CreateTextureFromSurface(render,meters);
        if(txt2==NULL){
                SDL_DestroyResources(window,render,background);
                sdl_error_quit("error in loading txt");
        }
        
x=-x;
before=now;
        }

/*-----------------------*/
        fixer.x=obj.x;
        fixer.y=obj.y+5;
        fixer.h=obj.h-y_diff_down-5;
        fixer.w=obj.w;

       if(obj.y+obj.h-y_diff_down>=grnd1.y){
                open=SDL_FALSE;
                goto retry;
                break;
       }
       
        for(int i=0;i<10;i++){
                
                if(SDL_HasIntersection(&fixer,&obstacle[i])){
                        open=SDL_FALSE;
                        goto retry;
                        break;
                }
        }

SDL_RenderPresent(render); 
        }

/*-------------------------------------------------------------*/
//party lost:
        retry:
        Mix_PlayChannel(-1,hit,0);
        Mix_PlayMusic(background_music,-1);
        open=SDL_TRUE;
        while(open){

                SDL_Event event;
                while(SDL_PollEvent(&event)){
                        if((event.key.keysym.sym==SDLK_RETURN)||(event.key.keysym.sym==SDLK_KP_ENTER)||(event.button.button==SDL_BUTTON_LEFT))
                                goto new_game;
                        else if((event.key.keysym.sym==SDLK_ESCAPE)||(event.type==SDL_QUIT)){
                                quit:
                                open=SDL_FALSE;
                                break;
                        }
                        }
                if((event.key.keysym.sym!=SDLK_ESCAPE)&&(event.type!=SDL_QUIT)){
                if(SDL_RenderCopy(render,game_over,NULL,&lost_rec)!=0){
                SDL_DestroyResources(window,render,background);
                sdl_error_quit("error in presenting gameover");
                }

                sprintf(dist,"%dm",distance);
                score_surface=TTF_RenderText_Solid(police,dist,blue);
                if(score_surface==NULL){
                        SDL_DestroyResources(window,render,background);
                        sdl_error_quit("error in loading score:");
                }
        
                final_score=SDL_CreateTextureFromSurface(render,score_surface);
                if(final_score==NULL){
                        SDL_DestroyResources(window,render,background);
                        sdl_error_quit("error in loading final score");
                 }
                if(SDL_RenderCopy(render,final_score,NULL,&fscore)!=0){
                SDL_DestroyResources(window,render,background);
                sdl_error_quit("error in presenting fscore");
                }
                SDL_RenderPresent(render);
        }}

/*-------------------------------------------------------*/
        Mix_FreeChunk(hit);
        Mix_FreeChunk(jump);
        Mix_FreeMusic(background_music);
        TTF_CloseFont(police);
        SDL_DestroyResources(window,render,background);
        SDL_Quit();
        TTF_Quit();
        return EXIT_SUCCESS;
}