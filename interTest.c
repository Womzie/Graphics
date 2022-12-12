#include "FPToolkit.c"
#include <string.h>
#include <math.h>
#include "M3d_matrix_toolsS.c"
#include <stdio.h>
#include <stdlib.h>



double findIntersect(double v1[3], double v2[3], double a[4]){
  double u;

  u = - (a[0] * v1[0]) - (a[1] * v1[1]) - (a[2] * v1[2]) - a[3];
  u /= (a[0] * v2[0]) + (a[1] * v2[1]) + (a[2] * v2[2]);

  return u;
}


int main(int argc, char **argv)
{
  double v1[3], v2[3], a[4];
  v1[0] = 3;
  v1[1] = 6;
  v1[2] = 2;

  v2[0] = 4;
  v2[1] = 9;
  v2[2] = 10;

  a[0] = -528;
  a[1] = 12;
  a[2] = 138;
  a[3] = 3108;

  printf("%lf \n", findIntersect(v1,v2,a));

}
