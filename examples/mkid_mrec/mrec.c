/*****************************************
 *
 * fonctions pour enregister
 * n milliseconde fois
 * le contenu de variables
 * qui peuvent êtres jouées
 * dans le même delay.
 *
 * Utile pour faire des replay
 * par exemple.
 *
 * Auteur: Asher256
 * Email:  asher256@gmail.com
 *
 * TODO: Ajouter gestion malloc
 * ne pas quitter s'il n'y a pas
 * assez de mémoire. au contraire
 * stopper l'enregistrement.
 *
 * ajouter frame ++ -- à l'envers et
 * en avant.
 *
 * save/load
 *****************************************/

#include <stdlib.h>
#include <string.h>
#include "mrec.h"

#define ERR_OUTMEM() {set_gfx_mode(GFX_TEXT,0,0,0,0);allegro_message("Out of memory...\n");exit(1);}

/******************************************
 * Frame ++ et --
 *
 * incrémenter la demo avec n millisecondes
 * (ou autre calcul de temps selon le
 * type de compteur enregistré dans le
 * programme).
 *
 * n peut être soit positif soit
 * négatif.
 *
 * retourne TRUE quand il est
 * arrivé au début ou à la fin.
 *
 * attention, cette fonction
 * stoppe dans la première
 * valeur après le counter de
 * premier change il est préférable
 * d'appeler rew après que cette
 * fonction ait retourné TRUE
 * (qui evut dire qu'elle a fini).
 ******************************************/
int mrec_counter_inc(MREC *rec, int n)
{
	char stop;

	//incrémente le compteur
	if(!rec->play && !rec->rec) {
		stop=TRUE;
		*rec->counter=rec->save_counter;
	}
	(*rec->counter)+=n;

	//exception
	if(n<0) {
		if(*rec->counter<0) {
			*rec->counter=0; //au cas ou rec->pos <=0
			//met le compteur de la position
			if(rec->pos>0) {
				//décrémente
				rec->pos--;

				//mets la nouvelle valeur
				mrec_change_to_var(rec);

				//met le compteur à la fin
				//du compteur de cette variable!
				*rec->counter=rec->change[rec->pos].counter;

				//mets le save counter
				if(stop) rec->save_counter=*rec->counter;
			}
			else {
				if(stop) rec->save_counter=*rec->counter;
				return TRUE;
			}
			//ici, ret FALSE bien sûre!
			//elle se fait en bas.
		}
	}
	else {  //normalement
		if(stop) rec->save_counter=*rec->counter;
		return mrec_exec(rec);
	}

	if(stop) rec->save_counter=*rec->counter;
	return FALSE; //sinon false!
}

/******************************************
 * REC
 *
 * commence l'enregistrement.
 ******************************************/
void mrec_rec(MREC *rec)
{
	mrec_make_initial_value(rec);
	rec->rec=TRUE;
	rec->play=FALSE;
	rec->save_counter=0;
}

/******************************************
 * PLAY
 ******************************************/
void mrec_play(MREC *rec)
{
	//si déjà play alors bye
	if(rec->play) return;
	rec->play=TRUE;
	rec->rec=FALSE;
	//mets le compteur comme il était
	*rec->counter=rec->save_counter;
}

/******************************************
 * STOP rec/play
 ******************************************/
void mrec_stop(MREC *rec)
{
	//si déjà stop alors pas d'autre stop
	if(!rec->play && !rec->rec) return;
	//mets les valeurs de stop, et sauve le compteur
	rec->rec=rec->play=FALSE;
	rec->save_counter=*rec->counter;
}

/******************************************
 * LOOP
 *
 * fonction qui se charge de tout
 * il suffit de la mettre dans la
 * boucle.
 *
 * au cas ou il est entrain de faire
 * play, il retourne TRUE si
 * fin de play.
 *
 * sinon, s'il est en rec il retourne
 * toujours FALSE car il n'as jamais fini
 * jusqu'à ce que l'on appelle stop.
 ******************************************/
int mrec_loop(MREC *rec)
{
	if(rec->rec)
		mrec_spy(rec);
	else if(rec->play)
		return mrec_exec(rec);
	return FALSE;
}

/******************************************
 * mrec_clear
 *
 * supprime tout ce qui a été
 * enregistré: en d'autres termes
 * enlever tout ce qui a été enregistré
 * dans mcredit_changed.
 *
 * fait automatiquement un rew
 ******************************************/
void mrec_clear(MREC *rec)
{
	free(rec->change);
	rec->change=0;
	rec->change_size=0;
	rec->pos=0;
}

/******************************************
 * inverse de mrec_init_initial_value
 ******************************************/
void mrec_make_initial_value(MREC *rec)
{
	int i;
//pas de reallocation car le size ne change pas!
	for(i=0; i<rec->var_size; i++) {
		memcpy(rec->var[i].initial_data,rec->var[i].ptr,rec->var[i].size);
	}
}

/******************************************
 * Mets les valeurs initiales qui
 * existaient au début, lors de
 * l'ajout des variables dans
 * MREC.
 ******************************************/
