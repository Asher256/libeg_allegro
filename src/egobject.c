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

//************** FONCTIONS PRINCIPALES D'AFFICHAGE ET DESTRUCTION *******************
// TEXT
static void _ego_draw_text(BITMAP *buffer, void *text, void *fnt, EG_INT x, EG_INT y, EG_INT d1)
{
	textout(buffer, (FONT *)fnt , (EG_CHAR *)text, x, y, d1);
}
static void _ego_destroy_text(void *data)
{
	EG_FREE(data);
}
static EG_INT _ego_text_width(EG_CHAR *data, EG_CHAR *data2, EG_INT d1, EG_INT d2)
{
	return text_length((FONT *)data2, (EG_CHAR *)data);
}
static EG_INT _ego_text_height(EG_CHAR *data, EG_CHAR *data2, EG_INT d1, EG_INT d2)
{
	return text_height((FONT *)data2);
}

// BMP
static void _ego_draw_bmp(BITMAP *buffer, void *bmp, void *fd0, EG_INT x, EG_INT y, EG_INT fd1)
{
	draw_sprite(buffer, (BITMAP *) bmp, x, y);
}
static void _ego_destroy_bmp(void *data)
{
	destroy_bitmap((BITMAP *)data);
}
static EG_INT _ego_bmp_width(EG_CHAR *data, EG_CHAR *data2, EG_INT d1, EG_INT d2)
{
	return ((BITMAP *)data)->w;
}
static EG_INT _ego_bmp_height(EG_CHAR *data, EG_CHAR *data2, EG_INT d1, EG_INT d2)
{
	return ((BITMAP *)data)->h;
}
//RLE
static void _ego_draw_rle(BITMAP *buffer, void *rle, void *fd0, EG_INT x, EG_INT y, EG_INT fd1)
{
	draw_rle_sprite(buffer, rle, x, y);
}
static void _ego_destroy_rle(void *data)
{
	destroy_rle_sprite((RLE_SPRITE *)data);
}
static EG_INT _ego_rle_width(EG_CHAR *data, EG_CHAR *data2, EG_INT d1, EG_INT d2)
{
	return ((RLE_SPRITE *)data)->w;
}
static EG_INT _ego_rle_height(EG_CHAR *data, EG_CHAR *data2, EG_INT d1, EG_INT d2)
{
	return ((RLE_SPRITE *)data)->h;
}
//EG_ANIM2D
static void _ego_draw_eganim2d_loop(BITMAP *buffer, void *anim, void *fd0, EG_INT x, EG_INT y, EG_INT fd1)
{
	eganim2d_draw_incloop((EG_ANIM2D *) anim, buffer, x, y);
}
static void _ego_draw_eganim2d_inc(BITMAP *buffer, void *anim, void *fd0, EG_INT x, EG_INT y, EG_INT fd1)
{
	eganim2d_draw_inc((EG_ANIM2D *) anim, buffer, x, y);
}
static void _ego_destroy_eganim2d(void *data)
{
	destroy_eganim2d((EG_ANIM2D *)data);
}
static EG_INT _ego_eganim2d_width(EG_CHAR *data, EG_CHAR *data2, EG_INT d1, EG_INT d2)
{
	return eganim2d_get_width((EG_ANIM2D *)data);
}
static EG_INT _ego_eganim2d_height(EG_CHAR *data, EG_CHAR *data2, EG_INT d1, EG_INT d2)
{
	return eganim2d_get_height((EG_ANIM2D *)data);
}
//space
static void _ego_draw_space(BITMAP *buffer, void *spc, void *fd0, EG_INT x, EG_INT y, EG_INT fd1) {}
static void _ego_destroy_space(void *data) {}
static EG_INT _ego_space_width(EG_CHAR *data, EG_CHAR *data2, EG_INT d1, EG_INT d2)
{
	return d1;
}
static EG_INT _ego_space_height(EG_CHAR *data, EG_CHAR *data2, EG_INT d1, EG_INT d2)
{
	return d2;
}
//*****************************************************************************

