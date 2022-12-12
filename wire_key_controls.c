// read the data from multiple xyz files and display any of the objects
#include "FPToolkit.c"
#include "M3d_matrix_toolsS.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


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
double sheight, swidth;




int main (int argc, char **argv)
{
  // lots of stuff here ....


  FILE *fin ;
  int key,w ;

  
  int  sign = 1 ;
  int  action = 't' ;  
  int  onum = 0 ; // onum marks the current object
  int  q,k ;
  double V[4][4] ;

  int numobs = 0;

  printf("%d \n", numobs);
  
  while (1) {

    M3d_make_identity (V) ;


    q = G_wait_key() ;
    
    if (q == 'q') {
      exit(0) ;

    } else if (q == 'c') {
      sign = -sign ;

    } else if (q == 't') {
      action = q ;

    } else if (q == 'r') {
      action = q ;

    } else if (('0' <= q) && (q <= '9')) {
      k = q - '0' ;  
      if (k < numobs) { onum = k ; }

    } else if ((q == 'x') && (action == 't')) {

    } else if ((q == 'y') && (action == 't')) {

    } else if ((q == 'z') && (action == 't')) {

    } else if ((q == 'x') && (action == 'r')) {

    } else if ((q == 'y') && (action == 'r')) {

    } else if ((q == 'z') && (action == 'r')) {

    } else {
      printf("no action\n") ;
    }



    M3d_mat_mult_points (x[onum],y[onum],z[onum],  V,
			 x[onum],y[onum],z[onum],numpoints[onum]+1) ;
      // the numpoints[onum]+1 is because we have stored the center
      // of the object at the arrays' end

    //G_rgb(0,0,0) ; 
    //G_clear() ;
    // G_rgb(0,0,1) ;
    //    draw_all_objects() ;
    //draw_single_object(onum) ;

    return 1;
  }


}

