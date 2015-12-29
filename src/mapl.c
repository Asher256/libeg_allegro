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

/*********************************
 * duplication
 ********************************/
EG_MAPL *egmapl_duplicate(EG_MAPL *map)
{
	EG_SHORT i;
	EG_MAPL *dest = create_egmapl(map->sizeof_layer, map->width, map->height);

	if (!dest) return 0;

	for (i = 0; i < map->sizeof_layer; i++)
	{
		dest->layer[i] = egmap_duplicate(dest->layer[i], map->layer[i]);

		if (!dest->layer[i])
		{
			destroy_egmapl(dest);
			return 0;
		}
	}

	return dest;
}

/*********************************
 * egmapl_filef_proc
 *********************************/
EG_SHORT egmapl_filef_proc(void *eg_filef_data, void *data)
{
	EG_SHORT i, n;
	EG_MAPL *lmap = data;

	for (i = 0; i < lmap->sizeof_layer; i++)
	{
		if ((n = egmap_filef_proc(eg_filef_data, data)))
			return n;
	}

	return FF_OK;
}

/*********************************
 * egmapl_clear[to]
 *********************************/
void egmapl_clearto(EG_MAPL *mapl, EG_SHORT val)
{
	EG_SHORT i;

	for (i = 0; i < mapl->sizeof_layer; i++)
		egmap_clearto(mapl->layer[i], val);
}
void egmapl_clear(EG_MAPL *mapl)
{
	egmapl_clearto(mapl, 0);
}

/*********************************
 * egmapl_put
 *********************************/
void egmapl_put(EG_MAPL *mapl, EG_SHORT layer, EG_SHORT pgx, EG_SHORT pgy, EG_SHORT val)
{
	if (layer < 0 || layer >= mapl->sizeof_layer)
	{
		mapl->err = TRUE;
		return;
	}

	egmap_put(mapl->layer[layer], pgx, pgy, val);
	mapl->err = mapl->layer[layer]->err;
}

/*********************************
 * egmapl_get
 *********************************/
EG_SHORT egmapl_get(EG_MAPL *mapl, EG_SHORT layer, EG_SHORT pgx, EG_SHORT pgy)
{
	EG_SHORT n;

	if (layer < 0 || layer >= mapl->sizeof_layer)
	{
		mapl->err = TRUE;
		return 0;
	}

	n = egmap_get(mapl->layer[layer], pgx, pgy);
	mapl->err = mapl->layer[layer]->err;
	return n;
}

/*********************************
 * resize
 ********************************/
void egmapl_cresize(EG_MAPL *mapl, EG_SHORT w, EG_SHORT h)
{
	egmapl_resize(mapl, w, h);
	egmapl_clear(mapl);
}
void egmapl_resize(EG_MAPL *mapl, EG_SHORT w, EG_SHORT h)
{
	EG_SHORT i;

	for (i = 0; i < mapl->sizeof_layer; i++)
	{
		egmap_resize(mapl->layer[i], w, h);
	}
}

/*********************************
 * CREATE/DESTROY
 *********************************/
EG_MAPL *create_egmapl(EG_SHORT max_layer, EG_SHORT width, EG_SHORT height)
{
	EG_MAPL *mapl;
	EG_SHORT i;

	// Allocation
	EG_MALLOC(mapl, sizeof(EG_MAPL));

	if (!mapl) return 0;

	// Max layer
	mapl->sizeof_layer = max_layer;
	mapl->width  = width;
	mapl->height = height;

	EG_MALLOC(mapl->layer, sizeof(EG_MAP *)*max_layer);

	if (!mapl->layer) return 0;

	// Allocation de tout les maps
	for (i = 0; i < max_layer; i++)
	{
		mapl->layer[i] = create_egmap(width, height);

		// erreur?
		if (!mapl->layer[i])
		{
			while (i-- != 0)
				destroy_egmap(mapl->layer[i]);

			EG_FREE(mapl->layer);
			EG_FREE(mapl);
			return 0;
		}
	}

	return mapl;
}

// DESTROY egmapl
void destroy_egmapl(EG_MAPL *mapl)
{
	EG_SHORT i;

	if (!mapl) return;

	for (i = 0; i < mapl->sizeof_layer; i++)
	{
		if (mapl->layer[i])
			destroy_egmap(mapl->layer[i]);
	}

	if (mapl->layer) EG_FREE(mapl->layer);

	EG_FREE(mapl);
}
