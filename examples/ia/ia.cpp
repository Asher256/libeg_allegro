/*  Programme d'intelligence artificielle
    Un objet qui suit un autre
 */

#include "iaengine.h"
#include "fonc.h"
#include "ia.h"  //Le fichier dat
#define TILE_SIZE 10
#define ADD 1

#define DEBUG

//******* DEBUG ********
//EgDebug debug;
#define Trace(n)
//debug.Trace(n)
#define OPEN_DEBUG_FILE()
//debug.Open("debug.txt")
//**********************

//****************** VARIABLES *****************************
BITMAP *bmpJoueur,bmpMonstre;

Carte carte(50,50);
Camera camera(&carte,TILE_SIZE,TILE_SIZE);
Pos posJoueur(&camera,TILE_SIZE,TILE_SIZE);
Pos posMonstre(&camera,TILE_SIZE,TILE_SIZE);
Pos monstre2(&camera,TILE_SIZE,TILE_SIZE);

//Cheat cheat(&counter3);
//Fps fps(&counter1);
//Timer timer(&counter2);

char posi[] = {KEY_P,KEY_O,KEY_S,KEY_I,NULL};
int cheat_posi;
//**********************************************************

/* Affiche un obstacle selon sa sprite
 */
void DrawCameraSprite(BITMAP *bmp,int obs,int x,int y)
{
     switch(obs)
     {
           case 1:
               draw_rle_sprite(bmp,(RLE_SPRITE*)datafile[sprite_plein].dat,x,y);
               break;
           case 0:
                draw_rle_sprite(bmp,(RLE_SPRITE *)datafile[sprite_vide].dat,x,y);
                break;
     }
}

/* Format de fichier de la carte
 *
 * Utilis‚ par save(); et load(); de la carte
 */
int Carte::fileFormat()
{
        HEADER("IA")

        AUTOVAR(&posJoueur.scx,sizeof(posJoueur.scx))
        AUTOVAR(&posJoueur.scy,sizeof(posJoueur.scy))
        AUTOVAR(&posJoueur.pgx,sizeof(posJoueur.pgx))
        AUTOVAR(&posJoueur.pgy,sizeof(posJoueur.pgy))

        AUTOVAR(&posMonstre.scx,sizeof(posMonstre.scx))
        AUTOVAR(&posMonstre.scy,sizeof(posMonstre.scy))
        AUTOVAR(&posMonstre.pgx,sizeof(posMonstre.pgx))
        AUTOVAR(&posMonstre.pgy,sizeof(posMonstre.pgy))

        return Carte2D::fileFormat();
}

/* Affiche les sprites
 *
 * Utilisation: draw_sprites(buffer_bitmap);
 *
 * Retour: non.
 */
void draw_sprites(BITMAP *bitmap)
{
	draw_sprite(bitmap,posJoueur.bitmap,posJoueur.getPositionX(),posJoueur.getPositionY());
	draw_sprite(bitmap,posMonstre.bitmap,posMonstre.getPositionX(),posMonstre.getPositionY());
	draw_sprite(bitmap,monstre2.bitmap,monstre2.getPositionX(),monstre2.getPositionY());
}

/* Prend et execute les information des p‚riph‚riques d'entr‚e.
 *
 * Utilisation: Input();
 *
 * Retour: Non.
 */
void input()
{
        if(key[KEY_LEFT]) posJoueur.decX(ADD);
        if(key[KEY_RIGHT]) posJoueur.addX(ADD);
        if(key[KEY_UP]) posJoueur.decY(ADD);
        if(key[KEY_DOWN]) posJoueur.addY(ADD);

        int x = mouse_x;
        int y = mouse_y;

        if(mouse_b&1) { camera.screenModifyCarte(x,y,1);}
        if(mouse_b&2) { camera.screenModifyCarte(x,y,0);}

        TestCheat();
}

void TestCheat()
{
        //int ret = cheat.testCheat();
        //if(ret==cheat_posi)
        //                   posJoueur.setCount(0,0);
}

/* ROUTINE MAIN (menu principal)
 */
int main()
{
        std_allegro_init(8);
        std_load_datafile();
        set_palette((RGB *)datafile[palette_principale].dat);
        OPEN_DEBUG_FILE();
        Trace("Fin d'initialisation d'allegro...\n");
	Trace("Installation de la souris...\n");

        //cheat_posi = cheat.insertCheat(posi,strlen(posi));

        //Optimise la camera avec le mode graphique actuel
	camera.optimize();

        //Cr‚ation des tiles
	posJoueur.CreateDessin(15);
	posMonstre.CreateDessin(1);
        monstre2.CreateDessin(2);

        Trace("Chargement de carte.ca.\n\n");
        if(carte.load("ia.ca"))
        {
                Trace("   - ERREUR DANS LA CHARGEMENT DE LA CARTE\n");
        }

        // Routines princiale, si echap alors il quitter le programme
        Trace("D‚but de la boucle du programme...\n");
        while(!key[KEY_ESC])
	{
                input();        //Les keys pour faire bouger le joueur

		//******* Il suit le joueur
		int repere = posMonstre.suivre(&posJoueur);
                monstre2.suivre(&posJoueur);
		//******* SI ON PEUT AFFICHER
		if(ok_to_draw())
		{ //Alors on affiche
			posJoueur.fixCamera();
                	//camera.clientPosition();
			camera.draw(buffer);
                        posMonstre.outputInformation(buffer,3,10,15);
			//textprintf(buffer,font,0,0,15,"Fps:%i CameraPlus:%i,%i Reperation:%i",fps.getFps(),camera.camera_plusx,camera.camera_plusy,repere);

                        show_mouse(buffer);
			draw_buffer();
		}

                //Simulation de la syncronisation
                //timer.whileTimerOut(20);
	}

        Trace("\n");
        Trace("Sauvegarde de carte.ca.\n");
        if(carte.save("ia.ca"))
        {
                Trace("   - ERREUR DANS LA SAUVEGARDE DE LA CARTE\n");
        }

        Trace("Fin du programme.\n");
	bye();
        return 0;
}
END_OF_MAIN();

