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

#define Maxframe(n)  (*n->get_info)(&n,EG_LANIM_MAXFRAME)
#define Nextframe(n) (*n->get_info)(&n,EG_LANIM_NEXTFRAME)
#define Width(n)     (*n->get_info)(&n,EG_LANIM_WIDTH)
#define Height(n)    (*n->get_info)(&n,EG_LANIM_HEIGHT)

//*******************************************
// FF/REW
//*******************************************
void eglanim_rew(EG_LANIM *lanim)
{
	lanim->actual_frame = 0;
}

void eglanim_ff(EG_LANIM *lanim)
{
	lanim->actual_frame = Maxframe(lanim) - 1;

	if (lanim->actual_frame < 0) lanim->actual_frame = 0;
}

//*******************************************
// WIDTH/HEIGHT
//*******************************************
EG_INT eglanim_get_width(EG_LANIM *lanim)
{
	return Width(lanim);
}

EG_INT eglanim_get_height(EG_LANIM *lanim)
{
	return Height(lanim);
}

//*******************************************
// INC / DEC
// retourne TRUE si fin (dans inc/dec seule-
// ment pas dans loop)
//*******************************************
EG_INT eglanim_incloop(EG_LANIM *lanim)
{
	if (eglanim_inc(lanim))
	{
		eglanim_rew(lanim);
		return TRUE;
	}

	return FALSE;
}

EG_INT eglanim_decloop(EG_LANIM *lanim)
{
	if (eglanim_dec(lanim))
	{
		eglanim_ff(lanim);
		return TRUE;
	}

	return FALSE;
}

EG_INT eglanim_dec(EG_LANIM *lanim)
{
	// Fin en dec?
	if (lanim->actual_frame == 0)
	{
		lanim->actual_frame = Maxframe(lanim) - 1;

		if (Nextframe(lanim))
		{
			lanim->actual_frame = 0;
			return TRUE;
		}
	}

	// prochaine frame?
	lanim->actual_frame--;

	if (Nextframe(lanim))
		return FALSE;

	// Sinon restoration
	lanim->actual_frame++;
	return FALSE;
}

// TRUE si demande ++ mais il ne peux l'assouvir
EG_INT eglanim_inc(EG_LANIM *lanim)
{
	EG_INT n = Maxframe(lanim);

	// pas de next frame alors...
	if (lanim->actual_frame == n - 1)
		return (Nextframe(lanim)) ? TRUE : FALSE; // fini!

	// NEXT FRAME
	else if (Nextframe(lanim))
	{
		lanim->actual_frame++;
		return FALSE; // pas encore fini !
	}

	return FALSE;
}

//*******************************************
// Draw
//*******************************************
void eglanim_draw(EG_LANIM *lanim, BITMAP *bmp, EG_INT x, EG_INT y)
{
	(*lanim->draw_anim)(lanim, bmp, x, y);
}

EG_INT eglanim_draw_inc(EG_LANIM *lanim, BITMAP *bmp, EG_INT x, EG_INT y)
{
	eglanim_draw(lanim, bmp, x, y);
	return eglanim_inc(lanim);
}

EG_INT eglanim_draw_incloop(EG_LANIM *lanim, BITMAP *bmp, EG_INT x, EG_INT y)
{
	eglanim_draw(lanim, bmp, x, y);
	return eglanim_incloop(lanim);
}

EG_INT eglanim_draw_dec(EG_LANIM *lanim, BITMAP *bmp, EG_INT x, EG_INT y)
{
	eglanim_draw(lanim, bmp, x, y);
	return eglanim_dec(lanim);
}

EG_INT eglanim_draw_decloop(EG_LANIM *lanim, BITMAP *bmp, EG_INT x, EG_INT y)
{
	eglanim_draw(lanim, bmp, x, y);
	return eglanim_decloop(lanim);
}


//*******************************************
// CREATE EG LANIM
//*******************************************
EG_LANIM *create_eglanim(void (*draw_anim)(void *lanim, BITMAP *bmp, EG_INT x, EG_INT y), EG_INT (*get_info)(void *lanim, EG_INT info_type))
{
	EG_LANIM *lanim;
	EG_MALLOC(lanim, sizeof(EG_LANIM));

	if (!lanim) return 0;

	lanim->actual_frame = 0;
	lanim->draw_anim = draw_anim;
	lanim->get_info = get_info;

	return lanim;
}

//*******************************************
// DESTROY EG LANIM
//*******************************************
void destroy_eglanim(EG_LANIM *anim)
{
	if (!anim) return;

	EG_FREE(anim);
}
