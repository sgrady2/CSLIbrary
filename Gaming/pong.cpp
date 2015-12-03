/*
FreePong
 Written by Robin Stjerndorff 2008
License: GPLV3
*/

#include <string>
#include <sstream>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
using namespace std;

const int Framerate=60;

bool run=true, BallDirection=false, UseBot=true;
int r_width=1024, r_height=768;
int Player1X, Player2X;
int Player1Score=0, Player2Score=0, MaxScore=15;
double BallY, BallX, BallAngle=0, Player1Y, Player2Y, MoveSpeed=3, BallSpeed=6;

SDL_Surface *bar, *bar2, *ball;
Mix_Chunk *SndGame, *SndHit, *SndScore;
TTF_Font *Font;

class Timer {
    private:
        int startTicks;

    public:
        Timer():startTicks(0) {}

    void start() {
        startTicks=SDL_GetTicks();    
    }

    int get_ticks() {
        return SDL_GetTicks() - startTicks;
    }
};

void RenderImg(SDL_Surface* a,SDL_Surface* b,int x,int y) {
    SDL_Rect c={x,y,0,0};
    SDL_BlitSurface(a,0,b,&c);
}

void ResetScene() {
    BallAngle=0;
    Player1X=10;
    Player1Y=r_height/2-bar->h/2;
    Player2X=r_width-12;
    Player2Y=r_height/2-bar->h/2;
    BallX=r_width/2-ball->w/2;
    BallY=r_height/2-ball->h/2;
    BallSpeed+=0.3;
    MoveSpeed+=0.3;
}

double SetBallAngle(double PlayerY) {
    if(BallY>PlayerY+bar->h/2+bar->h/4)
        return MoveSpeed;
    else if(BallY<PlayerY+bar->h/2-bar->h/4)
        return -MoveSpeed/1.5;
    else if(BallY>PlayerY+bar->h/2+bar->h/12)
        return MoveSpeed;
    else if(BallY<PlayerY+bar->h/2-bar->h/12)
        return -MoveSpeed/3;
    return 0;
}

void UpdateScore(SDL_Surface *screen) {
    static SDL_Color TextColor={28,28,28,0};
    SDL_Surface *tmp;
    string tmpstr;

    if(Player1Score<MaxScore && Player2Score<MaxScore) {
        stringstream sstream;
        sstream<<Player1Score<<" - "<<Player2Score;
        tmpstr=sstream.str();

        tmp=TTF_RenderUTF8_Blended(Font,tmpstr.c_str(),TextColor);
        RenderImg(tmp,screen,r_width/2-tmp->w/2,r_height/2-tmp->h/2);
        SDL_FreeSurface(tmp);
    }
    else {
        Player1Score=0;
        Player2Score=0;
        BallSpeed=8;
        MoveSpeed=3;
        ResetScene();
    }
}

