/******************************************************************
 *                      ______   ________
 *                     /\   __\ /\   __  \
 *                     \ \  \___\ \  \ \__\
 *                      \ \   __\\ \  \  ___
 *                       \ \  \___\ \  \_\  \
 *                        \ \_____\\ \_______\
 *                         \/____/  \________/
 *
 *                     *  Enhanced Game Library *
 *
 *
 * Description: Exemple de egcredit avec EG Library.
 *
 * Auteur:      Asher256
 * Email:       contact@asher256.com
 *
 ******************************************************************/

#include "header.h"

#define COL1 makecol(0xBB,0x00,0x55)
#define COL2 makecol(0xFF,0xFF,0xFF)
#define CLEAR_COL makecol(0x00,0x00,0x22)

int main()
{
	int ret = 0;
	EG_CREDIT *egcredit;  //Le crdit

	//Initialisation d'allegro
	std_allegro_init(640, 480, 16);
	install_sound(DIGI_NONE, DIGI_AUTODETECT, 0);
	set_volume(255, 255);
	std_load_datafile();

	egcredit = create_egcredit();

	//**************** INITIALISATION DU EG_CREDIT ********************
	egcredit_add_text   (egcredit, "Enhanced Game Library", (FONT*)datafile[font2].dat, COL1);
	egcredit_add_bmp    (egcredit, datafile[bmp_eglogo].dat);           //EG LIBRARY
	egcredit_add_space  (egcredit, 12);

	egcredit_add_text   (egcredit, "Programmation", (FONT*)datafile[font1].dat, COL1);
	egcredit_add_text   (egcredit, "Asher256", (FONT*)datafile[font2].dat, COL2);
	egcredit_add_space  (egcredit, 15);

	egcredit_add_text   (egcredit, "Graphisme et design", (FONT*)datafile[font1].dat, COL1);
	egcredit_add_text   (egcredit, "Asher256", (FONT*)datafile[font2].dat, COL2);
	egcredit_add_space  (egcredit, 15);

	egcredit_add_text   (egcredit, "Musique", (FONT*)datafile[font1].dat, COL1);
	egcredit_add_text   (egcredit, "Asher256", (FONT*)datafile[font2].dat, COL2);
	egcredit_add_space  (egcredit, 15);

	egcredit_add_text   (egcredit, "Remerciments speciales", (FONT*)datafile[font1].dat, COL1);
	egcredit_add_text   (egcredit, "Niki labelle", (FONT*)datafile[font2].dat, COL2);
	egcredit_add_text   (egcredit, "Radosh", (FONT*)datafile[font2].dat, COL2);
	egcredit_add_text   (egcredit, "Schwardzenegger :)", (FONT*)datafile[font2].dat, COL2);
	egcredit_add_text   (egcredit, "Guybrush treepwood", (FONT*)datafile[font2].dat, COL2);
	egcredit_add_text   (egcredit, "Madona mia", (FONT*)datafile[font2].dat, COL2);
	egcredit_add_space  (egcredit, 15);

	egcredit_add_text   (egcredit, "Information sur le egcredit", (FONT*)datafile[font1].dat, COL1);
	egcredit_add_text   (egcredit, "Ce egcredit est fait avec la classe \"Credit\" de \"EG Library\"", (FONT*)datafile[font2].dat, COL2);
	egcredit_add_text   (egcredit, "(Enhanced Game library), Par Asher256", (FONT*)datafile[font2].dat, COL2);

	//**************** finegcredit, de L'INITIALISATION ********************

	egcredit_rew(egcredit);

	text_mode(-1);  //met le texte d'allegro transparent

	play_midi((MIDI *)datafile[midi_credit].dat, 1);

	// Dbut de la boucle
	while (!key[KEY_ESC])
	{

		clear_to_color(buffer, CLEAR_COL);

		//Il fait monter le crdit et l'affiche dans le buffer
		ret = egcredit_moveup_draw(egcredit, buffer, 1);   //monte de 1 pixel (vitesse)

		textprintf(buffer, font, 2, 2, COL2, "EG_CREDIT_Y_POSITION:%i EG_CREDIT_HEIGHT:%i", egcredit_gety(egcredit, buffer), egcredit_get_height(egcredit));

		//Si le egcredit est fini il affiche un petit message
		if (ret == EG_CREDIT_END)
		{
			textout_centre(buffer, (FONT *)datafile[font1].dat, "END OF EG_CREDIT SCROLLER", SCREEN_W >> 1, SCREEN_H >> 1, COL1);
		}

		//Affiche le buffer  l'cran
		draw_buffer();

		//Simulation de vsync
		while (!(counter1 >= 20));
		counter1 = 0;
	}

	stop_midi();

	destroy_egcredit(egcredit);

	//fin !!
	bye();
	exit(0);
	return 0;
}
END_OF_MAIN();
