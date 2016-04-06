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

//************************************************************************************
// si d1 TRUE alors animation LOOP
// si d2 TRUE alors l'animation est joue
// si w ou h != -1 alors c'est la limite d'affichage de l'animation
// Bg contient la couleur d'arrire
// Automatiquement, d2 contient un temp BMP
//************************************************************************************
EG_INT d_eganim2d_proc(EG_INT msg, DIALOG *d, EG_INT c)
{
	static EG_ANIM2D *anim;       // Un poEG_INTeur vers une animation
	static EG_INT temp = 0;       // Il va servir  dtcter si l'animation est change
	static EG_INT width, height;  // Le width et height de la frame actuelle
	EG_INT changed = FALSE;       // Pour dtecter si l'animation a change
	EG_INT mouse_in_animation = FALSE;
	BITMAP *temp_bmp;

	// Met le poEG_INTeur
	anim = d->dp;

	// Allocation mmoire d'un buffer
	if (msg == MSG_START)
	{
		d->dp2 = (void *) create_bitmap(eganim2d_get_width(anim), eganim2d_get_height(anim));

		if (!d->dp2)
		{
			eg_out_of_memory();
		}

		return D_O_K;
	}

	// Dsallocation
	if (msg == MSG_END)
	{
		destroy_bitmap((BITMAP *)d->dp2);
		return D_O_K;
	}

	// Met le bmp
	temp_bmp = (BITMAP *) d->dp2;

	// Si l'on doit jouer l'animation alors la jouer !
	if (d->d2)
	{
		temp = anim->actual_frame;

		// Si le flag LOOP
		if (d->d1)
			eganim2d_incloop(d->dp);

		else
		{
			if (eganim2d_inc(d->dp))  //Si fin alors stopper cette animation
			{
				d->d2 = FALSE;
			}
		}

		// Si change
		if (temp != anim->actual_frame)
		{
			changed = TRUE;
		}
	}

	// Mise  jour de la liste
	if (msg == MSG_DRAW || changed)
	{
		/*if((d->w!=-1 && d->h!=-1) && (
		   (mouse_x>=d->x && mouse_y>=d->y && mouse_x<=d->x+d->w-1 && mouse_y<=d->y+d->h-1) ||
		   (mouse_x+mouse_sprite->w-1>=d->x && mouse_y+mouse_sprite->h-1>=d->y && mouse_x+mouse_sprite->w-1<=d->x+d->w-1 && mouse_y+mouse_sprite->h-1<=d->y+d->h-1)
		   ))*/
		mouse_in_animation = TRUE;

		if (mouse_in_animation) show_mouse(NULL);

		// Met le clip s'il le faut
		if (d->w != -1 && d->h != -1) set_clip(screen, d->x, d->y, d->x + d->w - 1, d->y + d->h - 1);

		// Met le width et height
		width = eganim2d_get_width(anim);
		height = eganim2d_get_height(anim);

		// Si frame actuelle plus grande que le buffer alors rallocation
		if (temp_bmp->w < width || temp_bmp->h < height)
		{
			destroy_bitmap((BITMAP *)d->dp2);
			d->dp2 = (void *) create_bitmap(width, height);

			if (!d->dp2)
			{
				eg_out_of_memory();
			}

			// Met le bmp
			temp_bmp = (BITMAP *) d->dp2;
		}


		// Sinon, si pas d'arrire plan alors sauver avant de putter ceci
		clear_to_color(temp_bmp, d->bg);
		eganim2d_draw(anim, temp_bmp, 0, 0);
		blit(temp_bmp, screen, 0, 0, d->x, d->y, temp_bmp->w, temp_bmp->h);

		// Remet le clip normalement, et affiche le background s'il le faut
		if (d->w != -1 && d->h != -1)
		{
			// S'il y a le background && la souris est dans l'animation
			rectfill(screen, d->x + width, d->y, d->x + d->w - 1, d->y + d->h - 1, d->bg);
			rectfill(screen, d->x, d->y + height, d->x + width - 1, d->y + d->h - 1, d->bg);

			set_clip(screen, 0, 0, screen->w, screen->h);
		}

		if (mouse_in_animation) show_mouse(screen);
	}

	return D_O_K;
}

