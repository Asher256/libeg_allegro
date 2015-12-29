//************************
//* Eg Animator
//* By Asher256 
//************************

#include <allegro.h>
#include <eg.h>

#include "data.h"

// Dclaration des fonctions principales du programme...
int mnu_copier_proc();
int mnu_couper_proc();
int mnu_coller_proc();
int mnu_changedepth_proc();
int mnu_about_proc();
int mnu_end_proc();
int mnu_saveas_proc();
int mnu_new_proc();
int mnu_open_proc();
int mnu_save_proc();
int loop_check_proc(int msg, DIALOG *d, int c);
int deplacer_vers_le_haut_button_proc(int msg, DIALOG *d, int c);
int deplacer_vers_le_bas_button_proc(int msg, DIALOG *d, int c);
int liste_de_bmp(int msg, DIALOG *d, int c);
int animation_proc(int msg, DIALOG *d, int c);
void bye();                             // Desallocation
void init_allegro();                    // Initialisation d'Allegro et DLG
char *list_getter(int i, int *size);    // Prend de la liste (interne)
void update_data();                     // De data vers cran
int ajouterframeapres_button_proc(int msg, DIALOG *d, int c);
int delete_button_proc(int msg, DIALOG *d, int c);
void ajouter_frame(char *path, char mode);
int ff_button_proc(int msg, DIALOG *d, int c);
int rew_button_proc(int msg, DIALOG *d, int c);
int stop_button_proc(int msg, DIALOG *d, int c);
int play_button_proc(int msg, DIALOG *d, int c);
int ajouterframe_button_proc(int msg, DIALOG *d, int c);
void configure_gui_colors();
int ega_button_proc(int msg, DIALOG *d, int c);

PALETTE palette;
char filename[80] = {0}; // Le fichier actuellement ouvert

// Le depth + les pointeurs
int depth = 8;
BITMAP *apercu = NULL;
EG_ANIM2D *animation = NULL;

DATAFILE *datafile = 0;
char *data_filename = "data.dat";

// Path de ajouterframe
static char path[80] = {0};

//*** TIMERS ***
volatile int animation_counter = 0;
void timer_handler(void);

//*** Variables pour le copier/coller/couper ***/
EG_FRAME frame_copied;
int frame_copied_flag = FALSE; //True s'il ya quelque chose dans cette frame

// Le W et H de l'apercu
#define BMPW 435
#define BMPH 352

// Les offsets dans dlg[] des objets les plus utiliss
#define LABEL_DEPTH           22
#define LABEL_MAXFRAME        23
#define LABEL_ACTUALFRAME     24
#define LISTBOX               1
#define ANIMATION             2

#define BG_COLOR              26

#define BEGIN_BUTTON_CLICK() int ret; ret = ega_button_proc(msg,d,c); if(ret==D_CLOSE) { ega_button_proc(MSG_DRAW,d,c);
#define END_BUTTON_CLICK()   return D_O_K; } return ret;

MENU menu_fichier[] = {{"&New", mnu_new_proc, 0, 0, 0},
	{"&Open...", mnu_open_proc, 0, 0, 0},
	{"&Save", mnu_save_proc, 0, 0, 0},
	{"Save &as...", mnu_saveas_proc, 0, 0, 0},
	{"", 0, 0, 0, 0},
	{"&Quit", mnu_end_proc, 0, 0, 0},
	{0, 0, 0, 0, 0}
};

MENU menu_edition[] =
{
	{"Cu&t", mnu_couper_proc, 0, 0, 0},
	{"&Copy", mnu_copier_proc, 0, 0, 0},
	{"&Paste", mnu_coller_proc, 0, 0, 0},
	{"", 0, 0, 0, 0},
	{"Change &gfx Depth", mnu_changedepth_proc, 0, 0, 0},
	{0, 0, 0, 0, 0}
};

MENU menu_aide[] =
{
	{"&About", mnu_about_proc, 0, 0, 0},
	{0, 0, 0, 0, 0}
};

