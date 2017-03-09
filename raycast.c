#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define hexbit(h) (h >= '0' && h <= '9' ? (h - '0') : (h >= 'a' && h <= 'f' ? 10 + h - 'a' : -1)) 
#define MAX 100000
#define PIXX 500
#define PIXY 500

//3-D vector declaration.

typedef float vector[3];

//Declaration for the grid.

struct pointlist {
	vector p;
	struct pointlist *next;
} *G[100][100][100];

//Vector array for storing the centers of the voxels.

vector V[MAX]; 

//x y and z intervals of the voxel pixels.

float xmin, xmax, ymin, ymax, zmin, zmax;

//Minimum allowed subsize of the grid.

int subsize;

//Vector function declarations.

int copy(vector, float, float, float);
float min(float, float);
float max(float, float);
float magnitude(vector);
void difference(vector, vector, vector);

void intersectPoint(vector, vector, vector, vector, vector);
float cubeIntersect(float, float, float, float, vector, vector);

//Color Conversion function declaration.

void RGB2HSV(float *, float *, float *, float, float, float);
void HSV2RGB(unsigned char [], float, float, float);

float recursiveIntersect(float x, float y, float z, int size, int curr_d, int max_d, vector a, vector d){
	int p, q, r, i, n_size;
	struct pointlist *temp;
	float mind = 1000000, n_x, n_y, n_z;
	if(curr_d == max_d - 1){
		p = (int)((x - xmin) / size);
		q = (int)((y - ymin) / size);
		r = (int)((z - zmin) / size);
		temp = G[p][q][r];
		while(temp){
			mind = min(mind, cubeIntersect(temp->p[0] - 0.5, temp->p[1] - 0.5, temp->p[2] - 0.5, 1, a, d));
			temp = temp->next;
		}
		return mind;
	}
	else if(cubeIntersect(x, y, z, size, a, d) >= 1000000){
		return mind;
	}
	else{
		n_size = size >> 1;
		for(i = 0; i < 8; i++){
			n_x = (i & 1) > 0 ? x + n_size : x;
			n_y = (i & 2) > 0 ? y + n_size : y;
			n_z = (i & 4) > 0 ? z + n_size : z;
			mind = min(mind, recursiveIntersect(n_x, n_y, n_z, n_size, curr_d + 1, max_d, a, d));
		}
	}
	return mind;
}

int main(int argc, char *argv[]){

	//Variable declarations.

	float x, y, z, mind, HUE, SAT, VAL, vrel;
	int v, i, j, p, q, r, n, RE, GR, BL, size, depth;
	vector a, d, s, bn, bp, ip;
	struct pointlist *temp;
	FILE *infile, *outfile;

	//rgb value of the pixel initialized to background value(white).

	unsigned char rgb[3] = {0, 0, 0};


	//Initializing the upper and lower bound.

	xmin = ymin = zmin = 1000000;
	xmax = ymax = zmax = -1000000;

	v = 0;

	//No input file given.

	if(argc < 3){
		printf("error : provide the input file\n");
		exit(1);
	}

	//Open input and output File

	infile = fopen(argv[1], "r");
	outfile = fopen("output.ppm", "wb");

	(void)fprintf(outfile, "P6\n%d %d\n255\n", PIXX, PIXY);

	//Read the number of the Voxels.

	fscanf(infile, "%d", &n);

	//Read the Voxel centers.

	for(i = 0; i < n; i++){
		fscanf(infile, "%f%f%f", &V[v][0], &V[v][1], &V[v][2]);
		if(V[v][0] > xmax) xmax = V[v][0];
		if(V[v][0] < xmin) xmin = V[v][0];
		if(V[v][1] > ymax) ymax = V[v][1];
		if(V[v][1] < ymin) ymin = V[v][1];
		if(V[v][2] > zmax) zmax = V[v][2];
		if(V[v][2] < zmin) zmin = V[v][2];
		v++;
	}
	xmin -= 0.5;
	ymin -= 0.5;
	zmin -= 0.5;
	xmax += 0.5;
	ymax += 0.5;
	zmax += 0.5;

	//Reading the Maximum depth of the grid.

	scanf("%d", &depth);

	//Computing the grid and subcude size.

	size = ceil(max(max(xmax - xmin, ymax - ymin), zmax - zmin));
	size = size >> 1;
	i = 0;
	while(size > 0){
		size = size >> 1;
		i++;
	}
	size = 1 << (i + 1);
	subsize = size >> (depth - 1);

	//Viewer coordinates.

	a[0] = xmin;
	a[1] = ymin;
	scanf("%f", &a[2]);

	//Initilazing the grid.

	for((x = xmin), p = 0; x < xmin + size; (x += subsize), p++)
		for((y = ymin), q = 0; y < ymin + size; (y += subsize), q++)
			for((z = zmin), r = 0; z < zmin + size; (z += subsize), r++)
				G[p][q][r] = 0; 

	//Finding the subcube for the each Voxel.

	for(i = 0; i < n; i++){
		p = (int)((V[i][0] - xmin) / subsize);
		q = (int)((V[i][1] - ymin) / subsize);
		r = (int)((V[i][2] - zmin) / subsize);
		temp = (struct pointlist *) malloc(sizeof(struct pointlist));
		copy(temp->p, V[i][0], V[i][1], V[i][2]);
		temp->next = G[p][q][r];
		G[p][q][r] = temp;
	}

	//Reading the rendering color

	RE = hexbit(argv[2][1]) * 16 + hexbit(argv[2][2]);
	GR = hexbit(argv[2][3]) * 16 + hexbit(argv[2][4]);
	BL = hexbit(argv[2][5]) * 16 + hexbit(argv[2][6]);
	RGB2HSV(&HUE, &SAT, &VAL, RE, GR, BL);

	//Initializing the background plane normal.

	copy(bn, 0, 0, -1);

	//Point on the background plane.

	copy(bp, xmin, ymin, zmin);

	//Ray Casting Logic

	for(j = PIXY - 1; j >= 0; j--){
		for(i = 0; i < PIXX; i++){
			copy(s, a[0] - PIXX/2 + i, a[1] - PIXY/2 + j, zmax + size + 10);
			difference(a, s, d);
			if((mind = recursiveIntersect(xmin, ymin, zmin, size, 0, depth, a, d)) >= 1000000.0) rgb[0] = rgb[1] = rgb[2] = 255;
			else{
				intersectPoint(bn, bp, a, d, ip);
				difference(a, ip, ip);
				vrel = mind/magnitude(ip);
				HSV2RGB(rgb, HUE, SAT, vrel);
			}
			(void)fwrite(rgb, 1, 3, outfile);
		}
	}
	return 0;
}