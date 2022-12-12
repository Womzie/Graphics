

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
int psize[MAXOBJS][MAXPOLYS] ; // size of poly
int con[MAXOBJS][MAXPOLYS][20] ; // conectivity info


double red[MAXOBJS][MAXPOLYS],grn[MAXOBJS][MAXPOLYS],blu[MAXOBJS][MAXPOLYS] ;


double xmid[MAXOBJS], ymid[MAXOBJS], xsmall[MAXOBJS], ysmall[MAXOBJS], xbig[MAXOBJS], ybig[MAXOBJS];
double sheight, swidth;

double px[MAXOBJS][MAXPOLYS][20];
double py[MAXOBJS][MAXPOLYS][20];

#include "FPToolkit.c"

double cx, cy;

double centerX(double x[], int n){
  double sumX = 0;
  for(int i = 0; i < n; i++){
    sumX = sumX + x[i];
  }


  return sumX/n;
}

double centerY(double y[], int n){
  double sumY = 0;
  for(int i = 0; i < n; i++){
    sumY = sumY + y[i];
  }
  return sumY/n;
}


int intersect_2_lines (double A[2], double B[2],
                       double C[2], double D[2],
                       double intersection[2])
// return 0 if lines do NOT intersect
// return 1 if they do  
{
  double a[2], b[2], c[2];
  double x[2], y[2];
  
  a[0] = A[1] - B[1];
  b[0] = B[0] - A[0];
  c[0] = (A[0] * B[1]) - (B[0] * A[1]);


  x[0] = C[0];
  x[1] = D[0];
  y[0] = C[1];
  y[1] = D[1];
  
  a[1] = C[1] - D[1];
  b[1] = D[0] - C[0];
  c[1] = (C[0] * D[1]) - (D[0] * C[1]);

  if(a[0]/b[0] == a[1]/b[1]){
    return 0;
  }
  

  intersection[0] = (b[0] * c[1] - b[1] * c[0]) / (a[0] * b[1] - a[1] * b[0]);
  intersection[1] = (a[1] * c[0] - a[0] * c[1]) / (a[0] * b[1] - a[1] * b[0]);

  return 1;
  
}



int good_bad(double a, double b, double c, double P[]){
  double valC, valP;
  valC = (a * cx) + (b * cy) + c;
  valP = (a * P[0]) + (b * P[1]) + c;

  if(valP * valC < 0){
    return 0;
  }
  return 1;
    
}





int in_out (double A[], double B[], double P[2])
// return 1 if point P is inside the convex polygon
// else return 0
{
  double a,b,c;


  G_rgb(1,1,1);
  G_fill_circle(cx,cy,5);
  
  int flag = 1;

  
  a = A[1] - B[1];
  b = B[0] - A[0];
  c = (A[0] * B[1]) - (B[0] * A[1]);

  if(good_bad(a,b,c, P) == 0){
    flag = 0;

  }
    
  return flag;
}




int  Clip_Polygon_Against_Convex_Window (
      double rx[],  double ry[], int rsize,
      double wx[],  double wy[], int wsize, int onum, int loc)
{

  cx = centerX(wx, wsize);
  cy = centerY(wy, wsize);
  //good1 -> good2 | good2
  //good  -> bad   | find intersect
  //bad   -> good  | find intersect, good
  //bad   -> bad   | nothing

  double r = 0;
  int s;
  for(int i = 0; i < wsize; i ++){
    int j = (i+1)%wsize;

    double P1[2], P2[2], W1[2], W2[2];
    double tempx[100], tempy[100];
    s = 0;
    
    W1[0] = wx[i];
    W1[1] = wy[i];

    W2[0] = wx[j];
    W2[1] = wy[j];

    for(int k = 0; k < rsize; k++){
      int l = (k+1)%rsize;

      P1[0] = rx[k];
      P1[1] = ry[k];
      
      P2[0] = rx[l];
      P2[1] = ry[l];
      
      if(in_out(W1, W2, P1) == 1){

	if(in_out(W1, W2, P2) == 1){
	    tempx[s] = P2[0];
	    tempy[s] = P2[1];

	    s++;
	}
	else{
	  double isect[2];
	  int d = intersect_2_lines(W1, W2, P1, P2, isect);
	  tempx[s] = isect[0];
	  tempy[s] = isect[1];
	  
	  s++;
	}
      }
      
      else{

	if(in_out(W1, W2, P2) == 1){
	  double isect[2];
	  int d = intersect_2_lines(W1, W2, P1, P2, isect);

	  tempx[s] = isect[0];
	  tempy[s] = isect[1];
	  s++;
	  tempx[s] = P2[0];
	  tempy[s] = P2[1];
	  s++;

	}
	else{
	  //printf("BAD \n");
	}

      }//end of else
     
      
    }//end of inner for

    for(int w = 0; w < s; w++){
      rx[w] = tempx[w];
      ry[w] = tempy[w];
      px[onum][loc][w] = tempx[w];
      py[onum][loc][w] = tempy[w];
    }
    rsize = s;
    r += 50;
  }  
  return rsize ;
}


