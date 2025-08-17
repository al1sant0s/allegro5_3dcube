#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "include/AllegroDefine.h"
#include "include/misc.h"
#include "include/base3d.h"
#define TOLERANCE 1.0E-09

extern ALLEGRO_COLOR GlobalColors[256][256][256];

struct Vec3d NullVector = {0.0, 0.0, 0.0};
struct Vec3d UnitVecX = {1.0, 0.0, 0.0};
struct Vec3d UnitVecY = {0.0, 1.0, 0.0};
struct Vec3d UnitVecZ = {0.0, 0.0, 1.0};

/****************************************************************************

			VECTORS FUNCTIONS

****************************************************************************/

double DotProduct3d(struct Vec3d* u, struct Vec3d* v){

	return ((u->x * v->x) + (u->y * v->y) + (u->z * v->z));

}

bool CrossProduct3d(struct Vec3d* u, struct Vec3d* v, struct Vec3d* w){

	//THIS FUNCTION RETURN TRUE IF CROSS PRODUCT IS POSSIBLE OR FALSE OTHERWISE!

	if(u == NULL || v == NULL || w == NULL){

		return false;

	}

	else{

		//If w vector was u or v, assigning the results directly could lead to wrong results.
		//This will make sure that the results are only assigned once all the values were calculated.
		double x = (u->y * v->z) - (u->z * v->y);
		double y = (u->z * v->x) - (u->x * v->z); 
		double z = (u->x * v->y) - (u->y * v->x);
	
		w->x = x;
		w->y = y;
		w->z = z;

		return true;

	}

}

double GetVectorLength(struct Vec3d* A){

	return sqrt(DotProduct3d(A, A));

}

void GetWorldPoint(struct TriangleFace* TriangleFace, double k1, double k2, struct Vec3d* Point){

	struct Vec3d BaseVec[2];
	Vector_Between_2_Points(&TriangleFace->Worldpoint[0], &TriangleFace->Worldpoint[1], &BaseVec[0]);
	Vector_Between_2_Points(&TriangleFace->Worldpoint[0], &TriangleFace->Worldpoint[2], &BaseVec[1]);

	AddScaledVectors(&BaseVec[0], k1, &BaseVec[1], k2, Point);
	AddVectors(&TriangleFace->Worldpoint[0], Point, Point);

}

void GetBaricenter(struct Vec3d* A, struct Vec3d* B, struct Vec3d* C, struct Vec3d* D){

	SetVector(A, (B->x + C->x + D->x)/3.0, (B->y + C->y + D->y)/3.0, (B->z + C->z + D->z)/3.0);

}

void SetBaricenter(struct Vec3d* A, double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3){

	SetVector(A, (x1 + x2 + x3)/3.0, (y1 + y2 + y3)/3.0, (z1 + z2 + z3)/3.0);

}

void Vector_Between_2_Points(struct Vec3d* A, struct Vec3d* B, struct Vec3d* C){

	//Place onto C one OC vector equal to AB vector.
	
	C->x = B->x - A->x;
	C->y = B->y - A->y;
	C->z = B->z - A->z;

}

void AddVectors(struct Vec3d* A, struct Vec3d* B, struct Vec3d* C){

	C->x = A->x + B->x;
	C->y = A->y + B->y;
	C->z = A->z + B->z;

}

void ScaleVector(struct Vec3d* A, double k, struct Vec3d* B){

	B->x = k * A->x;
	B->y = k * A->y;
	B->z = k * A->z;

}

void AddScaledVectors(struct Vec3d* A, double k, struct Vec3d* B, double l, struct Vec3d* C){

	struct Vec3d Vec[2];

	ScaleVector(A, k, &Vec[0]);
	ScaleVector(B, l, &Vec[1]);
	AddVectors(&Vec[0], &Vec[1], C);

}

void ComposeVector(struct Vec3d* A, struct Vec3d* B){

	A->x = B->x;
	A->y = B->y;
	A->z = B->z;

}

void NormalizeVector(struct Vec3d* A, struct Vec3d* B){

	ScaleVector(A, 1.0/GetVectorLength(A), B);

}

void SetVector(struct Vec3d* A, double x, double y, double z){

	A->x = x;
	A->y = y;
	A->z = z;

}

struct Vec3d** AllocVectors(size_t number){

	struct Vec3d** Vector = malloc(sizeof(struct Vec3d*) * number);
	struct Vec3d* VecPtr = malloc(sizeof(struct Vec3d) * number);

	for(unsigned i = 0; i < number; i++){

		Vector[i] = &VecPtr[i];

	}

	return Vector;

}

void FreeVectors(size_t number, struct Vec3d* Vector[number]){

	free(Vector[0]);
	free(Vector);

}

bool MatrixMultiplication(size_t AR, size_t AC, size_t BR, size_t BC, double A[AR][AC], double B[BR][BC], double C[AR][BC]){

	//THIS FUNCTION RETURN TRUE IF MUTIPLICATION IS POSSIBLE OR FALSE OTHERWISE!

	//MR = MATRIX M NUMBER OF ROWS
	//MC = MATRIX M NUMBER OF COLUMNS
	
	//C is a pointer to zero filled matrix of AR x BC size
	

	//Matrix multiplication is only possible when the number of columns of
	//the first matrix equals the number of rows of the second matrix
	
	if(AC == BR){

		for(unsigned i = 0; i < AR; i++){
		
			for(unsigned j = 0; j < BC; j++){
			
				for(unsigned k = 0; k < AC; k++){
				
					C[i][j] += A[i][k]*B[k][j];

				}

			}

		}

		return true;		

	}

	else{
	
		return false;

	}

}

void RotateVectorOnRoll(struct Vec3d* A, double theta){

	double xyz[1][3] = {{A->x, A->y, A->z}};

	double RotationX[3][3] = {
		{1, 0, 0},
		{0, cos(theta), -sin(theta)},
		{0, sin(theta), cos(theta)}
	};

	double res[1][3] = {0};

	MatrixMultiplication(1, 3, 3, 3, xyz, RotationX, res);

	A->x = res[0][0];
	A->y = res[0][1];
	A->z = res[0][2];

}

void RotateVectorOnPitch(struct Vec3d* A, double theta){

	double xyz[1][3] = {{A->x, A->y, A->z}};

	double RotationY[3][3] = {
		{cos(theta), 0, sin(theta)},
		{0, 1, 0},
		{-sin(theta), 0, cos(theta)}
	};

	double res[1][3] = {0};

	MatrixMultiplication(1, 3, 3, 3, xyz, RotationY, res);

	A->x = res[0][0];
	A->y = res[0][1];
	A->z = res[0][2];

}

