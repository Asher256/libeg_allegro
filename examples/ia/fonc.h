#include "header.h"

volatile int counter1=0,counter2=0,counter3=0,counter4=0,counter5=0;
volatile int counter_sync=0;
//Timer TimerSync(&counter_sync);
DATAFILE *datafile = 0;
BITMAP *buffer = 0;

void bps_counter()
{
        counter_sync++;
}
END_OF_FUNCTION(bps_counter);
void counter_proc()
{
        counter1++; counter2++; counter3++; counter4++; counter5++;

}
END_OF_FUNCTION(counter_proc);

// Initialisation d'allegro
void std_allegro_init(int depth)
{
        allegro_init();
        install_mouse();
        install_timer();
        install_keyboard();

        int card=GFX_AUTODETECT,w=320,h=200;

        set_color_depth(depth);
        if(set_gfx_mode(GFX_SAFE,320,200,0,0)<0)
        {
                set_gfx_mode(GFX_TEXT,80,25,0,0);
                allegro_message("Erreur dans le mode 320x200x%i...",depth);
                exit(1);
        }
        if(!gfx_mode_select(&card,&w,&h))
        {
                bye();
                exit(0);
        }

        if(set_gfx_mode(card,w,h,0,0)<0)
        {
                allegro_message("Erreur dans le chargement de l'ecran graphique %ix%ix%i bit...\n",w,h,depth);
                exit(1);
        }

        //Initialisation du timer
        LOCK_FUNCTION(counter_proc); LOCK_VARIABLE(counter1);
        LOCK_VARIABLE(counter2); LOCK_VARIABLE(counter3);
        LOCK_VARIABLE(counter4); LOCK_VARIABLE(counter5);

        LOCK_FUNCTION(bps_counter);
        LOCK_VARIABLE(counter_sync);
        install_int(counter_proc,1);    //chaque milliseconde
        //Pour l'affichage
        install_int_ex(bps_counter,BPS_TO_TIMER(BPS));

        //Cr‚ation du buffer
        buffer = create_bitmap(SCREEN_W,SCREEN_H);
        if(!buffer)
        {
                allegro_message("Erreur dans l'initialisation du buffer...\n");
                exit(1);
        }
        clear_bitmap(buffer);

}

//Chargement du DATAFILE
void std_load_datafile()
{
        datafile = load_datafile("ia.dat");
        if(!datafile)
        {
                allegro_message("Erreur dans le chargement du fichier ia.dat...\n");
                exit(1);
        }
}

int ok_to_draw()
{
        return /*TimerSync.timerOut(1)*/-1;
}

void draw_buffer()
{
        blit(buffer,screen,0,0,0,0,buffer->w,buffer->h);
}

//Quitter le prog
void bye()
{
        if(datafile) unload_datafile(datafile);
}
