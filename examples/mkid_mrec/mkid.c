/* Une démo d'EG avec un mini clone de MisterKid (QBasic)
 * Email:  asher256@gmail.com
 *
 * Touches:
 * --------
 * Touche E change de mode: jeu/edition
 *
 *           * Jeu:
 *           ======
 *                        S              Positionner au début du MAP
 *                        Alt            Sauter
 *                        Droit/gauche   Bouger
 *           * Editeur:
 *           ==========
 *                        1           Supprimer une colonne dans le map
 *                        2           Ajouter une colonne
 *                        Gauche      Scrolling camera gauche
 *                        Droite      Scrolling camera droit
 *                        +/-         Changer d'obstacle
 *                        Mouse B1    Remplir avec obstacle actuel
 *                        Mouse B2    Vider avec obstacle 0
 *                        V           Positionner MK devant le pointeur
 */

#include <allegro.h>
#include "eg.h"
#include "mkid.h"
#include "mrec.h"

#define EST   0
#define OUEST 1
#define SUD   2
#define NORD  3


// gfx mode select
int card=GFX_AUTODETECT,w=320,h=200;

// CREDIT
EG_CREDIT *credit=0;
int credit_enabled=FALSE;

// CHEATS
#define cheat_edit   0
#define cheat_credit 1
#define cheat_gfx    2
EG_UCHAR edit_str[] = {KEY_E,KEY_D,KEY_I,KEY_T};
EG_UCHAR credit_str[] = {KEY_C,KEY_R,KEY_E,KEY_D,KEY_I};
EG_UCHAR gfx_str[] = {KEY_G,KEY_F,KEY_X};

EG_CHEAT_CODE code[4] =
{
	{ edit_str, 4, TRUE },
	{ credit_str, 5 ,TRUE},
	{ gfx_str, 3, TRUE},
	{ 0,0,0}
};

EG_MAP *map=0;
EG_CHEAT cheat;
EG_CAMERA *camera=0;
EG_FILEF *filef=0;
EG_MPOS *mk=0;
EG_MPOS mrec_initial_mpos;
BITMAP *buffer=0;

EG_SHORT map_x,map_y;
int scr_x,scr_y;
EG_SHORT obstacle;
DATAFILE *data=0;

// Annimation
EG_LANIM *anim=0;           // Animation actuelle
EG_LANIM *anim_droit=0;
EG_LANIM *anim_gauche=0;

// Record de la demo
MREC *mrec=0; //aucune mrec au tout début

// Mister Kid
int sauter=0;               // si  >0 alors il saute... -1 tomber. 0 normal.
int marcher=FALSE;
int direction=EST;

// Vitesse + Gravite
#define MKANIM_MSEC       300-vitesse_x*60
#define VITESSE_X_MINIM   1
#define MAX_VITESSE_X     5
#define MAX_VITESSE_SCX   13
#define DEC_VITESSE_SCX   13

#define VITESSE_Y_MINIM   2
#define MAX_VITESSE_Y     4
#define MAX_VITESSE_SCY   10
#define DEC_VITESSE_SCY   10
EG_SHORT vitesse_x=VITESSE_X_MINIM;
EG_SHORT vitesse_y=VITESSE_Y_MINIM;
EG_SHORT vitesse_scy=0;
EG_SHORT vitesse_scx=0;

// Counter
volatile int counter=0;
volatile int mrec_counter=0;
volatile int cheat_counter=0;
volatile int fps_counter=0;
int actual_fps=0;
int fps=0;
volatile int bcounter=0;
volatile int bps_counter=0;

// EDIT
int actual_tile = 1;
int editeur=FALSE;

// Variables
void error(EG_CHAR *err);
int map_fileformat(void *eg_filef_data, void *data);
void bye();
void init();
void draw();
void do_game_input();
void do_editeur_input();
BITMAP *get_tile(int obstacle);
void handle();
void afficher_decore();

// Fonctions

