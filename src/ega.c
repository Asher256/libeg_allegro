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
#if 0
// Les fichiers statiques
static EG_ANIM2D *actual_eganim2d;   // Animation actuelle
static RGB *actual_pal;         // La palette actuelle
static EG_INT depth;               // LOAD: Le depth du fichier - SAVE: le depth  sauver
static EG_INT screen_depth;        // Le depth actuel du screen
static BITMAP *buffer = 0;
static EG_INT max_frame;

// Header d'une frame
static struct frame_header
{
	EG_INT w, h, delay;
} frame_header;

static EG_INT save_frame(EG_INT index);
static EG_INT load_frame(void);
static EG_INT saveload_8bit_one_bitmap(void);
static EG_INT saveload_hicolor_one_bitmap(void);
static EG_INT file_format(void);

// Chargement d'une seule frame
static EG_INT load_frame(void)
{
	EG_INT index;
	EG_INT ret;
	BITMAP *buffer2;

	// Charge le header de cette frame
	AUTOVAR(&frame_header, sizeof(frame_header));

	// Un buffer qui se EG_CHARge selon le depth du screen
	buffer = create_bitmap_ex(depth, frame_header.w, frame_header.h);

	if (!buffer) return FF_OUTMEM;

	// Echanger buffer avec buffer2
	buffer2 = create_bitmap_ex(screen_depth, frame_header.w, frame_header.h);

	if (!buffer2) return FF_OUTMEM;

	switch (depth)
	{
	case 8:
		if (ret = saveload_8bit_one_bitmap()) return ret;

		break;

	case 15:
	case 16:
		if (ret = saveload_hicolor_one_bitmap()) return ret;

		break;

	default:
		return EGA_UNSUPPORTED_FORMAT;
	}

	// Conversion
	if (screen_depth == 8)
	{
		select_palette(actual_pal);
	}

	// Change
	blit(buffer, buffer2, 0, 0, 0, 0, buffer->w, buffer->h);

	if (screen_depth == 8) unselect_palette();

	// Ajoute cette frame
	index = eganim2d_add_bmp(actual_eganim2d, buffer2, frame_header.delay);
	// Met allocated  true pour que  destroy_eganim2d efface ceci automatiquement
	actual_eganim2d->frame[index].allocated = TRUE;

	return FF_OK;
}

// Charge un 8 bit dans bitmap
static EG_INT saveload_8bit_one_bitmap(void)
{
	EG_INT x, y, d = 0;
	EG_CHAR *image;
	EG_INT lecture = buffer->w * buffer->h;
	EG_INT ret;
	EG_CHAR pixel;

	// Un mini buffer
	EG_MALLOC(image, lecture);

	if (!image) return FF_OUTMEM;

	// Charge l'image dans la tableau image
	if (FF_SAVE)
	{
		for (x = 0; x < buffer->w; x++)
		{
			for (y = 0; y < buffer->h; y++)
			{
				pixel = (EG_CHAR)getpixel(buffer, x, y);
				image[d++] = pixel;
			}
		}
	}

	// Sauve/Charge toute l'image depuis/dans un buffer
	AUTOVAR(image, lecture);

	// Met tous les pixels dans le buffer
	d = 0;

	if (FF_LOAD)
	{
		for (x = 0; x < buffer->w; x++)
		{
			for (y = 0; y < buffer->h; y++)
			{
				putpixel(buffer, x , y, image[d++]);
			}
		}
	}

	return FF_OK;
}

// Sauve et Charge un hi/true color ou hi color 15,16
static EG_INT saveload_hicolor_one_bitmap(void)
{
	EG_INT x, y, d = 0;
	EG_SHORT *image;
	EG_INT lecture = sizeof(EG_SHORT) * buffer->w * buffer->h;
	EG_INT ret;
	EG_SHORT pixel;

	// Un mini buffer
	EG_MALLOC(image, lecture);

	if (!image) return FF_OUTMEM;

	// Charge l'image dans la tableau image
	if (FF_SAVE)
	{
		for (x = 0; x < buffer->w; x++)
		{
			for (y = 0; y < buffer->h; y++)
			{
				pixel = (EG_SHORT)getpixel(buffer, x, y);
				image[d++] = pixel;
			}
		}
	}

	// Sauve/Charge toute l'image depuis/dans un buffer
	AUTOVAR(image, lecture);

	// Met tous les pixels dans le buffer
	d = 0;

	if (FF_LOAD)
	{
		for (x = 0; x < buffer->w; x++)
		{
			for (y = 0; y < buffer->h; y++)
			{
				putpixel(buffer, x , y, (EG_SHORT)image[d++]);
			}
		}
	}

	return FF_OK;
}

