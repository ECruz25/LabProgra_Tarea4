#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
//#include<SDL2/SDL_ttf.h>

#include<iostream>
#include<vector>
#include<list>
#include<fstream>

#include "Sho.h"
#include "EnemigoAzul.h"
#include "EnemigoVerde.h"
#include "EnemigoRojo.h"

using namespace std;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event Event;
SDL_Texture *background;
SDL_Rect rect_background;

string iniciales;
string iniciales_;

void Save(int frame, string iniciales)
{
    ofstream o("Score.txt");
//    o.write((char*)&frame,4);
    o<<frame;
//    o<<iniciales<<endl;
    o.close();
    ofstream u("Nombres.txt");
    u<<iniciales_;
    u.close();
}

int Print()
{
    int Score;
    ifstream i("Score.txt");
    i>>Score;
//    i.seekg(0);
//    i.read((char*)&Score, 4);
    i.close();
//    cout<<Score<<endl;
    return Score;
}

string PrintName()
{
    string inicial;
    ifstream n("Nombres.txt");
    n>>inicial;
    n.close();
    return inicial;
}
/*
void DisplayText(int frame)
{

    string score = ""+frame;

    TTF_Font* Sans = TTF_OpenFont("Monaco.ttf", 24);

    SDL_Color White = {255, 255, 255};

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, score, White);

    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_Rect Message_rect;
    Message_rect.x = 0;
    Message_rect.y = 0;
    Message_rect.w = 100;
    Message_rect.h = 100;

    SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
}
*/
void loopJuego()
{
    //Init textures
    int frame=0;
    int w=0,h=0;
    background = IMG_LoadTexture(renderer,"fondo.png");
    SDL_QueryTexture(background, NULL, NULL, &w, &h);
    rect_background.x = 0;
    rect_background.y = 0;
    rect_background.w = w;
    rect_background.h = h;


    list<Personaje*> personajes;
    personajes.push_back(new Sho(renderer,&personajes));
    personajes.push_back(new EnemigoVerde(renderer,&personajes));
    personajes.push_back(new EnemigoRojo(renderer,&personajes));

    //Main Loop
    bool done = false;
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    while(!done)
    {
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                exit(0);
            }
            if(Event.type == SDL_KEYDOWN)
            {
                if(Event.key.keysym.sym == SDLK_ESCAPE)
                {
                    return;
                }
            }
        }

        if(frame%1000==0)
        {
            personajes.push_back(new EnemigoAzul(renderer,&personajes));
        }

        for(list<Personaje*>::iterator p=personajes.begin();
                p!=personajes.end();
                p++)
            (*p)->act();

        SDL_SetRenderDrawColor(renderer, 255, 100, 0, 255);

        // Clear the entire screen to our selected color.
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, background, NULL, &rect_background);

        for(list<Personaje*>::iterator p=personajes.begin();
                p!=personajes.end();
                p++)
            (*p)->draw(renderer);

        for(list<Personaje*>::iterator p=personajes.begin();
                p!=personajes.end();
                p++)
            if((*p)->muerto)
            {
                if((*p)->type == "Sho")
                {
                    done = true;
                }
                personajes.erase(p);
                break;
            }

        SDL_RenderPresent(renderer);

        frame++;

        if(done)
        {
            if(Print()<frame)
            {
                cout<<"Ingrese sus iniciales: [_,_,_]"<<endl;
                cin>>iniciales;
                if(iniciales!=PrintName())
                {
                    for(int c=0;c<3;c++)
                    {
                        iniciales_+=iniciales[c];
                    }
                }
                Save(frame, iniciales_);
            }
            else
            {
                cout<<"Vuelvalo a intentar"<<endl;
                cout<<"Puntuacion mas alta: "<<endl;
                cout<<PrintName()<<": "<<Print()<<endl;
            }
        }
    }
}