void RotateVectorOnYaw(struct Vec3d* A, double theta){

	double xyz[1][3] = {{A->x, A->y, A->z}};

	double RotationZ[3][3] = {
		{cos(theta), sin(theta), 0},
		{-sin(theta), cos(theta), 0},
		{0, 0, 1}
	};

	double res[1][3] = {0};

	MatrixMultiplication(1, 3, 3, 3, xyz, RotationZ, res);

	A->x = res[0][0];
	A->y = res[0][1];
	A->z = res[0][2];

}

void DebugPrintVec3dCoordinates(struct Vec3d* A){

	printf("X = %lf ||| Y = %lf ||| Z = %lf\n", A->x, A->y, A->z);

}

/****************************************************************************

			CAMERA RELATED FUNCTIONS

****************************************************************************/

void RotateCameraOnRoll(struct Camera3d* Cam, double theta){

	for(unsigned i = 0; i < 3; i++){

		RotateVectorOnRoll(&Cam->Eyeline[i], theta);

	}

	for(unsigned i = 0; i < 4; i++){

		RotateVectorOnRoll(&Cam->Edge[i], theta);

	}

	RotateVectorOnRoll(&Cam->Eye.Roll, theta);
	RotateVectorOnRoll(&Cam->Eye.Pitch, theta);
	RotateVectorOnRoll(&Cam->Eye.Yaw, theta);


}

void RotateCameraOnPitch(struct Camera3d* Cam, double theta){

	for(unsigned i = 0; i < 3; i++){

		RotateVectorOnPitch(&Cam->Eyeline[i], theta);

	}

	for(unsigned i = 0; i < 4; i++){

		RotateVectorOnPitch(&Cam->Edge[i], theta);

	}

	RotateVectorOnPitch(&Cam->Eye.Roll, theta);
	RotateVectorOnPitch(&Cam->Eye.Pitch, theta);
	RotateVectorOnPitch(&Cam->Eye.Yaw, theta);

}

void RotateCameraOnYaw(struct Camera3d* Cam, double theta){

	for(unsigned i = 0; i < 3; i++){

		RotateVectorOnYaw(&Cam->Eyeline[i], theta);

	}

	for(unsigned i = 0; i < 4; i++){

		RotateVectorOnYaw(&Cam->Edge[i], theta);

	}

	RotateVectorOnYaw(&Cam->Eye.Roll, theta);
	RotateVectorOnYaw(&Cam->Eye.Pitch, theta);
	RotateVectorOnYaw(&Cam->Eye.Yaw, theta);

}

void RotateCameraOnEyeRoll(struct Camera3d* Cam, double theta){

	struct Vec3d Roll, Pitch, Yaw, Vy, Vz;
	ComposeVector(&Roll, &Cam->Eye.Roll);
	ComposeVector(&Pitch, &Cam->Eye.Pitch);
	ComposeVector(&Yaw, &Cam->Eye.Yaw);
	ComposeVector(&Vy, &UnitVecY);
	ComposeVector(&Vz, &UnitVecZ);

	for(unsigned i = 0; i < 3; i++){

		double projx, projy, projz;
		projx = DotProduct3d(&Cam->Eyeline[i], &Roll);
		projy = DotProduct3d(&Cam->Eyeline[i], &Pitch);
		projz = DotProduct3d(&Cam->Eyeline[i], &Yaw);
		
		struct Vec3d Vec;
		AddScaledVectors(&UnitVecX, projx, &UnitVecY, projy, &Vec);
		AddScaledVectors(&UnitVecZ, projz, &Vec, 1.0, &Vec);

		RotateVectorOnRoll(&Vec, theta);
		
		AddScaledVectors(&Roll, Vec.x, &Pitch, Vec.y, &Cam->Eyeline[i]);
		AddScaledVectors(&Yaw, Vec.z, &Cam->Eyeline[i], 1.0, &Cam->Eyeline[i]);

	}

	for(unsigned i = 0; i < 4; i++){

		double projx, projy, projz;
		projx = DotProduct3d(&Cam->Edge[i], &Roll);
		projy = DotProduct3d(&Cam->Edge[i], &Pitch);
		projz = DotProduct3d(&Cam->Edge[i], &Yaw);
		
		struct Vec3d Vec;
		AddScaledVectors(&UnitVecX, projx, &UnitVecY, projy, &Vec);
		AddScaledVectors(&UnitVecZ, projz, &Vec, 1.0, &Vec);

		RotateVectorOnRoll(&Vec, theta);
		
		AddScaledVectors(&Roll, Vec.x, &Pitch, Vec.y, &Cam->Edge[i]);
		AddScaledVectors(&Yaw, Vec.z, &Cam->Edge[i], 1.0, &Cam->Edge[i]);

	}

	RotateVectorOnRoll(&Vy, theta);
	AddScaledVectors(&Roll, Vy.x, &Pitch, Vy.y, &Cam->Eye.Pitch);
	AddScaledVectors(&Yaw, Vy.z, &Cam->Eye.Pitch, 1.0, &Cam->Eye.Pitch);

	RotateVectorOnRoll(&Vz, theta);
	AddScaledVectors(&Roll, Vz.x, &Pitch, Vz.y, &Cam->Eye.Yaw);
	AddScaledVectors(&Yaw, Vz.z, &Cam->Eye.Yaw, 1.0, &Cam->Eye.Yaw);

}

void RotateCameraOnEyePitch(struct Camera3d* Cam, double theta){

	struct Vec3d Roll, Pitch, Yaw, Vx, Vz;
	ComposeVector(&Roll, &Cam->Eye.Roll);
	ComposeVector(&Pitch, &Cam->Eye.Pitch);
	ComposeVector(&Yaw, &Cam->Eye.Yaw);
	ComposeVector(&Vx, &UnitVecX);
	ComposeVector(&Vz, &UnitVecZ);

	for(unsigned i = 0; i < 3; i++){

		double projx, projy, projz;
		projx = DotProduct3d(&Cam->Eyeline[i], &Roll);
		projy = DotProduct3d(&Cam->Eyeline[i], &Pitch);
		projz = DotProduct3d(&Cam->Eyeline[i], &Yaw);
		
		struct Vec3d Vec;
		AddScaledVectors(&UnitVecX, projx, &UnitVecY, projy, &Vec);
		AddScaledVectors(&UnitVecZ, projz, &Vec, 1.0, &Vec);

		RotateVectorOnPitch(&Vec, theta);
		
		AddScaledVectors(&Roll, Vec.x, &Pitch, Vec.y, &Cam->Eyeline[i]);
		AddScaledVectors(&Yaw, Vec.z, &Cam->Eyeline[i], 1.0, &Cam->Eyeline[i]);

	}

	for(unsigned i = 0; i < 4; i++){

		double projx, projy, projz;
		projx = DotProduct3d(&Cam->Edge[i], &Roll);
		projy = DotProduct3d(&Cam->Edge[i], &Pitch);
		projz = DotProduct3d(&Cam->Edge[i], &Yaw);
		
		struct Vec3d Vec;
		AddScaledVectors(&UnitVecX, projx, &UnitVecY, projy, &Vec);
		AddScaledVectors(&UnitVecZ, projz, &Vec, 1.0, &Vec);

		RotateVectorOnPitch(&Vec, theta);
		
		AddScaledVectors(&Roll, Vec.x, &Pitch, Vec.y, &Cam->Edge[i]);
		AddScaledVectors(&Yaw, Vec.z, &Cam->Edge[i], 1.0, &Cam->Edge[i]);

	}

	RotateVectorOnPitch(&Vx, theta);
	AddScaledVectors(&Roll, Vx.x, &Pitch, Vx.y, &Cam->Eye.Roll);
	AddScaledVectors(&Yaw, Vx.z, &Cam->Eye.Roll, 1.0, &Cam->Eye.Roll);

	RotateVectorOnPitch(&Vz, theta);
	AddScaledVectors(&Roll, Vz.x, &Pitch, Vz.y, &Cam->Eye.Yaw);
	AddScaledVectors(&Yaw, Vz.z, &Cam->Eye.Yaw, 1.0, &Cam->Eye.Yaw);

}

