#ifndef IA_ENGINE_H
#define IA_ENGINE_H

#include <allegro.h>
#include <2dglib.h>

//Version DEBUG
//#define DEBUG

/****** Classe de suivre  ********/
class SPos2D : public Pos2D
{
private:
       int objectif_pgx;
       int objectif_pgy;
       int objectif_fixe;
       Pos2D *destination_actuelle;
       Target2D chemin;
       //EgDebug debug;

       int ajusterX();
       int ajusterY();
       int testerLigneX(int,int,int);
       int testerLigneY(int,int,int);
       int testerDirecte1();
       int testerDirecte2();
       int makeDirecte();
       int bouger();
       void ajouterDirecte1();
       void ajouterDirecte2();
       int directe();
       int positionChangee();

public:
       void outputInformation(BITMAP *,int,int,int);
       virtual int testerObstacle(int);
       SPos2D(Camera2D *cam,int w,int h);
       int suivre(Pos2D *);
};
/*********************************/

#endif
