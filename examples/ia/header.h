#ifndef IA_H
#define IA_H

#include <stdlib.h>
#include <string.h>

#include "iaengine.h"

//L'affichage en BPS
#define BPS 50

void TestCheat();
void DrawCameraSprite(BITMAP *,int,int,int);
void draw_sprites(BITMAP *);
void input();
void std_allegro_init(int);
void std_load_datafile();
int ok_to_draw();
void draw_buffer();
void bye();

class Carte : public Carte2D
{
        public:
               Carte(int w,int h) : Carte2D(w,h) {}

               //Le format de fichier de la carte
               int fileFormat();
};

/****** CAMERA D'AFICHAGE *********/
class Camera : public Camera2D
{
	public:
		Camera(Carte2D *car,int tw,int th): Camera2D(car,tw,th) {}

		void draw(BITMAP *bmp);
};

/****** Position du joueur ********/
class Pos : public SPos2D
{
	public: 
	BITMAP *bitmap;
        OList list;

        //Constructeur
        Pos(Camera *cam,int w,int h): SPos2D(cam,w,h) {}

        //Pour faire bouger le personnage dans le map
        int decY(int plus=1);
        int addY(int plus=1);
        int decX(int plus=1);
        int addX(int plus=1);

        //Cr‚ation d'un tile vide (avec un simple clear_bitmap).
        void CreateDessin(int couleur);
};
/**********************************/


#endif
