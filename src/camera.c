/******************************************************************
 *                   ______   ________
 *                  /\   __\ /\   __  \
 *                  \ \  \___\ \  \ \__\
 *                   \ \   __\\ \  \  ___
 *                    \ \  \___\ \  \_\  \
 *                     \ \_____\\ \_______\
 *                      \/____/  \________/
 *
 *                    *  Enhanced Game Library *
 *
 * "Librairie pour la création de jeux vidéos avec Allegro
 *  contenant des routines faites spécialement pour gagner
 *  du temps et vous faciliter la tâche afin d'orienter votre
 *  concentration vers la création du jeu vidéo".
 *
 * Copyright (c) Asher256
 *
 * Enhanced Game Library est une librairie "libre" distribuée
 * sous la licence GNU. Sa source est librement distribuable.
 * Mais elle n'est distribuée sans aucune garantie ni de
 * fonctionnement, ni de dommages ou problèmes.
 *
 * Aucun code n'est parfait, donc si vous souhaitez contribuer
 * à la librairie, vous êtes les bienvenus.
 *
 * Si vous trouvez des Bugs, prévenez moi:
 *
 *                       asher256@gmail.com
 *
 ******************************************************************/

#include "internal.h"

void egcamera_draw_map(EG_CAMERA *camera, EG_MAP *map, BITMAP *bmp, void (*draw)(BITMAP *bmp, EG_OBS *obs))
{
	static EG_OBS obs;
	static EG_SHORT save_pgx;
	static EG_INT scr_x;

	// Met les limites
	EG_SHORT mx = camera->pgx + camera->width - 1;
	EG_SHORT my = camera->pgy + camera->height - 1;

	// init PG
	obs.pgx = camera->pgx;
	obs.pgy = camera->pgy;

	// calcul scr_x/y
	scr_x = obs.scr_x = egcamera_x_map2scr(camera, camera->pgx);
	obs.scr_y = egcamera_y_map2scr(camera, camera->pgy);

	// Mets les limites

	if (mx >= map->width)
	{
		mx = map->width - 1;
	}

	if (my >= map->height)
	{
		my = map->height - 1;
	}

	if (obs.pgx < 0)
	{
		obs.pgx = 0;
		scr_x = obs.scr_x -= camera->pgx * (*camera->spr_w);
	}

	if (obs.pgy < 0)
	{
		obs.pgy = 0;
		obs.scr_y -= camera->pgy * (*camera->spr_h);
	}

	// Si il n'affiche rien...
	if (mx <= obs.pgx || my <= obs.pgy) return;

	// save
	save_pgx = obs.pgx;

	for (; obs.pgy <= my; obs.pgy++)
	{
		for (obs.pgx = save_pgx; obs.pgx <= mx; obs.pgx++)
		{
			obs.value = map->line[obs.pgy][obs.pgx];
			(*draw)(bmp, &obs);
			obs.scr_x += (*camera->spr_w);
		}

		obs.scr_x = scr_x;
		obs.scr_y += (*camera->spr_h);
	}
}

//***************************************************************
// Fix camera
//***************************************************************
void egcamera_center(EG_CAMERA *camera, EG_SHORT pgx, EG_SHORT pgy, EG_SHORT scx, EG_SHORT scy)
{
	camera->pgx = pgx - (camera->width >> 1);
	camera->pgy = pgy - (camera->height >> 1);
	camera->scx = scx;
	camera->scy = scy;
}

void egcamera_center_count(EG_CAMERA *camera, EG_SHORT xcount, EG_SHORT ycount)
{
	egcamera_set_count(
	    camera,
	    xcount - ((camera->width >> 1) * (*camera->spr_w)),
	    ycount - ((camera->height >> 1) * (*camera->spr_h))
	);
}

//***************************************************************
// EG Camera PROC
//***************************************************************
EG_SHORT egcamera_filef_proc(void *eg_filef_data, void *data)
{
	EG_CAMERA *camera = data;
	AUTOVAR(&camera->pgx, sizeof(camera->pgx));
	AUTOVAR(&camera->pgy, sizeof(camera->pgy));
	AUTOVAR(&camera->scx, sizeof(camera->scx));
	AUTOVAR(&camera->scy, sizeof(camera->scy));
	return FF_OK;
}

//***************************************************************
// SIZE get/put
//***************************************************************
EG_SHORT egcamera_get_sprwidth(EG_CAMERA *camera)
{
	return *camera->spr_w;
}
EG_SHORT egcamera_get_sprheight(EG_CAMERA *camera)
{
	return *camera->spr_h;
}

EG_SHORT egcamera_get_width(EG_CAMERA *camera)
{
	return camera->width;
}

EG_SHORT egcamera_get_height(EG_CAMERA *camera)
{
	return camera->height;
}

