#include <eg.h>

#define error(condition, message, arg2) \
    if(condition) { \
        set_gfx_mode(GFX_TEXT,80,25,0,0); \
        if(arg2) allegro_message(message,arg2); else allegro_message(message); \
        exit(1); \
    }

volatile int counter = 0;
void handle(void)
{
	counter++;
}
END_OF_FUNCTION(handle);

int main(void)
{
	ANIM2D *animation;
	BITMAP *sprite1, *sprite2;
	PALETTE pal;
	int ret;

	allegro_init();
	//if(set_gfx_mode(GFX_VGA,320,200,0,0)<0) return 1;
	install_keyboard();
	LOCK_VARIABLE(counter);
	LOCK_FUNCTION(handle);
	install_int(handle, 1);

	set_color_depth(16);
	if (set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0) < 0) return 1;
	sprite1 = load_bmp("snap1.bmp", pal);
	sprite2 = load_bmp("snap.bmp", pal);
	animation = create_anim2d(&counter);
	error(!animation, "Out of memory...\n", 0);

	anim2d_add_bmp(animation, sprite1, 500);
	anim2d_add_bmp(animation, sprite2, 500);

	while (!key[KEY_ESC])
	{
		anim2d_draw_incloop(animation, screen, 0, 0);
	}
	if (ret = save_ega("anim.ega", animation, pal))
	{
		set_gfx_mode(GFX_TEXT, 80, 25, 0, 0);
		printf("Save Erreur nø%i\n", ret);
		exit(1);
	}

	set_color_depth(16);
	if (set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0) < 0) return 1;

	if (ret = load_ega("anim.ega", animation, pal))
	{
		set_gfx_mode(GFX_TEXT, 80, 25, 0, 0);
		printf("Load Erreur nø%i\n", ret);
		exit(1);
	}

	//set_palette(pal);

	//set_palette(pal);
	while (!key[KEY_ESC])
	{
		anim2d_draw_incloop(animation, screen, 0, 0);
	}
	destroy_bitmap(sprite1);
	destroy_bitmap(sprite2);

	return 0;
}
END_OF_MAIN();
