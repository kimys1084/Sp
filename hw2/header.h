#include <stdio.h>



typedef struct OBJECT_{
	float **V;
	int **F;
	float scale;
	
	int vertexNum;
	int faceNum;

}OBJECT;

typedef struct VECTOR3D_{
	float x;
	float y;
	float z;


}VECTOR3D;
