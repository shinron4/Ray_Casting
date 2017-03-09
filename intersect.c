#include <math.h>

//3-D vector declaration.

typedef float vector[3];

//Auxillary vector function.

float magnitude(vector p1){
	return sqrt(p1[0] * p1[0] + p1[1] * p1[1] + p1[2] * p1[2]);
}

void difference(vector p1, vector p2, vector res){
	res[0] = p2[0] - p1[0];
	res[1] = p2[1] - p1[1];
	res[2] = p2[2] - p1[2];
}

void scalarMult(vector p1, float t, vector res){
	res[0] = t * p1[0];
	res[1] = t * p1[1];
	res[2] = t * p1[2];
}

void cross(vector p1, vector p2, vector res){
	float mag;
	res[0] = p1[1]*p2[2] - p1[2]*p2[1];
	res[1] = p1[2]*p2[0] - p1[0]*p2[2];
	res[2] = p1[0]*p2[1] - p1[1]*p2[0];
	mag = magnitude(res);
	scalarMult(res, 1 / mag, res);
}

float dot(vector p1, vector p2){
	return p1[0]*p2[0] + p1[1]*p2[1] + p1[2]*p2[2];
}

int copy(vector v1, float x, float y, float z){
	v1[0] = x;
	v1[1] = y;
	v1[2] = z;
	return 1;
}

float max(float a, float b){
	return a > b ? a : b;
}

float min(float a, float b){
	return a < b ? a : b;
}

//Checking if vector p2 lies to the left of p1 on a plane whose normal is n

int leftOn(vector p1, vector p2, vector n){
	vector c;
	cross(p1, p2, c);
	if(dot(c, n) >= 0)
		return 1;
	else
		return 0;
}

//Finding intersction point with the plane.

void intersectPoint(vector n, vector p, vector a, vector d, vector i){
	float D, t;
	scalarMult(d, 1 / magnitude(d), d);
	D = -1 * dot(n, p);
	t = -1 * (D + dot(a, n)) / dot(n, d);
	scalarMult(d, - 1 * t, i);
	difference(i, a, i);	
}

//Intersection of a line with the ploygon.

float polygonIntersect(vector polygon[], int sides, vector a, vector d){

	//Variable declarations.

	vector v1, v2, n, p;
	float D, t;
	int j;

	//Finding the normal to the plane.

	difference(polygon[0], polygon[1], v1);
	difference(polygon[1], polygon[2], v2);
	cross(v1, v2, n);

	//Finding the intersection point with the plane. p is the point of intersection.

	intersectPoint(n, polygon[0], a, d, p);

	//Checking if intersection point lies insdie the polygon.

	for(j = 0; j < sides; j++){
		difference(polygon[j], polygon[(j + 1) % 4], v1);
		difference(polygon[j], p, v2);
		if(!leftOn(v1, v2, n))

			//If no return -1.

			return -1.0;
	}

	//If yes return the distance of point from the plane.

	difference(a, p, p);
	
	return magnitude(p);
}

float cubeIntersect(float x, float y, float z, float size, vector a, vector d){

	vector p1[4], p2[4], p3[4], p4[4], p5[4], p6[4];

	//Function declaration for finding the intersection of line with any polygonal face.

	float  polygonIntersect(vector [], int, vector, vector);
	float mind = 1000000, temp, min(float, float); //1000000 Distance of the background

	//Testing intersection with each face.

	copy(p1[0], x, y, z), copy(p1[1], x + size, y, z), copy(p1[2], x + size, y, z + size), copy(p1[3], x, y, z + size);

	if((temp = polygonIntersect(p1, 4, a, d)) > 0)
		mind = min(temp, mind);

	copy(p2[0], x, y, z + size), copy(p2[1], x + size, y, z + size), copy(p2[2], x + size, y + size, z + size), copy(p2[3], x, y + size, z + size);

	if((temp = polygonIntersect(p2, 4, a, d)) > 0)
		mind = min(temp, mind);

	copy(p3[0], x, y + size, z + size), copy(p3[1], x + size, y + size, z + size), copy(p3[2], x + size, y + size, z), copy(p3[3], x, y + size, z);

	if((temp = polygonIntersect(p3, 4, a, d)) > 0)
		mind = min(temp, mind);

	copy(p4[0], x, y + size, z), copy(p4[1], x + size, y + size, z), copy(p4[2], x + size, y, z), copy(p4[3], x, y, z);	

	if((temp = polygonIntersect(p4, 4, a, d)) > 0)
		mind = min(temp, mind);

	copy(p5[0], x, y, z), copy(p5[1], x, y, z + size), copy(p5[2], x, y + size, z + size), copy(p5[3], x, y + size, z);

	if((temp = polygonIntersect(p5, 4, a, d)) > 0)
		mind = min(temp, mind);

	copy(p6[0], x + size, y + size, z), copy(p6[1], x + size, y + size, z + size), copy(p6[2], x + size, y, z + size), copy(p6[3], x + size, y, z);

	if((temp = polygonIntersect(p6, 4, a, d)) > 0)
		mind = min(temp, mind);

	return mind;
}