int draw_object2(int onum){
  G_rgb(0,0,0);
  G_clear();

  for(int i = 0; i < numpolys[onum]; i++){
    G_rgb(red[onum][i], grn[onum][i], blu[onum][i]);
    G_fill_polygon(px[onum][i], py[onum][i], psize[onum][i]);
  }
  
}

int draw_object (int onum)
{
  int h,i,j ;
  double xp[100],yp[100] ;
  int np ;

  G_rgb(0,0,0) ;
  G_clear() ;

  for (i = 0 ; i < numpolys[onum] ; i++) {

    np = psize[onum][i] ;
    for (j = 0 ; j < np ; j++) {
      h = con[onum][i][j] ;
      xp[j] = x[onum][h] ;
      yp[j] = y[onum][h] ;
    }

    G_rgb(red[onum][i], grn[onum][i], blu[onum][i]) ;
    G_fill_polygon(xp,yp,np) ;
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
    fscanf(f,"%lf %lf",&x[onum][i],&y[onum][i]) ;
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

    
    // color info :
  for (i = 0 ; i < numpolys[onum] ; i++) {
    fscanf(f,"%lf %lf %lf",&red[onum][i],&grn[onum][i],&blu[onum][i]) ;
  }


  double t[3][3], s[3][3], m[3][3];
  findmiddle(onum);
  M2d_make_translation(t, -xmid[onum], -ymid[onum]);

  
  double scalex = (swidth * 0.8) / (xbig[onum] - xsmall[onum]);
  double scaley = (sheight * 0.8) / (ybig[onum] - ysmall[onum]);
  if(scalex <= scaley) M2d_make_scaling(s,scalex, scalex);
  if(scalex > scaley) M2d_make_scaling(s, scaley, scaley);

  
  
  M2d_make_translation(m, (swidth/2), (sheight/2));
  M2d_mat_mult(s, s, t);
  M2d_mat_mult(m, m, s);
  
  M2d_mat_mult_points(x[onum],y[onum],m, x[onum], y[onum], numpoints[onum]); // expensive, call as rarely as possible
  findmiddle(onum);

}

int click_and_save (double *x, double *y)
{
  int n ;
  double P[2] ;

  G_rgb(0,1,0.5) ;
  G_fill_rectangle(0,0,swidth,20) ;

  G_rgb(1,0,0) ;
  G_wait_click(P);

  n = 0 ;
  while (P[1] > 20) {
    x[n] = P[0] ;
    y[n] = P[1] ;
    G_circle(x[n],y[n],2) ;
    if (n > 0) {
      G_line(x[n-1],y[n-1], x[n],y[n]) ;
    }
    n++ ;
    G_wait_click(P) ;
  }
  G_line(x[n-1],y[n-1], x[0], y[0]);
  cx = centerX(x, n);
  cy = centerY(y,n);
  return n ;
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
      printf("enter name of xy file ") ;
      scanf("%s",fname) ;
  
      fin = fopen(fname,"r") ;
      if (fin == NULL) {
        break;
      }
      numobs++;
      read_object(fin,k) ;

    }
  }



  
  //  print_object(stdout, 0) ;

  G_init_graphics(swidth,sheight) ;

  G_rgb(0,0,0) ;
  G_clear() ;
  
  int onum = 0;
  int i = 0;

      
  double r[3][3], t[3][3], z[3][3];
  M2d_make_translation(t, -swidth/2, -sheight/2);
  M2d_make_rotation(r, 3 * M_PI/180);
  M2d_make_translation(z, swidth/2, sheight/2);
  M2d_mat_mult(r,  r,t);
  M2d_mat_mult(r,  z, r);  



  for(int b = 0; b < numobs; b++){
      for(int n = 0; n < numpolys[b]; n++){
      for(int m = 0; m < psize[b][n]; m++){
	int loc = con[b][n][m];
	px[b][n][m] = x[b][loc];
	py[b][n][m] = y[b][loc];
      }
    }
  }

  while(True){
    // center it
    
   



    draw_object2(onum) ;
    //G_rgb(1,0,0);
    //G_fill_circle(xmid[onum],ymid[onum],5);

    key = G_wait_key();
    if(key == 'q'){
      break;
    }
    if(key == 'c'){
      double wx[100], wy[100], wsize;
      
      wsize = click_and_save (wx, wy);      for(int i = 0; i < numpolys[onum];i++){
	psize[onum][i] = Clip_Polygon_Against_Convex_Window ( px[onum][i], py[onum][i], psize[onum][i], wx, wy, wsize, onum, i);
      }


      G_wait_key();
      draw_object2(onum);
      
    }
    
    else if(key == 'r'){
      for(int i = 0; i < numpolys[onum]; i++){
	M2d_mat_mult_points(px[onum][i], py[onum][i], r, px[onum][i], py[onum][i], psize[onum][i]);

      }

      draw_object2(onum);
    }

    else if ('0' <= key && key <= '9'){
      w = key - '0';
      if(w < numobs){onum = w;}

    }
    i ++;
  }

}


