
// open a sequence of .xy  files specified on the commmand line
// and draw them.

#include "FPToolkit.c"
#include <string.h>
#include <math.h>
#include "M2d_matrix_tools.c"

#define MAXOBJS 10
#define MAXPTS 59000
#define MAXPOLYS 57500



int numpoints [MAXOBJS];
int numpolys[MAXOBJS] ;
double x[MAXOBJS][MAXPTS] ;
double y[MAXOBJS][MAXPTS] ;
int psize[MAXOBJS][MAXPOLYS] ;
int con[MAXOBJS][MAXPOLYS][20] ;
double red[MAXOBJS][MAXPOLYS],grn[MAXOBJS][MAXPOLYS],blu[MAXOBJS][MAXPOLYS] ;
double xmid[MAXOBJS], ymid[MAXOBJS], xsmall[MAXOBJS], ysmall[MAXOBJS], xbig[MAXOBJS], ybig[MAXOBJS];
double sheight, swidth;





int print_object (FILE *fout, int onum)
{
  int i,j,k ;

  fprintf(fout, "%d\n",numpoints[onum]) ;

  for (i = 0 ; i < numpoints[onum] ; i++) {
    fprintf(fout, "%12.6lf %12.6lf\n",x[onum][i],y[onum][i]) ;
  }
  
  k = 0 ;
  for (i = 0 ; i < numpolys[onum] ; i++) {
    fprintf(fout, "%3d    ",psize[onum][i]) ;

    for (j = 0 ; j < psize[onum][i] ; j++) {
      fprintf(fout, "%2d ", con[onum][i][j]) ;
    }
    fprintf(fout, "\n") ;
  }


  for (i = 0 ; i < numpolys[onum] ; i++) {
    fprintf(fout,"%lf %lf %lf\n",red[onum][i],grn[onum][i],blu[onum][i]) ;
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





  while(True){
    // center it



    draw_object(onum) ;
    
    
    //G_rgb(1,0,0);
    //G_fill_circle(xmid[onum],ymid[onum],5);

    key = G_wait_key();
    if(key == 'q'){
      break;
    }
    
    else if(key == 'r'){
      
      M2d_mat_mult_points(x[onum], y[onum], r, x[onum], y[onum], numpoints[onum]);
      draw_object(onum);
    }

    else if ('0' <= key && key <= '9'){
      w = key - '0';
      if(w < numobs){onum = w;}

    }
    i ++;
  }

}


