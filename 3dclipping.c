
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

double red[10]   = {1, 0.55, 0.9, 0.9, 0.4, 0, 0, 0, 0, 0.5};
double green[10] = {0, 0.27, 0.1, 0, 0.1, 0.1, 0.5, 0.5, 0.5, 0.8};
double blue[10]  = {0, 0.07,  0.1, 0.5, 0.4, 0.5, 0.9, 0.5, 0.3, 0.1};

// constant background radiation
double ambient = 0.2;
double locLight[3] = {100,200,0};
double maxDiffuse = 0.5;
double SPECPOW = 50;



typedef
struct {
  int objnum ;
  int polynum ;
  double dist ;
}
THING ;

int n;
THING thing[MAXPTS];




double find_dist(int onum, int poly){
  
  double dist = 0;
  int i;
  for(i = 0; i < psize[onum][poly]; i ++){
    int loc = con[onum][poly][i];
    dist += z[onum][loc];
   
   
  }
  return dist/i;
}


void init_array(int numobs)
{
  int p = 0;
  for(int i = 0; i < numobs; i ++){
    for(int j = 0; j < numpolys[i]; j++){
      thing[p].objnum = i;
      thing[p].polynum = j;
      thing[p].dist = find_dist(i , j);
      p++;
    }
  }

  n = p;
}

void print_array()
{
  int i ;
  for (i = 0 ; i < n ; i++) {
    printf("%d %d %lf\n",thing[i].objnum, thing[i].polynum, thing[i].dist) ;
  }
  printf("\n") ;
}



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


void makeUnit(double* v){
  double dist = 0;

  for(int i = 0; i < 3; i++){
    dist += pow(v[i], 2);
  }

  dist = sqrt(dist);

  for(int i = 0; i < 3; i++){
    v[i] = v[i] / dist;
  }

}


double dotProduct(double v1[3], double v2[3]){
  return (v1[0] * v2[0]) + (v1[1] * v2[1]) + (v1[2] * v2[2]);
}




int good_bad(double A[4], double P[3], double good[3]){
  double valC, valP;
  valC = (A[0] * good[0]) + (A[1] * good[1]) + (A[2] * good[2])  + A[3];
  valP = (A[0] * P[0]) + (A[1] * P[1]) + (A[2] * P[2]) + A[3];

  if(valP * valC < 0){
    return 0;
  }
  return 1;
    
}


double findIntersect(double v1[3], double v2[3], double a[4]){
  double u;

  u = - (a[0] * v1[0]) - (a[1] * v1[1]) - (a[2] * v1[2]) - a[3];
  u /= (a[0] * v2[0]) + (a[1] * v2[1]) + (a[2] * v2[2]);

  return u;
}



int cut(double* xp,double* yp,double* zp, int np, double  A[4], double  good[3]){
  double tempx[100], tempy[100], tempz[100];
  int s = 0;

  
  for(int i = 0; i < np; i ++){
    //good1 -> good2 | good2
    //good  -> bad   | find intersect
    //bad   -> good  | find intersect, good
    //bad   -> bad   | nothing

    int j = (i+1) % np;

    double p1[3], p2[3], v[3];
    
    p1[0] = xp[i];
    p1[1] = yp[i];
    p1[2] = zp[i];

    p2[0] = xp[j];
    p2[1] = yp[j];
    p2[2] = zp[j];

    v[0] = p2[0] - p1[0];
    v[1] = p2[1] - p1[1];
    v[2] = p2[2] - p1[2];

    if(good_bad(A, p1, good) == 1){

      if(good_bad(A, p2, good) == 1){
      
	tempx[s] = p2[0];
	tempy[s] = p2[1];
	tempz[s] = p2[2];
	s++;
      }

      else{
	double u = findIntersect(p1, v, A);

	tempx[s] = p1[0] + u * v[0];
	tempy[s] = p1[1] + u * v[1];
	tempz[s] = p1[2] + u * v[2];
	s++;

      }
     
    }
    else{
      if(good_bad(A, p2, good) == 1){
	double u = findIntersect(p1, v, A);
	
	tempx[s] = p1[0] + u * v[0];
	tempy[s] = p1[1] + u * v[1];
	tempz[s] = p1[2] + u * v[2];
	s++;
	
	tempx[s] = p2[0];
	tempy[s] = p2[1];
	tempz[s] = p2[2];
	s++;
      }

    }
    
  }

  if(s > np){
    np = s;
  
  }
  for(int i = 0; i < np; i++){
    xp[i] = tempx[i];
    yp[i] = tempy[i];
    zp[i] = tempz[i];
  }
  
  
  return s;
}

