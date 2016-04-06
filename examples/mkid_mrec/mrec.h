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
 *****************************************/

#ifndef _MREC_H_
#define _MREC_H_

#ifndef ALLEGRO_H
#include <allegro.h>
#endif

//enregistrer les variables
//selon leur index
typedef struct {
	void *ptr;  //ptr vers donnée
	int size;   //size
	void *data; //le data initial (utilisé pour savoir si ptr a changé)
	void *initial_data; //valeur initiale, initialisée lors de var_add
} MVAR; //une variable

//pour enregistrer les changements
typedef struct {
	int counter; //nombre de millisecondes à attendre
		     //pour changer 'I'ndex
	int i;       //index de la var à changer
	void *data;  //données à y mettre qui contiennent var[i].size octets
} MCHANGE;

//struct qui regroupe le tout
typedef struct {
	volatile int *counter; //le compteur en millisecondes
	int save_counter;      //sauvegarde du compteur lors d'un stop

	MVAR *var;
	int var_size;  //nombre d'éléments dans **var
	
	//liste d'ordres de changement
	MCHANGE *change;
	int change_size;

	//pour le playing
	int pos;  //position dans change_size

	//state s'il est entrain de play/rec
	int rec, play;
	int pause;
} MREC;

//fonction haut niveau
int mrec_loop(MREC *rec);
void mrec_rew(MREC *rec);   //rebombiner jusqu'au début
void mrec_clear(MREC *rec); //supprime tout ce qui a été enregistré
void mrec_rec(MREC *rec);
int mrec_counter_inc(MREC *rec, int n);

//fonction création
MREC *create_mrec(volatile int *counter); //compteur millisecondes
void destroy_mrec(MREC *mrec);

//fonctions utiles
void mrec_make_var_data(MREC *rec, int i);
	
//fonctions bas niveau
void mrec_init_initial_value(MREC *rec); //mets les valeurs qui sont dans initial_value dans ptr pour avoir les premières valeurs
void mrec_make_initial_value(MREC *rec); //l'inverse de init_initial_value
int mrec_add_var(MREC *rec, void *ptr, int size); //retourne l'index
int mrec_add_change(MREC *rec, int i);       //Ajouter un changement de variable dans la liste des changemente
void mrec_spy(MREC *rec);   //fonction qui teste si qq chose a changé et l'enregistre
int  mrec_exec(MREC *rec);  //fonction qui joue à mettre dans une boucle
void mrec_change_to_var(MREC *rec); //mets la valeur qui es dans change[pos] dans var[i]

#endif

