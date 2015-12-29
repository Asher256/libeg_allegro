/********************************************************************************
 * Engine d'intellignece artificielle "Suivre une cible"
 * par Asher256 
 *
 * This source is distribued widh Gnu General Public license, Copyright (c) Free
 * Foundation Software/
 *
 ********************************************************************************/


#include <allegro.h>
#include <2dglib.h>

#include "iaengine.h"   //Declarations de la classe

/*#ifdef DEBUG
  #define OPEN_DEBUG_FILE() debug.Open("iadebug.txt")
  #define IA_TRACE(n) debug.Trace(n)
#else*/
  #define OPEN_DEBUG_FILE()
  #define IA_TRACE(n);
//#endif

// Affiche les informations actuelles
void SPos2D::outputInformation(BITMAP *bitmap,int x,int y,int col)
{
        textprintf(bitmap,font,x,y,col,"Pgy:%i Pgy:%i Scx:%i Scy:%i",pgx,pgy,scx,scy);
        y+=10;
        textprintf(bitmap,font,x,y,col,"Cible_Pgx:%i cible_Pgy:%i",objectif_pgx,objectif_pgy);
}

/* Bouger le personnage
 *
 * Retour: 1 S'il bouge
 *         0 S'il ne bouge pas
 */
int SPos2D::bouger()
{
        IA_TRACE("- On Bouge...\n");
        //PremiŠre Variables
        int ret = objectif_fixe;
        int addx=0,addy=0;

        //Met destination actuelle
        int destinationX = chemin.getX();
        int destinationY = chemin.getY();

        //Met les incr‚mentations
        if(destinationX>pgx)            // Met la destination
                            addx=1;     // Si pgx -> destination alors addx 1
        else if(destinationX<pgx)
                            addx=-1;
        //L'incr‚mentation Y
        if(destinationY>pgy)
                            addy=1;
        else if(destinationY<pgy)
                            addy=-1;

        //Ajustement si Scroll X ou Y >0
        if(addx)     //S'il va marcher en X
           if(!ajusterY())  //Si pas encore ajust‚ (avec ajustement auto)
                  return ret;
        if(addy)    //S'il va marcher en Y
           if(!ajusterX())  //Si pas encore ajust‚ (avec ajustement auto)
                  return ret;

        //Il marche en X et Y selon les variables addx et addy
        incX(addx);
        incY(addy);

        //S'il aboutit … la fin actuelle il passe au prochain BUT
        if(pgx==destinationX && pgy==destinationY)
                             if(chemin.next())    //Si 1 alors cela veut dire que c'est la fin !
                                             {
                                                 IA_TRACE("- Objectif Atteind :)\n");
                                                 objectif_fixe = ret = 0;
                                                 return ret;
                                             }
        return ret;
}

/* La routine pour suivre
 */
int SPos2D::suivre(Pos2D *destination)
{
        destination_actuelle = destination;
        IA_TRACE("\nBEGIN SUIVRE\n");
        if(!objectif_fixe)      //Si pas encore d'objectif fix‚
                          makeDirecte(); //Cherche s'il y a une nouvelle directe

        //Si l'objectif est fix‚ ou a ‚t‚ fix‚ avec makeDirecte();
        if(objectif_fixe)
        {
                //Teste si la position de la cible est chang‚e...
                //... si 'oui' alors il teste s'il y a une directe
                //... Si pas de directe
                if(positionChangee())
                {
                        IA_TRACE("\n- La cible change de position\n");
                        IA_TRACE("     * On cherche un nouveau Directe...\n");
                        if(!makeDirecte()) //S'il na trouve pas de nouvelle directe...
                        //... Il va quand mˆme … la destination.
                            {  objectif_fixe=1;  /* */   }
                        /*else: S'il a trouv‚ un directe alors il a automatiquement ‚t‚
                                mis avec makeDirecte() */

                }

                //Sinon il bouge !!
                return bouger();
        }
        IA_TRACE("\nFIN SUIVRE\n");
        return 0;
}

