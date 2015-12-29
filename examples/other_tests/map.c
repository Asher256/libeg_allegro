#include <allegro.h>
#include "eg.h"

EG_MAP *map;
EG_CAMERA *camera;
EG_FILEF *filef;
BITMAP *buffer;

int proc(void *eg_filef_data, void *data)
{
	HEADER("MAP");
	AUTOVAR(&camera->pgx,4);
	AUTOVAR(&camera->scx,4);
	AUTOVAR(&camera->pgy,4);
	AUTOVAR(&camera->scy,4);
	egmap_filef_proc(eg_filef_data,map);
	return F_OK;
}

int main(void)
{
	int x,y,px,py,i;

	allegro_init();
	install_keyboard();
	install_mouse();

	map = create_egmap(20,10);
	filef = create_egfilef(proc,FF_NORMAL);
	if(!map || !filef) {
		allegro_message("Erreur...\n");
		return 1;
	}

	map->line[0][0] = 'A';
	map->line[9][19] = 'A';
	map->line[9][0] = 'A';
	map->line[0][19] = 'A';


	// ** GFX MODE
	if(set_gfx_mode(GFX_AUTODETECT,320,200,0,0)<0) {
		allegro_message("%s\n",allegro_error);
		exit(1);
	}

	// BUFFER + CARTE
	buffer = create_bitmap(320,200);
	camera = create_egcamera(20,20,&map->width, &map->height);

	// Err
	if(!buffer || !camera) return 1;
	egfilef_load(filef,"file.map",map);

	// BOUCLE
	while(!key[KEY_ESC]) {
		poll_keyboard();
		poll_mouse();
		clear_bitmap(buffer);

		EG_CAMERA_BEGIN(camera,map,x,y,px,py,i)
		rectfill(buffer,EG_X,py,px+camera->spr_w-1,py+camera->spr_h-1,(!i)?15:10);
		EG_CAMERA_END

		if(mouse_b) {
			px = egcamera_x_scr2map(camera,mouse_x);
			py = egcamera_y_scr2map(camera,mouse_y);

			egmap_put(map,px,py,(mouse_b&1)?1:0);
		}
		if(key[KEY_LEFT] || mouse_x<=5)   egcamera_scx_dec(camera,1);
		if(key[KEY_RIGHT] || mouse_x>=SCREEN_W-5)  egcamera_scx_inc(camera,1);
		if(key[KEY_UP] || mouse_y<=5)     egcamera_scy_dec(camera,1);
		if(key[KEY_DOWN] || mouse_y>=SCREEN_H-5)   egcamera_scy_inc(camera,1);

		if(key[KEY_I]) {
			egmap_realloc(map,map->width-1,map->height);
		}
		if(key[KEY_K]) {
			camera->spr_w--;
			camera->spr_h--;
			while(key[KEY_K]) {
				poll_keyboard();
			}
		}
		if(key[KEY_L]) {
			camera->spr_w++;
			camera->spr_h++;
			while(key[KEY_L]) {
				poll_keyboard();
			}
		}

		egcamera_correct(camera);

		circle(buffer,mouse_x,mouse_y,7,0);
		blit(buffer,screen,0,0,0,0,320,200);
	}

	egfilef_save(filef,"file.map",map);

	destroy_bitmap(buffer);
	destroy_egcamera(camera);
	destroy_egmap(map);
	return 0;
}
END_OF_MAIN();
