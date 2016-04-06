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

//***********************************************************************
// Constructeur de la classe Credit
//***********************************************************************
EG_CREDIT *create_egcredit()
{
	EG_CREDIT *temp;

	EG_MALLOC(temp, sizeof(EG_CREDIT));

	if (!temp) return 0;

	EG_MALLOC(temp->animation_index, sizeof(EG_INT));
	temp->animation_index_size = 0;

	temp->true_height = 0;
	temp->position_y = 0;

	EG_MALLOC(temp->egcredit, sizeof(EG_OBJECT));

	if (!temp->egcredit) return 0;

	temp->egcredit_size = 0;

	return temp;
}

//***********************************************************************
// Destructeur de la classe Credit
//***********************************************************************
void destroy_egcredit(EG_CREDIT *egcredit)
{
	if (!egcredit) return;

	if (egcredit->animation_index) EG_FREE(egcredit->animation_index);

	if (egcredit->egcredit) EG_FREE(egcredit->egcredit);

	EG_FREE(egcredit);
}

//***********************************************************************
// _al_reallocation du nombre d'objet de egcredit
//***********************************************************************
static void _egcredit_reallocation(EG_CREDIT *egcredit, EG_INT sz)
{
	EG_REALLOC(egcredit->egcredit, sizeof(EG_OBJECT)*sz);
	egcredit->egcredit_size = sz;

	if (!egcredit->egcredit)    //si pas de mmoire
	{
		eg_out_of_memory();
	}
}

//***********************************************************************
// Ajoute un string dans le egcredit
//***********************************************************************
void egcredit_add_text(EG_CREDIT *egcredit, EG_CHAR *text, FONT *f, EG_INT color)
{
	_egcredit_reallocation(egcredit, egcredit->egcredit_size + 1);      //_al_reallocation
	egobject_init_text(&egcredit->egcredit[egcredit->egcredit_size - 1], text, f, color);
	egcredit->true_height += egobject_get_height(&egcredit->egcredit[egcredit->egcredit_size - 1]);
}

//***********************************************************************
// Ajout d'un pixel height
//***********************************************************************
void egcredit_add_space(EG_CREDIT *egcredit, EG_INT py)
{
	_egcredit_reallocation(egcredit, egcredit->egcredit_size + 1);
	egobject_init_space(&egcredit->egcredit[egcredit->egcredit_size - 1], 1, py);
	egcredit->true_height += egobject_get_height(&egcredit->egcredit[egcredit->egcredit_size - 1]);
}

//***********************************************************************
// Ajoute un bitmap dans le egcredit
//***********************************************************************
static void _egcredit_add_animation_index(EG_CREDIT *egcredit, EG_INT index)
{
	EG_REALLOC(egcredit->animation_index, sizeof(EG_INT) * (egcredit->animation_index_size + 1));
	egcredit->animation_index_size++;
	egcredit->animation_index[egcredit->animation_index_size - 1] = index;
}
void egcredit_add_eganim2d(EG_CREDIT *egcredit, EG_ANIM2D *anim, EG_INT loop)
{
	_egcredit_reallocation(egcredit, egcredit->egcredit_size + 1);
	egobject_init_eganim2d(&egcredit->egcredit[egcredit->egcredit_size - 1], anim, loop);

	// Insre le numro d'index de cette animation pour la calculer quand on la veut
	_egcredit_add_animation_index(egcredit, egcredit->egcredit_size - 1);
}

//***********************************************************************
// Retourne le height du egcredit
//***********************************************************************
EG_INT egcredit_get_height(EG_CREDIT *egcredit)
{
	static EG_INT i;
	EG_INT height_pp = egcredit->true_height;

	for (i = 0; i < egcredit->animation_index_size; i++)
		height_pp += egobject_get_height(&egcredit->egcredit[egcredit->animation_index[i]]);

	return height_pp;  //Retourne le height
}

//***********************************************************************
// Ajoute un RLE_SPRITE dans le egcredit
//***********************************************************************
void egcredit_add_rle(EG_CREDIT *egcredit, RLE_SPRITE *rle)
{
	_egcredit_reallocation(egcredit, egcredit->egcredit_size + 1);
	egobject_init_rle(&egcredit->egcredit[egcredit->egcredit_size - 1], rle);
	egcredit->true_height += egobject_get_height(&egcredit->egcredit[egcredit->egcredit_size - 1]);
}