/* mkid.c */
void error(EG_CHAR *err);
int map_fileformat(void *eg_filef_data, void *data);
void bye(void);
void bps_handle(void);
void handle(void);
void anim_draw(void *anim, BITMAP *buffer, int x, int y);
int get_info(void *anim, int info);
int video_mode(int w, int h);
void init(void);
BITMAP *get_tile(int obstacle);
void afficher_decore(void);
void afficher(BITMAP *buffer, EG_OBS *obs);
void draw(void);
void do_editeur_input(void);
EG_SHORT test(EG_SHORT pgx, EG_SHORT pgy);
int tester(int n);
void auguementer_vitesse_x(void);
void diminuer_vitesse_x(void);
void auguementer_vitesse_y(void);
void diminuer_vitesse_y(void);
void do_game_input(void);
void do_input(void);

/**************************************************
 * Erreur
 *************************************************/
void error(EG_CHAR *err)
{
	set_gfx_mode(GFX_TEXT,0,0,0,0);
	allegro_message("%s\n\n",err);
	bye();
	exit(1);
}

/**************************************************
 * FILE FORMAT
 *************************************************/
int map_fileformat(void *eg_filef_data, void *data)
{
	int n;
	HEADER("MAP");
	if((n=egmpos_filef_proc(eg_filef_data,mk))) return n;
	if((n=egmap_filef_proc(eg_filef_data,map))) return n;
	egmpos_camera_fix(mk,camera);
	egcamera_correct(camera);
	return FF_OK;
}

/**************************************************
 * BYE
 *************************************************/
void bye()
{
	if(map) {
		destroy_egmap(map);
		map=0;
	}
	destroy_mrec(mrec);
	if(credit) {
		destroy_egcredit(credit);
		credit=0;
	}
	if(anim_droit) {
		destroy_eglanim(anim_droit);
		anim_droit=0;
	}
	if(anim_gauche) {
		destroy_eglanim(anim_gauche);
		anim_gauche=0;
	}
	if(camera)  {
		destroy_egcamera(camera);
		camera=0;
	}
	if(mk)      {
		destroy_egmpos(mk);
		mk=0;
	}
	if(filef)   {
		destroy_egfilef(filef);
		filef=0;
	}
	if(buffer)  {
		destroy_bitmap(buffer);
		buffer=0;
	}
	if(data)    {
		unload_datafile(data);
		data=0;
	}
}

/**************************************************
 * COMPTEUR
 *************************************************/
void bps_handle()
{
	bps_counter++;
	bcounter++;
}
END_OF_FUNCTION(bps_handle);
void handle()
{
	counter++;
	fps_counter++;
	cheat_counter++;
}
END_OF_FUNCTION(handle);

void mrec_handle()
{
	mrec_counter++;
}
END_OF_FUNCTION(mrec_handle);

/**************************************************
 * INITIALISATION
 *************************************************/
void anim_draw(void *anim, BITMAP *buffer, int x,int y)
{
	BITMAP *spr;
	EG_LANIM *lanim=anim;
	// prends l'animation
	switch(lanim->actual_frame) {
	case 0:
		spr=data[NORMAL].dat;
		break;
	case 1:
		spr=data[MARCHER].dat;
		break;
	}
	// affiche
	if(anim==anim_gauche) {
		draw_sprite_h_flip(buffer,spr,x,y);
	} else {
		draw_sprite(buffer,spr,x,y);
	}
}
int get_info(void *anim, int info)
{
	switch(info) {
	case EG_LANIM_WIDTH:
	case EG_LANIM_HEIGHT:
		return 20;
	case EG_LANIM_MAXFRAME:
		return 2;
	case EG_LANIM_NEXTFRAME:
		if(counter>=MKANIM_MSEC) {
			counter=0;
			return TRUE;
		}
	}
	return 0;
}

