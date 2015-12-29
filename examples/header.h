#include <stdlib.h>
#include <string.h>

#include <allegro.h>
#include <eg.h>

#include "examples.h"

#define BPS 50

DATAFILE *datafile = 0;
BITMAP *buffer = 0;
volatile int counter1=0,counter2=0,counter3=0,counter4=0,counter5=0;
volatile int counter_sync=0;

void bps_counter(void)
{
        counter_sync++;
}
END_OF_FUNCTION(bps_counter);
void counter_proc(void)
{
        counter1++; counter2++; counter3++; counter4++; counter5++;
}
END_OF_FUNCTION(counter_proc);

// Initialisation d'allegro
void std_allegro_init(int w,int h,int depth)
{
        allegro_init();

        set_color_depth(depth);
        if(set_gfx_mode(GFX_SAFE,w,h,0,0)<0)
        {
                set_gfx_mode(GFX_TEXT,80,25,0,0);
                allegro_message("Erreur dans le chargement de l'ecran graphique %ix%ix%i bit...\n",w,h,depth);
                exit(1);
        }
        install_timer();
        install_keyboard();

        //Initialisation du timer
        LOCK_FUNCTION(counter_proc); LOCK_VARIABLE(counter1);
        LOCK_VARIABLE(counter2); LOCK_VARIABLE(counter3);
        LOCK_VARIABLE(counter4); LOCK_VARIABLE(counter5);

        LOCK_FUNCTION(bps_counter);
        LOCK_VARIABLE(counter_sync);
        install_int(counter_proc,1);    //chaque milliseconde
        //Pour l'affichage
        install_int_ex(bps_counter,BPS_TO_TIMER(BPS));

        //Cration du buffer
        buffer = create_bitmap(SCREEN_W,SCREEN_H);
        if(!buffer)
        {
                set_gfx_mode(GFX_TEXT,80,25,0,0);
                allegro_message("Erreur dans l'initialisation du buffer...\n");
                exit(1);
        }
        clear_bitmap(buffer);
}

//Chargement du DATAFILE
void std_load_datafile()
{
        datafile = load_datafile("examples.dat");
        if(!datafile)
        {
                set_gfx_mode(GFX_TEXT,80,25,0,0);
                allegro_message("Erreur dans le chargement du fichier examples.dat...\n");
                exit(1);
        }
}

void draw_buffer()
{
        blit(buffer,screen,0,0,0,0,buffer->w,buffer->h);
}

//Quitter le prog
void bye()
{
        if(buffer) { destroy_bitmap(buffer); buffer=0; }
        if(datafile) { unload_datafile(datafile); datafile=0; }
}
