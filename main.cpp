#include <bits/stdc++.h>
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
using namespace std;
const int TILE_SIZE = 50;
const int MAX_MAP_X = 24;
const int MAX_MAP_Y = 12;
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 600;
const string WINDOW_TITLE = "Ghost";

SDL_Window  * window;
SDL_Renderer * renderer;
SDL_Event e;
int Right = 0, Left = 0, up = 0, down = 0, t = 1;
int x = 10, y = 10, xb, yb, tsize = 50;
SDL_Texture *bg = NULL;
SDL_Texture *ma[3];
SDL_Texture *burger;
void logSDLError(ostream& os, const string &msg, bool fatal = false);
void logSDLError(ostream& os, const string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}
void initSDL(){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(cout, "SDL_Init", true);
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 ){
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
    }
    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) logSDLError(cout, "CreateWindow", true);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) logSDLError(cout, "CreateRenderer", true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
    }

}
void quitSDL(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
SDL_Texture* loadTexture(SDL_Renderer *renderer, const std::string &file)
{
    SDL_Texture *texture = nullptr;
	SDL_Surface *loadedImage = IMG_Load(file.c_str());
	if (loadedImage != nullptr){
		texture = SDL_CreateTextureFromSurface(renderer, loadedImage);
		SDL_FreeSurface(loadedImage);
	}
	return texture;
}
void renderTexture(SDL_Texture *texture, int x, int y, int w, int h)
{
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
    dst.w = w;
    dst.h = h;
	SDL_RenderCopy(renderer, texture, NULL, &dst);
}
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
void Move() {
    SDL_RenderClear(renderer);
    renderTexture(bg, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (Right == 1)
        renderTexture(ma[2], x, y, tsize, tsize);
    else
        renderTexture(ma[1], x, y, tsize, tsize);
    if ((x <= xb && xb <= x+tsize && y <= yb && yb <= y+tsize) || (x <= xb && xb <= x+tsize && y <= yb+30 && yb+30 <= y+tsize)) {
        t++;
        if (t == 3) {
            tsize += 5;
            t = 1;
        }
        xb = rand()%1000+10;
        yb = rand()%500+10;
    }
    else
        if ((x <= xb+30 && xb+30 <= x+tsize && y <= yb+30 && yb+30 <= y+tsize) || (x <= xb+30 && xb+30 <= x+tsize && y <= yb && yb <= y+tsize)) {
            t++;
            if (t == 3) {
                tsize += 15;
                t = 1;
            }
            xb = rand()%1000+10;
            yb = rand()%500+10;
        }
    renderTexture(burger, xb, yb, 30, 30);
    SDL_RenderPresent(renderer);
}
void input() {

}
bool go() {

    Right = 1;
    x = 10, y = 10, xb, yb, tsize = 50;
    srand(time(NULL));
    xb = rand()%1000+10;
    yb = rand()%500+10;
    renderTexture(bg, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    renderTexture(ma[2], x, y, tsize, tsize);
    renderTexture(burger, xb, yb, 30, 30);
    SDL_RenderPresent(renderer);
    t = 1;
    while(1) {
        if(SDL_PollEvent(&e) != 0&&e.type == SDL_QUIT)
            return 0;
        SDL_Delay(30);
        if ( SDL_PollEvent(&e) == 0)
            Move();
        Move();
        if(e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_RIGHT: {
                    Right = 1;
                    Left = 0;
                    up = 0;
                    down = 0;
                    x += 20;
                    break;
                }
                case SDLK_LEFT: {
                    Right = 0;
                    Left = 1;
                    up = 0;
                    down = 0;
                    x -= 20;
                    break;
                }
                case SDLK_UP: {
                    Right = 0;
                    Left = 0;
                    up = 1;
                    down = 0;
                    y -= 10;
                    break;
                }
                case SDLK_DOWN: {
                    Right = 0;
                    Left = 0;
                    up = 0;
                    down = 1;
                    y += 10;
                    break;
                }
            }
        }
        SDL_RenderPresent(renderer);
        if(x<10||y<10||x>1200-10-tsize||y>600-10-tsize)
            return 1;
        if(SDL_WaitEvent(&e) != 0 && e.type == SDL_QUIT)
            return 0;
    }
}

int main(int argc, char* argv[])
{
    initSDL();
    bg = loadTexture(renderer, "img/bg.jpg");
    SDL_Texture *play = loadTexture(renderer, "img/play.png");
    SDL_Texture *quit = loadTexture(renderer, "img/quit.png");
    burger = loadTexture(renderer, "img/burger.png");
    ma[1] = loadTexture(renderer, "img/ma1.png");
    ma[2] = loadTexture(renderer, "img/ma2.png");
    while (1) {
        while(1) {
            SDL_RenderClear(renderer);
            renderTexture(bg, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
            renderTexture(play, 500, 100, 200, 100);
            renderTexture(quit, 500, 250, 200, 100);
            SDL_RenderPresent(renderer);
            if ( SDL_WaitEvent(&e) != 0 ){
                if(e.type == SDL_QUIT){
                    quitSDL();
                    return 0;
                }
                if(e.type == SDL_MOUSEBUTTONDOWN) {
                    if(e.button.x >= 500 && e.button.x <= 700 && e.button.y >= 100 && e.button.y <= 200)
                        if(go()==0){
                            quitSDL();
                            return 0;
                        }

                    if(e.button.x >= 500 && e.button.x <= 700 && e.button.y >= 250 && e.button.y <= 350) {
                        quitSDL();
                        return 0;
                    }
                }
            }
        }
    }
    quitSDL();
    return 0;
}