void RotateCameraOnEyeYaw(struct Camera3d* Cam, double theta){

	struct Vec3d Roll, Pitch, Yaw, Vx, Vy;
	ComposeVector(&Roll, &Cam->Eye.Roll);
	ComposeVector(&Pitch, &Cam->Eye.Pitch);
	ComposeVector(&Yaw, &Cam->Eye.Yaw);
	ComposeVector(&Vx, &UnitVecX);
	ComposeVector(&Vy, &UnitVecY);

	for(unsigned i = 0; i < 3; i++){

		double projx, projy, projz;
		projx = DotProduct3d(&Cam->Eyeline[i], &Roll);
		projy = DotProduct3d(&Cam->Eyeline[i], &Pitch);
		projz = DotProduct3d(&Cam->Eyeline[i], &Yaw);
		
		struct Vec3d Vec;
		AddScaledVectors(&UnitVecX, projx, &UnitVecY, projy, &Vec);
		AddScaledVectors(&UnitVecZ, projz, &Vec, 1.0, &Vec);

		RotateVectorOnYaw(&Vec, theta);
		
		AddScaledVectors(&Roll, Vec.x, &Pitch, Vec.y, &Cam->Eyeline[i]);
		AddScaledVectors(&Yaw, Vec.z, &Cam->Eyeline[i], 1.0, &Cam->Eyeline[i]);

	}

	for(unsigned i = 0; i < 4; i++){

		double projx, projy, projz;
		projx = DotProduct3d(&Cam->Edge[i], &Roll);
		projy = DotProduct3d(&Cam->Edge[i], &Pitch);
		projz = DotProduct3d(&Cam->Edge[i], &Yaw);
		
		struct Vec3d Vec;
		AddScaledVectors(&UnitVecX, projx, &UnitVecY, projy, &Vec);
		AddScaledVectors(&UnitVecZ, projz, &Vec, 1.0, &Vec);

		RotateVectorOnYaw(&Vec, theta);
		
		AddScaledVectors(&Roll, Vec.x, &Pitch, Vec.y, &Cam->Edge[i]);
		AddScaledVectors(&Yaw, Vec.z, &Cam->Edge[i], 1.0, &Cam->Edge[i]);

	}

	RotateVectorOnYaw(&Vx, theta);
	AddScaledVectors(&Roll, Vx.x, &Pitch, Vx.y, &Cam->Eye.Roll);
	AddScaledVectors(&Yaw, Vx.z, &Cam->Eye.Roll, 1.0, &Cam->Eye.Roll);

	RotateVectorOnYaw(&Vy, theta);
	AddScaledVectors(&Roll, Vy.x, &Pitch, Vy.y, &Cam->Eye.Pitch);
	AddScaledVectors(&Yaw, Vy.z, &Cam->Eye.Pitch, 1.0, &Cam->Eye.Pitch);

}

void SetCamera3d(

		struct Camera3d* Cam, struct Vec3d* Pos, struct Vec3d* EyeRoll, struct Vec3d* EyePitch, struct Vec3d* EyeYaw,
		float fov, float movimentspeed, float rotationspeed, float drawdistance, double projectionwidth, double projectionheight,
		float alpha, float beta, float gama
		
){

	//Set camera
	
	ComposeVector(&Cam->Position, Pos);
	ComposeVector(&Cam->Eye.Roll, EyeRoll);
	ComposeVector(&Cam->Eye.Pitch, EyePitch);
	ComposeVector(&Cam->Eye.Yaw, EyeYaw);

	Cam->fov = fov;
	Cam->movimentspeed = movimentspeed;
	Cam->rotationspeed = rotationspeed;
	Cam->drawdistance = drawdistance;
	Cam->projectionwidth = projectionwidth / 2.0;
	Cam->projectionheight = projectionheight / 2.0;
	Cam->inverseprojectionwidth = 1.0 / Cam->projectionwidth;
	Cam->inverseprojectionheight = 1.0 / Cam->projectionheight;

	ScaleVector(EyePitch, Cam->projectionwidth, &Cam->Eyeline[0]);
	ScaleVector(EyeYaw, Cam->projectionheight, &Cam->Eyeline[1]);
	ScaleVector(EyeRoll, Cam->projectionwidth/(tan(fov/2.0)), &Cam->Eyeline[2]);

	AddVectors(&Cam->Eyeline[0], &Cam->Eyeline[1], &Cam->Edge[0]);
	AddVectors(&Cam->Edge[0], &Cam->Eyeline[2], &Cam->Edge[0]); //line 0

	AddScaledVectors(&Cam->Eyeline[0], -1.0, &Cam->Eyeline[1], 1.0, &Cam->Edge[1]);
	AddVectors(&Cam->Edge[1], &Cam->Eyeline[2], &Cam->Edge[1]); //line 1

	AddScaledVectors(&Cam->Eyeline[0], -1.0, &Cam->Eyeline[1], -1.0, &Cam->Edge[2]);
	AddVectors(&Cam->Edge[2], &Cam->Eyeline[2], &Cam->Edge[2]); //line 2

	AddScaledVectors(&Cam->Eyeline[0], 1.0, &Cam->Eyeline[1], -1.0, &Cam->Edge[3]);
	AddVectors(&Cam->Edge[3], &Cam->Eyeline[2], &Cam->Edge[3]); //line 3

	//Save screen distance
	Cam->screendistance = GetVectorLength(&Cam->Eyeline[2]);

	//Rotate Camera
	
	RotateCameraOnEyeRoll(Cam, alpha);
	RotateCameraOnEyePitch(Cam, beta);
	RotateCameraOnEyeYaw(Cam, gama);

}

