#ifndef COMFORMITY
#define COMFORMITY

#define PI 3.1415926 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

double F_sum (double *a, int n);

int Skip (FILE *fileName);

int Angle_sum (int a[][3], int n, int *r);

int Angle_cul (int *Knowed_Angle, int *Obser_Angle, int *ret, char mode);

int Coord_Forwd (double *Knowed_Coord, double *Distance, int *Azimu_Angle, double *Compt_Coord );

int Coord_Inv (double *Coord_A, double *Coord_B, int *Obser_Azimu);

int Angle_AS(int *Angle_1, int *Angle_2, int *Angle_Ret, char mode);

int Coord_Increment ( double *Distance, int *Azimuth, double *Increment, char mode);

int Correct_Azimuth ( int (*originalAzimuth)[3], int (*correctedAzimuth)[3], int *closureError, int segmentNum, char angleMode);

int Coordinate_Error ( int segmentNum, double (*deltaCoord)[2], double *startPoint, double *endPoint, double *errorResult, double *distance);

int Angle_Egde ( double *point_1st, double *point_2nd, int *azimuthResult);
#endif  //#ifndef COMFORMITY