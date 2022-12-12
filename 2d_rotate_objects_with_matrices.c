
// open a sequence of .xy  files specified on the commmand line
// and draw them.

#include "FPToolkit.c"
#include "M2d_matrix_tools.c"


#define MAXOBJS 10
#define MAXPTS 59000
#define MAXPOLYS 57500

int numobjects ;
int numpoints[MAXOBJS] ;
int numpolys[MAXOBJS] ;
double x[MAXOBJS][MAXPTS] ;
double y[MAXOBJS][MAXPTS] ;
int psize[MAXOBJS][MAXPOLYS] ;
int con[MAXOBJS][MAXPOLYS][20] ;
double red[MAXOBJS][MAXPOLYS],grn[MAXOBJS][MAXPOLYS],blu[MAXOBJS][MAXPOLYS] ;



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
}




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




void compute_object_bbox (int onum, double result[4])
{
  int i ;
  double minx,maxx,miny,maxy ;
  double bxcen,bycen,bwidth,bheight ;

  // finding bounding box for the object
  minx = maxx = x[onum][0] ;
  miny = maxy = y[onum][0] ;
  for (i = 0 ; i < numpoints[onum] ; i++) {
    if (x[onum][i] < minx) minx = x[onum][i] ;
    if (x[onum][i] > maxx) maxx = x[onum][i] ;

    if (y[onum][i] < miny) miny = y[onum][i] ;
    if (y[onum][i] > maxy) maxy = y[onum][i] ;
  }
  bxcen   = (minx + maxx)/2 ;
  bycen   = (miny + maxy)/2 ;
  bwidth  = (maxx - minx) ;
  bheight = (maxy - miny) ;

  result[0] = bxcen ;
  result[1] = bycen ;
  result[2] = bwidth ;
  result[3] = bheight ;
}






int main(int argc, char **argv)
{
  int wsize = 800 ;
  
  FILE *fin ;
  int onum,key,w ;
  double V[3][3],W[3][3] ;
  double bbox_res_info[4] ;
  double bbxcen,bbycen,bbwidth,bbheight ;
  double bbsquare,mag  ;
  
    
  if (argc < 2) {
    printf("pgm  in0.xyz  in1.xyz ... \n") ;
    exit(1) ;
  }


  numobjects = argc - 1 ;
  if (numobjects > MAXOBJS) {
    printf("numobjects = %d is greater than MAXOBJS = %d\n", numobjects, MAXOBJS) ;
    printf("numobjects truncated to %d\n", MAXOBJS) ;
    numobjects = MAXOBJS ;
  }

  
  for (onum = 0 ; onum < numobjects ; onum++) {

    fin = fopen(argv[onum+1],"r") ;
    if (fin == NULL) {
      printf("can't read file, %s\n",argv[onum+1]) ;
      exit(1) ;
    }

    read_object(fin, onum) ;



    //-----------------------------------------------

    // now we make all objects with a standard
    // center = (0,0)
    // and bbox size = 1
    
    compute_object_bbox (onum, bbox_res_info) ;

    bbxcen   = bbox_res_info[0] ;
    bbycen   = bbox_res_info[1] ;
    bbwidth  = bbox_res_info[2] ;
    bbheight = bbox_res_info[3] ;

    // size of square that encompasses bbox
    if (bbwidth > bbheight) { bbsquare = bbwidth ; } else { bbsquare = bbheight ;}
    mag = wsize / bbsquare ;  
    mag *= 0.8 ; // artistic
    
    M2d_make_identity (V) ;
    M2d_make_translation(W,  -bbxcen,-bbycen) ;
    M2d_mat_mult(V,  W,V) ;
    M2d_make_scaling(W,  mag,mag) ;
    M2d_mat_mult(V,  W,V) ;
    M2d_make_translation(W,  wsize/2, wsize/2) ;
    M2d_mat_mult(V,  W,V) ;
    
    M2d_mat_mult_points (x[onum],y[onum],  V,  x[onum],y[onum],numpoints[onum]) ;

  }



  
  
  //  print_object(stdout, 0) ;
  G_init_graphics(wsize,wsize) ;


    
  M2d_make_identity (V) ;
  M2d_make_translation(W, -wsize/2, -wsize/2) ;
  M2d_mat_mult(V,  W,V) ;
  
  //  M2d_make_rotation_degrees(W,  2) ;
  M2d_make_rotation(W,  2*M_PI/180) ;
  
  M2d_mat_mult(V,  W,V) ;
  M2d_make_translation(W,  wsize/2, wsize/2) ;
  M2d_mat_mult(V,  W,V) ;


  
  onum = 0 ;
  while (1) {
    G_rgb(0,0,0) ;
    G_clear() ;
    draw_object(onum) ;
    M2d_mat_mult_points (x[onum],y[onum],  V,  x[onum],y[onum],numpoints[onum]) ;
    
    key = G_wait_key() ;
    if (key == 'q') {
      break ;
    } else if (('0' <= key) && (key <= '9')) {
      w = key -'0' ;
      if (w < numobjects) { onum = w ; }
    }
  }

}


