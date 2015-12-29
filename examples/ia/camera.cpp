#include "header.h"

/* Routine d'affichage
 */
void Camera::draw(BITMAP *bmp)
{
     //Efface l'‚cran
     clear_bitmap(bmp);

     //Affiche les tiles
     BEGIN_CAMERA2D_DRAW()
         if(EG_ERROR) continue;
         DrawCameraSprite(bmp,EG_OBSTACLE,EG_X1,EG_Y1);
     END_CAMERA2D_DRAW()

     //Affiche les sprites des personnages
     draw_sprites(bmp);
}