// Sauve la frame
static EG_INT save_frame(EG_INT index)
{
	EG_INT ret;
	EG_INT x, y;
	void *frame = actual_eganim2d->frame[index].data;
	EG_INT type = actual_eganim2d->frame[index].type;
	BITMAP *buffer2;

	if (type)  //Si RLE
	{
		// Le bitmap ou il y a la frame de l'animation
		buffer2 = create_bitmap_ex(screen_depth, ((RLE_SPRITE *)frame)->w, ((RLE_SPRITE *)frame)->h);

		if (!buffer2) return FF_OUTMEM;

		clear_bitmap(buffer2);
		draw_rle_sprite(buffer2, (RLE_SPRITE *)frame, 0, 0);
	}

	else
	{
		buffer2 = frame;
	}

	// Convertissement au color depth actuel
	buffer = create_bitmap_ex(depth, buffer2->w, buffer2->h);

	if (!buffer) return FF_OUTMEM;

	blit(buffer2, buffer, 0, 0, 0, 0, buffer2->w, buffer2->h);

	// crit avant tout le header d'une frame
	frame_header.w = buffer->w;
	frame_header.h = buffer->h;
	frame_header.delay = actual_eganim2d->frame[index].msec;
	AUTOVAR(&frame_header, sizeof(frame_header));

	switch (depth)
	{
	case 8:
		if (ret = saveload_8bit_one_bitmap()) return ret;

		break;

	case 15:
	case 16:
		if (ret = saveload_hicolor_one_bitmap()) return ret;

		break;

	default:
		return EGA_UNSUPPORTED_FORMAT;
	}

	// Desallocation si on a fait la copie
	if (type) destroy_bitmap(buffer2);

	destroy_bitmap(buffer);
}

// Le format de fichier
static EG_INT file_format(void)
{
	EG_INT ret, i;

	// Met le screen depth
	screen_depth = bitmap_color_depth(screen);

	if (screen_depth > 8) get_palette(actual_pal);

	// Le header EGA
	HEADER("EGA")

	// Sauve/Charge le maximum de frames
	if (FF_SAVE) max_frame = actual_eganim2d->max_frame;

	AUTOVAR(&max_frame, sizeof(max_frame));

	// Mettre le bon depth
	if (FF_SAVE) depth = screen_depth;

	// Charge le maximum de depth
	AUTOVAR(&depth, sizeof(depth));

	// Si palette 256 couleurs
	if (depth == 8)
	{
		for (i = 0; i < 256; i++)
		{
			AUTOVAR(&actual_pal[i].r, 1); //Charge la palette avant tout
			AUTOVAR(&actual_pal[i].g, 1); //Charge la palette avant tout
			AUTOVAR(&actual_pal[i].b, 1); //Charge la palette avant tout
		}
	}

	// Pour un peu plus de reconaissance, un autre header
	HEADER("EGANIM");

	// Si color depth mauvais
	if (FF_LOAD && depth != 8 && depth != 15 && depth != 16 && depth != 24 && depth != 32) return FF_INCORRECT_FORMAT;

	// Charge|Sauve toutes les frames
	for (i = 0; i < actual_eganim2d->max_frame; i++)
	{
		// Si EG_CHARgement
		if (FF_LOAD)
		{
			if (ret = load_frame())
				return ret;
		}

		//*** Si sauvegarde
		else if (FF_SAVE)
		{
			if (ret = save_frame(i))
				return ret;
		}
	}

	if (FF_LOAD) actual_eganim2d->max_frame = max_frame;

	return FF_OK;
}

// Non zero si erreur de EG_CHARgement...
EG_INT eg_load_ega(const EG_CHAR *filename, EG_ANIM2D *eganim2d, PALETTE pal)
{
	EG_FILEF *egfilef;
	EG_INT ret = FF_OK;

	// Efface toute l'animation
	eganim2d_clear(eganim2d);

	// Le format de fichier avec pas de compression
	egfilef = create_egfilef(file_format, FF_PACKED);

	if (!egfilef) return FF_OUTMEM;

	// Met l'anim par dfaut
	actual_eganim2d = eganim2d;
	actual_pal = pal;

	// Charge le fichier
	ret = egfilef_load(egfilef, filename);

	// Destruction du format de fichiers
	destroy_egfilef(egfilef);
	return ret;
}

// Sauver une animation
EG_INT eg_save_ega(const EG_CHAR *filename, EG_ANIM2D *eganim2d, PALETTE pal)
{
	EG_INT ret;
	EG_FILEF *egfilef = create_egfilef(file_format, FF_PACKED);

	if (!egfilef) return FF_OUTMEM;

	// Met l'anim et la pal par dfaut
	actual_eganim2d = eganim2d;
	actual_pal = pal;

	ret = egfilef_save(egfilef, filename);

	destroy_egfilef(egfilef);
	return ret;
}
#endif
