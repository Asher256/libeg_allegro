#include "header.h"

int main(void)
{
	EG_CONSOLE *console;

	std_allegro_init(640, 480, 16);

	console = create_egconsole(40, 30, 15, 0);
	if (!console)
	{
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Pas assez de memoire...\n");
		bye();
		return 1;
	}

	while (!key[KEY_ESC])
	{
		poll_keyboard();


		// Affichage
		clear_bitmap(buffer);
		draw_buffer();
	}

	destroy_egconsole(console);
	bye();
}
END_OF_MAIN();
