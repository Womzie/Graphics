
// open a sequence of .xy  files specified on the commmand line
// and draw them.

#include "FPToolkit.c"
#include <string.h>
#include <math.h>
#include "M3d_matrix_toolsS.c"
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
double xmid[MAXOBJS], xsmall[MAXOBJS], xbig[MAXOBJS];
double ymid[MAXOBJS], ysmall[MAXOBJS], ybig[MAXOBJS];
double zmid[MAXOBJS], zsmall[MAXOBJS], zbig[MAXOBJS];
double sheight, swidth;

double h = 45 * M_PI/180;//half angle
double flip = 1;






int draw_object (int onum)
{
  int h,i,j ;
  double xp[100],yp[100], zp[100];
  double xb[100], yb[100];
  int np ;
  double H = tan(h);

  
  G_rgb(0,0,0) ;
  G_clear() ;
  G_rgb(1,0,0);

  for (i = 0 ; i < numpolys[onum] ; i++) {


    np = psize[onum][i] ;
    for (j = 0 ; j < np ; j++) {
      h = con[onum][i][j] ;
      xp[j] = x[onum][h] ;
      yp[j] = y[onum][h];
      zp[j] = z[onum][h];
      
      xb[j] = (400/H) * (xp[j]/zp[j]) + 400;
      yb[j] = (400/H) * (yp[j]/zp[j]) + 400;
      
    }
    double v1[3], v2[3], v3[3], vcross[3];
    double cross, dot, zero;
    double xhi = xp[0];
    double xlow = xp[0];
    double yhi = yp[0];
    double ylow = yp[0];
    double zhi = zp[0];
    double zlow = zp[0];

    double xmid, ymid, zmid;

    for(int k = 0; k < np; k++){
      if(xhi < xp[k]){xhi = xp[k];}
      if(xlow > xp[k]){xlow = xp[k];}
      if(yhi < yp[k]){yhi = yp[k];}
      if(ylow > yp[k]){ylow = yp[k];}
      if(zhi < zp[k]){zhi = zp[k];}
      if(zlow > zp[k]){zlow = zp[k];}
      
    }
    xmid = (xhi + xlow) /2;
    ymid = (yhi + ylow) /2;
    zmid = (zhi + zlow) /2;
    
    
    v1[0] = xmid - xp[1];
    v1[1] = ymid - yp[1];
    v1[2] = zmid - zp[1];
    
    v2[0] = xp[2] - xp[1];
    v2[1] = yp[2] - yp[1];
    v2[2] = zp[2] - zp[1];

    zero = M3d_x_product(vcross, v1, v2);
    
    if(zero == 0){
      printf("lol \n");
      for(int k = 0; k < np; k++){
	printf("%lf %lf %lf \n", xp[k], yp[k], zp[k]);
      }
    }
    
    v3[0] = xp[1];
    v3[1] = yp[1];
    v3[2] = zp[1];

    dot = (v3[0] * vcross[0]) + (v3[1] * vcross[1]) + (v3[2] * vcross[2]);

    if(flip * dot < 0){
      
      G_polygon(xb,yb,np) ;
    }
  }
  
  

  
}




void findmiddle(int onum){
  xsmall[onum] = swidth;
  xbig[onum] = 0;
  int i;
  for(i = 0; i < numpoints[onum]; i++){
    if(xsmall[onum] > x[onum][i]) xsmall[onum] = x[onum][i];
    if(xbig[onum] < x[onum][i]) xbig[onum] = x[onum][i];
    
  }
  xmid[onum] = (xbig[onum] + xsmall[onum]) / 2;

  
  ysmall[onum] = sheight;
  ybig[onum] = 0;
  for(i = 0; i < numpoints[onum]; i++){
    if(ysmall[onum] > y[onum][i]) ysmall[onum] = y[onum][i];
    if(ybig[onum] < y[onum][i]) ybig[onum] = y[onum][i];
    
  }
  ymid[onum] = (ybig[onum] + ysmall[onum]) / 2;

  zsmall[onum] = sheight;
  zbig[onum] = 0;
  for(i = 0; i < numpoints[onum]; i++){
    if(zsmall[onum] > z[onum][i]) zsmall[onum] = z[onum][i];
    if(zbig[onum] < z[onum][i]) zbig[onum] = z[onum][i];
    
  }
  zmid[onum] = (zbig[onum] + zsmall[onum]) / 2;
  
}



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
    z[onum][i] += 0.00000001;
  }
  findmiddle(onum);
  x[onum][i] = xmid[onum];
  y[onum][i] = ymid[onum];
  z[onum][i] = zmid[onum];

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
  swidth = 800;
  sheight = 800;

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


  int  sign = 1 ;
  int  action = 't' ;  
  int  onum = 0 ; // onum marks the current object
  int  q,k ;
  double xm, ym, zm;
  
  double V[4][4], Z[4][4], T[4][4] ;
  double trans = 0.5;
  double cs = cos(3 * M_PI/180);
  double sn = sin(3 * M_PI/180);
		  
  G_init_graphics(swidth,sheight) ;

  G_rgb(0,0,0) ;
  G_clear() ;

  G_rgb(1,0,0);
  draw_object(onum);
  
  while (1) {
    int n = numpoints[onum];
    xm = x[onum][n];
    ym = y[onum][n];
    zm = z[onum][n];


    M3d_make_identity (V) ;
    M3d_make_identity (T) ;
    M3d_make_identity (Z) ;


    G_rgb(1,0,0);
    draw_object(onum);
    
    q = G_wait_key() ;
    
    if (q == 'q') {
      exit(0) ;

    } else if (q == 'c') {
      sign = -sign ;

    } else if (q == 't') {
      action = q ;

    } else if (q == 'r') {
      action = q ;

    } else if(q == 'k'){
      flip *= -1;
    }

    else if (('0' <= q) && (q <= '9')) {
      k = q - '0' ;  
      if (k < numobs) { onum = k ; }

    } else if ((q == 'x') && (action == 't')) {
      M3d_make_translation(V, trans * sign, 0, 0);

    } else if ((q == 'y') && (action == 't')) {
      M3d_make_translation(V, 0, trans * sign, 0);

    } else if ((q == 'z') && (action == 't')) {
      M3d_make_translation(V, 0, 0,trans * sign);
      
    } else if ((q == 'x') && (action == 'r')) {
      M3d_make_translation(Z, 0, -ym, -zm);
      M3d_make_x_rotation_cs(V, cs, sn * sign);
      M3d_make_translation(T, 0, ym, zm);
      M3d_mat_mult(V, V, Z);
      M3d_mat_mult(V, T, V);
      
    } else if ((q == 'y') && (action == 'r')) {
      M3d_make_translation(Z, -xm, 0, -zm);
      M3d_make_y_rotation_cs(V, cs, sn * sign);
      M3d_make_translation(T, xm, 0, zm);
      M3d_mat_mult(V, V, Z);
      M3d_mat_mult(V, T, V);

    } else if ((q == 'z') && (action == 'r')) {
      M3d_make_translation(Z, -xm, -ym, 0);
      M3d_make_z_rotation_cs(V, cs, sn * sign);
      M3d_make_translation(T, xm, ym, 0);
      M3d_mat_mult(V, V, Z);
      M3d_mat_mult(V, T, V);

    } else {
      printf("no action\n") ;
    }

    M3d_mat_mult_points (x[onum],y[onum],z[onum],  V, x[onum],y[onum],z[onum],numpoints[onum]+1);

  }
  

}