int main(int argc, char **argv) {
    Timer FPSTimer;
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    Mix_OpenAudio(44100,AUDIO_S16SYS,2,1024);

    SDL_Event e;

    bar=IMG_Load("gfx/bar.png");
    bar2=IMG_Load("gfx/bar2.png");
    ball=IMG_Load("gfx/ball.png");

    SndGame=Mix_LoadWAV("sfx/game.wav");
    SndHit=Mix_LoadWAV("sfx/hit.wav");
    SndScore=Mix_LoadWAV("sfx/score.wav");

    Font=TTF_OpenFont("gfx/sans.ttf",40);

    SDL_Surface *screen=SDL_SetVideoMode(r_width,r_height,32,0);
    SDL_WM_SetCaption("FreePong","FreePong");
    SDL_ShowCursor(SDL_DISABLE);
    ResetScene();
    Mix_PlayChannel(-1,SndGame,0);

        while(run) {
            FPSTimer.start();
            SDL_PollEvent(&e); 
            if(e.type==SDL_QUIT)
                run=false;
            if(e.type==SDL_KEYDOWN) {
                switch(e.key.keysym.sym) {
                    case SDLK_ESCAPE: run=false; break;
                    case SDLK_F4:
                        static bool fullscreen=false;
                        fullscreen=!fullscreen;
                        screen=SDL_SetVideoMode(r_width,r_height,32,fullscreen?SDL_FULLSCREEN:0);
                    break;
                    case SDLK_q:
                        if(Player1Y>0)
                            Player1Y-=MoveSpeed;
                    break;
                    case SDLK_a:
                        if(Player1Y<r_height-bar->h)
                            Player1Y+=MoveSpeed;
                    break;
                    case SDLK_o:
                        if(!UseBot) {
                            if(Player2Y>0)
                                Player2Y-=MoveSpeed;
                        }
                    break;
                    case SDLK_l:
                        if(!UseBot) {
                            if(Player2Y<r_height-bar->h)
                                Player2Y+=MoveSpeed;
                        }
                    break;
                    default:break;
                }
            }

            if(BallX<-ball->h) {
                ++Player2Score;
                ResetScene();
                Mix_PlayChannel(-1,SndScore,0);
            }

            if(BallX>r_width+ball->h) {
                ++Player1Score;
                ResetScene();
                Mix_PlayChannel(-1,SndScore,0);
            }

            //if ball hits player 1
            if(BallX<=Player1X && BallY>Player1Y && BallY<Player1Y+bar->h) {
                BallDirection=true;
                BallAngle=SetBallAngle(Player1Y); //update ball angle for a player 1 hit
                Mix_PlayChannel(-1,SndHit,0);
            }

            //if ball hits player 2
            if(BallX+ball->w>=Player2X && BallY>Player2Y && BallY<Player2Y+bar->h) {
                BallDirection=false;
                BallAngle=SetBallAngle(Player2Y);
                Mix_PlayChannel(-1,SndHit,0);
            }

            if(UseBot) {
                if(BallY>=Player2Y+bar2->h/2)
                    Player2Y+=MoveSpeed*0.89;
                if(BallY<=Player2Y+bar2->h/2)
                    Player2Y-=MoveSpeed*0.89;
            }

            if(BallDirection) {
                BallX+=BallSpeed;
                BallY+=BallAngle;
            }
            else {
                BallX-=BallSpeed;
                BallY+=BallAngle;
            }

            if(BallY<=1||BallY>=r_height-ball->h) {
                Mix_PlayChannel(-1,SndHit,0);
                if(BallAngle)
                    BallAngle-=BallAngle*2;
                else
                    BallAngle+=BallAngle*2;
            }

            SDL_FillRect(screen,NULL,0);
            UpdateScore(screen);
            RenderImg(bar,screen,Player1X,int(Player1Y));
            RenderImg(bar2,screen,Player2X,int(Player2Y));
            RenderImg(ball,screen,int(BallX),int(BallY));
            SDL_Flip(screen);
            if(FPSTimer.get_ticks()<1000/Framerate)
                SDL_Delay((1000/Framerate)-FPSTimer.get_ticks());
        }
    SDL_Quit();
    return 0;
}  /*
FreePong
 Written by Robin Stjerndorff 2008
License: GPLV3
*/

#include <string>
#include <sstream>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
using namespace std;

const int Framerate=60;

bool run=true, BallDirection=false, UseBot=true;
int r_width=1024, r_height=768;
int Player1X, Player2X;
int Player1Score=0, Player2Score=0, MaxScore=15;
double BallY, BallX, BallAngle=0, Player1Y, Player2Y, MoveSpeed=3, BallSpeed=6;

SDL_Surface *bar, *bar2, *ball;
Mix_Chunk *SndGame, *SndHit, *SndScore;
TTF_Font *Font;

class Timer {
    private:
        int startTicks;

    public:
        Timer():startTicks(0) {}

    void start() {
        startTicks=SDL_GetTicks();    
    }

    int get_ticks() {
        return SDL_GetTicks() - startTicks;
    }
};

void RenderImg(SDL_Surface* a,SDL_Surface* b,int x,int y) {
    SDL_Rect c={x,y,0,0};
    SDL_BlitSurface(a,0,b,&c);
}

void ResetScene() {
    BallAngle=0;
    Player1X=10;
    Player1Y=r_height/2-bar->h/2;
    Player2X=r_width-12;
    Player2Y=r_height/2-bar->h/2;
    BallX=r_width/2-ball->w/2;
    BallY=r_height/2-ball->h/2;
    BallSpeed+=0.3;
    MoveSpeed+=0.3;
}

double SetBallAngle(double PlayerY) {
    if(BallY>PlayerY+bar->h/2+bar->h/4)
        return MoveSpeed;
    else if(BallY<PlayerY+bar->h/2-bar->h/4)
        return -MoveSpeed/1.5;
    else if(BallY>PlayerY+bar->h/2+bar->h/12)
        return MoveSpeed;
    else if(BallY<PlayerY+bar->h/2-bar->h/12)
        return -MoveSpeed/3;
    return 0;
}