// ret -1 si err
int video_mode(int w, int h)
{
	buffer = 0;

	// ** GFX MODE
	if(set_gfx_mode(GFX_AUTODETECT,w,h,0,0)<0)
		return -1;

	buffer = create_bitmap(SCREEN_W,SCREEN_H);
	if(!buffer)
		error("Out of memory...");

	return 0;
}

void init()
{
	BITMAP *temp;
	int n;

	allegro_init();
	atexit(bye);
	install_keyboard();
	install_mouse();
	install_timer();

	if(video_mode(w,h)<0)
		error("Erreur dans le chargement du mode graphique...");

	// Timer
	LOCK_FUNCTION(handle);
	LOCK_FUNCTION(bps_handle);
	LOCK_VARIABLE(counter);
	LOCK_VARIABLE(bcounter);
	LOCK_VARIABLE(fps_counter);
	LOCK_VARIABLE(bps_counter);
	LOCK_VARIABLE(cheat_counter);
	install_int(handle,1);
	install_int_ex(bps_handle, BPS_TO_TIMER(60));

	LOCK_FUNCTION(mrec_handle);
	LOCK_VARIABLE(mrec_counter);
	install_int(mrec_handle,1);

	// DATA
	data = load_datafile("mkid.dat");
	if(!data) {
		data = load_datafile("#");
		if(!data)
			error("Le programme ne trouve pas le fichier data.dat...");
	}
	set_palette(data[PALETTE].dat);

	install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, 0);
	set_volume(255,100);

	// BUFFER CAMERA MAP FILEF
	map = create_egmap(20,10);
	camera = create_egcamera(20,20,&map->width, &map->height);
	filef = create_egfilef(map_fileformat,FF_NORMAL);

	anim = anim_droit = create_eglanim(anim_draw,get_info);
	anim_gauche = create_eglanim(anim_draw,get_info);

	mk = create_egmpos(20,20,camera->spr_w,camera->spr_h);

	init_egcheat(&cheat,code,&counter);
	credit = create_egcredit();
	if(!credit || !camera || !map || !filef || !mk || !anim_droit || !anim_gauche)
		error("Pas assez de memoire...");

	if(egfilef_load(filef,"mkid.map",map)) {
		egmap_cresize(map,20,10);
	}
	text_mode(-1);

	//* CREDIT ***************************>>>
	egcredit_add_text(credit, "Credits", font, 10);
	egcredit_add_space(credit, 10);
	egcredit_add_text(credit, "Programmation", font, 14);
	egcredit_add_text(credit, "Asher256", font, 15);

	mrec = create_mrec(&mrec_counter);
	if(!mrec) exit(32);
}

/**************************************************
 * DRAW
 *************************************************/
BITMAP *get_tile(int obstacle)
{
	switch(obstacle) {
	case 1:
		return data[TILE1].dat;
		break;
	case 2:
		return data[TILE2].dat;
		break;
	case 3:
		return data[TILE3].dat;
		break;
	case 4:
		return data[TILE4].dat;
		break;
	case 5:
		return data[TILE5].dat;
		break;
	case 6:
		return data[TILE6].dat;
		break;
	case 7:
		return data[TILE7].dat;
		break;
	case 8:
		return data[TILE8].dat;
		break;
	case 9:
		return data[TILE9].dat;
		break;
	case 10:
		return data[TILE10].dat;
		break;
	case 11:
		return data[TILE11].dat;
		break;
	case 12:
		return data[TILE12].dat;
		break;
	case 13:
		return data[TILE13].dat;
		break;
	case 14:
		return data[TILE15].dat;
		break;
	case 15:
		return data[TILE15].dat;
		break;
	case 16:
		return data[TILE16].dat;
		break;
	case 17:
		return data[TILE17].dat;
		break;
	case 18:
		return data[TILE18].dat;
		break;
	}
	return 0;
}

/**************************************************
 * Afficher le décore...
 *************************************************/
void afficher_decore()
{
	BITMAP *bmp=data[DECORE].dat;
	stretch_blit(bmp,buffer,0,0,bmp->w,bmp->h,0,0,buffer->w,buffer->h);
}