int draw2(){
  int f = 0;
  int loc,i,j ;
  double xp[100],yp[100], zp[100];
  double xb[100], yb[100];
  int np ;
  double H = tan(h);
  double s = 1;
  double longest = thing[0].dist;
  double ambDif = ambient + maxDiffuse;
  double maxSpec = 1 - ambDif;
  
  for(i = n; i >= 0; i--){
    double intensity, diffuse, specular;
    double Nu[3], Lu[3], Eu[3], Ru[3];

    
    int o = thing[i].objnum;
    int p = thing[i].polynum;
    np = psize[o][p] ;
    for(j = 0; j < np; j++){
      loc = con[o][p][j] ;
      xp[j] = x[o][loc] ;
      yp[j] = y[o][loc];
      zp[j] = z[o][loc];

      //have to be moved after cut now
      
      
    }

    double hith, ya;
    hith = 0.5;
    ya = 1000;
    double good[3];
    good[0] = 0;
    good[1] = 0;
    good[2] = (hith + ya)/2;
  
    // 0x + 0y + 0z - 0.1 = 0
    double hither[4];
    hither[0] = 0;
    hither[1] = 0;
    hither[2] = 1;
    hither[3] = -1 * hith;

    double yan[4];
    yan[0] = 0;
    yan[1] = 0;
    yan[2] = 1;
    yan[3] = -1 * ya;

    double left[4];
    left[0] = 1;
    left[1] = 0;
    left[2] = -H;
    left[3] = 0;

    double right[4];
    left[0] = 1;
    left[1] = 0;
    left[2] = H;
    left[3] = 0;

    double bot[4];
    bot[0] = 0;
    bot[1] = 1;
    bot[2] = -H;
    bot[3] = 0;
    

    double top[4];
    top[0] = 0;
    top[1] = 1;
    top[2] = H;
    top[3] = 0;
    
    
    np = cut(xp,yp,zp,np, hither, good);
    np = cut(xp,yp,zp,np, yan,    good);
    np = cut(xp,yp,zp,np, left,   good);
    np = cut(xp,yp,zp,np, right,  good);
    np = cut(xp,yp,zp,np, top,    good);
    np = cut(xp,yp,zp,np, bot,    good);


   

    if(np > 0){
      f = 1;
    }

    
    for(int q = 0; q < np; q++){
      //printf("%lf ", zp[q]);
      
      xb[q] = (400/H) * (xp[q]/zp[q]) + 400;
      yb[q] = (400/H) * (yp[q]/zp[q]) + 400;

    }
    //printf("\n");




    ///COLORIZATION----------------------------------------------------------
    //
    //
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
    
    
    v1[0] = xp[1] - xmid;
    v1[1] = yp[1] - ymid;
    v1[2] = zp[1] - zmid;
    
    v2[0] = xp[2] - xmid;
    v2[1] = yp[2] - ymid;
    v2[2] = zp[2] - zmid;

    zero = M3d_x_product(Nu, v1, v2);
    
    Lu[0] = locLight[0] - xmid;
    Lu[1] = locLight[1] - ymid;
    Lu[2] = locLight[2] - zmid;
    
    Eu[0] = 0 - xmid;
    Eu[1] = 0 - ymid;
    Eu[2] = 0 - zmid;

    
    makeUnit(Nu);
    makeUnit(Lu);
    makeUnit(Eu);

    if(dotProduct(Nu,Lu) < 0 ){
      for(int k = 0; k<3; k++){
	Nu[k] *= -1;
      }
    }
    double NuLu = dotProduct(Nu, Lu);
    double NuEu = dotProduct(Nu, Eu);
    
    // reflection vector = (2h)*Nu - Lu, h = Nu.Lu
    double Rmod = 2 * NuLu;
    Ru[0] = Rmod * Nu[0] - Lu[0];
    Ru[1] = Rmod * Nu[1] - Lu[1];
    Ru[2] = Rmod * Nu[2] - Lu[2];

    makeUnit(Ru);

    
    double EuLu = dotProduct(Eu, Lu);
    double EuRu = dotProduct(Eu, Ru);

    

    //   = 0.3 * (Eu.Ru)^(specular power 50)
    specular = maxSpec * pow(EuRu, SPECPOW);
    diffuse = maxDiffuse * NuLu;

    
    double ar,ag,ab,f;
    
    
    if(diffuse > 0 && NuEu > 0){
    intensity = ambient + diffuse + specular;
    }
    else{
      intensity = ambient;
    }
    
      if(intensity < ambDif){
	f = intensity/ambDif;
	ar = red[o] * f;
	ag = green[o] * f;
	ab = blue[o] * f;
      }
      else{
	f = (intensity - ambDif)/(1-ambDif);

	ar = red[o] + f * (1- red[o]);
	ag = green[o] + f * (1 - green[o]);
	ab = blue[o] + f * (1 - blue[o]);
      }
      
	
    G_rgb(ar, ag ,ab);
    
    
    G_fill_polygon(xb, yb, np);
    
    G_rgb(0,0,0);
    //G_polygon(xb,yb,np);

  }
     
  if(f== 0){
    G_rgb(0,0,0);
    G_clear();
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


int compare (const void *p, const void *q)
{
  THING *a, *b ;

  a = (THING*)p ;
  b = (THING*)q ;

  if  (((*a).dist) < ((*b).dist)) return -1 ;
  else if (((*a).dist) > ((*b).dist)) return 1 ;
  else return 0 ;
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


  
  //init_array(numobs);
  //qsort (thing, n, sizeof(THING), compare ) ;
  //print_array();
  
  
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



  //G_rgb(1,0,0);
  //draw2();
  
  while (1) {
    init_array(numobs);
    qsort (thing, n, sizeof(THING), compare ) ;
    G_rgb(0,0,0) ;
    G_clear() ;

    int rt = numpoints[onum];
    xm = x[onum][rt];
    ym = y[onum][rt];
    zm = z[onum][rt];


    M3d_make_identity (V) ;
    M3d_make_identity (T) ;
    M3d_make_identity (Z) ;


    G_rgb(1,0,0);
    draw2();
    
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


