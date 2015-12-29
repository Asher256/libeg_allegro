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

static void eganim2d_insert_ex(EG_ANIM2D *anim, EG_INT index, void *bmp, EG_INT delay, EG_INT type);

//************************************************************************
// Copie frame_src dans frame_dest
//************************************************************************
void frame_cpy(EG_FRAME *frame_dest, EG_FRAME *frame_src)
{
	frame_dest->width = frame_src->width;
	frame_dest->height = frame_src->height;
	frame_dest->data = frame_src->data;
	frame_dest->draw = frame_src->draw;
	frame_dest->destroy = frame_src->destroy;
	frame_dest->msec = frame_src->msec;
	frame_dest->type = frame_src->type;
	frame_dest->allocated = frame_src->allocated;
}
//***********************************************************
// Swap utilis par eganim2d_insert_bmplast() et next
//***********************************************************
void frame_swap(EG_FRAME *frame1, EG_FRAME *frame2)
{
	EG_FRAME temp;
	frame_cpy(&temp, frame1);
	frame_cpy(frame1, frame2);
	frame_cpy(frame2, &temp);
}

//************************************************************************
// Suppression d'une frame
//************************************************************************
void eganim2d_delete_frame(EG_ANIM2D *anim, EG_INT index)
{
	static EG_INT i;

	// Si erreur alors quitter la fonction
	if (anim->max_frame <= 0 || index > anim->max_frame - 1) return;

	// Si l'on doit faire la dsallocation
	if (anim->frame[index].allocated)
		(*anim->frame[index].destroy)(&anim->frame[index].data);

	// Suppression de la frame
	for (i = index; i < anim->max_frame - 1; i++)
		frame_cpy(&anim->frame[i], &anim->frame[i + 1]);

	anim->max_frame--;
}

//************************************************************************
// Insertion d'un BMP
//************************************************************************
void eganim2d_insert_bmp(EG_ANIM2D *anim, EG_INT index, BITMAP *bmp, EG_INT delay)
{
	eganim2d_insert_ex(anim, index, (void *)bmp, delay, FALSE);
}
void eganim2d_insert_rle(EG_ANIM2D *anim, EG_INT index, BITMAP *rle, EG_INT delay)
{
	eganim2d_insert_ex(anim, index, (void *)rle, delay, TRUE);
}

//************************************************************************
// Insertion d'un BMP ou RLE
//************************************************************************
static void eganim2d_insert_ex(EG_ANIM2D *anim, EG_INT index, void *bmp, EG_INT delay, EG_INT type) //type==TRUE si rle
{
	static EG_INT i;
	// Sauve la frame
	static EG_FRAME save_frame;

	// Il peut accepter max_frame car eganim2d_insert_bmpnext lui donne cet ordre si la dernire frame est select.
	if (index > anim->max_frame) return;

	// Insertion  la fin
	if (!type)
		eganim2d_add_bmp(anim, (BITMAP *)bmp, delay);

	else
		eganim2d_add_rle(anim, (RLE_SPRITE *)bmp, delay);

	// Copie le truc ajout
	frame_cpy(&save_frame, &anim->frame[anim->max_frame - 1]);

	// Mise  jour de la liste de poEG_INTeurs
	for (i = anim->max_frame - 1; i > index; i--)
		frame_cpy(&anim->frame[i], &anim->frame[i - 1]);

	// Et enfin la touche finale
	frame_cpy(&anim->frame[index], &save_frame);
}

//***********************************************************************
//Retourne la hauteur et la largeur
//***********************************************************************
EG_INT eganim2d_get_width(EG_ANIM2D *anim /* Le poEG_INTeur vers l'animation */)
{

	if (anim->max_frame > 0)
		return *anim->frame[anim->actual_frame].width;

	return 0;
}

EG_INT eganim2d_get_height(EG_ANIM2D *anim)

{
	if (anim->max_frame > 0)
		return *anim->frame[anim->actual_frame].height;

	return 0;
}

//***********************************************************************
//retourne la frame actuelle
//***********************************************************************
EG_INT eganim2d_frame(EG_ANIM2D *anim)
{
	return anim->actual_frame;
}
void eganim2d_set_frame(EG_ANIM2D *anim, EG_INT n)
{
	if (n < 0) n = 0;

	if (n > anim->max_frame - 1) n = anim->max_frame - 1;

	anim->actual_frame = n;
}
EG_INT eganim2d_get_frame(EG_ANIM2D *anim)
{
	return anim->actual_frame;
}

