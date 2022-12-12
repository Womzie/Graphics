#include "FPToolkit.c"
#include <string.h>
#include <math.h>
#include "M3d_matrix_toolsS.c"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  FILE *fin ;
  int key,w ;

  if(argc == 3){
    char fname[100];

    double a,b,c;
    a = atof(argv[1]);
    b = atof(argv[2]);
    c = a+b;
    printf("%lf \n", c);
      
  }
}
