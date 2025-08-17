/****************************************************************************

			VECTORS DEFINITIONS

****************************************************************************/

struct Vec3d{

	double x;
	double y;
	double z;

};

struct OrientationVectorSet{

	struct Vec3d Roll; 	//X axis
	struct Vec3d Pitch;	//Y axis
	struct Vec3d Yaw;	//Z axis

};

/****************************************************************************

			POLYGONS DEFINITIONS

****************************************************************************/

struct Texture{

	ALLEGRO_BITMAP* Texture;
	unsigned width;
	unsigned height;
	NODE* Node;

};

struct Triangle{

	ALLEGRO_COLOR* TextureColor;
	double k[3][3]; //Original constants for paralel planes
	struct TriangleFace* TriangleFace;

};

struct TriangleFace{

	ALLEGRO_COLOR* Color;
	struct Texture* Texture;
	size_t triangle_number;
	float u, v, max_area, texture_rotation;
	struct Vec3d Worldpoint[3];
	NODE TriangleList;

};

struct Polygon{

	struct TriangleFace** TriangleFace;
	size_t facenumber;
	NODE* Node;

};


/****************************************************************************

			CAMERA AND 3D STUFF

****************************************************************************/

struct Camera3d{

	struct Vec3d Position, Edge[4], Eyeline[3];
	struct OrientationVectorSet Eye;
	float movimentspeed, rotationspeed;
	float fov;
	float drawdistance, screendistance;
	double projectionwidth, projectionheight;
	double inverseprojectionwidth, inverseprojectionheight;

};

/****************************************************************************

			      FUNCTIONS PROTOTYPES


			       VECTORS FUNCTIONS

****************************************************************************/

double DotProduct3d(struct Vec3d* u, struct Vec3d* v);

bool CrossProduct3d(struct Vec3d* u, struct Vec3d* v, struct Vec3d* w);

double GetVectorLength(struct Vec3d* A);

void GetWorldPoint(struct TriangleFace* TriangleFace, double k1, double k2, struct Vec3d* Point);

void GetBaricenter(struct Vec3d* A, struct Vec3d* B, struct Vec3d* C, struct Vec3d* D);

void SetBaricenter(struct Vec3d* A, double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3);

void Vector_Between_2_Points(struct Vec3d* A, struct Vec3d* B, struct Vec3d* C);

void AddVectors(struct Vec3d* A, struct Vec3d* B, struct Vec3d* C);

void ScaleVector(struct Vec3d* A, double k, struct Vec3d* B);

void AddScaledVectors(struct Vec3d* A, double k, struct Vec3d* B, double l, struct Vec3d* C);

void ComposeVector(struct Vec3d* A, struct Vec3d* B);

void NormalizeVector(struct Vec3d* A, struct Vec3d* B);

void SetVector(struct Vec3d* A, double x, double y, double z);

struct Vec3d** AllocVectors(size_t number);

void FreeVectors(size_t number, struct Vec3d* Vector[number]);

bool MatrixMultiplication(size_t AR, size_t AC, size_t BR, size_t BC, double A[AR][AC], double B[BR][BC], double C[AR][BC]);

void RotateVectorOnRoll(struct Vec3d* A, double theta);

void RotateVectorOnPitch(struct Vec3d* A, double theta);

void RotateVectorOnYaw(struct Vec3d* A, double theta);

void DebugPrintVec3dCoordinates(struct Vec3d* A);

/****************************************************************************

			CAMERA RELATED FUNCTIONS

****************************************************************************/

void RotateCameraOnRoll(struct Camera3d* Cam, double theta);

void RotateCameraOnPitch(struct Camera3d* Cam, double theta);

void RotateCameraOnYaw(struct Camera3d* Cam, double theta);

void RotateCameraOnEyeRoll(struct Camera3d* Cam, double theta);

void RotateCameraOnEyePitch(struct Camera3d* Cam, double theta);

void RotateCameraOnEyeYaw(struct Camera3d* Cam, double theta);

void SetCamera3d(

		struct Camera3d* Cam, struct Vec3d* Pos, struct Vec3d* EyeRoll, struct Vec3d* EyePitch, struct Vec3d* EyeYaw,
		float fov, float movimentspeed, float rotationspeed, float drawdistance, double projectionwidth, double projectionheight,
		float alpha, float beta, float gama
		
);

size_t EnablePolygonFacesToDraw(struct Camera3d* Cam, size_t screenwidth, size_t screenheight, NODE* PolyList, ALLEGRO_VERTEX Vertices[]);

/****************************************************************************

			POLYGONS FUNCTIONS

****************************************************************************/

void PolygonCreate(struct Polygon** Poly, size_t facenumber);

void PolygonDestroy(struct Polygon** Poly);

void AddPolygonToList(NODE* List, size_t facenumber);

void RemovePolygonFromList(NODE* Polynode);

struct Polygon* GetPolygonFromList(NODE* Polynode, size_t offset);

void TextureCreate(struct Texture** Texture, char* pathname);

void TextureDestroy(struct Texture** Texture);

void AddTextureToList(NODE* List, char* pathname);

void RemoveTextureFromList(NODE* Texturenode);

struct Texture* GetTextureFromList(NODE* Texturenode, size_t offset);

void SetTriangleFace(
		
		struct TriangleFace* TriangleFace,
		ALLEGRO_COLOR* Primarycolor, struct Texture* Texture, float u, float v, float max_area, float texture_rotation,
		double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3
		
);

void CloneTriangleFace(struct TriangleFace* A, struct TriangleFace* B, ALLEGRO_COLOR* Color);

void ProjectTriangleOntoTexture(struct Triangle* Triangle);

size_t ProjectTrianglesOntoScreen(struct Camera3d* Cam, size_t sw, size_t sh, struct TriangleFace** FaceList, size_t k, ALLEGRO_VERTEX V[]);

void TranslateTriangleFace(struct TriangleFace* TriangleFace, struct Vec3d* TranslateVec);

void RotateTriangleFaceAtRoll(struct TriangleFace* TriangleFace, float theta);

void RotateTriangleFaceAtPitch(struct TriangleFace* TriangleFace, float theta);

void RotateTriangleFaceAtYaw(struct TriangleFace* TriangleFace, float theta);

void RotateTriangleFaceOnRoll(struct TriangleFace* TriangleFace, float theta);

void RotateTriangleFaceOnPitch(struct TriangleFace* TriangleFace, float theta);

void RotateTriangleFaceOnYaw(struct TriangleFace* TriangleFace, float theta);

void GetTriangleFaceBaricenter(struct Vec3d* A, struct TriangleFace* TriangleFace);

void ResizeTriangleFace(struct TriangleFace* TriangleFace, float resize_factor);

void ResizePolygon(struct Polygon* Poly, float resize_factor);

bool CheckTriangleFaceVisibilityToCamera(struct Camera3d* Cam, struct TriangleFace* TriangleFace);

void Cramer3d(double A[3][4], struct Vec3d* Point);

void TriangleFaceMerge(struct TriangleFace* TriangleFaceList[], struct Camera3d* Cam, unsigned left, unsigned mid, unsigned right);

void TriangleFaceMergeSort(struct TriangleFace* TriangleFaceList[], struct Camera3d* Cam, unsigned left, unsigned right);