MENU menu_principal[] = {{"&File", 0, menu_fichier, 0, 0},
	{"&Edit", 0, menu_edition, 0, 0},
	{"&Help", 0, menu_aide, 0, 0},
	{0, 0, 0, 0, 0}
};

DIALOG dlg[] =
{
	/* (proc)                             (x)    (y)   (w)   (h) (fg)        (bg)  (key) (flags)       (d1) (d2) (dp)               (dp2) (dp3) */
	{ d_keyboard_proc,                     0,     0,    0,    0,   0,          0,    0,    0,          0,   0,   NULL,              NULL, NULL },
	{ liste_de_bmp,                        8,    32,  184,  409,   0,         15,    0,    0,          0,   0,   list_getter,       NULL, NULL },
	{ animation_proc,                    200,   120, BMPW, BMPH,   0,          0,    0,    0,          TRUE, 0,   NULL,            NULL, NULL },
	{ rew_button_proc,                    64,   448,   32,   24,   0,         15,    0,    D_EXIT,     0,   0,   "&Rew",            NULL, NULL },
	{ ff_button_proc,                    104,   448,   32,   24,   0,         15,    0,    D_EXIT,     0,   0,   "&FF",             NULL, NULL },
	{ stop_button_proc,                  144,   448,   48,   24,   0,         15,    0,    D_EXIT,     0,   0,   "&Stop",           NULL, NULL },
	{ d_box_proc,                        200,    32,  168,   80,  16,   BG_COLOR,    0,    0,          0,   0,   NULL,              NULL, NULL },
	{ ajouterframeapres_button_proc,     240,    64,  120,   16,   0,         15,    0,    D_EXIT,     0,   0,   "Add &Before", NULL, NULL },
	{ ajouterframe_button_proc,          240,    40,  120,   16,   0,         15,    0,    D_EXIT,     0,   0,   "Add &After", NULL, NULL },
	{ play_button_proc,                    8,   448,   48,   24,   0,         15,    0,    D_EXIT,     0,   0,   "&Play",           NULL, NULL },
	{ delete_button_proc,                208,    88,  152,   16,   0,         15,    0,    D_EXIT,     0,   0,   "&Delete Frame",   NULL, NULL },
	{ deplacer_vers_le_haut_button_proc, 208,    40,   24,   16,   0,         15,    0,    D_EXIT,     0,   0,   "/\\",             NULL, NULL },
	{ deplacer_vers_le_bas_button_proc,  208,    64,   24,   16,   0,         15,    0,    D_EXIT,     0,   0,   "\\/",             NULL, NULL },
	{ d_ctext_proc,                      240,    28,   29,    8,   0,    BG_COLOR,   0,    0,          0,   0,   " Frames ",        NULL, NULL },
	{ d_box_proc,                        376,    32,  260,   79,   0,    BG_COLOR,   0,    0,          0,   0,   NULL,              NULL, NULL },
	{ d_ctext_proc,                      421,    28,   80,    8,   0,    BG_COLOR,   0,    0,          0,   0,   " Options ",       NULL, NULL },
	{ loop_check_proc,                   385,    41,  241,   11,   0,    BG_COLOR,   0,    D_SELECTED, 1,   0,   " Play in Loop",   NULL, NULL },
	{ d_box_proc,                        376,    64,  260,   48,   0,    BG_COLOR,   0,    0,          0,   0,   NULL,              NULL, NULL },
	{ d_ctext_proc,                      414,    60,   88,    8,   0,    BG_COLOR,   0,    0,          0,   0,   " Infos ",         NULL, NULL },
	{ d_text_proc,                       384,    72,   48,    8,   0,    BG_COLOR,   0,    0,          0,   0,   "Depth:",          NULL, NULL },
	{ d_text_proc,                       384,    84,   72,    8,   0,    BG_COLOR,   0,    0,          0,   0,   "Max Frame:",      NULL, NULL },
	{ d_text_proc,                       384,    96,  100,   12,   0,    BG_COLOR,   0,    0,          0,   0,   "Actual Frame:",   NULL, NULL },
	{ d_text_proc,                       496,    72,   76,    8,   0,    BG_COLOR,   0,    0,          0,   0,   "8",               NULL, NULL },
	{ d_text_proc,                       496,    84,   76,   12,   0,    BG_COLOR,   0,    0,          0,   0,   "0",               NULL, NULL },
	{ d_text_proc,                       496,    96,   76,    8,   0,    BG_COLOR,   0,    0,          0,   0,   "0",               NULL, NULL },
	{ d_menu_proc,                         0,     0,    0,    0,   0,           0,   0,    0,          0,   0,   menu_principal,    NULL, NULL },
	{ NULL,                                0,     0,    0,    0,   0,           0,   0,    0,          0,   0,   NULL,              NULL, NULL }
};

