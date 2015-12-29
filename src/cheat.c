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
// Change le timeout
//***********************************************************************
void egcheat_set_timeout(EG_CHEAT *egcheat, EG_INT timeout)
{
	egcheat->timeout = timeout;
}
//***********************************************************************
// Retourne le timeout
//***********************************************************************
EG_INT egcheat_get_timeout(EG_CHEAT *egcheat)
{
	return egcheat->timeout;
}

//***********************************************************************
// Création d'un nouveau Cheat
//
// dans init_egcheat le dernier élément de code doit contenue des NULL
// like allegro gui.
//***********************************************************************
void init_egcheat(EG_CHEAT *cheat, EG_CHEAT_CODE *code, volatile EG_INT *counter)
{
	cheat->code = code;
	cheat->actual_char = 0;
	cheat->actual_egcheat = -1;
	cheat->timeout = EG_CHEAT_DEFAULT_TIMEOUT;
	cheat->counter = counter;

	// calcul sizeof code
	cheat->sizeof_code = 0;

	while (code->code)
	{
		cheat->sizeof_code++;
		code++;
	}
}
EG_CHEAT *create_egcheat(volatile EG_INT *counter)
{
	EG_CHEAT *temp;
	EG_MALLOC(temp, sizeof(EG_CHEAT));

	if (!temp) return 0;

	EG_MALLOC(temp->code, sizeof(EG_CHEAT_CODE));

	if (!temp->code)
	{
		EG_FREE(temp);
		return 0;  //Erreur dans l'instanciation
	}

	temp->sizeof_code = 0;
	temp->actual_char = 0;
	temp->actual_egcheat = -1;
	temp->timeout = EG_CHEAT_DEFAULT_TIMEOUT;
	temp->counter = counter;

	return temp;
}

//***********************************************************************
// Destruction d'un egcheat
//***********************************************************************
void destroy_egcheat(EG_CHEAT *egcheat)
{
	if (!egcheat) return;

	if (egcheat->code) EG_FREE(egcheat->code);

	if (egcheat) EG_FREE(egcheat);
}

//***********************************************************************
// ENABLED
//***********************************************************************
EG_INT egcheat_enabled(EG_CHEAT *cheat, EG_INT index)
{
	if (index < 0 || index >= cheat->sizeof_code) return FALSE;

	return cheat->code[index].enabled;
}
void egcheat_set_enabled(EG_CHEAT *cheat, EG_INT index, EG_INT enabled)
{
	if (index < 0 || index >= cheat->sizeof_code) return;

	cheat->code[index].enabled = enabled;
}

//***********************************************************************
// Modification d'un egcheat
//***********************************************************************
void egcheat_modify(EG_CHEAT *egcheat, EG_INT index, EG_UCHAR *code, EG_INT len)
{
	if (index >= 0 && index < egcheat->sizeof_code)
	{
		egcheat->code[index].code   = code;
		egcheat->code[index].len    = len;
	}
}

//***********************************************************************
// Ajoute un egcheat dans une liste de egcheat's
//
// ATTENTION seulement avec un egcheat créé avec create_egcheat
//***********************************************************************
EG_INT egcheat_add(EG_CHEAT *egcheat, EG_UCHAR *code, EG_INT len)
{
	egcheat->sizeof_code++;

	EG_REALLOC(egcheat->code, egcheat->sizeof_code * sizeof(EG_CHEAT_CODE));

	if (!egcheat->code)
	{
		eg_out_of_memory();
	}

	egcheat_modify(egcheat, egcheat->sizeof_code - 1, code, len);
	egcheat->code[egcheat->sizeof_code - 1].enabled = TRUE;

	return egcheat->sizeof_code - 1;
}

