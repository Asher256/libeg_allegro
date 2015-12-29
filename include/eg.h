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

#ifndef EG_LIBRARY_H
#define EG_LIBRARY_H
#ifndef ALLEGRO_H
#include <allegro.h>
#endif

// ******* Le version d'EG *********
#define EG_VERSION       1
#define EG_SUB_VERSION   0
#define EG_VERSION_STR   "0.2"
#define EG_DATE_STR      ""

// ******* File format ***********
#define FF_OK                     0  // Pas d'erreur
#define FF_DISK_PROTECTED         1  // Disque plein ou proteg en criture
#define FF_ERROR_IN_OPEN          2  // Erreur ds l'ouverture du fichier
#define FF_NOT_EXIST              3  // Le fichier n'existe pas
#define FF_INCORRECT_FORMAT       4  // Format de fichier incorrect
#define FF_ERROR_IN_FILE          5  // Le fichier est dfectueux
#define FF_OUTMEM                 6
#define FF_USER                   7  // Valeur de retour de l'utilisateur
                                     // FF_USER ou FF_USER+1

// Les return de load_ega()
#define EGA_UNSUPPORTED_FORMAT          FF_USER

// Les return de save_ega
typedef int               EG_INT;
typedef unsigned int      EG_UINT;
typedef char              EG_CHAR;
typedef unsigned char     EG_UCHAR;
typedef short             EG_SHORT;
typedef unsigned short    EG_USHORT;
typedef EG_UINT           EG_SIZET;

//STRUCT ANIMATION 2D
typedef struct {
   EG_INT *width;     //Pointeur vers le width et le height
   EG_INT *height;
   void *data;
   void (*draw) (BITMAP *, void *, EG_INT, EG_INT);
   void (*destroy) (void *);
   EG_INT msec;
   EG_CHAR type;  // 0 bmp, 1 Rle
   EG_CHAR allocated;
} EG_FRAME;
typedef struct {
   EG_FRAME *frame;
   EG_INT max_frame_allocation;
   EG_INT max_frame;
   EG_INT actual_frame;
   volatile EG_INT *counter;
} EG_ANIM2D;

//STRUCT EG_OBJECT
typedef struct {
   EG_INT (*width)  (EG_CHAR *,EG_CHAR *,EG_INT,EG_INT);     //Pointeur vers le width et le height
   EG_INT (*height) (EG_CHAR *,EG_CHAR *,EG_INT,EG_INT);
   void *data;
   void *data2;
   void (*draw) (BITMAP *, void *, void *, EG_INT, EG_INT, EG_INT);
   void (*destroy) (void *);
   EG_INT d1;
   EG_INT d2;
   EG_CHAR type;  // EGO_TEXT EGO_RLE EGO_BMP EGO_EGANIM2D
   EG_CHAR allocated;
} EG_OBJECT;

// STRUCT EG_CREDIT
typedef struct
{
        EG_INT position_y;
        EG_OBJECT *egcredit;
        EG_INT egcredit_size;
        EG_INT true_height;
        EG_INT *animation_index;   //Les EG Object variables
        EG_INT animation_index_size;
} EG_CREDIT;

// EG_FILEF
typedef struct {
   //Le pointeur de fonction ou il y a le format de fichier
   EG_INT (*proc_file_format)(void *eg_filef_data,void *data);  
   EG_INT ret;
   EG_CHAR *filename;              //Le nom de fichier ouvert ou sauvegard
   EG_CHAR mode;                   //Le mode (FF_PACKED ou FF_NORMAL)
   PACKFILE *file;                 //Le handle
   EG_CHAR load;                   //Le Flag s'il est entrain de EG_CHARger
   EG_CHAR save;                   //Le flag s'il est entrain de sauver
} EG_FILEF;

// EG_RECT
typedef struct {
    EG_INT x,y,w,h;
} EG_RECT;

