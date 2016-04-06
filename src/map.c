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

static EG_SHORT egmap_realloc_line(EG_MAP *map);
static EG_SHORT egmap_realloc_map(EG_MAP *map, EG_SHORT width, EG_SHORT height);

/***************************************
 * Clear TO
 ***************************************/
void egmap_clear(EG_MAP *map)
{
	egmap_clearto(map, 0);
}
void egmap_clearto(EG_MAP *map, EG_SHORT val)
{
	memset(map->map, val, map->width * map->height * sizeof(map->map));
}

//*********************************
// Copie map dans dest
// et retourne poEG_SHORTeur de
// dest (variable dans le cas
// de désallocation)
//
// dest peut avoir 0 comme valeur
// et cela veut dire que c'est
// une allocation et non une
// réallocation.
//*********************************
EG_MAP *egmap_duplicate(EG_MAP *dest, EG_MAP *map)
{
	if (!dest)
	{
		dest = create_egmap(map->width, map->height);

		if (!dest) return 0;
	}

	else
	{
		if (egmap_realloc(dest, map->width, map->height))
		{
			if (dest->map) free(dest->map);

			if (dest->line) free(dest->line);

			free(dest);
			return 0;
		}
	}

	// Copie
	memcpy(dest->map, map->map, sizeof(map->map)*map->width * map->height);
	memcpy(dest->line, map->line, sizeof(map->line)*map->height);
	return dest;
}

//*********************************
// Modify Offset
//
// si err, return 0 et map->err
// contient TRUE. Sinon il
// contient FALSE.
//*********************************
void egmap_put(EG_MAP *map, EG_SHORT pgx, EG_SHORT pgy, EG_SHORT val)
{
	if (pgx < 0 || pgy < 0 || pgx >= map->width || pgy >= map->height)
	{
		map->err = TRUE;
		return;
	}

	else
		map->err = 0;

	map->line[pgy][pgx] = val;
}

//*********************************
// Get offset
//
// si err, return 0 et map->err
// contient TRUE. Sinon il
// contient FALSE.
//*********************************
EG_SHORT egmap_get(EG_MAP *map, EG_SHORT pgx, EG_SHORT pgy)
{
	if (pgx < 0 || pgy < 0 || pgx >= map->width || pgy >= map->height)
	{
		map->err = TRUE;
		return 0;
	}

	else
		map->err = 0;

	return map->line[pgy][pgx];
}

//*********************************
// Proc filef
//*********************************
EG_SHORT egmap_filef_proc(void *eg_filef_data, void *data)
{
	EG_MAP *map = data;
	EG_MAP save_map;

	// save
	if (FF_LOAD) memcpy(&save_map, data, sizeof(EG_MAP));

	// Width et height
	AUTOVAR(&map->width, sizeof(map->width));
	AUTOVAR(&map->height, sizeof(map->height));

	// resize MAP
	if (FF_LOAD)
	{
		if (egmap_malloc(map, map->width, map->height))
		{
			// erreur?
			// DONC remet les anciennes valeurs
			if (map->map)
			{
				EG_FREE(map->map);
				map->map = 0;
			}

			if (map->line)
			{
				EG_FREE(map->line);
				map->line = 0;
			}

			memcpy(data, &save_map, sizeof(EG_MAP));
			return FF_OUTMEM;
		}

		else   // on efface les anciennes valeurs
		{
			EG_FREE(save_map.map);
			EG_FREE(save_map.line);
		}
	}

	// Le map
	AUTOVAR(map->map, map->width * map->height * sizeof(map->map));

	return FF_OK;
}

//**************************************************************
// Like realloc mais quitte avec out of memory si erreur...
// (haut niveau)
//**************************************************************
void egmap_resize(EG_MAP *map, EG_SHORT width, EG_SHORT height)
{
	if (egmap_realloc(map, width, height))
	{
		eg_out_of_memory();
	}
}
// resize + clear
void egmap_cresize(EG_MAP *map, EG_SHORT width, EG_SHORT height)
{
	egmap_resize(map, width, height);
	egmap_clear(map);
}

