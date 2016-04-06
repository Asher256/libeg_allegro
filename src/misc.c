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

// INC pgx,pgy...
void eg_inc(EG_SHORT *pg, EG_SHORT *sc, EG_SHORT size, EG_SHORT n)
{
	if (n < 0)
		eg_sc_dec(pg, sc, size, -n);

	else
		eg_sc_inc(pg, sc, size, n);
}

// inc sc[xy] pg[xy]
void eg_sc_inc(EG_SHORT *pg, EG_SHORT *sc, EG_SHORT size, EG_SHORT add)
{
	*sc += add;

	if (*sc >= size)
	{
		*sc %= size;
		*pg += 1;
	}
}
void eg_sc_dec(EG_SHORT *pg, EG_SHORT *sc, EG_SHORT size, EG_SHORT dec)
{
	*sc -= dec;

	if (*sc < 0)
	{
		*sc = size + *sc;
		*pg -= 1;
	}
}