// CONSOLE
typedef struct {
	EG_CHAR **lh;
	EG_INT sizeof_lh;
	
	FONT *font;
	EG_INT color;

	EG_CHAR *actual_cmd;
	EG_INT cmd_strlen_limit;	
	EG_INT cmd_curpos;

	void (*draw_bg)(BITMAP *bmp, EG_RECT *point);
	void (*draw_cursor)(BITMAP *bmp, EG_RECT *point, EG_INT color);
	
	EG_INT w,h;
} EG_CONSOLE;
	
// MAP
typedef struct {
	EG_SHORT width,height;
	EG_SHORT **line;    // ptr line[x*unit][y] //une ligne 
	EG_SHORT *map;     // le map en liste...
	EG_CHAR err;
} EG_MAP;

// Layer map
typedef struct {
	EG_MAP **layer;
	EG_SHORT sizeof_layer;
	EG_SHORT width,height;
	EG_SHORT err;
} EG_MAPL;

typedef struct {
	EG_SHORT pgx,pgy;
	EG_SHORT scx,scy;
	EG_SHORT width,height;
	EG_SHORT spr_size[2];
	EG_SHORT *spr_w,*spr_h;
	EG_SHORT *map_w,*map_h; // dans mpos, elle contiennent spr_w, spr_h de la camera
	EG_CHAR err;
} EG_CAMERA;

typedef EG_CAMERA EG_MPOS;

// LANIM
typedef struct { //low level anim
	EG_INT actual_frame;
	void (*draw_anim)(void *lanim, BITMAP *bmp, EG_INT x, EG_INT y);
	EG_INT  (*get_info)(void *lanim, EG_INT info_type); // ret TRUE si passer à autre frame
} EG_LANIM;

//********************** EG LANIM ********************************
#define EG_LANIM_NEXTFRAME 0
#define EG_LANIM_WIDTH     1 
#define EG_LANIM_HEIGHT    2
#define EG_LANIM_MAXFRAME  3

EG_INT egmapl_get_height(EG_MAPL *mapl);
EG_INT egmapl_get_width(EG_MAPL *mapl);

EG_INT eglanim_dec(EG_LANIM *lanim);
EG_INT eglanim_inc(EG_LANIM *lanim);
EG_INT eglanim_decloop(EG_LANIM *lanim);
EG_INT eglanim_incloop(EG_LANIM *lanim);
		
void eglanim_draw(EG_LANIM *lanim, BITMAP *bmp, EG_INT x, EG_INT y);
EG_INT eglanim_draw_decloop(EG_LANIM *lanim, BITMAP *bmp, EG_INT x, EG_INT y); 
EG_INT eglanim_draw_dec(EG_LANIM *lanim, BITMAP *bmp, EG_INT x, EG_INT y);
EG_INT eglanim_draw_incloop(EG_LANIM *lanim, BITMAP *bmp, EG_INT x, EG_INT y);
EG_INT eglanim_draw_inc(EG_LANIM *lanim, BITMAP *bmp, EG_INT x, EG_INT y); 
void eglanim_draw(EG_LANIM *lanim, BITMAP *bmp, EG_INT x, EG_INT y);

EG_LANIM *create_eglanim(void (*draw_anim)(void *lanim, BITMAP *bmp, EG_INT x, EG_INT y),EG_INT (*get_info)(void *lanim, EG_INT info_type));
void destroy_eglanim(EG_LANIM *anim);

void eglanim_rew(EG_LANIM *lanim);
void eglanim_ff(EG_LANIM *lanim);

//****************************************************************

//********************** MPOS ************************************

EG_INT   egmpos_x_map2scr(EG_MPOS *mpos, EG_SHORT pgx);
EG_INT   egmpos_y_map2scr(EG_MPOS *mpos, EG_SHORT pgy);
EG_SHORT egmpos_x_scr2map(EG_MPOS *mpos, EG_INT scr_x);
EG_SHORT egmpos_y_scr2map(EG_MPOS *mpos, EG_INT scr_y);

EG_SHORT egmpos_get_sprheight(EG_MPOS *mpos);
EG_SHORT egmpos_get_sprwidth(EG_MPOS *mpos);