//*******************************************
// realloc
//
// 1 si err
//
// Quand il fait err. Il peut laisser
// certains poEG_SHORTeur sans free (il faut
// le faire en dehor de cette fonction
// en testant si = 0)
//*******************************************
static EG_SHORT egmap_realloc_map(EG_MAP *map, EG_SHORT width, EG_SHORT height)
{
	EG_UINT old_sizeof_map = sizeof(map->map) * map->width * map->height;
	EG_UINT sizeof_map = sizeof(map->map) * width * height;
	EG_SHORT *save_map = 0; //si save_map=0 alors c'est une allocation
	EG_SHORT os = 0, od = 0;
	EG_SHORT x, y;

	// Sauvegarde
	if (map->map)
	{
		EG_MALLOC(save_map, old_sizeof_map);

		if (!save_map) return 1;

		memcpy(save_map, map->map, old_sizeof_map);
	}

	// Réalloque le map
	EG_REALLOC(map->map, sizeof_map);

	if (!map->map)
	{
		EG_FREE(save_map);
		return 1;
	}

	memset(map->map, 0, sizeof_map);

	// Conversion des valeurs
	if (save_map)  // si reallocation
	{


		for (x = 0; x < ((width < map->width) ? width : map->width); x++)
		{
			for (y = 0; y < ((height < map->height) ? height : map->height); y++)
			{
				os = x + (y * map->width);
				od = x + (y * width);
				map->map[od] = save_map[os];
			}
		}
	}

	map->width = width;
	map->height = height;

	EG_FREE(save_map);
	return 0;
}

//**********************************
// Reallocation de line selon
// width et height
// nonz si err
//**********************************
static EG_SHORT egmap_realloc_line(EG_MAP *map)
{
	EG_SHORT i;
	EG_SHORT n = 0;

	// Allocation des lignes
	EG_REALLOC(map->line, sizeof(EG_CHAR *)*map->height);

	if (!map->line) return 1;

	for (i = 0; i < map->height; i++)
	{
		map->line[i] = &map->map[n];
		n += map->width;
	}

	return 0;
}

//*************************************
// Réallocation d'une carte
//
// Les valeurs de map peuvent êtres
// modifiées ou mises à 0 en cas de
// outmem. si il sont à 0 alors
// realloc fait comme si c'était malloc.
//*************************************
EG_SHORT egmap_realloc(EG_MAP *map, EG_SHORT width, EG_SHORT height)
{
	EG_SHORT err;

	if (width <= 0)  width = 1;

	if (height <= 0) height = 1;

	if (!(err = egmap_realloc_map(map, width, height)))
		err = egmap_realloc_line(map);

	return err;
}

EG_SHORT egmap_malloc(EG_MAP *map, EG_SHORT width, EG_SHORT height)
{
	EG_UINT sizeof_map = width * height * sizeof(map->map);

	if (width <= 0)  width = 1;

	if (height <= 0) height = 1;

	// Allocation du map
	EG_MALLOC(map->map, sizeof_map);

	if (!map->map) return 1;

	memset(map->map, 0, sizeof_map);

	// vars
	map->width = width;
	map->height = height;

	// Allocation des collones
	map->line = 0;

	if (egmap_realloc_line(map))
	{
		EG_FREE(map->map);
		map->map = 0;
		return 1;
	}

	return 0;
}

//*********************************
// CREATE EX
//*********************************
EG_MAP *create_egmap(EG_SHORT width, EG_SHORT height)
{
	EG_MAP *map;

	// le map
	EG_MALLOC(map, sizeof(EG_MAP));

	if (!map) return 0;

	map->err = 0;

	// Allocation
	if (egmap_malloc(map, width, height))
	{
		if (map->map)
		{
			EG_FREE(map->map);
			map->map = 0;
		}

		if (map->line)
		{
			EG_FREE(map->line);
			map->line = 0;
		}

		EG_FREE(map);
		return 0;
	}

	return map;
}

//*********************************
// DESTROY
//*********************************
void destroy_egmap(EG_MAP *map)
{
	if (!map) return;

	if (map->line) EG_FREE(map->line);

	if (map->map) EG_FREE(map->map);

	EG_FREE(map);
}
