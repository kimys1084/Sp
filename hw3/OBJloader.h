#include <string.h>
#include <fstream>
#include <iostream>
#include <stdio.h>


#define SCALE 0.5

using namespace std;

class OBJECT{
	private:
		int faceSize;
		int vertexSize;
		int vertexNormalSize;
		int **F;
		float **V;
		float **VN;

	public:
		void loadOBJFile(const char* filename);
		float vertex(int i, int j);
		float vertexNorm(int i, int j);
		int face(int i, int j);
		int getFaceSize();
		void deleteOBJ();
};


void OBJECT::loadOBJFile(const char * filename){

	char lineData[200];
	int f_cnt=0, v_cnt=0, vn_cnt=0;
 
	ifstream inFile(filename);

	while(!inFile.eof()){
		inFile.getline(lineData, 200);

		if ( !strncmp(lineData, "v ",2)){v_cnt++; }
		else if( !strncmp(lineData, "vn", 2)){ vn_cnt++; }
		else if( !strncmp(lineData, "f ", 2)){f_cnt++; }

	}
	faceSize = f_cnt;
	vertexSize = v_cnt;
	vertexNormalSize = vn_cnt;

	//---------------------------------
	cout << "OBJ Loading Success!" << endl;
	cout << faceSize << endl;
	cout << vertexSize << endl;
	cout << vertexNormalSize << endl;

	//---------------------------------


	inFile.close();
	//memory allocate

	F = new int*[faceSize];
	for( int i=0; i < faceSize; i++){ F[i] = new int[3]; }
	V = new float*[vertexSize];
	for (int i=0; i < vertexSize; i++){ V[i] =  new float[3]; }
	VN = new float*[vertexNormalSize];
	for (int i=0; i < vertexNormalSize; i++){ VN[i] = new float[3]; }

	//---------------

	ifstream inFile1(filename);

	char buf[3][40];
	char ch[3];
	v_cnt = f_cnt = vn_cnt = 0;
	while(!inFile1.eof()){
		inFile1.getline(lineData, 200);

		if ( !strncmp(lineData, "v ",2)){
			sscanf(lineData, "%s %s %s %s", ch, buf[0], buf[1], buf[2]);
			V[v_cnt][0] = atof(buf[0])*SCALE;
			V[v_cnt][1] = atof(buf[1])*SCALE;
			V[v_cnt][2] = atof(buf[2])*SCALE;
			v_cnt++;
		}
		else if( !strncmp(lineData, "vn", 2)){ 
			sscanf(lineData, "%s %s %s %s", ch, buf[0], buf[1], buf[2]);
			VN[vn_cnt][0] = atof(buf[0]);
			VN[vn_cnt][1] = atof(buf[1]);
			VN[vn_cnt][2] = atof(buf[2]);
			vn_cnt++;
		}
		else if( !strncmp(lineData, "f ", 2)){
			sscanf(lineData, "%s %s %s %s", ch, buf[0], buf[1], buf[2]);
		/*	F[f_cnt][0] = atoi(buf[0])-1;
			F[f_cnt][1] = atoi(buf[1])-1;
			F[f_cnt][2] = atoi(buf[2])-1; */
			int tmp;
			sscanf(buf[0], "%d//%d", &F[f_cnt][0], &tmp);
			sscanf(buf[1], "%d//%d", &F[f_cnt][1], &tmp);
			sscanf(buf[2], "%d//%d", &F[f_cnt][2], &tmp);
			F[f_cnt][0]-=1;
			F[f_cnt][1]-=1;
			F[f_cnt][2]-=1;
			f_cnt++;
		}

	}		
	inFile1.close();

	return;
} 

float OBJECT::vertex(int i, int j){ return V[i][j]; }
float OBJECT::vertexNorm(int i, int j){ return VN[i][j]; }
int OBJECT::face(int i, int j){ return F[i][j]; }
int OBJECT::getFaceSize(){ return faceSize; }
void OBJECT::deleteOBJ(){

	for( int i=0; i < faceSize; i++){ delete[] F[i];}
	delete[] F;

	for( int i=0; i < vertexSize; i++){ delete[] V[i];}
	delete[] V;

	for( int i=0; i < vertexNormalSize; i++){ delete[] VN[i];}
	delete[] VN;

	return;
}