EG_SHORT egmpos_collision(EG_MPOS *mpos, EG_SHORT addx, EG_SHORT addy, EG_SHORT (*test)(EG_SHORT pgx, EG_SHORT pgy));
EG_SHORT egmpos_collision_x(EG_MPOS *mpos, EG_SHORT addx,  EG_SHORT (*test)(EG_SHORT pgx, EG_SHORT pgy));
EG_SHORT egmpos_collision_y(EG_MPOS *mpos, EG_SHORT addy, EG_SHORT (*test)(EG_SHORT pgx, EG_SHORT pgy));

void egmpos_camera_fix(EG_MPOS *mpos, EG_CAMERA *camera);

EG_SHORT egmpos_get_ycount(EG_MPOS *mpos);
EG_SHORT egmpos_get_xcount(EG_MPOS *mpos);
void egmpos_set_ycount(EG_MPOS *mpos, EG_SHORT cy);
void egmpos_set_xcount(EG_MPOS *mpos, EG_SHORT cx);
void egmpos_set_count(EG_MPOS *mpos, EG_SHORT cx, EG_SHORT cy);

EG_MPOS *create_egmpos(EG_SHORT w, EG_SHORT h, EG_SHORT *spr_w, EG_SHORT *spr_h);
void init_egmpos(EG_MPOS *mpos, EG_SHORT w, EG_SHORT h, EG_SHORT *spr_w, EG_SHORT *spr_h);
void destroy_egmpos(EG_MPOS *mpos);

void egmpos_inc(EG_MPOS *mpos, EG_SHORT inc_x, EG_SHORT inc_y);
void egmpos_scy_dec(EG_MPOS *mpos, EG_SHORT dec);
void egmpos_scy_inc(EG_MPOS *mpos, EG_SHORT add);
void egmpos_scx_dec(EG_MPOS *mpos, EG_SHORT dec);
void egmpos_scx_inc(EG_MPOS *mpos, EG_SHORT add);

void egmpos_pgy_dec(EG_MPOS *mpos, EG_SHORT dec);
void egmpos_pgy_inc(EG_MPOS *mpos, EG_SHORT add);
void egmpos_pgx_dec(EG_MPOS *mpos, EG_SHORT dec);
void egmpos_pgx_inc(EG_MPOS *mpos, EG_SHORT add);

EG_SHORT egmpos_filef_proc(void *eg_filef_data, void *data);

void egmpos_set_size(EG_MPOS *mpos, EG_SHORT w, EG_SHORT h);
void egmpos_set_height(EG_MPOS *mpos, EG_SHORT height);
void egmpos_set_width(EG_MPOS *mpos, EG_SHORT width);

EG_SHORT egmpos_get_width(EG_MPOS *mpos);
EG_SHORT egmpos_get_height(EG_MPOS *mpos);
//*****************************************************************

//**********************  MAPL ************************************
EG_SHORT egmapl_filef_proc(void *eg_filef_data, void *data);
		
void egmapl_clear(EG_MAPL *mapl);
void egmapl_clearto(EG_MAPL *mapl, EG_SHORT val);

void egmapl_put(EG_MAPL *mapl, EG_SHORT layer, EG_SHORT pgx, EG_SHORT pgy, EG_SHORT val);
EG_SHORT egmapl_get(EG_MAPL *mapl, EG_SHORT layer, EG_SHORT pgx, EG_SHORT pgy);
		
void egmapl_resize(EG_MAPL *mapl, EG_SHORT w, EG_SHORT h);
		
EG_MAPL *create_egmapl(EG_SHORT max_layer, EG_SHORT width, EG_SHORT height);
void destroy_egmapl(EG_MAPL *mapl);
//********************************************************************

