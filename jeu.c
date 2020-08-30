#include <time.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <FMOD/fmod.h>
#include "jeu.h"


/* Fonction du jeu */
void jouer(SDL_Surface *ecran)
{
char a_score[20];
srand(time(NULL));
/* Initialisation FMOD */
FMOD_SYSTEM *system;
FMOD_SOUND *tir=NULL;
FMOD_System_Create(&system);
FMOD_System_Init(system, 2, FMOD_INIT_NORMAL, NULL);
FMOD_System_CreateSound(system, "pan.wav", FMOD_CREATESAMPLE, 0, &tir);
/* Initialisation SDL */
SDL_Surface *viseur=NULL, *fond=NULL, *cible=NULL, *score=NULL;
SDL_Rect pos_viseur, pos_fond, pos_cible, pos_score;
SDL_Event event;
int continuer=1, t_p=0, t_a=0, compteur=0, montrer=0, x=0, y=0, shoot=0; /* (x,y)==(abssise,ordonnée) */
/* Initialisation TTF */
TTF_Font *police=NULL;
SDL_Color couleur={255, 255, 255};
TTF_Init();
police=TTF_OpenFont("angelina.ttf", 65);
sprintf(a_score, "Score : %d", shoot);
/* Sprites */
viseur=IMG_Load("viseur.png");
fond=IMG_Load("fond.jpg");
pos_fond.x=0;
pos_fond.y=0;
cible=IMG_Load("zozor.bmp");
SDL_SetColorKey(cible, SDL_SRCCOLORKEY, SDL_MapRGB(cible->format, 0, 0, 255));
/* On rend la cible transparente un peu */
// SDL_SetAlpha(cible, SDL_SRCALPHA, 220);
/* On met le curseur au milieu et on le rend invisible */
SDL_WarpMouse(ecran->w/2, ecran->h/2);
SDL_ShowCursor(SDL_DISABLE);
while (continuer)
{
SDL_PollEvent(&event);
   switch (event.type)
   {
   case SDL_QUIT: continuer=0; break;
   case SDL_MOUSEBUTTONDOWN: /* SDL_MOUSEBUTTONUP: */
   // if (event.button.button==SDL_BUTTON_LEFT)
        switch (event.button.button)
        { 
        case SDL_BUTTON_LEFT: 
        // SDL_WaitEvent(&event);
        FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, tir, 0, NULL); 
           /* Collisions */
           if (event.button.x>=pos_cible.x && event.button.x<=pos_cible.x+cible->w && event.button.y>=pos_cible.y && event.button.y<=pos_cible.y+cible->h)
           {
           shoot++;
           sprintf(a_score, "Score : %d", shoot);
           SDL_WaitEvent(&event); /* On attend le prochain évenement */
           }
        break; 
        } break;
   case SDL_MOUSEMOTION:
   pos_viseur.x=event.motion.x-(viseur->w/2); 
   pos_viseur.y=event.motion.y-(viseur->h/2);
   break;            
   }
t_a=SDL_GetTicks();
if (t_a - t_p >= 100 ) /* 30ms pour minimiser l'utilisation du CPU */
{
compteur++;
t_p=t_a;   
   if (compteur==10) /* 1 seconde (100ms*10) */
   {
   compteur=0; /* Remise a zéro */
      if (montrer==0)
      {
      montrer=1;
      x=generer(6, 1);  /* 6 (800/6=133pixel) on peut mettre plus ce que vous voulez */
      y=generer(4, 1); /* 4 (600/4=150pixel) pour ne pas toucher le texte score a l'ecran */
      pos_cible.x=(ecran->w/x)-(cible->w); 
      pos_cible.y=(ecran->h/y)-(cible->h); /* -cible->w ou h pour ne pas sortir des marges de l'ecran */
      }
      else 
      {
      pos_cible.x=ecran->w; /* La cible n'est plus visible on change sa position alors pour eviter l'augmentation du score si on clic sur le vide ou était la cible */
      pos_cible.y=ecran->h;/* On peut mettre -1 ou ecran->w +1 mais ça ne doit pas etre a l'intérieur de l'ecran */
      montrer=0;
      }
   }
}
else SDL_Delay(100 - (t_a - t_p));
// SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
score=TTF_RenderText_Blended(police, a_score, couleur);
pos_score.x=(ecran->w)-(score->w); /* La taille du texte dans score peut changer donc on le met a l'intérieur de la boucle */
pos_score.y=0;
SDL_BlitSurface(fond, NULL, ecran, &pos_fond);
SDL_BlitSurface(score, NULL, ecran, &pos_score);
if (montrer==1) SDL_BlitSurface(cible, NULL, ecran, &pos_cible);
SDL_BlitSurface(viseur, NULL, ecran, &pos_viseur);  
SDL_Flip(ecran);
}
/* Vidage TTF */
TTF_CloseFont(police);
TTF_Quit();
/* Vidage SDL */
SDL_FreeSurface(viseur);
SDL_FreeSurface(score);
SDL_FreeSurface(fond);
SDL_FreeSurface(cible);
/* Vidage FMOD */
FMOD_Sound_Release(tir);
FMOD_System_Close(system);
FMOD_System_Release(system);
}


/* Fonction qui génère un nombre aléatoire */
int generer(int max, int min)
{
int n_aleatoire=(rand()%((max+1)-min))+min;
return n_aleatoire;
}