//***********************************************************************
// Routine principale, Teste si un egcheat tap au clavier
//***********************************************************************
EG_INT egcheat_test(EG_CHEAT *egcheat)
{
	static EG_INT xkey, i, j;

	//si pas de touche en cours
	if (egcheat->actual_egcheat == -1)
	{
		if (keypressed())
		{
			xkey = readkey() >> 8;
		}

		else
			return EG_CHEAT_NONE;  //pas de key

		//cherche dans tout les egcheat
		for (i = 0; i < egcheat->sizeof_code; i++)
		{

			if (!egcheat->code[i].enabled || egcheat->code[i].len <= 0)
				continue;      //si len du code<=0 alors pas la peine de l'essayer

			//s'il appuye sur la premire touche du code
			if (xkey == egcheat->code[i].code[0])
			{
				egcheat->actual_egcheat = i;
				egcheat->actual_char = 1;
				*egcheat->counter = 0;
				return EG_CHEAT_WRITE;
			}
		}

		//si aucun key alors faire en sorte qu'il n'a pas appuyé
		//sur ce dernier

		return EG_CHEAT_NONE;      //de toute faon la prochaine boucle il reconnaitra le cde
	}

	//sinon s'il travaille déjà sur un cde
	else
	{
		//si le code est OK
		if (egcheat->actual_char >= egcheat->code[egcheat->actual_egcheat].len)
		{
			i = egcheat->actual_egcheat;
			egcheat->actual_egcheat = -1;
			//  egcheat->actual_char = 0;
			return i;
		}

		//si le temps est fini alors STOP Cheat
		if (*egcheat->counter >= egcheat->timeout)
		{
			egcheat->actual_egcheat = -1;
			return EG_CHEAT_NONE;
		}      //endcountermsec

		else     //sinon s'il est encore temps alors il prend le keyd
		{
			//############# DETECTION SI KEY APPUYE #################
			//detecte s'il appuye sur un key
			if (keypressed())
			{
				xkey = readkey() >> 8; //Prend un key

				if (xkey == egcheat->code[egcheat->actual_egcheat].code[egcheat->actual_char]) //si le key
				{
					*egcheat->counter = 0;                //le timer  0
					egcheat->actual_char++;
					return EG_CHEAT_WRITE;
				}

				else
				{
					//############ SI ERREUR DE CODE ALORS CHERCHE UN AUTRE Cheat DS LA TABLE ########
					//sinon ici s'il n'appuye pas sur le bon key
					//alors il cherche s'il y a une clef qui est
					//similaire  l'actuelle de 0  caractaire actuel
					//et teste
					for (i = 0; i < egcheat->sizeof_code; i++) //cherche s'il n'y a pas identique
					{
						if (!egcheat->code[i].enabled && i == egcheat->actual_egcheat)
							continue;

						//si le lenght ne dpasse pas le cde choisis par i
						if (egcheat->actual_char + 1 <= egcheat->code[i].len)
						{
							//Teste ce string s'il est comparable  ce dernier
							//l'avant ce lenght
							for (j = 0; j <= egcheat->actual_char; j++)
							{
								//si != alors quitter le egcheat actuel

								//S'il a trouv le bon caractaire
								if (j == egcheat->actual_char)   //si c'est le carac actuel
								{
									if (xkey == egcheat->code[i].code[j]) //s'il trouve le bon caractaire
									{
										//mettre ce egcheat par dfaut
										egcheat->actual_egcheat = i;    //le mettre avec i
										egcheat->actual_char++;

										return EG_CHEAT_WRITE;
									}
								}

								else      //sinon (moin que caractaire actuel)
								{

									if (egcheat->code[i].code[j] != egcheat->code[egcheat->actual_egcheat].code[j])
										break;  //quitter le test
								}

							}
						}
					} //endfor

					egcheat->actual_egcheat = -1;   //si pas de remplacement
					return EG_CHEAT_NONE;              //alors fin de egcheat

				}//end else

			}//end if (keypressed()

			return EG_CHEAT_WRITE;
		}//end else
	}//end else2

	return EG_CHEAT_NONE;
}

