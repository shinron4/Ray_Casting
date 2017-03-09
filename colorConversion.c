#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define hexbit(h) (h >= '0' && h <= '9' ? (h - '0') : (h >= 'a' && h <= 'f' ? 10 + h - 'a' : -1)) 

void RGB2HSV(float *H, float *S, float *V, float R, float G, float B){
	float M, m;
	R /= 255;
	G /= 255;
	B /= 255;
	M = R > G && R > B ? R : (G > B ? G : B);
	m = R < G && R < B ? R : (G < B ? G : B);
	if(M - m == 0) *H = 0;
	else if(M == R) *H = (int)ceil((G - B)/(M - m)) % 6;
	else if(M == G) *H = (B - R)/(M - m) + 2;
	else if(M == B) *H = (R - G)/(M - m) + 4;
	*H *= 60;
	*S = (M == 0 ? 0 : (M - m)/M);
	*V = M;
}


void HSV2RGB(unsigned char rgb[], float fH, float fS, float fV) {
  float fC = fV * fS;
  float fHPrime = fmod(fH / 60.0, 6);
  float fX = fC * (1 - fabs(fmod(fHPrime, 2) - 1));
  float fM = fV - fC;
  float fR, fG, fB;
  if(0 <= fHPrime && fHPrime < 1) {fR = fC;fG = fX;fB = 0;}
  else if(1 <= fHPrime && fHPrime < 2) {fR = fX;fG = fC;fB = 0;}
  else if(2 <= fHPrime && fHPrime < 3) {fR = 0;fG = fC;fB = fX;}
  else if(3 <= fHPrime && fHPrime < 4) {fR = 0;fG = fX;fB = fC;}
  else if(4 <= fHPrime && fHPrime < 5) {fR = fX;fG = 0;fB = fC;}
  else if(5 <= fHPrime && fHPrime < 6) {fR = fC;fG = 0;fB = fX;}
  else {fR = 0;fG = 0;fB = 0;}
  
  fR += fM;
  fG += fM;
  fB += fM;
  rgb[0] = (int)ceil(fR*255);
  rgb[1] = (int)ceil(fG*255);
  rgb[2] = (int)ceil(fB*255);
 }