size_t EnablePolygonFacesToDraw(struct Camera3d* Cam, size_t screenwidth, size_t screenheight, NODE* PolyList, ALLEGRO_VERTEX Vertices[]){

	size_t drawnumber = 0;
	size_t k = 0;
	NODE FaceList = {NULL, NULL, NULL};

	while(PolyList != NULL){

		struct Polygon* Poly = (struct Polygon*) PolyList->ptr;

		for(unsigned i = 0; i < Poly->facenumber; i++){

			if(CheckTriangleFaceVisibilityToCamera(Cam, Poly->TriangleFace[i])){

				AddNodeToList(Poly->TriangleFace[i], &FaceList);
				k++;

			}

		}

		PolyList = PolyList->Next;
	}

	if(k){

		struct TriangleFace** SortFaceList = malloc(sizeof(struct TriangleFace*) * k);
		NODE* Facenode = FaceList.Next;

		//Fill an array with all faces to be drawn

		for(unsigned i = 0; i < k; i++){
		
			SortFaceList[i] = Facenode->ptr;
			Facenode = Facenode->Next;
		
		}

		TriangleFaceMergeSort(SortFaceList, Cam, 0, k - 1); //Sort them

		drawnumber = ProjectTrianglesOntoScreen(Cam, screenwidth, screenheight, SortFaceList, k, Vertices);

		//Destroy all nodes

		ClearListFromStartToEnd(FaceList.Next, NULL);
		free(SortFaceList);

	}

	return drawnumber;
}

size_t ProjectTrianglesOntoScreen(struct Camera3d* Cam, size_t sw, size_t sh, struct TriangleFace** FaceList, size_t k, ALLEGRO_VERTEX V[]){

	size_t halfsw = sw >> 1;
	size_t halfsh = sh >> 1;
	size_t drawnumber = 0;

	//Setting an equation to get the point of the line the goes trough the camera's projection plane
	struct Vec3d CamPlanePoint;
	AddVectors(&Cam->Position, &Cam->Eyeline[2], &CamPlanePoint);

	float line = DotProduct3d(&Cam->Eye.Roll, &CamPlanePoint) - DotProduct3d(&Cam->Eye.Roll, &Cam->Position);

	for(unsigned i = 0; i < k; i++){

		struct Vec3d Point[4], ScreenOrigin, WP01, WP02, CamWorldPointZero, ScreenOriginCamPosition;

		AddVectors(&Cam->Position, &Cam->Eyeline[2], &ScreenOrigin);

		Vector_Between_2_Points(&Cam->Position, &FaceList[i]->Worldpoint[0], &Point[0]);
		Vector_Between_2_Points(&Cam->Position, &FaceList[i]->Worldpoint[1], &Point[1]);
		Vector_Between_2_Points(&Cam->Position, &FaceList[i]->Worldpoint[2], &Point[2]);

		AddScaledVectors(&Cam->Position, 1, &Point[0], line/DotProduct3d(&Cam->Eye.Roll, &Point[0]), &Point[0]);
		AddScaledVectors(&Cam->Position, 1, &Point[1], line/DotProduct3d(&Cam->Eye.Roll, &Point[1]), &Point[1]);
		AddScaledVectors(&Cam->Position, 1, &Point[2], line/DotProduct3d(&Cam->Eye.Roll, &Point[2]), &Point[2]);

		/*
		Vector_Between_2_Points(&ScreenOrigin, &Point[0], &Vec[0]);
		Vector_Between_2_Points(&ScreenOrigin, &Point[1], &Vec[1]);
		Vector_Between_2_Points(&ScreenOrigin, &Point[2], &Vec[2]);

		float x1 = halfsw * (1.0 + (DotProduct3d(&Vec[0], &Cam->Eye.Pitch) * Cam->inverseprojectionwidth));
		float y1 = halfsh * (1.0 - (DotProduct3d(&Vec[0], &Cam->Eye.Yaw) * Cam->inverseprojectionheight));

		float x2 = halfsw * (1.0 + (DotProduct3d(&Vec[1], &Cam->Eye.Pitch) * Cam->inverseprojectionwidth));
		float y2 = halfsh * (1.0 - (DotProduct3d(&Vec[1], &Cam->Eye.Yaw) * Cam->inverseprojectionheight));

		float x3 = halfsw * (1.0 + (DotProduct3d(&Vec[2], &Cam->Eye.Pitch) * Cam->inverseprojectionwidth));
		float y3 = halfsh * (1.0 - (DotProduct3d(&Vec[2], &Cam->Eye.Yaw) * Cam->inverseprojectionheight));

		al_draw_filled_triangle(x1,y1,x2,y2,x3,y3, GlobalColors[255][255][0]);
		*/
		//al_draw_filled_circle(halfsw,halfsh, 3, GlobalColors[255][255][255]);

		Vector_Between_2_Points(&FaceList[i]->Worldpoint[0], &FaceList[i]->Worldpoint[1], &WP01);
		Vector_Between_2_Points(&FaceList[i]->Worldpoint[0], &FaceList[i]->Worldpoint[2], &WP02);
		Vector_Between_2_Points(&Cam->Position, &FaceList[i]->Worldpoint[0], &CamWorldPointZero);
		Vector_Between_2_Points(&ScreenOrigin, &Cam->Position, &ScreenOriginCamPosition);

		double dot_roll_camwp0 = DotProduct3d(&Cam->Eye.Roll, &CamWorldPointZero);
		double dot_roll_wp01 = DotProduct3d(&Cam->Eye.Roll, &WP01);
		double dot_roll_wp02 = DotProduct3d(&Cam->Eye.Roll, &WP02);

		double dot_pitch_origin = DotProduct3d(&Cam->Eye.Pitch, &ScreenOriginCamPosition);
		double dot_pitch_camwp0 = DotProduct3d(&Cam->Eye.Pitch, &CamWorldPointZero);
		double dot_pitch_wp01 = DotProduct3d(&Cam->Eye.Pitch, &WP01);
		double dot_pitch_wp02 = DotProduct3d(&Cam->Eye.Pitch, &WP02);

		double dot_yaw_origin = DotProduct3d(&Cam->Eye.Yaw, &ScreenOriginCamPosition);
		double dot_yaw_camwp0 = DotProduct3d(&Cam->Eye.Yaw, &CamWorldPointZero);
		double dot_yaw_wp01 = DotProduct3d(&Cam->Eye.Yaw, &WP01);
		double dot_yaw_wp02 = DotProduct3d(&Cam->Eye.Yaw, &WP02);

		NODE* Trianglenode = FaceList[i]->TriangleList.Next;

		while(Trianglenode != NULL){

			struct Triangle* Triangle = (struct Triangle*) Trianglenode->ptr;
			unsigned index = drawnumber * 3;

			for(unsigned j = 0; j < 3; j++){

				double w = line/(dot_roll_camwp0 + (Triangle->k[j][0] * dot_roll_wp01) + (Triangle->k[j][1] * dot_roll_wp02));
				double x = w * (dot_pitch_camwp0 + (Triangle->k[j][0] * dot_pitch_wp01) + (Triangle->k[j][1] * dot_pitch_wp02));
				double y = w * (dot_yaw_camwp0 + (Triangle->k[j][0] * dot_yaw_wp01) + (Triangle->k[j][1] * dot_yaw_wp02));

				V[index + j].x = 1.0 + ((x + dot_pitch_origin) * Cam->inverseprojectionwidth);
				V[index + j].y = 1.0 - ((y + dot_yaw_origin) * Cam->inverseprojectionheight);
				V[index + j].z = 0.0;

				V[index + j].x *= halfsw;
				V[index + j].y *= halfsh;

				V[index + j].color = *Triangle->TextureColor;

			}


			drawnumber++;
			Trianglenode = Trianglenode->Next;

		}

	}

	return drawnumber;

}

