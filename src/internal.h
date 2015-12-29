#include <string.h>   //Gestion de strings
#include <stdlib.h>   //par ex. pour malloc
#include <math.h>     //Fonctions de trigonomtrie

#include <allegro.h>
#include "eg.h"

//Out of memory standart de EG
#define eg_out_of_memory()  set_gfx_mode(GFX_TEXT,80,25,0,0);\
                            allegro_message("Out of memory...\n");\
                            exit(1)

#define EG_CHEAT_DEFAULT_TIMEOUT 500           //Le delay par dfaut 500 msec

#define EG_MALLOC(p, size) p = malloc(size)
#define EG_REALLOC(p, size) p = realloc(p,size)
#define EG_FREE(p) free(p)