/**************************************************
 * DRAW
 *************************************************/
void afficher(BITMAP *buffer, EG_OBS *obs)
{
	BITMAP *spr;
	if((spr=get_tile(obs->value)))
		draw_sprite(buffer,spr,obs->scr_x,obs->scr_y);
}
void draw()
{
	int x,y;
	BITMAP *spr;

	acquire_bitmap(buffer);
	afficher_decore();
	textprintf(buffer,font,10,10,1,"------------------ %i",mrec->pos);

	// Les sprites
	if(key[KEY_O])
	{
		EG_CAMERA_BEGIN(camera,map,map_x,map_y,scr_x,scr_y,obstacle)
		if((spr=get_tile(obstacle)))
			draw_sprite(buffer,spr,scr_x,scr_y);
		EG_CAMERA_END
	}
	else
		egcamera_draw_map(camera,map,buffer,afficher);

	// pointeur souris ds EDITEUR
	if(editeur) {
		putpixel(buffer,mouse_x,mouse_y,1);
		if(get_tile(actual_tile))
			draw_sprite(buffer,get_tile(actual_tile),mouse_x+12,mouse_y+9);

		rect(buffer,
			 egcamera_x_map2scr(camera,0),
			 egcamera_y_map2scr(camera,0),
			 egcamera_x_map2scr(camera,map->width),
			 egcamera_y_map2scr(camera,map->height),
			 15
			);
	}

	// Afficher MisterKid
	x=egcamera_x_map2scr(camera,mk->pgx)+mk->scx;
	y=egcamera_y_map2scr(camera,mk->pgy)+mk->scy;

	// Sauter?
	if(editeur)
		draw_sprite(buffer,data[NORMAL].dat,x,y);
	else if(sauter>0) {
		if(direction==0)
			draw_sprite(buffer,data[SAUTER].dat,x,y);
		else
			draw_sprite_h_flip(buffer,data[SAUTER].dat,x,y);
	}

	// Tomber
	else if(sauter==-1) {
		if(direction==0)
			draw_sprite(buffer,data[TOMBER].dat,x,y);
		else
			draw_sprite_h_flip(buffer,data[TOMBER].dat,x,y);
	}

	// Anim Bouger?
	else if(marcher && sauter==0)
		eglanim_draw_incloop(anim,buffer,x,y);

	// normal?
	else {
		if(vitesse_x>VITESSE_X_MINIM)
			eglanim_draw_incloop((direction)?anim_gauche:anim_droit,buffer,x,y);
		else if(direction==0)
			draw_sprite(buffer,data[NORMAL].dat,x,y);
		else
			draw_sprite_h_flip(buffer,data[NORMAL].dat,x,y);
	}

	textprintf(buffer,font,10,10,14,"FPS: %i",fps);

	release_bitmap(buffer);

	acquire_screen();
	blit(buffer,screen,0,0,0,0,buffer->w,buffer->h);
	release_screen();
}

/**************************************************
 * EDITEUR INPUT
 *************************************************/
