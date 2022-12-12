
// open a sequence of .xy  files specified on the commmand line
// and draw them.

#include "FPToolkit.c"
#include <string.h>
#include <math.h>
#include "M2d_matrix_tools.c"
#include <stdio.h>
#include <stdlib.h>


#define MAXOBJS 10
#define MAXPTS 59000
#define MAXPOLYS 57500


int numpoints [MAXOBJS];
int numpolys[MAXOBJS] ;
double x[MAXOBJS][MAXPTS] ;
double y[MAXOBJS][MAXPTS] ;
double z[MAXOBJS][MAXPTS] ;

int psize[MAXOBJS][MAXPOLYS] ; // size of poly
int con[MAXOBJS][MAXPOLYS][20] ; // conectivity info





int read_object(FILE *f, int onum)
{
  int i,j,k ;

  
  // point info
  fscanf(f,"%d",&numpoints[onum]) ;

  if (numpoints[onum] >= MAXPTS) {
    // need an extra for object centering
    printf("MAXPTS = %d :  exceeded.\n",MAXPTS) ;
    exit(1) ;
  }

  for (i = 0 ; i < numpoints[onum] ; i++) {
    fscanf(f,"%lf %lf %lf",&x[onum][i],&y[onum][i], &z[onum][i]) ;
  }

  // connectivity info
  fscanf(f,"%d",&numpolys[onum]) ;
  if (numpolys[onum] > MAXPOLYS) {
    printf("MAXPOLYS = %d :  exceeded.\n",MAXPOLYS) ;
    exit(1) ;
  }
    
  k = 0 ;
  for (i = 0 ; i < numpolys[onum] ; i++) {
    fscanf(f,"%d",&psize[onum][i]) ;
    for (j = 0 ; j < psize[onum][i] ; j++) {
      fscanf(f,"%d",&con[onum][i][j]) ;
    } // end for j
  } // end for i

}




int main(int argc, char **argv)
{
  FILE *fin ;
  int key,w ;

  int numobs = 0;

  if(argc > 1){
    char fname[100] ;
    for(int i = 1; i < argc; i++){

      memset(fname, 0, sizeof fname);

      int j = 0 ;
      while (argv[i][j] != '\0') {
	fname[j] = argv[i][j];
	j++ ;
      }
     

      fin = fopen(fname,"r") ;
      if (fin == NULL) {
        printf("Cant open file %s\n", fname);
      }
      read_object(fin,i-1) ;
      numobs++;
    }
  
  }
  else{
    char fname[100];
    for(int k = 0; k < MAXOBJS; k++){
      printf("enter name of xyz file ") ;
      scanf("%s",fname) ;
  
      fin = fopen(fname,"r") ;
      if (fin == NULL) {
        break;
      }
      numobs++;
      read_object(fin,k) ;

    }
  }

  for(int i = 0; i < numpoints[0]; i++){
    printf("%lf %lf %lf \n", x[0][i], y[0][i], z[0][i]);
  }

}


