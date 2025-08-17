#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "include/misc.h"

NODE* AddNodeToList(void* ptr, NODE* Node){

	NODE* Lastnode = Node;

	while(Lastnode->Next != NULL){
	
		Lastnode = Lastnode->Next;
	
	}

	Lastnode->Next = malloc(sizeof(NODE));
	Lastnode->Next->ptr = ptr;
	Lastnode->Next->Previous = Lastnode;
	Lastnode->Next->Next = NULL;

	return Lastnode->Next;

}

NODE* InsertOnListBeforeNode(void* ptr, NODE* Node){

	NODE* Newnode = malloc(sizeof(NODE));

	Newnode->ptr = ptr;
	Newnode->Next = Node;
	Newnode->Previous = Node->Previous;
	Node->Previous = Newnode;
	
	if(Newnode->Previous != NULL){

		Newnode->Previous->Next = Newnode;
	
	}

	return Newnode;

}

NODE* InsertOnListAfterNode(void* ptr, NODE* Node){

	NODE* Newnode = malloc(sizeof(NODE));

	Newnode->ptr = ptr;
	Newnode->Previous = Node;
	Newnode->Next = Node->Next;
	Node->Next = Newnode;
	
	if(Newnode->Next != NULL){

		Newnode->Next->Previous = Newnode;
	
	}

	return Newnode;

}


void RemoveNodeFromList(NODE* Node){

	if(Node->Previous != NULL){

		Node->Previous->Next = Node->Next;

	}
	
	if(Node->Next != NULL){

		Node->Next->Previous = Node->Previous;

	}

	free(Node);

}

void ClearListFromStartToEnd(NODE* Node, void (*RemoveSomethingFromList)(NODE* Somenode)){

	if(RemoveSomethingFromList == NULL){

		while(Node != NULL){

			NODE* Target = Node;
			Node = Node->Next;
			RemoveNodeFromList(Target);

		}

	}

	else{
	
		while(Node != NULL){

			NODE* Target = Node;
			Node = Node->Next;
			(*RemoveSomethingFromList)(Target);
		
		}

	}

}

NODE* GetNodeFromList(NODE* Start, size_t offset){

	while(Start != NULL && offset--){
	
		Start = Start->Next;

	}

	return Start;

}

void merge(double array[], unsigned left, unsigned mid, unsigned right){

	unsigned n1 = mid - left + 1;
	unsigned n2 = right - mid;
	double L[n1], R[n2];

	for(unsigned i = 0; i < n1; i++){
	
		L[i] = array[left + i];

	}

	for(unsigned i = 0; i < n2; i++){
	
		R[i] = array[mid + 1 + i];

	}

	unsigned i = 0;
       	unsigned j = 0;
	unsigned k = left;

	while(i < n1 && j < n2){
	
		if(L[i] <= R[j]){
		
			array[k] = L[i];
			i++;
		
		}

		else{
		
			array[k] = R[j];
			j++;

		}

		k++;	

	}

	while(i < n1){
	
		array[k++] = L[i++];

	}

	while(j < n2){
	
		array[k++] = R[j++];

	}

}

void mergesort(double array[], unsigned left, unsigned right){

	if(left < right){
	
		unsigned mid = left + (right - left)/2;
		mergesort(array, left, mid);
		mergesort(array, mid + 1, right);
		merge(array, left, mid, right);
	
	}

}

double det(unsigned n, double a[n][n]){

	switch(n){

		case 0:
			return 1;
		break;

		case 1:
			return a[0][0];
		break;

		case 2:
			return ((a[0][0] * a[1][1]) - (a[1][0] * a[0][1]));
		break;

		default:{

			unsigned k = n - 1;
			double b[100][100] = {0};

			for(unsigned i = 0; i < n; i++){

				for(unsigned j = 0; j < n; j++){
			
					b[i][j] = a[i][j];

				}

			}

			bool det_sign = false;
			
			while(n > 2){

				n--; //Get last indice from matrix

				//Check if last value is not zero
				if(b[n][n]){

					//Translate into a (N - 1)(N - 1) matrix
					//Note that N is the current order of the matrix, not the variable n
					for(unsigned j = 0; j < n; j++){

						double rational = b[n][j]/b[n][n];

						for(unsigned i = 0; i < n; i++){
						
							b[i][j] -= b[i][n] * rational;
						
						}
					
					}

				}

				//Find a new column with last component different from zero
				else{
					
					double c = 0.0;
					unsigned m = 0;
					
					//Search for a new column
					while(m < n && !b[n][m]){
						m++;
					}

					if(m == n){
					
						return 0; //New column not found
					}

					else{

						//New column found, switch this column with the last one
						for(unsigned i = 0; i <= n; i++){

							c = b[i][n];
							b[i][n] = b[i][m];
							b[i][m] = c;

						}

						det_sign = !det_sign; //Invert sign
						n++; //Redo this iteration
						continue;
					}

				}

			}

			//Calculate determinant
			double det = ((b[0][0] * b[1][1]) - (b[1][0] * b[0][1]));

			for(unsigned i = 2; i <= k; i++){
			
				det *= b[i][i];

			}

			return (det_sign ? -det : det);

			break;

		}

	}

}