// **********************  CAMERA  ************************************
typedef struct {
	EG_SHORT pgx,pgy;
	EG_INT scr_x,scr_y;
	EG_SHORT value;
} EG_OBS;
#define EG_CAMERA_BEGIN(camera,map,map_x,map_y,scr_x,scr_y,obstacle) for(map_x=camera->pgx;map_x<camera->pgx+camera->width;map_x++) { for(map_y=camera->pgy;map_y<camera->pgy+camera->height;map_y++) { scr_x = egcamera_x_map2scr(camera,map_x); scr_y = egcamera_y_map2scr(camera,map_y); obstacle = egmap_get(map,map_x,map_y); if(!map->err) {
#define EG_CAMERA_END }}}

EG_INT egmpos_x_map2scr(EG_MPOS *mpos, EG_SHORT pgx);
EG_INT egmpos_y_map2scr(EG_MPOS *mpos, EG_SHORT pgy);
		
void egcamera_draw_map(EG_CAMERA *camera, EG_MAP *map, BITMAP *bmp, void (*draw)(BITMAP *bmp, EG_OBS *obs));

EG_SHORT egcamera_get_sprwidth(EG_CAMERA *camera);
EG_SHORT egcamera_get_sprheight(EG_CAMERA *camera);

void egcamera_center(EG_CAMERA *camera, EG_SHORT pgx, EG_SHORT pgy, EG_SHORT scx, EG_SHORT scy);

EG_SHORT egcamera_filef_proc(void *eg_filef_data, void *data);

EG_SHORT egcamera_get_width(EG_MPOS *camera);
EG_SHORT egcamera_get_height(EG_MPOS *camera);

void egcamera_set_size(EG_CAMERA *camera, EG_SHORT w, EG_SHORT h);
void egcamera_set_height(EG_CAMERA *camera, EG_SHORT height);
void egcamera_set_width(EG_CAMERA *camera, EG_SHORT width);

EG_SHORT egcamera_x_scr2map(EG_CAMERA *camera, EG_INT xscr);
EG_SHORT egcamera_y_scr2map(EG_CAMERA *camera, EG_INT yscr);
EG_INT egcamera_x_map2scr(EG_CAMERA *camera, EG_SHORT xmap);
EG_INT egcamera_y_map2scr(EG_CAMERA *camera, EG_SHORT ymap);

EG_SHORT egcamera_correct(EG_CAMERA *camera);
EG_SHORT egcamera_correct_x(EG_CAMERA *camera);
EG_SHORT egcamera_correct_y(EG_CAMERA *camera);
EG_SHORT egcamera_correct_x_inf(EG_CAMERA *camera);
EG_SHORT egcamera_correct_x_sup(EG_CAMERA *camera);
EG_SHORT egcamera_correct_y_inf(EG_CAMERA *camera);
EG_SHORT egcamera_correct_y_sup(EG_CAMERA *camera);

EG_SHORT egcamera_get_ycount(EG_CAMERA *camera);
EG_SHORT egcamera_get_xcount(EG_CAMERA *camera);
void egcamera_set_ycount(EG_CAMERA *camera, EG_SHORT cy);
void egcamera_set_xcount(EG_CAMERA *camera, EG_SHORT cx);
void egcamera_set_count(EG_CAMERA *camera, EG_SHORT cx, EG_SHORT cy);

void egcamera_optimize_ex(EG_CAMERA *camera, EG_SHORT w, EG_SHORT h);
void egcamera_optimize(EG_CAMERA *camera);
		
void egcamera_inc(EG_CAMERA *camera, EG_SHORT inc_x, EG_SHORT inc_y);
void egcamera_scy_dec(EG_CAMERA *camera, EG_SHORT dec);
void egcamera_scy_inc(EG_CAMERA *camera, EG_SHORT add);
void egcamera_scx_dec(EG_CAMERA *camera, EG_SHORT dec);
void egcamera_scx_inc(EG_CAMERA *camera, EG_SHORT add);

void egcamera_pgy_dec(EG_CAMERA *camera, EG_SHORT dec);
void egcamera_pgy_inc(EG_CAMERA *camera, EG_SHORT add);
void egcamera_pgx_dec(EG_CAMERA *camera, EG_SHORT dec);
void egcamera_pgx_inc(EG_CAMERA *camera, EG_SHORT add);