/* Teste s'il y a une Directe 1
 *
 * Retour: Retourne 1 S'il y a un Directe 1
 */
int SPos2D::testerDirecte1()
{
        IA_TRACE("* On teste s'il y a une Directe 1\n");
        int pgx1=pgx, pgx2=destination_actuelle->pgx;
        int pgy1=pgy, pgy2=destination_actuelle->pgy;

        //Swapper les variables
        /*
        if(pgx1>pgx2)
                     {
                        int temp = pgx1;
                        pgx1 = pgx2;
                        pgx2 = temp;
                     }
        if(pgy1>pgy2)
                     {
                        int temp = pgy1;
                        pgy1 = pgy2;
                        pgy2 = temp;
                     }
        */
        //Tester le camp X
        int err = testerLigneX(pgx1,pgx2,pgy1);

        //Pas de directe
        if(err) {
          IA_TRACE("    - NON... Directe 1 non trouv‚e :(\n");
          return 0; }

        //Le Y
        err = testerLigneY(pgy1,pgy2,pgx2);

        if(err==0)      //Si le test y et x son bons
             {
                IA_TRACE("    - Directe 1 trouv‚e :)\n");
                return 1;
             }

        IA_TRACE("    - NON... Directe 1 non trouv‚e :(\n");
        return 0;
}

/* Teste s'il y a une Directe 1
 *
 * Retour: Retourne 1 S'il y a un Directe 1
 */
int SPos2D::testerDirecte2()
{
        IA_TRACE("* On teste s'il y a une Directe 2\n");
        int pgx1=pgx, pgx2=destination_actuelle->pgx;
        int pgy1=pgy, pgy2=destination_actuelle->pgy;

        //Swapper les variables
        /*if(pgx1>pgx2)
                     {
                        int temp = pgx1;
                        pgx1 = pgx2;
                        pgx2 = temp;
                     }
        if(pgy1>pgy2)
                     {
                        int temp = pgy1;
                        pgy1 = pgy2;
                        pgy2 = temp;
                     }
          */
        //Le Y
        int err = testerLigneY(pgy1,pgy2,pgx1);

        //S'il y a erreur alors bye
        if(err)
        {
           IA_TRACE("    - NON... Directe 2 non trouv‚e :)\n");
           return 0;
        }

        //Tester le camp X
        err = testerLigneX(pgx1,pgx2,pgy2);

        if(err==0)      //Si le test y et x son bons
                  {
                        IA_TRACE("    - Directe 2 trouv‚e :)\n");
                        return 1;
                  }
        IA_TRACE("    - NON... Directe 2 non trouv‚e :)\n");
        return 0;
}

/* Teste s'il y a un obstacle dans un ligne
 *
 * Retour: 1 s'il y a un obstacle
 */
int SPos2D::testerLigneX(int x1,int x2,int y)
{
        if(x1>x2)
                 {
                        int temp = x1;
                        x1 = x2;
                        x2 = temp;
                 }

        for(int i=x1;i<=x2;i++)
        {
                int obstacle = camera->carte->getObstacle(i,y);
                //if(camera->carte->err()) continue;
                if(testerObstacle(obstacle))
                                            return 1;
        }
        return 0;       //La ligne est O K ! ! !
}

/* Teste s'il y a un obstacle dans une cologne
 *
 * Retour: 1 s'il y a un obstacle
 */
int SPos2D::testerLigneY(int y1,int y2,int x)
{
        if(y1>y2)
                 {
                        int temp = y1;
                        y1 = y2;
                        y2 = temp;
                 }

        //Sil a trouv‚ un obstacle alors ...
        for(int i=y1;i<=y2;i++)
        {
                int obstacle = camera->carte->getObstacle(x,i);
                //if(camera->carte->err()) continue;
                if(testerObstacle(obstacle))
                                            return 1;
        }
        return 0;       //La ligne est O K ! ! !
}

/* Teste si un obstacle est interdit d'accŠs
 *
 * Retour: 1 Si le num‚ro d'obstacle est invalide
 */
int SPos2D::testerObstacle(int obstacle)
{
        if(obstacle) return 1;
        return 0;
}

