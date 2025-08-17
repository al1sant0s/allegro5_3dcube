#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "include/AllegroDefine.h"
#include "include/misc.h"
#include "include/base3d.h"
#define W 3.555555555556
#define H 2.0
#define FOV 75
#define DRAWDISTANCE 30

ALLEGRO_COLOR GlobalColors[256][256][256];

extern struct Vec3d NullVector;
extern struct Vec3d UnitVecX;
extern struct Vec3d UnitVecY;
extern struct Vec3d UnitVecZ;

int main(void){

	//Allegro Setup
	ALLEGRO_DISPLAY* Display;
	ALLEGRO_TIMER* Timer;
	ALLEGRO_EVENT_QUEUE* Queue;
	ALLEGRO_FONT* Font;
	ALLEGRO_EVENT Event;

	SetAllegro(&Display, SW, SH, &Timer, REFRESH_RATE, &Queue, &Font, GlobalColors);
	unsigned key[ALLEGRO_KEY_MAX] = {0};
	enum colors {Blue, Red, Yellow, Green, Purple, White};
	ALLEGRO_COLOR basic_colors[] = {
		[Red] = al_map_rgb(255,0,0),
		[Green] = al_map_rgb(0,255,0),
		[Blue] = al_map_rgb(0,0,255),
		[Yellow] = al_map_rgb(255,255,0), 
		[Purple] = al_map_rgb(128,0,128), 
		[White] = al_map_rgb(255,255,255),
	};

	ALLEGRO_VERTEX* Vertices = malloc(sizeof(ALLEGRO_VERTEX) * 10 * 200000);

	//3D Setup
	struct Vec3d Vec[50] = {{0.0}};
	NODE PolyList = {NULL, NULL, NULL};
	NODE TextureList = {NULL, NULL, NULL};
	struct Polygon* Poly;
	struct Texture* Texture;

	//Set a camera
	struct Camera3d Cam;

	ScaleVector(&UnitVecX, -15.0, &Vec[0]);
	ScaleVector(&UnitVecY, 0.0, &Vec[1]);
	ScaleVector(&UnitVecZ, 0.0, &Vec[2]);
	AddVectors(&Vec[0], &Vec[1], &Vec[3]);
	AddVectors(&Vec[3], &Vec[2], &Vec[3]);
	SetCamera3d(&Cam, &Vec[3], &UnitVecX, &UnitVecY, &UnitVecZ, DEGREE_TO_RADIAN(FOV), 5.0E-02, 5.0E-01, DRAWDISTANCE, W, H, 0, 0, 0);

	//Create a cube
	AddPolygonToList(&PolyList, 12);
	AddTextureToList(&TextureList, "image/wooden_crate.png");
	AddTextureToList(&TextureList, "image/Brick Texture 86_3D_shrinked.png");
	AddTextureToList(&TextureList, "image/abstract_yellow.png");

	Poly = GetPolygonFromList(&PolyList, 1);

	float tam[2] = {128.0, 2.0};

	puts("Now rendering\n");
	SetTriangleFace(

			Poly->TriangleFace[0], &basic_colors[Blue], GetTextureFromList(&TextureList, 1),
			2.0f/3.0f, 2.0f/3.0f, 4.0, DEGREE_TO_RADIAN(0),
		       	-0.0, -tam[0], -tam[0], 
			-0.0, tam[0], tam[0], 
			-0.0, tam[0], -tam[0]

	);

	SetTriangleFace(

			Poly->TriangleFace[1], &basic_colors[Blue], GetTextureFromList(&TextureList, 1),
			1.0f/3.0f, 1.0f/3.0f, 4.0, DEGREE_TO_RADIAN(0),
		       	-0.0, -tam[0], -tam[0], 
			-0.0, -tam[0], tam[0], 
			-0.0, tam[0], tam[0]

	);

	ResizeTriangleFace(Poly->TriangleFace[0], tam[1]/(2.0 * tam[0]));
	ResizeTriangleFace(Poly->TriangleFace[1], tam[1]/(2.0 * tam[0]));
	

	tam[1] /= 2.0;

	SetBaricenter(&Vec[0], -tam[1], -tam[1], -tam[1], -tam[1], +tam[1], +tam[1], -tam[1], +tam[1], -tam[1]);
	SetBaricenter(&Vec[1], -tam[1], -tam[1], -tam[1], -tam[1], -tam[1], +tam[1], -tam[1], +tam[1], +tam[1]);
	GetTriangleFaceBaricenter(&Vec[2], Poly->TriangleFace[0]);
	GetTriangleFaceBaricenter(&Vec[3], Poly->TriangleFace[1]);
	Vector_Between_2_Points(&Vec[2], &Vec[0], &Vec[0]);
	Vector_Between_2_Points(&Vec[3], &Vec[1], &Vec[1]);
	TranslateTriangleFace(Poly->TriangleFace[0], &Vec[0]);
	TranslateTriangleFace(Poly->TriangleFace[1], &Vec[1]);

	puts("Now copying\n");
	CloneTriangleFace(Poly->TriangleFace[2], Poly->TriangleFace[0], &basic_colors[Red]);
	CloneTriangleFace(Poly->TriangleFace[3], Poly->TriangleFace[1], &basic_colors[Red]);
	CloneTriangleFace(Poly->TriangleFace[4], Poly->TriangleFace[0], &basic_colors[Yellow]);
	CloneTriangleFace(Poly->TriangleFace[5], Poly->TriangleFace[1], &basic_colors[Yellow]);
	CloneTriangleFace(Poly->TriangleFace[6], Poly->TriangleFace[0], &basic_colors[Purple]);
	CloneTriangleFace(Poly->TriangleFace[7], Poly->TriangleFace[1], &basic_colors[Purple]);
	CloneTriangleFace(Poly->TriangleFace[8], Poly->TriangleFace[0], &basic_colors[White]);
	CloneTriangleFace(Poly->TriangleFace[9], Poly->TriangleFace[1], &basic_colors[White]);
	CloneTriangleFace(Poly->TriangleFace[10], Poly->TriangleFace[0], &basic_colors[Green]);
	CloneTriangleFace(Poly->TriangleFace[11], Poly->TriangleFace[1], &basic_colors[Green]);

	RotateTriangleFaceOnYaw(Poly->TriangleFace[2], +DEGREE_TO_RADIAN(90));
	RotateTriangleFaceOnYaw(Poly->TriangleFace[3], +DEGREE_TO_RADIAN(90));
	RotateTriangleFaceOnYaw(Poly->TriangleFace[4], -DEGREE_TO_RADIAN(90));
	RotateTriangleFaceOnYaw(Poly->TriangleFace[5], -DEGREE_TO_RADIAN(90));
	RotateTriangleFaceOnYaw(Poly->TriangleFace[6], +DEGREE_TO_RADIAN(180));
	RotateTriangleFaceOnYaw(Poly->TriangleFace[7], +DEGREE_TO_RADIAN(180));

	RotateTriangleFaceOnRoll(Poly->TriangleFace[8], +DEGREE_TO_RADIAN(90));
	RotateTriangleFaceOnRoll(Poly->TriangleFace[9], +DEGREE_TO_RADIAN(90));
	RotateTriangleFaceOnRoll(Poly->TriangleFace[10], -DEGREE_TO_RADIAN(90));
	RotateTriangleFaceOnRoll(Poly->TriangleFace[11], -DEGREE_TO_RADIAN(90));

	RotateTriangleFaceOnPitch(Poly->TriangleFace[8], -DEGREE_TO_RADIAN(90));
	RotateTriangleFaceOnPitch(Poly->TriangleFace[9], -DEGREE_TO_RADIAN(90));
	RotateTriangleFaceOnPitch(Poly->TriangleFace[10], +DEGREE_TO_RADIAN(90));
	RotateTriangleFaceOnPitch(Poly->TriangleFace[11], +DEGREE_TO_RADIAN(90));

	puts("Done!\n");

	//Main Loop Setup
	bool do_draw = false;
	bool loop = true;
	double previous_time = 0.0;


	al_start_timer(Timer);

	while(loop){

		al_wait_for_event(Queue, &Event);

		switch(Event.type){

			case ALLEGRO_EVENT_TIMER:
				
				do_draw = true;
							
				if(key[ALLEGRO_KEY_W]){

					ScaleVector(&Cam.Eye.Roll, Cam.movimentspeed, &Vec[0]);
					AddVectors(&Cam.Position, &Vec[0], &Cam.Position);

				}

				if(key[ALLEGRO_KEY_S]){

					ScaleVector(&Cam.Eye.Roll, -Cam.movimentspeed, &Vec[0]);
					AddVectors(&Cam.Position, &Vec[0], &Cam.Position);

				}

				if(key[ALLEGRO_KEY_D]){

					ScaleVector(&Cam.Eye.Pitch, Cam.movimentspeed, &Vec[0]);
					AddVectors(&Cam.Position, &Vec[0], &Cam.Position);

				}

				if(key[ALLEGRO_KEY_A]){

					ScaleVector(&Cam.Eye.Pitch, -Cam.movimentspeed, &Vec[0]);
					AddVectors(&Cam.Position, &Vec[0], &Cam.Position);

				}

				if(key[ALLEGRO_KEY_K]){

					ScaleVector(&Cam.Eye.Yaw, Cam.movimentspeed, &Vec[0]);
					AddVectors(&Cam.Position, &Vec[0], &Cam.Position);

				}

				if(key[ALLEGRO_KEY_J]){

					ScaleVector(&Cam.Eye.Yaw, -Cam.movimentspeed, &Vec[0]);
					AddVectors(&Cam.Position, &Vec[0], &Cam.Position);

				}

				if(key[ALLEGRO_KEY_E]){
					
					RotateCameraOnEyeRoll(&Cam, DEGREE_TO_RADIAN(Cam.rotationspeed));

				}

				if(key[ALLEGRO_KEY_Q]){

					RotateCameraOnEyeRoll(&Cam, -DEGREE_TO_RADIAN(Cam.rotationspeed));

				}

				if(key[ALLEGRO_KEY_UP]){
					
					RotateCameraOnEyePitch(&Cam, DEGREE_TO_RADIAN(Cam.rotationspeed));

				}

				if(key[ALLEGRO_KEY_DOWN]){
					
					RotateCameraOnEyePitch(&Cam, -DEGREE_TO_RADIAN(Cam.rotationspeed));

				}

				if(key[ALLEGRO_KEY_RIGHT]){
					
					RotateCameraOnEyeYaw(&Cam, DEGREE_TO_RADIAN(Cam.rotationspeed));

				}

				if(key[ALLEGRO_KEY_LEFT]){
					
					RotateCameraOnEyeYaw(&Cam, -DEGREE_TO_RADIAN(Cam.rotationspeed));

				}

				if(key[ALLEGRO_KEY_PAD_6]){

					for(unsigned i = 0; i < Poly->facenumber; i++){
					
						RotateTriangleFaceOnYaw(Poly->TriangleFace[i], DEGREE_TO_RADIAN(0.5));

					}
				
				}
				
				if(key[ALLEGRO_KEY_PAD_4]){

					for(unsigned i = 0; i < Poly->facenumber; i++){
					
						RotateTriangleFaceOnYaw(Poly->TriangleFace[i], -DEGREE_TO_RADIAN(0.5));

					}
				
				}
				
				if(key[ALLEGRO_KEY_PAD_8]){

					for(unsigned i = 0; i < Poly->facenumber; i++){
					
						RotateTriangleFaceOnRoll(Poly->TriangleFace[i], DEGREE_TO_RADIAN(0.5));

					}
				
				}
				
				if(key[ALLEGRO_KEY_PAD_2]){

					for(unsigned i = 0; i < Poly->facenumber; i++){
					
						RotateTriangleFaceOnRoll(Poly->TriangleFace[i], -DEGREE_TO_RADIAN(0.5));

					}
				
				}
				
				if(key[ALLEGRO_KEY_PAD_PLUS]){

					for(unsigned i = 0; i < Poly->facenumber; i++){
					
						RotateTriangleFaceOnPitch(Poly->TriangleFace[i], DEGREE_TO_RADIAN(0.5));

					}
				
				}
				
				if(key[ALLEGRO_KEY_PAD_MINUS]){

					for(unsigned i = 0; i < Poly->facenumber; i++){
					
						RotateTriangleFaceOnPitch(Poly->TriangleFace[i], -DEGREE_TO_RADIAN(0.5));

					}
				
				}
				

				if(key[ALLEGRO_KEY_ESCAPE]){

					loop = false;

				}

				for(unsigned i = 0; i < ALLEGRO_KEY_MAX; i++){

					key[i] >>= key[i] & KEY_RELEASED;

				}

				break;

			case ALLEGRO_EVENT_KEY_DOWN:		
				
				key[Event.keyboard.keycode] = KEY_SEEN;
				break;

			case ALLEGRO_EVENT_KEY_UP:		
			
				key[Event.keyboard.keycode] = KEY_RELEASED;
				break;

			case ALLEGRO_EVENT_DISPLAY_CLOSE:

				loop = false;
				break;
	}

	//Draw the frame
	if(do_draw && al_is_event_queue_empty(Queue)){
	
		//Clear window
		al_clear_to_color(al_map_rgb(67, 163, 249));

		//Draw our world

		unsigned drawnumber = EnablePolygonFacesToDraw(&Cam, SW, SH, PolyList.Next, Vertices);
		unsigned count = drawnumber;

		al_draw_prim(Vertices, NULL, NULL, 0, 3 * drawnumber, ALLEGRO_PRIM_TRIANGLE_LIST);

		//Draw some text to it
		
		char cam_x[36];
		char cam_y[36];
		char cam_z[36];
		char trignumber[36];
		char fps[36];

		sprintf(cam_x, "Cam x Position: %.02lf", Cam.Position.x);
		sprintf(cam_y, "Cam y Position: %.02lf", Cam.Position.y);
		sprintf(cam_z, "Cam z Position: %.02lf", Cam.Position.z);
		sprintf(trignumber, "Drawn triangles: %u", count);
		sprintf(fps, "FPS: %.0lf", 1.0 / (al_get_time() - previous_time));

		previous_time = al_get_time();

		al_draw_text(Font, al_map_rgb_f(255, 255, 255), 5, 25, ALLEGRO_ALIGN_LEFT, cam_x);
		al_draw_text(Font, al_map_rgb_f(255, 255, 255), 5, 40, ALLEGRO_ALIGN_LEFT, cam_y);
		al_draw_text(Font, al_map_rgb_f(255, 255, 255), 5, 55, ALLEGRO_ALIGN_LEFT, cam_z);
		al_draw_text(Font, al_map_rgb_f(255, 255, 255), 5, 70, ALLEGRO_ALIGN_LEFT, trignumber);
		al_draw_text(Font, al_map_rgb_f(255, 255, 255), SW - 90, 25, ALLEGRO_ALIGN_LEFT, fps);

//		loop = false;


		//Display a new frame
		al_flip_display();
		do_draw = false;

		}
	}

	//Free resources
	
	ClearListFromStartToEnd(PolyList.Next, &RemovePolygonFromList);
	ClearListFromStartToEnd(TextureList.Next, &RemoveTextureFromList);
	free(Vertices);
	al_destroy_font(Font);
	al_destroy_timer(Timer);
	al_destroy_event_queue(Queue);
	al_destroy_display(Display);

	return EXIT_SUCCESS;

}


/*	vim :set path+="/home/alisson/Code/Libs/Allegro/sdk/include/allegro5" */