void do_editeur_input()
{
	int i;

	// Editeur
	if(mouse_b) {
		map_x = egcamera_x_scr2map(camera,mouse_x);
		map_y = egcamera_y_scr2map(camera,mouse_y);
		egmap_put(map,map_x,map_y,(mouse_b&1)?actual_tile:0);
	}

	// Actual Tile ++
	if(key[KEY_PLUS_PAD] && actual_tile<18) {
		actual_tile++;
		while(key[KEY_PLUS_PAD]) {
			poll_keyboard();
		}
	}

	if(key[KEY_V]) {
		mk->pgx = egcamera_x_scr2map(camera,mouse_x);
		mk->pgy = egcamera_y_scr2map(camera,mouse_y);
		mk->scx=mk->scy=0;

		// Err?
		if(mk->pgx<0) mk->pgx=0;
		else if(mk->pgx>=map->width) mk->pgx=map->width-1;
		if(mk->pgy<0) mk->pgy=0;
		else if(mk->pgy>=map->height) mk->pgy=map->height-1;

	}

	if(key[KEY_MINUS_PAD] && actual_tile>1) {
		actual_tile--;
		while(key[KEY_MINUS_PAD]) {
			poll_keyboard();
		}
	}

	// Resize
	if(key[KEY_1] && map->width>17) { // --
		egmap_resize(map, map->width-1, map->height);
		while(key[KEY_1]) {
			poll_keyboard();
		}
	}

	if(key[KEY_2]) { // --
		egmap_resize(map, map->width+1, map->height);
		while(key[KEY_2]) {
			poll_keyboard();
		}
	}

	// Resize
	if(key[KEY_3] && map->width>17) { // --
		egmap_resize(map, map->width, map->height-1);
		while(key[KEY_3]) {
			poll_keyboard();
		}
	}

	if(key[KEY_4]) { // --
		egmap_resize(map, map->width, map->height+1);
		while(key[KEY_4]) {
			poll_keyboard();
		}
	}


	if(key[KEY_DOWN] || mouse_y>=SCREEN_H-6) {
		for(i=0; i<2; i++) egcamera_scy_inc(camera,1);
	}

	if(key[KEY_UP] || mouse_y<=5) {
		for(i=0; i<2; i++) egcamera_scy_dec(camera,1);
	}

	// Déplacement
	if(key[KEY_LEFT] || mouse_x<=5) {
		for(i=0; i<2; i++) egcamera_scx_dec(camera,1);
	}

	if(key[KEY_RIGHT] || mouse_x>=SCREEN_W-6) {
		for(i=0; i<2; i++) egcamera_scx_inc(camera,1);
	}
}

/**************************************************
 * TESTER
 *
 * 0 N
 * 1 S
 * 2 E
 * 3 O
 *************************************************/
EG_SHORT test(EG_SHORT pgx, EG_SHORT pgy)
{
	int n = egmap_get(map, pgx, pgy);
	if(map->err) return TRUE;
	return n;
}
int tester(int n)
{
	switch(n) {
	case NORD:
		return egmpos_collision_y(mk,-1,test);
	case SUD:
		return egmpos_collision_y(mk,mk->height,test);
	case EST:
		return egmpos_collision_x(mk,mk->width,test);
	case OUEST:
		return egmpos_collision_x(mk,-1,test);
	}
	return FALSE;
}


/**************************************************
 * GAME INPUT
 *************************************************/
void auguementer_vitesse_x()
{
	if(vitesse_x<MAX_VITESSE_X) eg_sc_inc(&vitesse_x,&vitesse_scx,MAX_VITESSE_SCX,1);
}

void diminuer_vitesse_x()
{
	if(vitesse_x>VITESSE_X_MINIM) eg_sc_dec(&vitesse_x,&vitesse_scx,DEC_VITESSE_SCX,1);
}

void auguementer_vitesse_y()
{	if(vitesse_y<MAX_VITESSE_Y)
		eg_sc_inc(&vitesse_y,&vitesse_scy,MAX_VITESSE_SCY,1);
}

void diminuer_vitesse_y()
{
	if(vitesse_y>VITESSE_Y_MINIM) eg_sc_dec(&vitesse_y,&vitesse_scy,DEC_VITESSE_SCY,1);
}

/**************************************************
 * Un système de commandes...
 *
 **************************************************/

int key_s=0, key_left=0, key_down=0, key_up=0, key_right=0;
int key_alt=0;

/**************************************************
 * GAME INPUT
 *************************************************/