/****************************************************************************

			POLYGONS FUNCTIONS

****************************************************************************/

void PolygonCreate(struct Polygon** Poly, size_t facenumber){

	*Poly = malloc(sizeof(struct Polygon));
	(*Poly)->TriangleFace = malloc(sizeof(struct TriangleFace*) * facenumber);
	struct TriangleFace* TriangleFacePtr = malloc(sizeof(struct TriangleFace) * facenumber);

	for(unsigned i = 0; i < facenumber; i++){
	
		(*Poly)->TriangleFace[i] = &TriangleFacePtr[i];

	}

	(*Poly)->facenumber = facenumber;

}

void PolygonDestroy(struct Polygon** Poly){

	//Clear all trianglefaces data first
	for(unsigned i = 0; i < (*Poly)->facenumber; i++){
	
		free((*Poly)->TriangleFace[i]->TriangleList.Next->ptr); //Free reserved space for the triangles
		ClearListFromStartToEnd((*Poly)->TriangleFace[i]->TriangleList.Next, NULL); //Free list of triangles

	}

	free((*Poly)->TriangleFace[0]); //Free reserved space for the trianglefaces
	free((*Poly)->TriangleFace); //Free array of pointers to trianglefaces
	free(*Poly); //Free polygon
	*Poly = NULL;

}

void AddPolygonToList(NODE* List, size_t facenumber){

	List = AddNodeToList(NULL, List);
	PolygonCreate((struct Polygon**) &List->ptr, facenumber);
	((struct Polygon*) List->ptr)->Node = List; 
	
}

void RemovePolygonFromList(NODE* Polynode){

	PolygonDestroy((struct Polygon**) &Polynode->ptr);
	RemoveNodeFromList(Polynode);

}

struct Polygon* GetPolygonFromList(NODE* Polynode, size_t offset){

	Polynode = GetNodeFromList(Polynode, offset);
	return (Polynode != NULL ? (struct Polygon*) Polynode->ptr : NULL);

}

void TextureCreate(struct Texture** Texture, char* pathname){

	*Texture = malloc(sizeof(struct Texture));
	(*Texture)->Texture = al_load_bitmap(pathname);

	if((*Texture)->Texture != NULL){
	
		(*Texture)->width = al_get_bitmap_width((*Texture)->Texture);
		(*Texture)->height = al_get_bitmap_width((*Texture)->Texture);


	}

	else{
	
		(*Texture)->width = 0;
		(*Texture)->height = 0;

	}

}

void TextureDestroy(struct Texture** Texture){

	free(*Texture);
	*Texture = NULL;

}

void AddTextureToList(NODE* List, char* pathname){

	List = AddNodeToList(NULL, List);
	TextureCreate((struct Texture**) &List->ptr, pathname);
	((struct Texture*) List->ptr)->Node = List; 
	
}

void RemoveTextureFromList(NODE* Texturenode){

	TextureDestroy((struct Texture**) &Texturenode->ptr);
	RemoveNodeFromList(Texturenode);

}

struct Texture* GetTextureFromList(NODE* Texturenode, size_t offset){

	Texturenode = GetNodeFromList(Texturenode, offset);
	return (Texturenode != NULL ? (struct Texture*) Texturenode->ptr : NULL);

}

