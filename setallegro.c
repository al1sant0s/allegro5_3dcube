#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

void check_component(bool status, char description[]){

	if(!status){

		fprintf(stderr, "Could not %s!\nExiting process now.\n", description);
		exit(EXIT_FAILURE);

	}

}

void SetAllegro(
		
		ALLEGRO_DISPLAY** display, unsigned displaywidth, unsigned displayheight,
		ALLEGRO_TIMER** timer, double refresh_rate,
		ALLEGRO_EVENT_QUEUE** queue, ALLEGRO_FONT** font, ALLEGRO_COLOR Colors[256][256][256]
		
){

	//Allegro 5 Start Routines

	check_component(al_init(), "start Allegro");
	check_component(al_install_keyboard(), "install the keyboard");
	check_component(al_init_image_addon(), "start image addon");
	check_component(al_init_primitives_addon(), "start primitives addon");
	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

	//Set up the basic components and load assets

	ALLEGRO_DISPLAY* Display = al_create_display(displaywidth, displayheight);
	ALLEGRO_TIMER* Timer = al_create_timer(refresh_rate);
	ALLEGRO_EVENT_QUEUE* Queue = al_create_event_queue();
	ALLEGRO_FONT* Font = al_create_builtin_font();

	check_component(Display, "create a display");
	check_component(Timer, "create a timer");
	check_component(Queue, "create an event queue");
	check_component(Font, "create a builtin font");

	//Register events sources and start timer

	al_register_event_source(Queue, al_get_keyboard_event_source());
	al_register_event_source(Queue, al_get_display_event_source(Display));
	al_register_event_source(Queue, al_get_timer_event_source(Timer));

	*display = Display;
	*timer = Timer;
	*queue = Queue;
	*font = Font;

	//Create all the colors
	
	if(Colors != NULL){
	
		for(unsigned i = 0; i < 256; i++){

			for(unsigned j = 0; j < 256; j++){

				for(unsigned k = 0; k < 256; k++){
				
					Colors[i][j][k] = al_map_rgb(i, j, k);

				}
			
			}
		
		}
	
	}

}