/* Teste s'il y a une Directe
 *
 * Retour: 1 Si Directe Nø1
 *         2 Si Directe Nø2
 *         0 S'il n'y a pas de Directe
 */
int SPos2D::directe()
{
        if(testerDirecte1())              //S'il a trouv‚ la Directe Nø1
                          return 1;
        else if(testerDirecte2())         //S'il a trouv‚ la Directe Nø2
                          return 2;

        return 0;
}

/* Ajoute un directe si possible
 *
 * Retour: 1 S'il a ajout‚ un directe I
 *         2 S'il a ajout‚ un Directe II
 *         0 S'il n'a pas trouv‚ de directe
 */
int SPos2D::makeDirecte()
{
        //S'il est arriv‚ alors pas la peine de songer …
        if(pgx==destination_actuelle->pgx && pgy==destination_actuelle->pgy)
         {
                //Ajustement de x et y
                ajusterX();
                ajusterY();
                return 0;
         }

        switch(directe())       //Trouve un directe
        {
                case 1:         //Directe Nø 1
                     ajouterDirecte1();
                     return 1;
                case 2:
                     ajouterDirecte2();
                     return 2;
        }
        objectif_fixe = 0;      //Plus d'objectif, stopper le boulot.
        return 0;     //Retourner 0 en guise de contacte avec la sub qui apelle.
}

/* Ajoute le chemin pour aboutir au but avec le Directe1
 *
 * Retour: Rien.
 */
void SPos2D::ajouterDirecte1()
{
        IA_TRACE("* Ajoute Directe 1\n");
        //Efface tout l'ancien chemin
        chemin.clear();

        // Met les valeurs de la cible dans les flags membres
        objectif_pgx = destination_actuelle->pgx;
        objectif_pgy = destination_actuelle->pgy;
        objectif_fixe = 1;     //Au boulot !!

        //Ajouter le chemin
        chemin.add(objectif_pgx,pgy);
        chemin.add(objectif_pgx,objectif_pgy);
}

/* Ajoute le chemin pour aboutir au but avec le Directe2
 *
 * Retour: Rien.
 */
void SPos2D::ajouterDirecte2()
{
        IA_TRACE("* Ajoute Directe 2\n");
        //Efface tout l'ancien chemin
        chemin.clear();

        // Met les valeurs de la cible dans les flags membres
        objectif_pgx = destination_actuelle->pgx;
        objectif_pgy = destination_actuelle->pgy;
        objectif_fixe = 1;     //Au boulot !!

       //Ajouter le chemin
        chemin.add(pgx,objectif_pgy);
        chemin.add(objectif_pgx,objectif_pgy);
}


/* Teste si la position de la cible est chang‚e selon la variable membre
 * destination_actuelle et les variables objectif_pgx et objectif_pgy
 *
 * Retour: 1 Si la position est chang‚e
 *         0 Si la position est la mˆme (pas de changement)
 */
int SPos2D::positionChangee()
{
        if(destination_actuelle->pgx!=objectif_pgx ||
           destination_actuelle->pgy!=objectif_pgy)
                  return 1;   //Ah ma vieille cible... t'a chang‚ !!
        return 0; //La cible n'a pas chang‚e de position
}

/* Ajustement de scx
 *
 * Retour: 1 Si ajust‚
 *         0 Si pas encore ajust‚
 */
int SPos2D::ajusterX()
{
        if(scx==0) return 1;
        decX();
        if(scx==0) return 1;
        return 0;
}

/* Ajustement de scy
 *
 * Retour: 1 Si ajust‚
 *         0 Si pas encore ajust‚
 */
int SPos2D::ajusterY()
{
        if(scy==0) return 1;
        decY();
        if(scy==0) return 1;
        return 0;
}

/* Le constructeur
 */
SPos2D::SPos2D(Camera2D *cam,int w,int h):Pos2D(cam,w,h)
{
        OPEN_DEBUG_FILE();
        objectif_pgx=objectif_pgy=objectif_fixe=0;
}