void init_egcamera(EG_CAMERA *camera, EG_SHORT spr_w, EG_SHORT spr_h,EG_SHORT *map_width, EG_SHORT *map_height);
void init_egcamera_ex(EG_CAMERA *camera, EG_SHORT screen_w, EG_SHORT screen_h,EG_SHORT spr_w, EG_SHORT spr_h,EG_SHORT *map_width, EG_SHORT *map_height);

EG_CAMERA *create_egcamera(EG_SHORT spr_w, EG_SHORT spr_h,EG_SHORT *map_width, EG_SHORT *map_height);
EG_CAMERA *create_egcamera_ex(EG_SHORT screen_w, EG_SHORT screen_h,EG_SHORT spr_w, EG_SHORT spr_h,EG_SHORT *map_width, EG_SHORT *map_height);
void destroy_egcamera(EG_CAMERA *camera);
// ********************************************************************
  
// *************************** MAP 2D *********************************
EG_MAP *egmap_duplicate(EG_MAP *dest, EG_MAP *map);
void egmap_clear(EG_MAP *map);
void egmap_clearto(EG_MAP *map, EG_SHORT val);

void egmap_put(EG_MAP *map,EG_SHORT pgx, EG_SHORT pgy,EG_SHORT val);
EG_SHORT egmap_get(EG_MAP *map,EG_SHORT pgx,EG_SHORT pgy);
		
EG_SHORT egmap_realloc(EG_MAP *map, EG_SHORT width, EG_SHORT height);
EG_SHORT egmap_malloc(EG_MAP *map, EG_SHORT width, EG_SHORT height);

void egmap_resize(EG_MAP *map, EG_SHORT width, EG_SHORT height);
void egmap_cresize(EG_MAP *map, EG_SHORT width, EG_SHORT height);

EG_SHORT egmap_filef_proc(void *eg_filef_data, void *data);
EG_MAP *create_egmap(EG_SHORT width, EG_SHORT height);
void destroy_egmap(EG_MAP *map);
// ********************************************************************

// ************************** CONSOLE ********************************
EG_CONSOLE *create_egconsole(EG_INT l, EG_INT c, EG_INT color, void (*draw)(BITMAP *bmp, EG_RECT *point));
void destroy_egconsole(EG_CONSOLE *egconsole);

void egconsole_setcursor(EG_CONSOLE *egconsole, EG_CHAR cursor);
void egconsole_setfont(EG_CONSOLE *egconsole, FONT *f);

void egconsole_draw(EG_CONSOLE *egconsole, BITMAP *bitmap, EG_INT x, EG_INT y);
void egconsole_addcmd(EG_CONSOLE *egconsole, void (*event)(EG_CONSOLE *egconsole, EG_INT argc, EG_CHAR *argv));
// *******************************************************************

// ************************** GUI ***********************************
EG_INT d_eganim2d_proc(EG_INT msg, DIALOG *d, EG_INT c);
// ******************************************************************

// *********************** File Format ******************************
//Erreur dans save et load de la classe CARTE
#define FF_PACKED 1
#define FF_NORMAL 0

#define AUTOVAR(pvar,size) if((((EG_FILEF *)eg_filef_data)->ret=egfilef_autovar_proc((EG_FILEF *)eg_filef_data,pvar,size))) return ((EG_FILEF *)eg_filef_data)->ret;

#define HEADER(str) if(((EG_FILEF *)eg_filef_data)->ret=egfilef_header_proc((EG_FILEF *)eg_filef_data,str)) return ((EG_FILEF *)eg_filef_data)->ret;

#define FF_LOAD ((EG_FILEF *)eg_filef_data)->load
#define FF_SAVE ((EG_FILEF *)eg_filef_data)->save

