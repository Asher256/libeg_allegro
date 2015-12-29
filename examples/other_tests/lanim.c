#include <allegro.h>
#include <eg.h>

EG_LANIM *anim;
#define W 30
#define H 30

void draw(void *anim, BITMAP *bmp, int x, int y)
{
	rectfill(screen,x,y,x+W,y+H,0);
	switch(((EG_LANIM *)anim)->actual_frame) {
	case 0:
		line(screen,x,y,x+W,y+H,15);
		line(screen,x+W,y,x,y+H,15);
		break;
	case 1:
		line(screen,x+W>>1,y,x+W>>1,y+H,14);
		line(screen,x,y+H>>1,x+W,y+H>>1,14);
		break;
	case 2:
		rect(screen,x,y,x+W,y+H,13);
		break;
	case 3:
		rectfill(screen,x,y,x+W,y+H,10);
		break;
	}
}

int info(void *anim, int type)
{
	switch(type)
	{
	case EG_LANIM_MAXFRAME:
		return 4;
	case EG_LANIM_NEXTFRAME:
		return TRUE;
	case EG_LANIM_WIDTH:
		return W;
	case EG_LANIM_HEIGHT:
		return H;
	}
	return 0;
}


int main(void)
{
	allegro_init();
	if(set_gfx_mode(GFX_AUTODETECT,320,200,0,0)<0) return 1;
	install_keyboard();
	install_timer();

	anim = create_eglanim(draw,info);

	while(!key[KEY_ESC]) {
		eglanim_draw_decloop(anim,screen,10,10);
		rest(50);
	}

	destroy_eglanim(anim);
}
END_OF_MAIN();