//***********************************************************************
//retourne la maximum d'images
//***********************************************************************
EG_INT eganim2d_max_frame(EG_ANIM2D *anim)
{
	return anim->max_frame;
}

//==== Les fonctions EG_INTernes de draw ======
static void _draw_sprite(BITMAP *buffer, void *data, EG_INT x, EG_INT y)
{
	draw_sprite(buffer, (BITMAP *)data, x, y);
}

static void _draw_rle_sprite(BITMAP *buffer, void *data, EG_INT x, EG_INT y)
{
	draw_rle_sprite(buffer, (RLE_SPRITE *)data, x, y);
}

static void _destroy_rle_sprite(void *data)
{
	destroy_rle_sprite((RLE_SPRITE *) data);
}

static void _destroy_bitmap(void *data)
{
	destroy_bitmap((BITMAP *) data);
}

//***********************************************************************
//Ajoute un bitmap dans l'animation
//***********************************************************************
EG_INT eganim2d_add_bmp(EG_ANIM2D *anim, BITMAP *bitmap, EG_INT delay)
{
	anim->max_frame++;

	if (anim->max_frame > anim->max_frame_allocation)
	{
		anim->max_frame_allocation = anim->max_frame;
		EG_REALLOC(anim->frame, sizeof(EG_FRAME)*anim->max_frame);
	}

	// S'il n'y a pas assez de mmoire
	if (!anim->frame)
	{
		eg_out_of_memory();
	}

	anim->frame[anim->max_frame - 1].data = (void *) bitmap;
	anim->frame[anim->max_frame - 1].msec = delay;
	anim->frame[anim->max_frame - 1].draw = _draw_sprite;
	anim->frame[anim->max_frame - 1].destroy = _destroy_bitmap;
	anim->frame[anim->max_frame - 1].type = EGO_BMP;
	anim->frame[anim->max_frame - 1].width = &bitmap->w;
	anim->frame[anim->max_frame - 1].height = &bitmap->h;
	anim->frame[anim->max_frame - 1].allocated = 0;
	return anim->max_frame - 1;
}
EG_INT eganim2d_add_rle(EG_ANIM2D *anim, RLE_SPRITE *bitmap, EG_INT delay)
{
	anim->max_frame++;

	if (anim->max_frame > anim->max_frame_allocation)
	{
		anim->max_frame_allocation = anim->max_frame;
		EG_REALLOC(anim->frame, sizeof(EG_FRAME)*anim->max_frame);
	}

	// S'il n'y a pas assez de mmoire
	if (!anim->frame)
	{
		eg_out_of_memory();
	}

	anim->frame[anim->max_frame - 1].data = (void *) bitmap;
	anim->frame[anim->max_frame - 1].msec = delay;
	anim->frame[anim->max_frame - 1].draw = _draw_rle_sprite;
	anim->frame[anim->max_frame - 1].destroy = _destroy_rle_sprite;
	anim->frame[anim->max_frame - 1].type = EGO_RLE;
	anim->frame[anim->max_frame - 1].width = &bitmap->w;
	anim->frame[anim->max_frame - 1].height = &bitmap->h;
	anim->frame[anim->max_frame - 1].allocated = 0;
	return anim->max_frame - 1;
}


//***********************************************************************
//Efface toute l'animation
//***********************************************************************
void eganim2d_clear(EG_ANIM2D *anim)
{
	EG_INT i;

	for (i = 0; i < anim->max_frame; i++)
	{
		//Si elle a t alloque par une sub non par l'utilisateur
		if (anim->frame[i].allocated)
		{
			(*anim->frame[i].destroy) (anim->frame[i].data);
		}
	}

	anim->actual_frame = 0;
	anim->max_frame = 0;
}

//***********************************************************************
//Affiche et Incrmente un frame en loop
//***********************************************************************
EG_INT eganim2d_draw_incloop(EG_ANIM2D *anim, BITMAP *buffer, EG_INT x, EG_INT y)
{
	eganim2d_draw(anim, buffer, x, y);
	return eganim2d_incloop(anim);
}
EG_INT eganim2d_draw_decloop(EG_ANIM2D *anim, BITMAP *buffer, EG_INT x, EG_INT y)
{
	eganim2d_draw(anim, buffer, x, y);
	return eganim2d_decloop(anim);
}

