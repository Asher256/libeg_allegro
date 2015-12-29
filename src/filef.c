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

//***********************************************************************
// Constructeur
//***********************************************************************
EG_FILEF *create_egfilef(EG_INT (*proc_file_format)(void *eg_filef_data, void *data), EG_INT mode)
{
	EG_FILEF *temp;
	EG_MALLOC(temp, sizeof(EG_FILEF));

	if (!temp) return 0;

	// Il met le mode d'ouverture et de lecture
	temp->mode = FF_NORMAL;

	if (mode) temp->mode = FF_PACKED;

	temp->proc_file_format = proc_file_format;
	temp->filename = "";
	return temp;
}
void destroy_egfilef(EG_FILEF *egfilef)
{
	EG_FREE(egfilef);
}

//***********************************************************************
// Sauve la carte dans un fichier binaire
//
// Erreurs dans save et load:
//              0 Pas d'erreur
//              1 Disque plein ou protg en criture
//              2 Erreur dans l'ouverture du fichier
//***********************************************************************
EG_INT egfilef_save(EG_FILEF *egfilef, const EG_CHAR *filename, void *data)
{
	EG_INT ret;
	//Ouvre le fichier en criture
	EG_CHAR *ActualMode = F_WRITE;

	if (egfilef->mode == FF_PACKED) ActualMode = F_WRITE_PACKED;

	egfilef->file = pack_fopen(filename, ActualMode);

	if (!egfilef->file) return FF_ERROR_IN_OPEN;   //Si erreur dans l'ouverture du fichier

	//---------------- Dbut ---------------
	egfilef->load = FALSE;
	egfilef->save = TRUE;
	ret = (*egfilef->proc_file_format) (egfilef, data);
	//--------------------------------------

	egfilef->save = FALSE;

	pack_fclose(egfilef->file);
	return ret;
}

//***********************************************************************
// Charge la carte dans un fichier binaire selon file_format();
// Valeur de retour:
//              3 Le fichier n'existe pas
//              4 Format de Fichier incorrect
//              5 Le fichier est dfectueux
//***********************************************************************
EG_INT egfilef_load(EG_FILEF *egfilef, const EG_CHAR *filename, void *data)
{
	EG_INT ret;
	EG_CHAR *ActualMode = F_READ;

	if (!exists(filename)) return FF_NOT_EXIST; //le fichier n'existe pas

	//Charge l'option
	if (egfilef->mode) ActualMode = F_READ_PACKED;

	//Ouvre le fichier
	egfilef->file = pack_fopen(filename, ActualMode);

	if (!egfilef->file) return FF_ERROR_IN_OPEN;    //erreur dans ouverture du fichier

	//---------------- Dbut ---------------
	egfilef->load = TRUE;
	egfilef->save = FALSE;
	ret = (*egfilef->proc_file_format) (egfilef, data);
	//--------------------------------------

	egfilef->load = FALSE;
	pack_fclose(egfilef->file);
	return ret;
}

//***********************************************************************
//Pour la macro HEADER() de file_format
//***********************************************************************
EG_INT egfilef_header_proc(EG_FILEF *egfilef, const EG_CHAR *str)
{
	EG_INT ret;
	long len = ustrlen(str);
	EG_CHAR *header;

	if (egfilef->load)
	{
		EG_MALLOC(header, len + 1);

		if (!header) return FF_OUTMEM;

		header[len] = 0;   //len car il doit tre un string zero

		ret = pack_fread(header, len, egfilef->file);

		if (ret < len)
		{
			EG_FREE(header);
			return FF_ERROR_IN_FILE;    //Fichier dfectueux
		}

		if (strcmp(header, str)) //Si header != alors quitter
		{
			EG_FREE(header);
			return FF_INCORRECT_FORMAT;    //Format de fichier incorrect
		}
	}

	else if (egfilef->save)
	{
		//crit le header
		ret = pack_fwrite(str, len, egfilef->file);

		if (ret < len)
			return FF_DISK_PROTECTED;
	}

	return FF_OK;
}

//***********************************************************************
//Pour la macro AUTOVAR()
//***********************************************************************
EG_INT egfilef_autovar_proc(EG_FILEF *egfilef, void *pvar, EG_INT size)
{
	EG_INT ret;

	if (egfilef->load)
	{
		ret = pack_fread(pvar, size, egfilef->file);

		if (ret < size)
			return FF_ERROR_IN_FILE;
	}

	else if (egfilef->save)
	{
		ret = pack_fwrite(pvar, size, egfilef->file);

		if (ret < size)
			return FF_DISK_PROTECTED;
	}

	return 0;
}