void mrec_init_initial_value(MREC *rec)
{
	int i;
	for(i=0; i<rec->var_size; i++)
		memcpy(rec->var[i].ptr,rec->var[i].initial_data,rec->var[i].size);
}

/******************************************
 * Mets valeur qui est dans
 * change[pos] dans var[i].
 ******************************************/
void mrec_change_to_var(MREC *rec)
{
	void *p1 = rec->var[rec->change[rec->pos].i].ptr;
	void *p2 = rec->change[rec->pos].data;
	memcpy(p1,p2,rec->var[rec->change[rec->pos].i].size);
}

/******************************************
 * PLAY
 *
 * Joue dans une boucle le contenu de
 * change[]
 *
 * stoppe quand il a dépasse ou égale
 * change_size.
 *
 * utiliser mrec_rew pour mettre la
 * position à zéro par rapport à
 * change_size
 *
 * TRUE si fini
 * FALSE si pas encore
 ******************************************/
int mrec_exec(MREC *rec)
{
	if(rec->pos>=rec->change_size) return TRUE;
	if(*rec->counter>=rec->change[rec->pos].counter) {
		mrec_change_to_var(rec); //mets la var actuelle
		rec->pos++; //va vers la prochaine position
	}
	return FALSE;
}

/******************************************
 * SPY
 *
 * espionne toutes les variables
 * et instrit celle qui as changée dans
 * change[].
 *
 * à mettre dans la boucle ou il y a
 * les keys par ex.
 ******************************************/
void mrec_spy(MREC *rec)
{
	static int i;
	for(i=0; i<rec->var_size; i++) {
		//si différents
		if(memcmp(rec->var[i].ptr,rec->var[i].data,rec->var[i].size)!=0)
			mrec_add_change(rec,i);
	}
}

/******************************************
 * REW
 ******************************************/
void mrec_rew(MREC *rec)
{
	rec->pos=0;
	//initial_data to data
	mrec_init_initial_value(rec);
}

/******************************************
 * Mets dans var.data le contenu de
 * ptr avec size.
 ******************************************/
void mrec_make_var_data(MREC *rec, int i)
{
	memcpy(rec->var[i].data,rec->var[i].ptr,rec->var[i].size);
}

/******************************************
 * ADD
 ******************************************/
int mrec_add_var(MREC *rec, void *ptr, int size)     //retourne l'index
{
	void *p = realloc(rec->var, (rec->var_size+1)*sizeof(MVAR));
	if(!p) ERR_OUTMEM();

	//oki, ici copie ce qui a été alloqué!
	rec->var=p;
	rec->var_size++;
	{
		int i=rec->var_size-1;

		//mets data
		rec->var[i].data=malloc(size);
		if(!rec->var[i].data) ERR_OUTMEM();
		rec->var[i].ptr=ptr;
		rec->var[i].size=size;

		//mets data en utilisant ptr et size
		rec->var_size--; //par mesure de précausion, on ne sait jamais il pourrait quitter avec un outmem
		mrec_make_var_data(rec,i);

		//création de la valeur initiale
		rec->var[i].initial_data=malloc(size);
		if(!rec->var[i].initial_data) ERR_OUTMEM();
		memcpy(rec->var[i].initial_data,ptr,size);

		rec->var_size++; //on remet la valeur
		return i;
	}
}

//fait le make_var_data automatiquement
//change le pos par la dernière position
int mrec_add_change(MREC *rec, int i)        //Ajouter un changement de variable dans la liste des changemente
{
	void *p = realloc(rec->change, (rec->change_size+1)*sizeof(MCHANGE));
	if(!p) ERR_OUTMEM();

	//oki, ici copie ce qui a été alloqué!
	{
		int ci=rec->change_size; //prends l'avant dernier élément
		int size=rec->var[i].size; //size en octet de la var pointée par i

		rec->change=p;

		if(!(rec->change[ci].data=malloc(size)))
			ERR_OUTMEM();

		//mets la nouvelle valeur dans change
		memcpy(rec->change[ci].data,rec->var[i].ptr,size);

		//mets le data
		mrec_make_var_data(rec,i);

		//mets les données dans la MCHANGE
		rec->change[ci].i = i;
		rec->change[ci].counter=*rec->counter;

		//si il n'y a aucune erreur, incrémenter le size!
		rec->change_size++;

		//mets le pos à la fin
		rec->pos = rec->change_size-1;
	}

	return i;
}

/******************************************
 * Create/destroy
 ******************************************/
MREC *create_mrec(volatile int *counter)
{
	MREC *rec = malloc(sizeof(MREC));
	memset(rec,0,sizeof(MREC));
	rec->counter=counter;
	return rec;
}

void destroy_mrec(MREC *rec)
{
	int i;
	if(!rec) return;
	for(i=0; i<rec->var_size; i++) {
		free(rec->var[i].initial_data);
		free(rec->var[i].data);
	}
	for(i=0; i<rec->change_size; i++)
		free(rec->change[i].data);
	free(rec);
}


