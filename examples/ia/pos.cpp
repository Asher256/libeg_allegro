#include "header.h"

void Pos::CreateDessin(int couleur)
{
        bitmap = create_bitmap(width,height);
        clear_to_color(bitmap,couleur);
}


int Pos::addX(int plus)
{
     int ret = 0;
     for(int i=0;i<plus;i++)
     {
         list.initialise();
         addObstacleListX(&list,width);

         for(int i=0;i<list.getMax();i++)
         {  if(list.getVal(i))
               return 1; }

         // Sinon il la fait bouger
         ret = SPos2D::addX();
     }
     return ret;
}

int Pos::decX(int plus)
{
     int ret = 0;
     for(int i=0;i<plus;i++)
     {
        list.initialise();
        addObstacleListX(&list,-plus);

        for(int i=0;i<list.getMax();i++)
        {  if(list.getVal(i))
               return 1; }

        ret = SPos2D::decX();
     }
     return ret;
}

int Pos::addY(int plus)
{
     int ret = 0;
     for(int i=0;i<plus;i++)
     {
        list.initialise();
        addObstacleListY(&list,height);

        for(int i=0;i<list.getMax();i++)
        {  if(list.getVal(i))
               return 1; }

        ret = SPos2D::addY(plus);
     }
     return ret;
}

int Pos::decY(int plus)
{
     int ret = 0;
     for(int i=0;i<plus;i++)
     {
        list.initialise();
        addObstacleListY(&list,-plus);

        for(int i=0;i<list.getMax();i++)
        {  if(list.getVal(i))
               return 1; }

        ret = SPos2D::decY(plus);
     }
     return ret;
}
