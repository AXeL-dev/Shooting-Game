/* Crée le : 04/05/2013 */
/* Modifié le : 05/05/2013 */
/* Commentaire : Il y'a d'autre algo plus puissant biensur, mais c'est ma manière de faire... */
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <FMOD/fmod.h>
#include "jeu.h"


/* Fonction principale */
int main(int argc, char *argv[])
{
/* SDL */
SDL_Surface *ecran=NULL, *start=NULL;
SDL_Rect pos_start;
SDL_Event event;
int debut=1;
SDL_Init(SDL_INIT_VIDEO);
SDL_WM_SetIcon(IMG_Load("viseur.png"), NULL);
ecran=SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
SDL_WM_SetCaption("Jeu de tir", NULL);
start=IMG_Load("start.jpg");
pos_start.x=0;
pos_start.y=0;
while (debut)
{
SDL_WaitEvent(&event);
   switch (event.type)
   {
   case SDL_QUIT: debut=0; break;
   case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
        {
        case SDLK_KP1: jouer(ecran); break;
        case SDLK_KP2: debut=0; break;
        } break;       
   }
SDL_BlitSurface(start, NULL, ecran, &pos_start);
SDL_Flip(ecran);
}
SDL_FreeSurface(start);
SDL_Quit();
return EXIT_SUCCESS;
}