//***********************************************************************
//Affiche et Incrmente un frame
//***********************************************************************
EG_INT eganim2d_draw_inc(EG_ANIM2D *anim, BITMAP *buffer, EG_INT x, EG_INT y)
{
	eganim2d_draw(anim, buffer, x, y); //Affiche le bitmap
	return eganim2d_inc(anim);  //Retourne le ret de la frame incrmente
}
EG_INT eganim2d_draw_dec(EG_ANIM2D *anim, BITMAP *buffer, EG_INT x, EG_INT y)
{
	eganim2d_draw(anim, buffer, x, y); //Affiche le bitmap
	return eganim2d_dec(anim);  //Retourne le ret de la frame incrmente
}

//***********************************************************************
//Affiche le bitmap actuel dans l'animation
//***********************************************************************
void eganim2d_draw(EG_ANIM2D *anim, BITMAP *buffer, EG_INT x, EG_INT y)
{
	if (anim->max_frame > 0)    //si au moin il y a un frame->data
		(*anim->frame[anim->actual_frame].draw) (buffer, anim->frame[anim->actual_frame].data, x, y);
}

//***********************************************************************
//Revenir au dbut de l'animation
//***********************************************************************
void eganim2d_rew(EG_ANIM2D *anim)
{
	*anim->counter = 0;
	anim->actual_frame = 0;      //c'est tout !
}

void eganim2d_ff(EG_ANIM2D *anim)
{
	*anim->counter = 0;
	anim->actual_frame = anim->max_frame - 1;    //c'est tout !

	if (anim->actual_frame < 0) anim->actual_frame = 0;
}

//***********************************************************************
// Incrmente avec loop
//***********************************************************************
EG_INT eganim2d_incloop(EG_ANIM2D *anim)
{
	static EG_INT ret;

	if ((ret = eganim2d_inc(anim)))
		anim->actual_frame = 0;

	return ret;
}
EG_INT eganim2d_decloop(EG_ANIM2D *anim)
{
	static EG_INT ret;

	if ((ret = eganim2d_dec(anim)))
		anim->actual_frame = anim->max_frame - 1;

	return ret;
}


//***********************************************************************
//Il incrmente un frame puis quand il arrive  la fin alors il stoppe
//Quand il est stopp alors il retourne 1
//***********************************************************************
EG_INT eganim2d_inc(EG_ANIM2D *anim)
{
	//il teste si la prochaine ne dpasse pas la fin
	if (*anim->counter >= anim->frame[anim->actual_frame].msec)
	{
		*anim->counter = 0;

		//si fin de compteur
		if (anim->actual_frame + 1 < anim->max_frame)
		{
			anim->actual_frame++;
		}

		else
			return 1;
	}

	//sinon 0
	return 0;       //tout est ok
}
EG_INT eganim2d_dec(EG_ANIM2D *anim)
{
	EG_INT pos = anim->actual_frame - 1;

	// Si bug alors position du timer=maximum
	if (pos < 0)
	{
		pos = anim->max_frame - 1;
	}

	//il teste si la prochaine ne dpasse pas la fin
	if (*anim->counter >= anim->frame[pos].msec)
	{
		*anim->counter = 0;

		//Si compteur actuel n'est pas moin que 0 alors dec sinon, ret 1
		if (anim->actual_frame > 0)
		{
			anim->actual_frame--;
		}

		else
			return 1;
	}

	//sinon 0
	return 0;       //tout est ok
}

//***********************************************************************
// Constructeur
//***********************************************************************
EG_ANIM2D *create_eganim2d(volatile EG_INT *counter)
{
	EG_ANIM2D *temp;

	EG_MALLOC(temp, sizeof(EG_ANIM2D));

	if (!temp) return temp;

	temp->max_frame = 0;
	temp->max_frame_allocation = 0;
	temp->actual_frame = 0;

	EG_MALLOC(temp->frame, sizeof(EG_FRAME));

	if (!temp->frame)
	{
		eg_out_of_memory();
	}

	temp->counter = counter;
	return temp;
}

//***********************************************************************
//destructeur
//***********************************************************************
void destroy_eganim2d(EG_ANIM2D *anim)
{
	EG_INT i;

	for (i = 0; i < anim->max_frame; i++)
	{
		//Si elle a t alloque par une sub non par l'utilisateur
		if (anim->frame[i].allocated)
		{
			(*anim->frame[i].destroy) (anim->frame[i].data);
		}
	}

	EG_FREE(anim->frame);
	EG_FREE(anim);
}