void do_game_input()
{
	int i;
	static int changement_direction=FALSE;
	static int alt_pressed=FALSE;

	key_s=key_left=key_right=key_up=key_alt=key_down=0;

	//s'il n'est pas entrain de faire play
	//alors clavier...
	if(!mrec->play) {
		if(key[KEY_P]) {
			mrec_rew(mrec);
			memcpy(mk,&mrec_initial_mpos,sizeof(EG_MPOS));
			mrec_play(mrec);
		}
		mrec_loop(mrec); //mets le tout à 0
		if(key[KEY_S]) key_s=1;
		if(key[KEY_LEFT]) key_left=1;
		if(key[KEY_RIGHT]) key_right=1;
		if(key[KEY_UP]) key_up=1;
		if(key[KEY_DOWN]) key_down=1;
		if(key[KEY_ALT]) key_alt=1;
	}

	if(mrec_loop(mrec)) exit(33); //exit 33 veut dire fin de play !

	if(key_s) {
		mk->pgx=mk->pgy=mk->scx=mk->scy=direction=vitesse_scx=vitesse_scy=0;
		vitesse_x=VITESSE_X_MINIM;
		vitesse_y=VITESSE_Y_MINIM;
		egmpos_camera_fix(mk,camera);
		egcamera_correct(camera);
	}

	// GAUCHE
	if(key_left) {
		for(i=0; i<vitesse_x; i++) {
			if(tester(OUEST)) break;
			egmpos_scx_dec(mk,1);
		}

		anim=anim_gauche;
		direction=OUEST;

		if(i!=0) {
			if(!sauter) auguementer_vitesse_x();

			if(anim==anim_droit) {
				vitesse_scx=0;
				vitesse_x=VITESSE_X_MINIM;
				eglanim_rew(anim_gauche);
			}
			egmpos_camera_fix(mk,camera);
			egcamera_correct(camera);
			marcher=TRUE;
		}
	}

	// DROITE
	else if(key_right) {
		for(i=0; i<vitesse_x; i++) {
			if(tester(EST)) break;
			egmpos_scx_inc(mk,1);
		}

		anim=anim_droit;
		direction=EST;

		// Il a bougé?
		if(i!=0) {
			if(!sauter) auguementer_vitesse_x();

			if(anim==anim_gauche) {
				vitesse_scx=0;
				vitesse_x=VITESSE_X_MINIM;
				eglanim_rew(anim_droit);
			}
			egmpos_camera_fix(mk,camera);
			egcamera_correct(camera);
			marcher=TRUE;
		}
	}
	else {
		// Pas tout ce qui est en haut alors NORMAL
		if(vitesse_x>VITESSE_X_MINIM) {
			if(anim==anim_gauche) {
				for(i=0; i<vitesse_x; i++) {
					if(tester(OUEST)) break;
					egmpos_scx_dec(mk,1);
				}
			}
			else {
				for(i=0; i<vitesse_x; i++) {
					if(tester(EST)) break;
					egmpos_scx_inc(mk,1);
				}
			}
			diminuer_vitesse_x();
			egmpos_camera_fix(mk,camera);
			egcamera_correct(camera);
		}
		marcher=FALSE;
	}

	// il devrait tomber?
	if(sauter==0 && !tester(SUD)) {
		sauter=-1;
		vitesse_y=VITESSE_Y_MINIM;
		vitesse_scy=0;
	}

	// Il TOMBE
	else if(sauter==-1 || (!key_alt && sauter>40))
	{
		sauter=-1;
		for(i=0; i<vitesse_y; i++) {
			if(tester(SUD)) {
				sauter=0;
				break;
			}
			egmpos_scy_inc(mk,1);
		}

		if(sauter!=0) {
			auguementer_vitesse_y();
			egmpos_camera_fix(mk,camera);
			egcamera_correct(camera);
		}
	}

	// Ordre sauter (avec cond d'avoir  lâché ALT)
	else if(!sauter && alt_pressed==FALSE && key_alt && !tester(NORD)) {
		alt_pressed=TRUE;
		play_sample(data[SFX_SAUTER].dat,255,128,1000,FALSE);

		vitesse_y=MAX_VITESSE_Y;
		vitesse_scx=0;

		sauter=1;
	}

	// Il est entrain de sauter
	else if(sauter>0) {
		for(i=0; i<vitesse_y; i++) {
			if(tester(NORD)) {
				sauter=-1;
				break;
			}
			egmpos_scy_dec(mk,1);
			sauter++;
		}

		if(sauter!=-1) {
			diminuer_vitesse_y();
			egmpos_camera_fix(mk,camera);
			egcamera_correct(camera);

			if(sauter>80)  // 40+1 car 1 ajouté qd on touche la 1st fois
				sauter=-1;
		}
	}
	if(!key_alt) alt_pressed=FALSE;
}

