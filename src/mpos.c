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
 *                       contact@asher256.com
 *
 ******************************************************************/

#include "internal.h"


//*******************************************************
// scr2map
//*******************************************************
EG_INT   egmpos_x_map2scr(EG_MPOS *mpos, EG_SHORT pgx)
{
	return egcamera_x_map2scr(mpos, pgx) + mpos->scx;
}

EG_INT   egmpos_y_map2scr(EG_MPOS *mpos, EG_SHORT pgy)
{
	return egcamera_y_map2scr(mpos, pgy) + mpos->scy;
}

EG_SHORT egmpos_x_scr2map(EG_MPOS *mpos, EG_INT scr_x)
{
	return egcamera_x_scr2map(mpos, scr_x);
}

EG_SHORT egmpos_y_scr2map(EG_MPOS *mpos, EG_INT scr_y)
{
	return egcamera_y_scr2map(mpos, scr_y);
}

//*******************************************************
// Obstacle
//
// cette fonction appelle (*test) le nombre de fois
// d'ont elle a besoin en passant pgx et pgy de
// l'obstacle.
// si (*test) retourne nonz alors elle stoppe le test
// et retourne nonz.
//
// xy_incdec pas de nombre négatif ds add dec
// mais _xy oui, c possible
//*******************************************************
EG_SHORT egmpos_collision_x(EG_MPOS *mpos, EG_SHORT addx, EG_SHORT (*test)(EG_SHORT pgx, EG_SHORT pgy))
{
	EG_SHORT i;

	for (i = 0; i < mpos->height; i += (*mpos->spr_h - 1))
	{
		if (egmpos_collision(mpos, addx, i, test)) return TRUE;
	}

	if (i > mpos->height - 1)
		return egmpos_collision(mpos, addx, mpos->height - 1, test);

	return FALSE;
}

EG_SHORT egmpos_collision_y(EG_MPOS *mpos, EG_SHORT addy, EG_SHORT (*test)(EG_SHORT pgx, EG_SHORT pgy))
{
	EG_SHORT i;

	for (i = 0; i < mpos->width; i += (*mpos->spr_w - 1))
	{
		if (egmpos_collision(mpos, i, addy, test)) return TRUE;
	}

	if (i > mpos->width - 1)
		return egmpos_collision(mpos, mpos->width - 1, addy, test);

	return FALSE;
}

EG_SHORT egmpos_collision(EG_MPOS *mpos, EG_SHORT addx, EG_SHORT addy, EG_SHORT (*test)(EG_SHORT pgx, EG_SHORT pgy))
{
	static EG_SHORT pgx, pgy, scx, scy;

	pgx = mpos->pgx;
	pgy = mpos->pgy;

	if (addx)
	{
		scx = mpos->scx;
		eg_inc(&pgx, &scx, *mpos->spr_w, addx);
	}

	if (addy)
	{
		scy = mpos->scy;
		eg_inc(&pgy, &scy, *mpos->spr_h, addy);
	}

	if ((*test)(pgx, pgy))
		return TRUE;

	return FALSE;
}

//*******************************************************
// Fixer camera
//*******************************************************
void egmpos_camera_fix(EG_MPOS *mpos, EG_CAMERA *camera)
{
	egcamera_center(camera, mpos->pgx, mpos->pgy, mpos->scx, mpos->scy);
}

//*******************************************************
// SIZE
//*******************************************************
EG_SHORT egmpos_get_ycount(EG_MPOS *mpos)
{
	return egcamera_get_ycount(mpos);
}

EG_SHORT egmpos_get_xcount(EG_MPOS *mpos)
{
	return egcamera_get_xcount(mpos);
}

void egmpos_set_ycount(EG_MPOS *mpos, EG_SHORT cy)
{
	egcamera_set_ycount(mpos, cy);
}

void egmpos_set_xcount(EG_MPOS *mpos, EG_SHORT cx)
{
	egcamera_set_xcount(mpos, cx);
}

void egmpos_set_count(EG_MPOS *mpos, EG_SHORT cx, EG_SHORT cy)
{
	egcamera_set_count(mpos, cx, cy);
}

//*******************************************************
// SIZE
//*******************************************************
EG_SHORT egmpos_get_sprwidth(EG_MPOS *mpos)
{
	return *mpos->spr_h;
}

EG_SHORT egmpos_get_sprheight(EG_MPOS *mpos)
{
	return *mpos->spr_w;
}

EG_SHORT egmpos_get_width(EG_MPOS *mpos)
{
	return mpos->width;
}

EG_SHORT egmpos_get_height(EG_MPOS *mpos)
{
	return mpos->height;
}

void egmpos_set_size(EG_MPOS *mpos, EG_SHORT w, EG_SHORT h)
{
	mpos->width = w;
	mpos->height = h;
}

void egmpos_set_height(EG_MPOS *mpos, EG_SHORT height)
{
	mpos->height = height;
}

void egmpos_set_width(EG_MPOS *mpos, EG_SHORT width)
{
	mpos->width = width;
}

//*******************************************************
// déplacements
//*******************************************************
EG_SHORT egmpos_filef_proc(void *eg_filef_data, void *data)
{
	return egcamera_filef_proc(eg_filef_data, data);
}

//*******************************************************
// déplacements
//*******************************************************
void egmpos_inc(EG_MPOS *mpos, EG_SHORT inc_x, EG_SHORT inc_y)
{
	egcamera_inc(mpos, inc_x, inc_y);
}

void egmpos_scy_dec(EG_MPOS *mpos, EG_SHORT dec)
{
	egcamera_scy_dec(mpos, dec);
}

void egmpos_scy_inc(EG_MPOS *mpos, EG_SHORT add)
{
	egcamera_scy_inc(mpos, add);
}

void egmpos_scx_dec(EG_MPOS *mpos, EG_SHORT dec)
{
	egcamera_scx_dec(mpos, dec);
}

void egmpos_scx_inc(EG_MPOS *mpos, EG_SHORT add)
{
	egcamera_scx_inc(mpos, add);
}

void egmpos_pgy_dec(EG_MPOS *mpos, EG_SHORT dec)
{
	egmpos_pgy_dec(mpos, dec);
}

void egmpos_pgy_inc(EG_MPOS *mpos, EG_SHORT add)
{
	egmpos_pgy_inc(mpos, add);
}

void egmpos_pgx_dec(EG_MPOS *mpos, EG_SHORT dec)
{
	egmpos_pgx_dec(mpos, dec);
}

void egmpos_pgx_inc(EG_MPOS *mpos, EG_SHORT add)
{
	egmpos_pgx_inc(mpos, add);
}

//*******************************************************
// CREATE/DESTROY
//*******************************************************
void init_egmpos(EG_MPOS *mpos, EG_SHORT w, EG_SHORT h, EG_SHORT *spr_w, EG_SHORT *spr_h)
{
	mpos->pgx = mpos->pgy = mpos->scx = mpos->scy = 0;
	mpos->width = w;
	mpos->height = h;
	mpos->spr_w = spr_w;
	mpos->spr_h = spr_h;
}
EG_MPOS *create_egmpos(EG_SHORT w, EG_SHORT h, EG_SHORT *spr_w, EG_SHORT *spr_h)
{
	EG_MPOS *mpos;
	EG_MALLOC(mpos, sizeof(EG_MPOS));

	if (!mpos) return 0;

	init_egmpos(mpos, w, h, spr_w, spr_h);
	return mpos;
}

void destroy_egmpos(EG_MPOS *mpos)
{
	destroy_egcamera(mpos);
}