void SetTriangleFace(
		
		struct TriangleFace* TriangleFace,
		ALLEGRO_COLOR* Primarycolor, struct Texture* Texture, float u, float v, float max_area, float texture_rotation,
		double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3
		
){

	SetVector(&TriangleFace->Worldpoint[0], x1, y1, z1);
	SetVector(&TriangleFace->Worldpoint[1], x2, y2, z2);
	SetVector(&TriangleFace->Worldpoint[2], x3, y3, z3);

	struct Vec3d BaseVec[2];

	Vector_Between_2_Points(&TriangleFace->Worldpoint[0], &TriangleFace->Worldpoint[1], &BaseVec[0]);
	Vector_Between_2_Points(&TriangleFace->Worldpoint[0], &TriangleFace->Worldpoint[2], &BaseVec[1]);

	TriangleFace->Color = Primarycolor;
	TriangleFace->Texture = Texture;
	TriangleFace->u = u;
	TriangleFace->v = v;
	TriangleFace->max_area = max_area;
	TriangleFace->texture_rotation = texture_rotation;

	TriangleFace->TriangleList.ptr = NULL;
	TriangleFace->TriangleList.Previous = NULL;
	TriangleFace->TriangleList.Next = NULL;

	struct Vec3d Vertex[3], Vec[3];

	SetVector(&Vertex[0], x1, y1, z1);
	SetVector(&Vertex[1], x2, y2, z2);
	SetVector(&Vertex[2], x3, y3, z3);

	Vector_Between_2_Points(&Vertex[0], &Vertex[1], &Vec[0]);
	Vector_Between_2_Points(&Vertex[0], &Vertex[2], &Vec[1]);
	CrossProduct3d(&Vec[0], &Vec[1], &Vec[2]);

	size_t count = 1;
	double area = GetVectorLength(&Vec[2])/2.0;

	while(area && area > max_area && max_area){
		
		area /= 2.0;
		count <<= 1;

	}

	struct Vec3d** PointList[3] = {AllocVectors(count), AllocVectors(count), AllocVectors(count)};

	SetVector(PointList[0][0], x1, y1, z1);
	SetVector(PointList[1][0], x3, y3, z3);
	SetVector(PointList[2][0], x2, y2, z2);

	size_t k = 0;

	while(k != count - 1){
	
		size_t j = 0;

		while(j <= k){
			
			struct Vec3d P1, P2, P3;
			ComposeVector(&P1, PointList[0][j]);
			ComposeVector(&P2, PointList[1][j]);
			ComposeVector(&P3, PointList[2][j]);

			double mx = (P1.x + P2.x)/2.0;
			double my = (P1.y + P2.y)/2.0;
			double mz = (P1.z + P2.z)/2.0;

			SetVector(PointList[0][j], P1.x, P1.y, P1.z);
			SetVector(PointList[1][j], P3.x, P3.y, P3.z);
			SetVector(PointList[2][j], mx, my, mz);

			SetVector(PointList[0][j + k + 1], P2.x, P2.y, P2.z);
			SetVector(PointList[1][j + k + 1], P3.x, P3.y, P3.z);
			SetVector(PointList[2][j + k + 1], mx, my, mz);

			j++;
		
		}

		k += j;
	
	}

	struct Triangle** Triangle = malloc(sizeof(struct Triangle*) * count);
	struct Triangle* TrianglePtr = malloc(sizeof(struct Triangle) * count);

	for(size_t i = 0; i < count; i++){

		Triangle[i] = &TrianglePtr[i];

		Vector_Between_2_Points(&TriangleFace->Worldpoint[0], PointList[0][i], &Vec[0]);
		Vector_Between_2_Points(&TriangleFace->Worldpoint[0], PointList[1][i], &Vec[1]);
		Vector_Between_2_Points(&TriangleFace->Worldpoint[0], PointList[2][i], &Vec[2]);

		double det = (BaseVec[0].y * BaseVec[1].z) - (BaseVec[0].z * BaseVec[1].y);

		//x = k1ax + k2bx;
		//y = k1ay + k2by;
		//z = k1az + k2bz;

		Triangle[i]->k[0][0] = ((Vec[0].y * BaseVec[1].z) - (Vec[0].z * BaseVec[1].y))/det;
		Triangle[i]->k[0][1] = ((BaseVec[0].y * Vec[0].z) - (BaseVec[0].z * Vec[0].y))/det;
		Triangle[i]->k[1][0] = ((Vec[1].y * BaseVec[1].z) - (Vec[1].z * BaseVec[1].y))/det;
		Triangle[i]->k[1][1] = ((BaseVec[0].y * Vec[1].z) - (BaseVec[0].z * Vec[1].y))/det;
		Triangle[i]->k[2][0] = ((Vec[2].y * BaseVec[1].z) - (Vec[2].z * BaseVec[1].y))/det;
		Triangle[i]->k[2][1] = ((BaseVec[0].y * Vec[2].z) - (BaseVec[0].z * Vec[2].y))/det;

		Triangle[i]->TriangleFace = TriangleFace;

		ProjectTriangleOntoTexture(Triangle[i]);

		AddNodeToList(Triangle[i], &TriangleFace->TriangleList);

	}

	TriangleFace->triangle_number = count;

	free(Triangle);
	FreeVectors(count, PointList[0]);
	FreeVectors(count, PointList[1]);
	FreeVectors(count, PointList[2]);

}

void CloneTriangleFace(struct TriangleFace* A, struct TriangleFace* B, ALLEGRO_COLOR* Color){

	ComposeVector(&A->Worldpoint[0], &B->Worldpoint[0]);
	ComposeVector(&A->Worldpoint[1], &B->Worldpoint[1]);
	ComposeVector(&A->Worldpoint[2], &B->Worldpoint[2]);

	A->Color = (Color != NULL ? Color : B->Color);
	A->Texture = B->Texture;
	A->u = B->u;
	A->v = B->v;
	A->max_area = B->max_area;
	A->texture_rotation = B->texture_rotation;

	A->TriangleList.ptr = NULL;
	A->TriangleList.Previous = NULL;
	A->TriangleList.Next = NULL;

	NODE* TriangleNode = B->TriangleList.Next;

	struct Triangle** Triangle = malloc(sizeof(struct Triangle*) * B->triangle_number);
	struct Triangle* TrianglePtr = malloc(sizeof(struct Triangle) * B->triangle_number);

	for(size_t i = 0; i < B->triangle_number; i++){

		Triangle[i] = &TrianglePtr[i];
	
		struct Triangle* Target = (struct Triangle*) TriangleNode->ptr;

		Triangle[i]->k[0][0] = Target->k[0][0];
		Triangle[i]->k[0][1] = Target->k[0][1];
		Triangle[i]->k[1][0] = Target->k[1][0];
		Triangle[i]->k[1][1] = Target->k[1][1];
		Triangle[i]->k[2][0] = Target->k[2][0];
		Triangle[i]->k[2][1] = Target->k[2][1];

		Triangle[i]->TriangleFace = A;
		Triangle[i]->TextureColor = (B->Texture != NULL ? Target->TextureColor : A->Color);

		AddNodeToList(Triangle[i], &A->TriangleList);
	
		TriangleNode = TriangleNode->Next;

	}

	free(Triangle);
	A->triangle_number = B->triangle_number;

}

void ProjectTriangleOntoTexture(struct Triangle* Triangle){

	if(Triangle->TriangleFace->Texture != NULL){
	
		struct Texture* Texture = Triangle->TriangleFace->Texture;
		struct TriangleFace* TriangleFace = Triangle->TriangleFace;

		al_lock_bitmap(Texture->Texture, al_get_bitmap_format(Texture->Texture), ALLEGRO_LOCK_READONLY);

		struct Vec3d TextureVec, Vec[3], Baricenter;
		int r, g, b;
		float u, v;
		ALLEGRO_COLOR TextureColor;

		GetWorldPoint(TriangleFace, Triangle->k[0][0], Triangle->k[0][1], &Vec[0]);
		GetWorldPoint(TriangleFace, Triangle->k[1][0], Triangle->k[1][1], &Vec[1]);
		GetWorldPoint(TriangleFace, Triangle->k[2][0], Triangle->k[2][1], &Vec[2]);
		GetTriangleFaceBaricenter(&Baricenter, TriangleFace);
		GetBaricenter(&TextureVec, &Vec[0], &Vec[1], &Vec[2]);
		Vector_Between_2_Points(&Baricenter, &TextureVec, &TextureVec);
		
		SetVector(&TextureVec, DotProduct3d(&TextureVec, &UnitVecY), DotProduct3d(&TextureVec, &UnitVecZ), 0.0);
		RotateVectorOnYaw(&TextureVec, Triangle->TriangleFace->texture_rotation);

		u = (Triangle->TriangleFace->u * Triangle->TriangleFace->Texture->width) + TextureVec.x;
		v = (Triangle->TriangleFace->v * Triangle->TriangleFace->Texture->height) - TextureVec.y;

		TextureColor = al_get_pixel(Triangle->TriangleFace->Texture->Texture, u, v);
		r = round(TextureColor.r * 255.0);
		g = round(TextureColor.g * 255.0);
		b = round(TextureColor.b * 255.0);
		Triangle->TextureColor = &GlobalColors[r][g][b];

		al_unlock_bitmap(Texture->Texture);
	}

	else{
	
		if(Triangle->TriangleFace->Color != NULL){

			Triangle->TextureColor = Triangle->TriangleFace->Color;

		}

		else{

			int r, g, b;
			r = rand() % 256;
			g = rand() % 256;
			b = rand() % 256;

			Triangle->TextureColor = &GlobalColors[r][g][b];

		
		}

	}

}

