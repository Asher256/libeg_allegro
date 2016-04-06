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
 * Description: Exemple de egcheat code avec Eg Library.
 *
 * Auteur:      Asher256
 * Email:       asher256@gmail.com
 *
 ******************************************************************/

#include "header.h"

/* L'index des egcheat */
int egcheat_index1, egcheat_index2, ret;

/* Les egcheats (scancode) */
unsigned char code1[] = {KEY_E, KEY_G, KEY_T, KEY_I, KEY_P, KEY_S, 0x00};
unsigned char code2[] = {KEY_H, KEY_E, KEY_L, KEY_L, KEY_O, 0x00};

/* Initialisation d'Allegro */
void init()
{
	//Initialisation d'allegro
	std_allegro_init(640, 480, 8);

	install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, 0);

	set_volume(255, 255);
	std_load_datafile();
}

int main()
{
	EG_CHEAT *egcheat;

	init();

	// Cration du egcheat
	egcheat = create_egcheat(&counter1);
	// Si pas assez de mmoire
	if (!egcheat)
	{
		set_gfx_mode(GFX_TEXT, 80, 25, 0, 0);
		allegro_message("Pas assez de mmoire...\n");
		allegro_exit();
	}

	// Ajoute les egcheats dans la variable `egcheat`
	egcheat_index1 = egcheat_add(egcheat, code1, strlen(code1));
	egcheat_index2 = egcheat_add(egcheat, code2, strlen(code2));

	//Boucle principale
	while (!key[KEY_ESC])
	{
		clear_bitmap(buffer);
		textout_centre(buffer, font, "Demonstration de la gestion de EG_CHEAT en temps reel", SCREEN_W >> 1, 30, 12);

		/** Routines princiaple  qui teste le egcheat tap **/
		ret = egcheat_test(egcheat);

		//s'il a fait un egcheat alors mettre un son
		if (ret >= 0) play_sample((SAMPLE *)datafile[sample_ok].dat, 255, 128, 1000, 0);

		// Et affiche le message du egcheat correspondent
		if (ret == egcheat_index1)
		{
			clear_bitmap(screen);
			textout_centre(screen, font, "\"EGTIPS\": Cheat ok !!!", SCREEN_W >> 1, SCREEN_H >> 1, 10);
			while (!key[KEY_ENTER]);
		}
		//Si c'est le deuxime egcheat "hello"
		else if (ret == egcheat_index2)
		{
			clear_bitmap(screen);
			textout_centre(screen, font, "\"HELLO\": Cheat ok !!!", SCREEN_W >> 1, SCREEN_H >> 1, 10);
			while (!key[KEY_ENTER]);
		}

		//S'il n'crit aucun egcheat
		if (ret == EG_CHEAT_NONE)
			textout(buffer, font, "Pas de egcheat...", 0, 0, 15);

		//S'il est entrain d'crire le egcheat
		else if (ret == EG_CHEAT_WRITE)
			textout(buffer, font, "Vous etes entrain d'ecrire le egcheat...", 0, 0, 15);

		//Affichage
		textout_centre(buffer, font, "Ecrivez le egcheat: \"EGTIPS\" ou \"HELLO\".", SCREEN_W >> 1, SCREEN_H >> 1, 14);

		draw_buffer();   //Affiche le buffer
	}

	// Ne pas oublier de librer la mmoire !!!
	destroy_egcheat(egcheat);

	bye();  //Quitter
	return 0;
}
END_OF_MAIN();