// *** Changer le depth (d'affichage)
int mnu_changedepth_proc()
{
	return D_O_K;
}

// Copier la frame actuelle dans la mmoire
int mnu_copier_proc()
{
	frame_copied_flag = TRUE;
	frame_cpy(&frame_copied, &animation->frame[dlg[LISTBOX].d1]);
	return D_O_K;
}

// Coller dans la frame actuelle
int mnu_coller_proc()
{
	if (frame_copied_flag)
	{
		frame_cpy(&animation->frame[dlg[LISTBOX].d1], &frame_copied);
		update_data();
	}
	return D_O_K;
}

// Couper cette frame
int mnu_couper_proc()
{
	mnu_copier_proc();
	eganim2d_delete_frame(animation, dlg[LISTBOX].d1);
	frame_copied_flag = TRUE;
	update_data();
	return D_O_K;
}

// ABOUT
int mnu_about_proc()
{
	alert("EG Animator Version 1.0", "Useful program to create EGA Animation", "By Asher256", "&Ok", 0, 0, 0);
	return D_O_K;
}

// ** MENU NOUVEAU
int mnu_new_proc()
{
	if (alert("Are you sure to create new Animation?", 0, 0, "&Ok", "&No", 0, 0) == 1)
	{
		ustrcpy(filename, "");
		eganim2d_clear(animation);
		update_data();
	}
	return D_O_K;
}

// ** MENU OUVRIR **
int mnu_open_proc()
{
	return D_O_K;
}

// ** MENU SAUVER **
int mnu_save_proc()
{
	return D_O_K;
}

// ** MENU SAUVER SOUS **
int mnu_saveas_proc()
{
	return D_O_K;
}

int mnu_end_proc()
{
	return D_CLOSE;
}

int loop_check_proc(int msg, DIALOG *d, int c)
{
	int ret = d_check_proc(msg, d, c);

	if (msg == MSG_CLICK || msg == MSG_CHAR)
	{
		if (dlg[ANIMATION].d1) dlg[ANIMATION].d1 = FALSE;
		else dlg[ANIMATION].d1 = TRUE;
		return ret;
	}
	return ret;
}

int deplacer_vers_le_bas_button_proc(int msg, DIALOG *d, int c)
{
	BEGIN_BUTTON_CLICK();
	if (dlg[LISTBOX].d1 < animation->max_frame - 1)
	{
		frame_swap(&animation->frame[dlg[LISTBOX].d1 + 1], &animation->frame[dlg[LISTBOX].d1]);
		dlg[LISTBOX].flags |= D_DIRTY;
		dlg[LISTBOX].d1++;
		update_data();
	}
	END_BUTTON_CLICK();
}

int deplacer_vers_le_haut_button_proc(int msg, DIALOG *d, int c)
{
	BEGIN_BUTTON_CLICK();
	if (dlg[LISTBOX].d1 > 0)
	{
		frame_swap(&animation->frame[dlg[LISTBOX].d1 - 1], &animation->frame[dlg[LISTBOX].d1]);
		dlg[LISTBOX].flags |= D_DIRTY;
		dlg[LISTBOX].d1--;
		update_data();
	}
	END_BUTTON_CLICK();
}

