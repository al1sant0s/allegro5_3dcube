#include <allegro5/allegro5.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#define REFRESH_RATE 1.0/60.0
#define KEY_SEEN 1
#define KEY_RELEASED 2
#define SW 960
#define SH 540

void SetAllegro(
		
		ALLEGRO_DISPLAY** display, unsigned displaywidth, unsigned displayheight,
		ALLEGRO_TIMER** timer, double refresh_rate,
		ALLEGRO_EVENT_QUEUE** queue, ALLEGRO_FONT** font, ALLEGRO_COLOR Colors[256][256][256]
		
);