void egcamera_set_size(EG_CAMERA *camera, EG_SHORT w, EG_SHORT h)
{
	egcamera_set_width(camera, w);
	egcamera_set_height(camera, h);
}
void egcamera_set_width(EG_CAMERA *camera, EG_SHORT width)
{
	if (width <= 0) width = 1;

	camera->width = width;
}
void egcamera_set_height(EG_CAMERA *camera, EG_SHORT height)
{
	if (height <= 0) height = 1;

	camera->height = height;
}

//***************************************************************
// Conversion screen -> MAP et vice versa.
//***************************************************************
EG_SHORT egcamera_x_scr2map(EG_CAMERA *camera, EG_INT xscr)
{
	EG_INT p = ((xscr + camera->scx - 1) / (*camera->spr_w)) + camera->pgx;

	if (p >= *camera->map_w || p < 0) return -1;

	return (EG_SHORT) p;
}

EG_SHORT egcamera_y_scr2map(EG_CAMERA *camera, EG_INT yscr)
{
	EG_INT p = ((yscr + camera->scy - 1) / (*camera->spr_h)) + camera->pgy;

	if (p >= *camera->map_h || p < 0) return -1;

	return (EG_SHORT) p;
}
EG_INT egcamera_x_map2scr(EG_CAMERA *camera, EG_SHORT xmap)
{
	return ((*camera->spr_w) * (xmap - camera->pgx)) - camera->scx;
}
EG_INT egcamera_y_map2scr(EG_CAMERA *camera, EG_SHORT ymap)
{
	return ((*camera->spr_h) * (ymap - camera->pgy)) - camera->scy;
}

//***************************************************************
// Optimize la camera pour un écran graphique
//
// camera.width,height
//***************************************************************
void egcamera_optimize_ex(EG_CAMERA *camera, EG_SHORT w, EG_SHORT h)
{
	camera->width  = (EG_SHORT)(w / (*camera->spr_w)) + 1 + ((w % (*camera->spr_w) != 0) ? 1 : 0);
	camera->height = (EG_SHORT)(h / (*camera->spr_h)) + 1 + ((h % (*camera->spr_h) != 0) ? 1 : 0);

	if (camera->width <= 0) camera->width = 1;

	if (camera->height <= 0) camera->height = 1;
}

void egcamera_optimize(EG_CAMERA *camera)
{
	egcamera_optimize_ex(camera, SCREEN_W, SCREEN_H);
}

//***************************************************************
// Conversion sc+pg en COUNT
//***************************************************************
void egcamera_set_count(EG_CAMERA *camera, EG_SHORT cx, EG_SHORT cy)
{
	egcamera_set_xcount(camera, cx);
	egcamera_set_ycount(camera, cy);
}
void egcamera_set_xcount(EG_CAMERA *camera, EG_SHORT cx)
{
	camera->pgx = cx / (*camera->spr_w);
	camera->scx = cx % (*camera->spr_w);
}
void egcamera_set_ycount(EG_CAMERA *camera, EG_SHORT cy)
{
	camera->pgy = cy / (*camera->spr_w);
	camera->scy = cy % (*camera->spr_w);
}
EG_SHORT egcamera_get_xcount(EG_CAMERA *camera)
{
	return (camera->pgx * (*camera->spr_w)) + camera->scx;
}
EG_SHORT egcamera_get_ycount(EG_CAMERA *camera)
{
	return (camera->pgy * (*camera->spr_h)) + camera->scy;
}

//***************************************************************
// Optimiser la camera pour ne pas dépasser les limites
// du map.
//
// (prendre en compte les limites du MAP
//
// ret TRUE si il a correcté
//***************************************************************
EG_SHORT egcamera_correct(EG_CAMERA *camera)
{
	return egcamera_correct_x(camera) | egcamera_correct_y(camera);
}

EG_SHORT egcamera_correct_x(EG_CAMERA *camera)
{
	return egcamera_correct_x_inf(camera) | egcamera_correct_x_sup(camera);
}

EG_SHORT egcamera_correct_y(EG_CAMERA *camera)
{
	return egcamera_correct_y_inf(camera) | egcamera_correct_y_sup(camera);
}

EG_SHORT egcamera_correct_x_inf(EG_CAMERA *camera)
{
	if (camera->pgx < 0)
	{
		camera->pgx = 0;
		camera->scx = 0;
		return TRUE;
	}

	return FALSE;
}

EG_SHORT egcamera_correct_x_sup(EG_CAMERA *camera)
{
	if (camera->pgx + camera->width > *camera->map_w)
	{
		camera->pgx = *camera->map_w - camera->width + 1;

		if (camera->pgx < 0) camera->pgx = 0;

		camera->scx = 0;
		return TRUE;
	}

	return FALSE;
}

EG_SHORT egcamera_correct_y_inf(EG_CAMERA *camera)
{
	if (camera->pgy < 0)
	{
		camera->pgy = 0;
		camera->scy = 0;
		return TRUE;
	}

	return FALSE;
}