void UpdateScore(SDL_Surface *screen) {
    static SDL_Color TextColor={28,28,28,0};
    SDL_Surface *tmp;
    string tmpstr;

    if(Player1Score<MaxScore && Player2Score<MaxScore) {
        stringstream sstream;
        sstream<<Player1Score<<" - "<<Player2Score;
        tmpstr=sstream.str();

        tmp=TTF_RenderUTF8_Blended(Font,tmpstr.c_str(),TextColor);
        RenderImg(tmp,screen,r_width/2-tmp->w/2,r_height/2-tmp->h/2);
        SDL_FreeSurface(tmp);
    }
    else {
        Player1Score=0;
        Player2Score=0;
        BallSpeed=8;
        MoveSpeed=3;
        ResetScene();
    }
}

int main(int argc, char **argv) {
    Timer FPSTimer;
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    Mix_OpenAudio(44100,AUDIO_S16SYS,2,1024);

    SDL_Event e;

    bar=IMG_Load("gfx/bar.png");
    bar2=IMG_Load("gfx/bar2.png");
    ball=IMG_Load("gfx/ball.png");

    SndGame=Mix_LoadWAV("sfx/game.wav");
    SndHit=Mix_LoadWAV("sfx/hit.wav");
    SndScore=Mix_LoadWAV("sfx/score.wav");

    Font=TTF_OpenFont("gfx/sans.ttf",40);

    SDL_Surface *screen=SDL_SetVideoMode(r_width,r_height,32,0);
    SDL_WM_SetCaption("FreePong","FreePong");
    SDL_ShowCursor(SDL_DISABLE);
    ResetScene();
    Mix_PlayChannel(-1,SndGame,0);

        while(run) {
            FPSTimer.start();
            SDL_PollEvent(&e); 
            if(e.type==SDL_QUIT)
                run=false;
            if(e.type==SDL_KEYDOWN) {
                switch(e.key.keysym.sym) {
                    case SDLK_ESCAPE: run=false; break;
                    case SDLK_F4:
                        static bool fullscreen=false;
                        fullscreen=!fullscreen;
                        screen=SDL_SetVideoMode(r_width,r_height,32,fullscreen?SDL_FULLSCREEN:0);
                    break;
                    case SDLK_q:
                        if(Player1Y>0)
                            Player1Y-=MoveSpeed;
                    break;
                    case SDLK_a:
                        if(Player1Y<r_height-bar->h)
                            Player1Y+=MoveSpeed;
                    break;
                    case SDLK_o:
                        if(!UseBot) {
                            if(Player2Y>0)
                                Player2Y-=MoveSpeed;
                        }
                    break;
                    case SDLK_l:
                        if(!UseBot) {
                            if(Player2Y<r_height-bar->h)
                                Player2Y+=MoveSpeed;
                        }
                    break;
                    default:break;
                }
            }

            if(BallX<-ball->h) {
                ++Player2Score;
                ResetScene();
                Mix_PlayChannel(-1,SndScore,0);
            }

            if(BallX>r_width+ball->h) {
                ++Player1Score;
                ResetScene();
                Mix_PlayChannel(-1,SndScore,0);
            }

            //if ball hits player 1
            if(BallX<=Player1X && BallY>Player1Y && BallY<Player1Y+bar->h) {
                BallDirection=true;
                BallAngle=SetBallAngle(Player1Y); //update ball angle for a player 1 hit
                Mix_PlayChannel(-1,SndHit,0);
            }

            //if ball hits player 2
            if(BallX+ball->w>=Player2X && BallY>Player2Y && BallY<Player2Y+bar->h) {
                BallDirection=false;
                BallAngle=SetBallAngle(Player2Y);
                Mix_PlayChannel(-1,SndHit,0);
            }

            if(UseBot) {
                if(BallY>=Player2Y+bar2->h/2)
                    Player2Y+=MoveSpeed*0.89;
                if(BallY<=Player2Y+bar2->h/2)
                    Player2Y-=MoveSpeed*0.89;
            }

            if(BallDirection) {
                BallX+=BallSpeed;
                BallY+=BallAngle;
            }
            else {
                BallX-=BallSpeed;
                BallY+=BallAngle;
            }

            if(BallY<=1||BallY>=r_height-ball->h) {
                Mix_PlayChannel(-1,SndHit,0);
                if(BallAngle)
                    BallAngle-=BallAngle*2;
                else
                    BallAngle+=BallAngle*2;
            }

            SDL_FillRect(screen,NULL,0);
            UpdateScore(screen);
            RenderImg(bar,screen,Player1X,int(Player1Y));
            RenderImg(bar2,screen,Player2X,int(Player2Y));
            RenderImg(ball,screen,int(BallX),int(BallY));
            SDL_Flip(screen);
            if(FPSTimer.get_ticks()<1000/Framerate)
                SDL_Delay((1000/Framerate)-FPSTimer.get_ticks());
        }
    SDL_Quit();
    return 0;
}