int liste_de_bmp(int msg, DIALOG *d, int c)
{
	int ret = d_list_proc(msg, d, c);

	// Si click
	if (msg == MSG_CLICK ||  msg == MSG_CHAR)
	{
		animation->actual_frame = dlg[LISTBOX].d1;
		update_data();
	}
	return ret;
}

int animation_proc(int msg, DIALOG *d, int c)
{
	static int ret;
	int actual_frame = animation->actual_frame;

	if (msg == MSG_START || msg == MSG_END) return D_O_K;

	ret = d_eganim2d_proc(msg, d, c);

	// Si l'animation a change alors mettre  jour l'cran
	if (actual_frame != animation->actual_frame)
		update_data();

	return ret;
}

void timer_handler(void)
{
	animation_counter++;
}
END_OF_FUNCTION(timer_handler);

int ega_button_proc(int msg, DIALOG *d, int c)
{
	return d_button_proc(msg, d, c);
}

int ff_button_proc(int msg, DIALOG *d, int c)
{
	BEGIN_BUTTON_CLICK();
	eganim2d_ff(animation);
	// Affichage de la forme ;)
	update_data();
	END_BUTTON_CLICK();
}

// fonction principale d'ajout de frame
// mode = TRUE alors aprs sinon, alors avant
void ajouter_frame(char *path, char mode)
{
	BITMAP *bmp;

	if (!exists(path))
	{
		alert("File not exist...", 0, 0, "ok", 0, 0, 0);
		return;
	}

	// rallocation
	bmp = load_bitmap(path, palette);
	if (!bmp)
	{
		alert("Error in loading file...", 0, 0, "ok", 0, 0, 0);
	}

	if (!mode)
		eganim2d_insert_bmp(animation, dlg[LISTBOX].d1, bmp, 100);
	else
		eganim2d_insert_bmp(animation, dlg[LISTBOX].d1 + 1, bmp, 100);
}

int ajouterframeapres_button_proc(int msg, DIALOG *d, int c)
{
	//static BITMAP *bmp;
	BEGIN_BUTTON_CLICK();
	if (file_select("Add Frame", path, "BMP;PCX;LBM"))
	{
		ajouter_frame(path, TRUE);
		update_data();
	}
	END_BUTTON_CLICK();
}

int delete_button_proc(int msg, DIALOG *d, int c)
{
	BEGIN_BUTTON_CLICK();
	if (alert("Are you sure to delete this frame?", 0, 0, "&Ok", "&No", 0, 0) == 1)
	{
		eganim2d_delete_frame(animation, dlg[LISTBOX].d1);
		update_data();
	}
	END_BUTTON_CLICK();
}

int ajouterframe_button_proc(int msg, DIALOG *d, int c)
{
	//static BITMAP *bmp;
	BEGIN_BUTTON_CLICK();
	if (file_select("Add Frame", path, "BMP;PCX;LBM"))
		ajouter_frame(path, FALSE);
	END_BUTTON_CLICK();
}

int rew_button_proc(int msg, DIALOG *d, int c)
{
	BEGIN_BUTTON_CLICK();
	eganim2d_rew(animation);
	update_data();
	END_BUTTON_CLICK();
}

int play_button_proc(int msg, DIALOG *d, int c)
{
	BEGIN_BUTTON_CLICK();
	dlg[ANIMATION].d2 = TRUE;
	END_BUTTON_CLICK();
}

int stop_button_proc(int msg, DIALOG *d, int c)
{
	BEGIN_BUTTON_CLICK();
	eganim2d_rew(animation);
	dlg[ANIMATION].d2 = FALSE;
	END_BUTTON_CLICK();
}