EG_FILEF *create_egfilef(EG_INT (*proc_file_format)(void *eg_filef_data,void *data), EG_INT mode);
EG_INT egfilef_load(EG_FILEF *egfilef, const EG_CHAR *filename, void *data);
EG_INT egfilef_save(EG_FILEF *egfilef, const EG_CHAR *filename, void *data);
void destroy_egfilef(EG_FILEF *egfilef);
EG_INT egfilef_autovar_proc(EG_FILEF *egfilef, void *pvar, EG_INT size);
EG_INT egfilef_header_proc(EG_FILEF *egfilef, const EG_CHAR *str);
// ******************************************************************

// ************************ Credit **********************************
//Pour crédit val retournée par egcredit_up
//si fini alors 1 sinon alors EG_CREDIT_OK (0).
#define EG_CREDIT_OK  0
//Le crdit est fini
#define EG_CREDIT_END 1

EG_INT egcredit_get_height(EG_CREDIT *egcredit);
void egcredit_add_bmp(EG_CREDIT *egcredit, BITMAP *bmp);
void egcredit_add_rle(EG_CREDIT *egcredit, RLE_SPRITE *rle);
void egcredit_add_text(EG_CREDIT *egcredit, EG_CHAR *text, FONT *f, EG_INT color);
void egcredit_add_space(EG_CREDIT *egcredit, EG_INT py); //SpaceY
void egcredit_add_eganim2d(EG_CREDIT *egcredit, EG_ANIM2D *anim, EG_INT loop);
EG_CREDIT *create_egcredit();
void destroy_egcredit(EG_CREDIT *egcredit);
EG_INT egcredit_moveup(EG_CREDIT *egcredit, BITMAP *buffer, EG_INT p);
EG_INT egcredit_draw(EG_CREDIT *egcredit, BITMAP *buffer);
EG_INT egcredit_moveup_draw(EG_CREDIT *egcredit, BITMAP *buffer, EG_INT p);
EG_INT egcredit_gety(EG_CREDIT *egcredit, BITMAP *buffer);
void egcredit_rew(EG_CREDIT *egcredit);
// ******************************************************************

// ********************* Animation de sprites ***********************
EG_INT eg_load_ega(const EG_CHAR *filename, EG_ANIM2D *eganim2d, PALETTE pal);
EG_INT eg_save_ega(const EG_CHAR *filename, EG_ANIM2D *eganim2d, PALETTE pal);
void frame_cpy(EG_FRAME *frame_dest, EG_FRAME *frame_src);
void frame_swap(EG_FRAME *frame1,EG_FRAME *frame2);
void eganim2d_delete_frame(EG_ANIM2D *anim, EG_INT index);
void eganim2d_insert_bmp(EG_ANIM2D *anim, EG_INT index, BITMAP *bmp, EG_INT delay);
void eganim2d_insert_rle(EG_ANIM2D *anim, EG_INT index, BITMAP *rle, EG_INT delay);
EG_INT eganim2d_get_frame(EG_ANIM2D *anim);
EG_ANIM2D *create_eganim2d(volatile EG_INT *counter);
void destroy_eganim2d(EG_ANIM2D *anim);
void eganim2d_clear(EG_ANIM2D *anim);
EG_INT eganim2d_add_bmp(EG_ANIM2D *anim, BITMAP *bitmap, EG_INT delay);
EG_INT eganim2d_add_rle(EG_ANIM2D *anim, RLE_SPRITE *bitmap, EG_INT delay);
EG_INT eganim2d_inc(EG_ANIM2D *anim);
EG_INT eganim2d_incloop(EG_ANIM2D *anim);
EG_INT eganim2d_draw_incloop(EG_ANIM2D *anim, BITMAP *buffer, EG_INT x, EG_INT y);
EG_INT eganim2d_draw_inc(EG_ANIM2D *anim, BITMAP *buffer, EG_INT x, EG_INT y);
EG_INT eganim2d_dec(EG_ANIM2D *anim);
EG_INT eganim2d_decloop(EG_ANIM2D *anim);
EG_INT eganim2d_draw_decloop(EG_ANIM2D *anim, BITMAP *buffer, EG_INT x, EG_INT y);
EG_INT eganim2d_draw_dec(EG_ANIM2D *anim, BITMAP *buffer, EG_INT x, EG_INT y);
void eganim2d_draw(EG_ANIM2D *anim, BITMAP *buffer, EG_INT x, EG_INT y);
void eganim2d_ff(EG_ANIM2D *anim);
void eganim2d_rew(EG_ANIM2D *anim);
EG_INT eganim2d_get_width(EG_ANIM2D *anim);
EG_INT eganim2d_get_height(EG_ANIM2D *anim);
void eganim2d_set_frame(EG_ANIM2D *anim, EG_INT n);
// ******************************************************************