void TranslateTriangleFace(struct TriangleFace* TriangleFace, struct Vec3d* TranslateVec){

	AddVectors(&TriangleFace->Worldpoint[0], TranslateVec, &TriangleFace->Worldpoint[0]);
	AddVectors(&TriangleFace->Worldpoint[1], TranslateVec, &TriangleFace->Worldpoint[1]);
	AddVectors(&TriangleFace->Worldpoint[2], TranslateVec, &TriangleFace->Worldpoint[2]);

}

void RotateTriangleFaceAtRoll(struct TriangleFace* TriangleFace, float theta){

	struct Vec3d Baricenter;
	GetTriangleFaceBaricenter(&Baricenter, TriangleFace);

	ScaleVector(&Baricenter, -1.0, &Baricenter);
	TranslateTriangleFace(TriangleFace, &Baricenter);

	RotateTriangleFaceOnRoll(TriangleFace, theta);

	ScaleVector(&Baricenter, -1.0, &Baricenter);
	TranslateTriangleFace(TriangleFace, &Baricenter);

}

void RotateTriangleFaceAtPitch(struct TriangleFace* TriangleFace, float theta){

	struct Vec3d Baricenter;
	GetTriangleFaceBaricenter(&Baricenter, TriangleFace);

	ScaleVector(&Baricenter, -1.0, &Baricenter);
	TranslateTriangleFace(TriangleFace, &Baricenter);

	RotateTriangleFaceOnPitch(TriangleFace, theta);

	ScaleVector(&Baricenter, -1.0, &Baricenter);
	TranslateTriangleFace(TriangleFace, &Baricenter);

}

void RotateTriangleFaceAtYaw(struct TriangleFace* TriangleFace, float theta){

	struct Vec3d Baricenter;
	GetTriangleFaceBaricenter(&Baricenter, TriangleFace);

	ScaleVector(&Baricenter, -1.0, &Baricenter);
	TranslateTriangleFace(TriangleFace, &Baricenter);

	RotateTriangleFaceOnYaw(TriangleFace, theta);

	ScaleVector(&Baricenter, -1.0, &Baricenter);
	TranslateTriangleFace(TriangleFace, &Baricenter);

}

void RotateTriangleFaceOnRoll(struct TriangleFace* TriangleFace, float theta){

	RotateVectorOnRoll(&TriangleFace->Worldpoint[0], theta);
	RotateVectorOnRoll(&TriangleFace->Worldpoint[1], theta);
	RotateVectorOnRoll(&TriangleFace->Worldpoint[2], theta);

}

void RotateTriangleFaceOnPitch(struct TriangleFace* TriangleFace, float theta){

	RotateVectorOnPitch(&TriangleFace->Worldpoint[0], theta);
	RotateVectorOnPitch(&TriangleFace->Worldpoint[1], theta);
	RotateVectorOnPitch(&TriangleFace->Worldpoint[2], theta);

}

void RotateTriangleFaceOnYaw(struct TriangleFace* TriangleFace, float theta){

	RotateVectorOnYaw(&TriangleFace->Worldpoint[0], theta);
	RotateVectorOnYaw(&TriangleFace->Worldpoint[1], theta);
	RotateVectorOnYaw(&TriangleFace->Worldpoint[2], theta);

}

void GetTriangleFaceBaricenter(struct Vec3d* A, struct TriangleFace* TriangleFace){

	GetBaricenter(A, &TriangleFace->Worldpoint[0], &TriangleFace->Worldpoint[1], &TriangleFace->Worldpoint[2]);

}

void ResizeTriangleFace(struct TriangleFace* TriangleFace, float resize_factor){

	struct Vec3d Baricenter[2], TranslateVec;

	GetTriangleFaceBaricenter(&Baricenter[0], TriangleFace);

	ScaleVector(&TriangleFace->Worldpoint[0], resize_factor, &TriangleFace->Worldpoint[0]);
	ScaleVector(&TriangleFace->Worldpoint[1], resize_factor, &TriangleFace->Worldpoint[1]);
	ScaleVector(&TriangleFace->Worldpoint[2], resize_factor, &TriangleFace->Worldpoint[2]);

	GetTriangleFaceBaricenter(&Baricenter[1], TriangleFace);

	Vector_Between_2_Points(&Baricenter[1], &Baricenter[0], &TranslateVec);
	TranslateTriangleFace(TriangleFace, &TranslateVec);

}

void ResizePolygon(struct Polygon* Poly, float resize_factor){

	if(Poly != NULL){
	
		for(unsigned i = 0; i < Poly->facenumber; i++){

			ResizeTriangleFace(Poly->TriangleFace[i], resize_factor);

		}

	
	}

}

