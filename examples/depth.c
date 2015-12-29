#include <allegro.h>

int main(void)
{
	BITMAP *bmp;
	PALETTE pal;
	allegro_init();
	if (set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0) < 0)
	{
		set_gfx_mode(GFX_TEXT, 80, 25, 0, 0);
		allegro_message("Erreur dans le chargement de l'ecran graphique...\n");
		return 1;
	}
	install_keyboard();

	bmp = load_bmp("snap.bmp", pal);
	if (!bmp)
	{
		set_gfx_mode(GFX_TEXT, 80, 25, 0, 0);
		allegro_message("Erreur dans le chargement du fichier snap.bmp...\n");
		return 1;
	}
	set_palette(pal);

	blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

	readkey();

	set_color_depth(24);
	if (set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0) < 0)
	{
		set_gfx_mode(GFX_TEXT, 80, 25, 0, 0);
		allegro_message("Erreur dans le chargement de l'ecran graphique...\n");
		return 1;
	}

	blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

	readkey();

	destroy_bitmap(bmp);

	return 0;
}