void instrucciones()
{

    int w=0,h=0;
    SDL_Texture *menu_how = IMG_LoadTexture(renderer,"instrucciones.png");
    SDL_QueryTexture(menu_how, NULL, NULL, &w, &h);
    rect_background.x = 0;
    rect_background.y = 0;
    rect_background.w = w;
    rect_background.h = h;
    while(true)
    {
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                exit(0);
            }
            if(Event.type == SDL_KEYDOWN)
            {
                if(Event.key.keysym.sym == SDLK_ESCAPE)
                {
                    return;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, menu_how, NULL, &rect_background);

        SDL_RenderPresent(renderer);

    }
}

class MenuButton
{
public:
    SDL_Texture *selected;
    SDL_Texture *unselected;
    SDL_Rect rect;
    bool is_selected;
    MenuButton(string selected, string unselected, int x, int y)
    {
        this->selected = IMG_LoadTexture(renderer,selected.c_str());
        this->unselected = IMG_LoadTexture(renderer,unselected.c_str());
        SDL_QueryTexture(this->selected, NULL, NULL, &rect.w, &rect.h);
        rect.x = x;
        rect.y = y;
        is_selected = false;
    }

    void render()
    {
        if(is_selected)
            SDL_RenderCopy(renderer,selected,NULL,&rect);
        else
            SDL_RenderCopy(renderer,unselected,NULL,&rect);
    }

    void select()
    {
        is_selected = true;
    }

    void unselect()
    {
        is_selected = false;
    }
};

void mainMenu()
{

    int opcion = 1;
    SDL_Texture *menu_fondo = IMG_LoadTexture(renderer,"menu_fondo.png");
    SDL_Rect menu_rect;
    vector<MenuButton*>buttons;
    buttons.push_back(new MenuButton("button1_selected.png","button1_unselected.png",300,300));
    buttons.push_back(new MenuButton("button2_selected.png","button2_unselected.png",300,400));
    buttons.push_back(new MenuButton("button3_selected.png","button3_unselected.png",300,500));
    SDL_QueryTexture(menu_fondo, NULL, NULL, &menu_rect.w, &menu_rect.h);
    menu_rect.x = 0;
    menu_rect.y = 0;

    while(true)
    {
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                return;
            }
            if(Event.type == SDL_KEYDOWN)
            {
                if(Event.key.keysym.sym == SDLK_2)
                {
                    exit(0);
                }
                if(Event.key.keysym.sym == SDLK_1)
                {
                    loopJuego();
                }
                if(Event.key.keysym.sym == SDLK_ESCAPE)
                {
                    return;
                }
                if(Event.key.keysym.sym == SDLK_DOWN)
                {
                    opcion++;
                    if(opcion > 4)
                        opcion = 3;
                    if(opcion == 4)
                        opcion = 1;
                }
                if(Event.key.keysym.sym == SDLK_UP)
                {
                    opcion--;
                    if(opcion < 0)
                        opcion = 1;
                    if(opcion == 0)
                        opcion = 3;

                }
                if(Event.key.keysym.sym == SDLK_RETURN)
                {
                    switch(opcion)
                    {
                        case 1:
                            loopJuego();
                        break;
                        case 2:
                            instrucciones();
                        break;
                        case 3:
                            exit(0);
                        break;
                    }
                }
            }
        }

        SDL_RenderCopy(renderer,menu_fondo,NULL,&menu_rect);


        for(int i=0;i<buttons.size();i++)
        {
            buttons[i]->unselect();
        }
        buttons[opcion-1]->select();
        for(int i=0;i<buttons.size();i++)
        {
            buttons[i]->render();
        }

        SDL_RenderPresent(renderer);
    }
}

int main( int argc, char* args[] )
{
    //Init SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return 10;
    }
    //Creates a SDL Window
    if((window = SDL_CreateWindow("Image Loading", 100, 100, 1024/*WIDTH*/, 768/*HEIGHT*/, SDL_WINDOW_RESIZABLE | SDL_RENDERER_PRESENTVSYNC)) == NULL)
    {
        return 20;
    }
    //SDL Renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );
    if (renderer == NULL)
    {
        std::cout << SDL_GetError() << std::endl;
        return 30;
    }

    mainMenu();

	return 0;
}