//***********************************************************************
//Constructeur
//***********************************************************************
EG_OBJECT *create_egobject()
{
	EG_OBJECT *temp;

	EG_MALLOC(temp, sizeof(EG_OBJECT));

	if (!temp) return NULL;

	temp->allocated = 0;

	egobject_init_text(temp, "Empty object !!", font, 15);

	return temp;    //Retourne le temp !
}
void destroy_egobject(EG_OBJECT *obj)
{
	// Destruction de l'objet si spcifi
	if (obj->allocated)
		(*obj->destroy) (obj->data);

	// By mon pote !,
	EG_FREE(obj);
}

//***********************************************************************
//Retourne le width
//***********************************************************************
EG_INT egobject_get_width(EG_OBJECT *obj)
{
	return (*obj->width)(obj->data, obj->data2, obj->d1, obj->d2);
}

//***********************************************************************
//Retourne le height
//***********************************************************************
EG_INT egobject_get_height(EG_OBJECT *obj)
{
	return (*obj->height)(obj->data, obj->data2, obj->d1, obj->d2);
}

//***********************************************************************
//Affiche l'objet
//***********************************************************************
void egobject_draw(EG_OBJECT *obj, BITMAP *buffer, EG_INT x, EG_INT y)
{
	(*obj->draw) (buffer, obj->data, obj->data2, x, y, obj->d1);
}

//******************************************************
// insertion du font
//******************************************************
void egobject_init_text(EG_OBJECT *obj, const EG_CHAR *str, FONT *f, EG_INT d1)
{
	obj->allocated = 0;
	obj->type     = EGO_TEXT;
	obj->data     = (void *) str;
	obj->data2    = (void *) f;
	obj->d1       = d1;
	obj->draw     = _ego_draw_text;
	obj->destroy  = _ego_destroy_text;
	obj->width    = _ego_text_width;
	obj->height   = _ego_text_height;
}

//******************************************************
//Convertit cet objet en bmp
//******************************************************
void egobject_init_bmp(EG_OBJECT *obj, BITMAP *bmp)
{
	obj->allocated = 0;
	obj->type    = EGO_BMP;
	obj->data    = (void *) bmp;
	obj->data2   = (void *) bmp;
	obj->draw    = _ego_draw_bmp;
	obj->destroy = _ego_destroy_bmp;
	obj->width   = _ego_bmp_width;
	obj->height  = _ego_bmp_height;
}

//******************************************************
//Le Convertit en espace y
//******************************************************
void egobject_init_space(EG_OBJECT *obj, EG_INT px, EG_INT py)
{
	obj->allocated = 0;
	obj->type    = EGO_SPACE;
	obj->data    = 0;
	obj->data2   = 0;
	obj->d1      = px;     //Il profite de la variable d1
	obj->d2      = py;     //Il profite de la variable d1
	obj->draw    = _ego_draw_space;
	obj->destroy = _ego_destroy_space;
	obj->width   = _ego_space_width;
	obj->height  = _ego_space_height;
}

//******************************************************
//Le convertit en Animation
//******************************************************
void egobject_init_eganim2d(EG_OBJECT *obj, EG_ANIM2D *anim, EG_INT loop)
{
	obj->allocated = 0;
	obj->type    = EGO_EGANIM2D;
	obj->data    = (void *) anim;
	obj->data2    = (void *) anim;

	if (loop)
		obj->draw    = _ego_draw_eganim2d_loop;

	else
		obj->draw    = _ego_draw_eganim2d_inc;

	obj->destroy = _ego_destroy_eganim2d;
	obj->width   = _ego_eganim2d_width;
	obj->height  = _ego_eganim2d_height;
}

//******************************************************
// RLE SPRITE
//******************************************************
void egobject_init_rle(EG_OBJECT *obj, RLE_SPRITE *rle)
{
	obj->allocated = 0;
	obj->type    = EGO_RLE;
	obj->data    = (void *) rle;
	obj->data2   = (void *) rle;
	obj->draw    = _ego_draw_rle;
	obj->destroy = _ego_destroy_rle;
	obj->width   = _ego_rle_width;
	obj->height  = _ego_rle_height;
}

