#define M_PI 3.14159265358979323846
#define DEGREE_TO_RADIAN(X) ((X) * M_PI / 180.0)

struct Node{

	void* ptr;
	struct Node* Next;
	struct Node* Previous;

};

typedef struct Node NODE;

NODE* AddNodeToList(void* ptr, NODE* Node);

NODE* InsertOnListBeforeNode(void* ptr, NODE* Node);

NODE* InsertOnListAfterNode(void* ptr, NODE* Node);

void RemoveNodeFromList(NODE* Node);

void ClearListFromStartToEnd(NODE* Node, void (*RemoveSomethingFromList)(NODE* Somenode));

NODE* GetNodeFromList(NODE* Start, size_t offset);

void merge(double array[], unsigned left, unsigned mid, unsigned right);

void mergesort(double array[], unsigned left, unsigned right);

double det(unsigned n, double a[n][n]);