/**************************************************
 * INPUT
 *************************************************/
void do_input()
{
	int p=-1;
	int c,wi,he;
	BITMAP *save;
	poll_keyboard();
	poll_mouse();

	p = egcheat_test(&cheat);

	if(p>=0) play_sample(data[CHEATOK].dat,255,128,1000,FALSE);

	if(p==cheat_gfx) {
		if(!editeur) midi_pause();
		wi=w;
		he=h;
		clear(screen);
		set_palette(default_palette);
		if(gfx_mode_select(&card,&w,&h)) {
			if(video_mode(wi,he)<0) {
				//revenir
				if(video_mode(wi,he)<0)
					error("Ne peux revenir au mode graphique précédent...");
			}
		}
		fade_out(5);
		egcamera_optimize(camera);
		egmpos_camera_fix(mk,camera);
		egcamera_correct(camera);
		draw();
		if(!editeur) midi_resume();
		fade_in(data[PALETTE].dat,5);
	}
	else if(p==cheat_credit) {
		save=create_bitmap(SCREEN_W,SCREEN_H);
		if(save) {
			blit(buffer,save,0,0,0,0,SCREEN_W,SCREEN_H);
			egcredit_rew(credit);
			p=EG_CREDIT_OK;
			while(!(key[KEY_ENTER] || p==EG_CREDIT_END)) {
				poll_keyboard();
				blit(save,buffer,0,0,0,0,SCREEN_W,SCREEN_H);
				p=egcredit_moveup_draw(credit,buffer,1);
				blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
			}
			destroy_bitmap(save);
		}
	}
	else if(p==cheat_edit) {
		editeur = (editeur)?FALSE:TRUE;
		egmpos_camera_fix(mk,camera);
		if(!editeur) {
			midi_resume();
			egcamera_correct(camera);
			vitesse_x=VITESSE_X_MINIM;
		}
		else
			midi_pause();
		fade_out(5);
		draw();
		fade_in(data[PALETTE].dat,5);
	}

	if(editeur)
		do_editeur_input();
	else
		do_game_input();
}

int main(void)
{
	init();

	play_midi(data[MIDI].dat, TRUE);

	//mets les variables pour la création de la
	//démo
	mrec_add_var(mrec,&key_s,sizeof(int));
	mrec_add_var(mrec,&key_alt,sizeof(int));
	mrec_add_var(mrec,&key_left,sizeof(int));
	mrec_add_var(mrec,&key_right,sizeof(int));
	mrec_add_var(mrec,&key_down,sizeof(int));
	mrec_add_var(mrec,&key_up,sizeof(int));

	//commence l'enregistrement...
	memcpy(&mrec_initial_mpos,mk,sizeof(EG_MPOS));
	mrec_rec(mrec);

	// BOUCLE
	while(!key[KEY_ESC]) {
		bcounter=0;

		while(bps_counter>0) {
			if(fps_counter>=2000) {
				fps=actual_fps;
				actual_fps=0;
				fps_counter=0;
			}
			do_input();
			bps_counter--;
		}

		draw();
		actual_fps++;
		//while(!(bcounter>=1));
	}

	egfilef_save(filef,"mkid.map",map);

	bye();

	return 0;
}
END_OF_MAIN();

/* vim: set ts=4 nowrap: */