//***********************************************************************
// Ajoute un bitmap dans le egcredit
//***********************************************************************
void egcredit_add_bmp(EG_CREDIT *egcredit, BITMAP *bmp)
{
	_egcredit_reallocation(egcredit, egcredit->egcredit_size + 1);
	egobject_init_bmp(&egcredit->egcredit[egcredit->egcredit_size - 1], bmp);
	egcredit->true_height += egobject_get_height(&egcredit->egcredit[egcredit->egcredit_size - 1]);
}

//***********************************************************************
// Faire monter le egcredit (comme dans les films)
//***********************************************************************
EG_INT egcredit_moveup(EG_CREDIT *egcredit, BITMAP *buffer, EG_INT p)
{
	//si par exemple -3 il devient 3
	if (p < 0) p *= -1;

	//Si moin que 0 alors pas la peine d'y penser mon petit EG... BYE !!!!
	if (buffer->h + egcredit->position_y + egcredit_get_height(egcredit) - p < 0)
		return EG_CREDIT_END;

	egcredit->position_y -= p; //il fait monter le egcredit

	return EG_CREDIT_OK;
}

//***********************************************************************
// Retourne la position y
//***********************************************************************
EG_INT egcredit_gety(EG_CREDIT *egcredit, BITMAP *buffer)
{
	return buffer->h + egcredit->position_y;
}

//***********************************************************************
// Revenir au dbut
//***********************************************************************
void egcredit_rew(EG_CREDIT *egcredit)
{
	egcredit->position_y = 0;
}

//***********************************************************************
// Affiche et monte le egcredit
// retourne 1 si fin de egcredit
//***********************************************************************
EG_INT egcredit_moveup_draw(EG_CREDIT *egcredit, BITMAP *buffer, EG_INT p)
{
	static EG_INT i, fin_objet, centre;
	EG_INT w = buffer->w, h = buffer->h;  //W et H du buffer
	EG_INT true_y = h + egcredit->position_y; // La VRAI position y (egcredit_y+buffer_h)
	EG_INT egcredit_height = egcredit_get_height(egcredit);
	static EG_INT actual_object_height, actual_object_width;

	//si par exemple -5 elle devient 5
	if (p < 0) p *= -1;

	if (true_y + egcredit_height < 0) //si c'est la fin
		return EG_CREDIT_END;  //alors retourner la fin !!

	//Parcourt tous les egcredits
	for (i = 0; i < egcredit->egcredit_size; i++)
	{
		// La position de fin de cet objet
		actual_object_width = egobject_get_width(&egcredit->egcredit[i]);
		actual_object_height = egobject_get_height(&egcredit->egcredit[i]);

		// Y pos c'est la position actuelle de Y+HEIGHT (de l'objet actuel)
		fin_objet = true_y + actual_object_height - 1;

		//Si Y+height est moin que l'cran alors il SKIP
		if (fin_objet < 0)
		{
			//ajoute le y pour le prochain objet
			true_y += actual_object_height;
			continue;
		}

		//S'il le "Y" dpasse l'cran alors on a rien  faire ici
		if (true_y > h) break;

		//Calcule le centre
		centre = (w - actual_object_width) >> 1;

		//Affiche l'objet
		egobject_draw(&egcredit->egcredit[i], buffer, centre, true_y);

		// Incrmente le y actuel
		true_y += actual_object_height;  //ajoute le y pour le prochain objet
	}

	if (egcredit_gety(egcredit, buffer) + egcredit_height < 0) // Si c'est la fin
		return EG_CREDIT_END;  //alors retourner la fin !!

	else
		egcredit->position_y -= p;

	return EG_CREDIT_OK;
}

//***********************************************************************
// Afficher seulement
//***********************************************************************
EG_INT egcredit_draw(EG_CREDIT *egcredit, BITMAP *buffer)
{
	return egcredit_moveup_draw(egcredit, buffer, 0);
}