bool CheckTriangleFaceVisibilityToCamera(struct Camera3d* Cam, struct TriangleFace* TriangleFace){

			
		struct Vec3d CamWP[3], Bounds[3][2], Normal;

		Vector_Between_2_Points(&Cam->Position, &TriangleFace->Worldpoint[0], &CamWP[0]);
		Vector_Between_2_Points(&Cam->Position, &TriangleFace->Worldpoint[1], &CamWP[1]);
		Vector_Between_2_Points(&Cam->Position, &TriangleFace->Worldpoint[2], &CamWP[2]);
		Vector_Between_2_Points(&TriangleFace->Worldpoint[0], &TriangleFace->Worldpoint[1], &Bounds[0][0]);
		Vector_Between_2_Points(&TriangleFace->Worldpoint[0], &TriangleFace->Worldpoint[2], &Bounds[0][1]);
		Vector_Between_2_Points(&TriangleFace->Worldpoint[1], &TriangleFace->Worldpoint[0], &Bounds[1][0]);
		Vector_Between_2_Points(&TriangleFace->Worldpoint[1], &TriangleFace->Worldpoint[2], &Bounds[1][1]);
		Vector_Between_2_Points(&TriangleFace->Worldpoint[2], &TriangleFace->Worldpoint[0], &Bounds[2][0]);
		Vector_Between_2_Points(&TriangleFace->Worldpoint[2], &TriangleFace->Worldpoint[1], &Bounds[2][1]);
		CrossProduct3d(&Bounds[0][1], &Bounds[0][0], &Normal);
		NormalizeVector(&Normal, &Normal);


		if(	DotProduct3d(&CamWP[0], &Cam->Eye.Roll) < 0.0 &&
			DotProduct3d(&CamWP[1], &Cam->Eye.Roll) < 0.0 &&
			DotProduct3d(&CamWP[2], &Cam->Eye.Roll) < 0.0		){
		
			return false;

		}

		if(	GetVectorLength(&CamWP[0]) > Cam->drawdistance &&
			GetVectorLength(&CamWP[1]) > Cam->drawdistance &&
			GetVectorLength(&CamWP[2]) > Cam->drawdistance		){

			return false;

		}

		if(DotProduct3d(&Cam->Eye.Roll, &Normal) < cos(3.0*Cam->fov/2.0)){
		
			return false;
		
		}

		struct Vec3d OrtoVectors[4];
		CrossProduct3d(&Cam->Edge[0], &Cam->Edge[1], &OrtoVectors[0]);
		CrossProduct3d(&Cam->Edge[1], &Cam->Edge[2], &OrtoVectors[1]);
		CrossProduct3d(&Cam->Edge[2], &Cam->Edge[3], &OrtoVectors[2]);
		CrossProduct3d(&Cam->Edge[3], &Cam->Edge[0], &OrtoVectors[3]);

		for(unsigned i = 0; i < 3; i++){

			double placing[4] = {
				DotProduct3d(&CamWP[i], &OrtoVectors[0]),
				DotProduct3d(&CamWP[i], &OrtoVectors[1]),
				DotProduct3d(&CamWP[i], &OrtoVectors[2]),
				DotProduct3d(&CamWP[i], &OrtoVectors[3]),
			};

			if(placing[0] >= 0.0 && placing[1] >= 0.0 && placing[2] >= 0.0 && placing[3] >= 0.0){

				return true;

			}

		}

		//Setting an equation to get the point of the line the goes trough the camera's projection plane

		float line = DotProduct3d(&Normal, &TriangleFace->Worldpoint[0]) - DotProduct3d(&Normal, &Cam->Position);

		for(unsigned j = 0; j < 4; j++){

			struct Vec3d Point, Vec[3];

			AddScaledVectors(&Cam->Position, 1, &Cam->Eyeline[j], line/DotProduct3d(&Vec[6], &Cam->Eyeline[j]), &Point);
		
			Vector_Between_2_Points(&TriangleFace->Worldpoint[0], &Point, &Vec[0]);
			Vector_Between_2_Points(&TriangleFace->Worldpoint[1], &Point, &Vec[1]);
			Vector_Between_2_Points(&TriangleFace->Worldpoint[2], &Point, &Vec[2]);

			size_t c = 6;
			for(unsigned a = 0; a < 3; a++){
			
				for(unsigned b = 0; b < 2; b++){
				
					if(DotProduct3d(&Vec[a], &Bounds[a][b]) > 0.0){
					
						c--;
					
					}
				
				}
			
			}

			if(!c){
			
				return true;

			}

		}


	return false;

}

void Cramer3d(double A[3][4], struct Vec3d* Point){

	double B[3][3];
	double P[3] = {0.0};
	
	for(unsigned i = 0; i < 3; i++){

		for(unsigned j = 0; j < 3; j++){
	
			B[i][j] = A[i][j];

		}

	}

	double	dta = det(3, B);

	if(fabs(dta) > 01E-09){
			
		for(unsigned f = 0; f < 3; f++){

			for(unsigned i = 0; i < 3; i++){

				for(unsigned j = 0; j < 3; j++){
			
					//Replace j column with last column
					B[i][j] = (j != f ? A[i][j] : A[i][3]);

				}

			}


			//Find dtx

			 P[f] = det(3, B)/dta;

		}

	}

	Point->x = P[0];
	Point->y = P[1];
	Point->z = P[2];

}

void TriangleFaceMerge(struct TriangleFace* TriangleFaceList[], struct Camera3d* Cam, unsigned left, unsigned mid, unsigned right){

	unsigned n1 = mid - left + 1;
	unsigned n2 = right - mid;
	struct TriangleFace* L[n1];
	struct TriangleFace* R[n2];

	for(unsigned i = 0; i < n1; i++){
	
		L[i] = TriangleFaceList[left + i];

	}

	for(unsigned i = 0; i < n2; i++){
	
		R[i] = TriangleFaceList[mid + 1 + i];

	}

	unsigned i = 0;
       	unsigned j = 0;
	unsigned k = left;

	while(i < n1 && j < n2){

		struct Vec3d Vec[6], LBac, RBac;
		GetTriangleFaceBaricenter(&LBac, L[i]);
		GetTriangleFaceBaricenter(&RBac, R[j]);

		Vector_Between_2_Points(&L[i]->Worldpoint[0], &L[i]->Worldpoint[1], &Vec[0]);
		Vector_Between_2_Points(&L[i]->Worldpoint[0], &L[i]->Worldpoint[2], &Vec[1]);
		Vector_Between_2_Points(&R[j]->Worldpoint[0], &R[j]->Worldpoint[1], &Vec[2]);
		Vector_Between_2_Points(&R[j]->Worldpoint[0], &R[j]->Worldpoint[2], &Vec[3]);
		CrossProduct3d(&Vec[0], &Vec[1], &Vec[4]);
		CrossProduct3d(&Vec[2], &Vec[3], &Vec[5]);
		NormalizeVector(&Vec[4], &Vec[4]);
		NormalizeVector(&Vec[5], &Vec[5]);
		

		if(DotProduct3d(&Cam->Eye.Roll, &Vec[4]) - GetVectorLength(&LBac) > DotProduct3d(&Cam->Eye.Roll, &Vec[5]) - GetVectorLength(&RBac)){
		
			TriangleFaceList[k] = L[i];
			i++;
		
		}

		else{
		
			TriangleFaceList[k] = R[j];
			j++;

		}

		k++;	

	}

	while(i < n1){
	
		TriangleFaceList[k++] = L[i++];

	}

	while(j < n2){
	
		TriangleFaceList[k++] = R[j++];

	}

}

void TriangleFaceMergeSort(struct TriangleFace* TriangleFaceList[], struct Camera3d* Cam, unsigned left, unsigned right){

	if(left < right){
	
		unsigned mid = left + (right - left)/2;
		TriangleFaceMergeSort(TriangleFaceList, Cam, left, mid);
		TriangleFaceMergeSort(TriangleFaceList, Cam, mid + 1, right);
		TriangleFaceMerge(TriangleFaceList, Cam, left, mid, right);
	
	}

}
