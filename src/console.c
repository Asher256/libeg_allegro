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

#define RGS_VALIDATION 1
#define RGS_FINCHAINE  2
#define RGS_CHANGEMENT 3
#define OGS_CURSEURSTATIQUE 1
static EG_CHAR al_getstr_traitement(EG_CHAR *ChaineDestination, EG_SHORT Maximum, EG_SHORT *Curseur, EG_SHORT option, EG_CHAR (*test_ajout)(EG_CHAR c));

/**************************************************************
* Lire une chaine de caracteres au clavier (sans afficher)
* dans une variable.
*
* (sous programme à mettre dans une boucle)
* (CETTE FONCTION APPELLE poll_keyboard AUTOMATIQUEMENT)
*
* Arguments:
* ===========
*    ChaineDestination      Comme son nom l'indique
*    Maximum                Le maximum de caractère dans la chaine
*    Curseur                La position du curseur dans la chaine
*                           mettre -1 dans le début.
*    option                 Options (voir plus en bas)
*    EG_CHAR test_ajout(EG_CHAR c) NULL pour ne pas utiliser cette fonction.
*                           al_getstr appelle cette fonction à chaque
*                           qu'elle ajoute un caractère dans la chaine
*                           afin de tester si celui ci est valide.
*                           S'il est valide, la fonction doit retourner c
*                           et 'c' sera ajouté dans la chaine.
*                           Sinon, elle doit retourne 0.
*
* Retourne:
* ==========
*     RGS_VALIDATION        Si l'utilisateur appuie sur entrée.
*     RGS_FINCHAINE         Si l'utilisateur ajoute un caractère alors que
*                           la chaine est pleine (len=maximum).
*     RGS_CHANGEMENT        Si l'utilisateur Ajoute/Supprime un caractère
*                           de la chaine.
*
* Options:
* ========
*     OGS_CURSEURSTATIQUE   Interdit la modification de la position du
*                           Curseur manuellement avec droite et gauche.
*
**************************************************************/
static EG_CHAR al_getstr_traitement( EG_CHAR *ChaineDestination, EG_SHORT Maximum, EG_SHORT *Curseur, EG_SHORT option, EG_CHAR (*test_ajout)(EG_CHAR c))
{
	EG_INT CodeAscii;
	EG_INT ScanCode;
	EG_INT len;

	poll_keyboard();

	if (!ChaineDestination) return 0;

	if (!keypressed()) return 0;

	//Lecture du clavier: ASCII + SCANCODE
	CodeAscii = readkey();
	ScanCode = CodeAscii >> 8;
	CodeAscii &= 0xFF;

	// Vars diverses
	len = ustrsize(ChaineDestination);

	// Si curseur = -1 alors mettre le curseur à la fin de la chaine
	if (*Curseur == -1)
		*Curseur = len;

	// Les SCANCODES AVANT TOUT !!
	if (ScanCode == KEY_ENTER || CodeAscii == 13)
		return RGS_VALIDATION;

	// BACKSPACE <---
	else if (ScanCode == KEY_BACKSPACE)
	{
		// Pour ne pas enregistrer backspace comme ascii
		// je met cette condition à l'EG_INTérieur (pour
		// le return).
		if (len > 0)
		{
			uremove(ChaineDestination, --*Curseur);
			return RGS_CHANGEMENT;
		}

		return 0;
	}

	// LEFT
	else if (ScanCode == KEY_LEFT)
	{
		if (*Curseur > 0 && !(option & OGS_CURSEURSTATIQUE))
			*Curseur -= 1;

		return 0;
	}

	// RIGHT
	else if (ScanCode == KEY_RIGHT)
	{
		if (*Curseur < len && !(option & OGS_CURSEURSTATIQUE))
			*Curseur += 1;

		return 0;
	}

	// S'il a tapé une touche
	else if (CodeAscii > 13 && len < Maximum)
	{
		if (test_ajout)
		{
			CodeAscii = (*test_ajout)(CodeAscii);
		}

		if (CodeAscii != 0)
		{
			uinsert(ChaineDestination, *Curseur, (EG_CHAR)CodeAscii);
			*Curseur += 1;
		}

		return RGS_CHANGEMENT;
	}

	return 0;
}

//*************************************************************
// EG_INTernal draw
//*************************************************************
static void EG_INTernal_drawcursor(BITMAP *bmp, EG_RECT *point, EG_INT color)
{
	line(bmp, point->x, point->y, point->x, point->y + point->h - 1, color);
}

//*************************************************************
// Change le font
//*************************************************************
void egconsole_setfont(EG_CONSOLE *egconsole, FONT *f)
{
	EG_INT newsize, i;

	egconsole->font = f;

	// reallocation
	newsize = egconsole->h / text_height(f);
	EG_REALLOC(egconsole->lh, sizeof(EG_CHAR **) * newsize);

	if (!egconsole->lh)
	{
		eg_out_of_memory();
	}

	// met à 0 tout new str
	for (i = egconsole->sizeof_lh; i < newsize; i++)
		egconsole->lh[i] = 0;
}

//*************************************************************
// Création d'une EG-Console
//*************************************************************
EG_CONSOLE *create_egconsole(EG_INT w, EG_INT h, EG_INT color, void (*draw)(BITMAP *bmp, EG_RECT *point))
{
	EG_CONSOLE *console;

	// Allocation
	if (!(EG_MALLOC(console, sizeof(EG_CONSOLE)))) return 0;

	// Lines history
	console->lh = 0;
	console->sizeof_lh = 0;
	egconsole_setfont(console, font);

	// Actual CMD
	console->actual_cmd = 0;
	console->cmd_curpos = -1;

	if (!(EG_MALLOC(console->actual_cmd, 256 + 1)))
	{
		EG_FREE(console);
		return 0;
	}

	// Affichage
	console->draw_bg = draw;
	console->draw_cursor = EG_INTernal_drawcursor;

	// W et H
	console->w = w;
	console->h = h;

	return console;
}

//*************************************************************
// Destruction
//*************************************************************
void destroy_egconsole(EG_CONSOLE *egconsole)
{
	EG_INT i;

	if (!egconsole) return;

	if (egconsole->lh)
	{
		// del tt ls lh
		for (i = 0; i < egconsole->sizeof_lh; i++)
		{
			if (egconsole->lh[i])
			{
				EG_FREE(egconsole->lh[i]);
			}
		}

		EG_FREE(egconsole->lh);
	}

	if (egconsole->actual_cmd)
		EG_FREE(egconsole->actual_cmd);

	EG_FREE(egconsole);
}