// ************************ EG OBJECT ********************************
#define EGO_BMP      0
#define EGO_RLE      1
#define EGO_TEXT     2
#define EGO_EGANIM2D   3
#define EGO_SPACE    4

EG_OBJECT *create_egobject();
void egobject_init_bmp(EG_OBJECT *obj, BITMAP *bmp);
void egobject_init_rle(EG_OBJECT *obj, RLE_SPRITE *rle);
void egobject_init_eganim2d(EG_OBJECT *obj, EG_ANIM2D *anim, EG_INT loop);
void egobject_init_text(EG_OBJECT *obj, const EG_CHAR *text, FONT *f, EG_INT color);
void egobject_init_space(EG_OBJECT *obj, EG_INT px, EG_INT py);
void destroy_egobject(EG_OBJECT *obj);
EG_INT egobject_get_width(EG_OBJECT *obj);
EG_INT egobject_get_height(EG_OBJECT *obj);
void egobject_draw(EG_OBJECT *obj, BITMAP *bmp, EG_INT x, EG_INT y);
// *******************************************************************


// *********************** TRIGONOMETRIE *****************************
#define EG_PI 3.141592654
EG_INT optimize_angle(EG_INT);
double deg_sin(EG_INT);   //retourne le sin en degrs
double deg_cos(EG_INT);   //mme chose pour le cos
double deg_tan(EG_INT);   //mme chose pour la tangeante
double deg2rad(EG_INT);   //Convertit un degrs en valeur radian
// *******************************************************************

// ****************************** EG_CHEAT ******************************
typedef struct
{
        EG_UCHAR *code;
        EG_INT len;
		EG_CHAR enabled;
} EG_CHEAT_CODE;
typedef struct
{
        EG_INT timeout;
        volatile EG_INT* counter;
        EG_CHEAT_CODE *code;
        EG_INT sizeof_code;
        EG_INT actual_char;
        EG_INT actual_egcheat;
} EG_CHEAT;
#define EG_CHEAT_WRITE    -2         //L'utilisateur est entrain d'crire
#define EG_CHEAT_NONE     -1         //Il n'crit rien

EG_INT egcheat_enabled(EG_CHEAT *cheat, EG_INT index);
void egcheat_set_enabled(EG_CHEAT *cheat, EG_INT index, EG_INT enabled);
void init_egcheat(EG_CHEAT *cheat, EG_CHEAT_CODE *code, volatile EG_INT *counter);

void egcheat_set_timeout(EG_CHEAT *egcheat, EG_INT timeout);
EG_INT egcheat_get_timeout(EG_CHEAT *egcheat);
EG_CHEAT *create_egcheat(volatile EG_INT *counter);
void destroy_egcheat(EG_CHEAT *egcheat);
void egcheat_modify(EG_CHEAT *egcheat, EG_INT index, EG_UCHAR *code, EG_INT len);
EG_INT egcheat_add(EG_CHEAT *egcheat, EG_UCHAR *code, EG_INT len);
EG_INT egcheat_test(EG_CHEAT *egcheat);
// *******************************************************************

// ************************   MISC   *********************************
void eg_sc_inc(EG_SHORT *pg, EG_SHORT *sc, EG_SHORT size, EG_SHORT add);
void eg_sc_dec(EG_SHORT *pg, EG_SHORT *sc, EG_SHORT size, EG_SHORT dec);
void eg_inc(EG_SHORT *pg, EG_SHORT *sc, EG_SHORT size, EG_SHORT n);
// *******************************************************************

#endif