EG_SHORT egcamera_correct_y_sup(EG_CAMERA *camera)
{
	if (camera->pgy + camera->height > *camera->map_h)
	{
		camera->pgy = (*camera->map_h) - camera->height + 1;

		if (camera->pgy < 0) camera->pgy = 0;

		camera->scy = 0;
		return TRUE;
	}

	return FALSE;
}

//***************************************************************
// Déplacement général scx
//
// inc_x,EG_SHORT_y peuvent avoir des valeurs >0 ou <0 et =0
//***************************************************************
void egcamera_inc(EG_CAMERA *camera, EG_SHORT inc_x, EG_SHORT inc_y)
{
	if (inc_x >= 0)
		egcamera_scx_inc(camera, inc_x);

	else
		egcamera_scx_dec(camera, abs(inc_x));

	if (inc_y >= 0)
		egcamera_scy_inc(camera, inc_y);

	else
		egcamera_scy_dec(camera, inc_y);
}

//***************************************************************
// POSITION GENERALE
//***************************************************************
void egcamera_pgy_dec(EG_CAMERA *camera, EG_SHORT dec)
{
	camera->pgy -= dec;
}

void egcamera_pgy_inc(EG_CAMERA *camera, EG_SHORT add)
{
	camera->pgy += add;
}

void egcamera_pgx_dec(EG_CAMERA *camera, EG_SHORT dec)
{
	camera->pgx -= dec;
}

void egcamera_pgx_inc(EG_CAMERA *camera, EG_SHORT add)
{
	camera->pgx += add;
}

//***************************************************************
// SCROLL X
//***************************************************************
// ** Décrémentation de scx (avec pgx)
void egcamera_scy_dec(EG_CAMERA *camera, EG_SHORT dec)
{
	camera->scy -= dec;

	if (camera->scy < 0)
	{
		camera->scy = (*camera->spr_h) + camera->scy;
		camera->pgy--;
	}
}

// Incrémentation de scx (avec pgx)
void egcamera_scy_inc(EG_CAMERA *camera, EG_SHORT add)
{
	camera->scy += add;

	if (camera->scy >= (*camera->spr_h))
	{
		camera->scy %= (*camera->spr_h);
		camera->pgy += 1;
	}
}


// ** Décrémentation de scx (avec pgx)
void egcamera_scx_dec(EG_CAMERA *camera, EG_SHORT dec)
{
	camera->scx -= dec;

	if (camera->scx < 0)
	{
		camera->scx = (*camera->spr_w) + camera->scx;
		camera->pgx--;
	}
}

// Incrémentation de scx (avec pgx)
void egcamera_scx_inc(EG_CAMERA *camera, EG_SHORT add)
{
	camera->scx += add;

	if (camera->scx >= (*camera->spr_w))
	{
		camera->scx %= (*camera->spr_w);
		camera->pgx += 1;
	}
}

//********************************************************
// CREATION
//
// Camera_w/h automatiquement dans create_egcamera
//********************************************************
EG_CAMERA *create_egcamera(EG_SHORT spr_w, EG_SHORT spr_h, EG_SHORT *map_w, EG_SHORT *map_h)
{
	return create_egcamera_ex(SCREEN_W, SCREEN_H, spr_w, spr_h, map_w, map_h);
}

void init_egcamera(EG_CAMERA *camera, EG_SHORT spr_w, EG_SHORT spr_h, EG_SHORT *map_width, EG_SHORT *map_height)
{
	init_egcamera_ex(camera, SCREEN_W, SCREEN_H, spr_w, spr_w, map_width, map_height);
}

void init_egcamera_ex(EG_CAMERA *camera, EG_SHORT screen_w, EG_SHORT screen_h, EG_SHORT spr_w, EG_SHORT spr_h, EG_SHORT *map_width, EG_SHORT *map_height)
{
	camera->map_w = map_width;
	camera->map_h = map_height;
	camera->pgx = camera->pgy = camera->scx = camera->scy = 0;

	camera->spr_w = &camera->spr_size[0];
	camera->spr_h = &camera->spr_size[1];

	*camera->spr_w = spr_w;
	*camera->spr_h = spr_h;

	egcamera_optimize_ex(camera, screen_w, screen_h);
}

EG_CAMERA *create_egcamera_ex(EG_SHORT screen_w, EG_SHORT screen_h, EG_SHORT spr_w, EG_SHORT spr_h, EG_SHORT *map_width, EG_SHORT *map_height)
{
	EG_CAMERA *camera;
	EG_MALLOC(camera, sizeof(EG_CAMERA));

	if (!camera) return 0;

	init_egcamera_ex(camera, screen_w, screen_h, spr_w, spr_h, map_width, map_height);
	return camera;
}

//
// DESTRUCTION
//
void destroy_egcamera(EG_CAMERA *camera)
{
	if (camera)
	{
		EG_FREE(camera);
	}
}