// Retourne un vrac de la liste
char *list_getter(int i, int *size)
{
	static char string[50];
	*size = animation->max_frame;
	if (i >= *size || i < 0) return 0;
	usprintf(string, "F%i> w:%i h:%i d:%i", i, *animation->frame[i].width, *animation->frame[i].height, animation->frame[i].msec);
	return string;
}

// Configuration auto des couleurs de la gui
void configure_gui_colors()
{
	gui_bg_color = makecol(255, 255, 255);
	gui_fg_color = makecol(0, 0, 0);
}

void init_allegro()
{
	int i;
	BITMAP *swap_bmp;
	allegro_init();
	atexit(bye);
	install_joystick(JOY_TYPE_AUTODETECT);
	if (set_gfx_mode(GFX_SAFE, 640, 480, 0, 0) < 0)
	{
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("%s\n", allegro_error);
		exit(1);
	}
	install_mouse();
	install_keyboard();
	install_timer();

	LOCK_FUNCTION(timer_handler);
	LOCK_VARIABLE(animation_counter);
	install_int(timer_handler, 1);

	set_palette(default_palette);

	animation = create_eganim2d(&animation_counter);

	if (!animation)
	{
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Out of memory...\n");
		exit(1);
	}

	datafile = load_datafile(data_filename);
	if (!datafile)
	{
		datafile = load_datafile("#");
		if (!datafile)
		{
			set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
			allegro_message("Error in loading %s...\n", data_filename);
			exit(1);
		}
	}

	dlg[LISTBOX].dp = list_getter;              //La liste
	dlg[ANIMATION].dp = animation;              //L'apercu (bmp)

	swap_bmp = create_bitmap(32, 32);
	clear_to_color(swap_bmp, 5);
	eganim2d_add_bmp(animation, swap_bmp, 100);
	swap_bmp = create_bitmap(32, 32);
	clear_to_color(swap_bmp, 4);
	eganim2d_add_bmp(animation, swap_bmp, 100);

	// Met la palette par dfaut
	for (i = 0; i < 256; i++)
	{
		palette[i].r = default_palette[i].r;
		palette[i].g = default_palette[i].g;
		palette[i].b = default_palette[i].b;
	}

	configure_gui_colors();

	font = datafile[THIN].dat;
	clear_to_color(screen, BG_COLOR);
}

// Mise  jour du data  l'ecran
void update_data()
{
	static char number[20];
	static char number2[20];
	static char number3[20];

	usprintf(number, "%i", depth);
	dlg[LABEL_DEPTH].dp = number;

	usprintf(number2, "%i", animation->max_frame);
	dlg[LABEL_MAXFRAME].dp = number2;

	usprintf(number3, "%i", animation->actual_frame);
	dlg[LABEL_ACTUALFRAME].dp = number3;

	dlg[LISTBOX].d1 = animation->actual_frame;

	(*dlg[LISTBOX].proc)(MSG_DRAW, &dlg[LISTBOX], 0);
	(*dlg[LABEL_MAXFRAME].proc)(MSG_DRAW, &dlg[LABEL_MAXFRAME], 0);
	(*dlg[LABEL_ACTUALFRAME].proc)(MSG_DRAW, &dlg[LABEL_ACTUALFRAME], 0);
	(*dlg[LABEL_DEPTH].proc)(MSG_DRAW, &dlg[LABEL_DEPTH], 0);
	(*dlg[ANIMATION].proc)(MSG_DRAW, &dlg[ANIMATION], 0);
}

// Desallocation
void bye()
{
	if (animation)
	{
		destroy_eganim2d(animation);
		animation = 0;
	}
	if (datafile)
	{
		unload_datafile(datafile);
		datafile = 0;
	}
}

int main(int argc, char **argv)
{
	init_allegro();

debut:
	if (do_dialog(dlg, -1))
	{
		if (alert("Really want to quit?", 0, 0, "&Ok", "&No", 0, 0) != 1)
			goto debut;
	}

	bye();
	return 0;
}
END_OF_MAIN();

