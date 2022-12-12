#include "FPToolkit.c"
#include "M2d_matrix_tools.c"


int swidth, sheight ;


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
    if (n > 0) { G_line(x[n-1],y[n-1], x[n],y[n]) ;}
    n++ ;
    G_wait_click(P) ;
  }

  return n ;
}




int main()
{
  double xp[1000],yp[1000] ;
  int n ;
  double P[2],Q[2] ;

  swidth = 700 ; sheight = 700 ;
  G_init_graphics(swidth, sheight) ;
  G_rgb(0,0,0) ;
  G_clear() ;

  G_rgb(1,0,0) ;
  n = click_and_save(xp,yp) ;
  G_rgb(0,1,0) ;
  G_fill_polygon(xp,yp,n) ;

  G_wait_click(P) ;
  G_rgb(0,0,1) ;
  G_fill_circle(P[0],P[1],2) ;

  G_wait_click(Q) ;
  G_rgb(0,0,1) ;
  G_fill_circle(Q[0],Q[1],2) ;  

  G_rgb(1,1,1) ;
  G_line(P[0],P[1],  Q[0],Q[1]) ;

  // you need to insert the necessary code here
  double pqx[2], pqy[2];
  pqx[0] = P[0];
  pqx[1] = Q[0];

  pqy[0] = P[1];
  pqy[1] = Q[1];
  
  
  double dx = Q[0] - P[0];
  double dy = Q[1] - P[1];

  double da = - atan2(dy, dx);
  printf("%lf", da);

  double m[3][3], r[3][3], z[3][3], b[3][3], f[3][3], r2[3][3];

  if(Q[1] < P[1]){
    M2d_make_translation(z, -Q[0], -Q[1]);
    M2d_make_translation(b, Q[0], Q[1]);
  }

  
  else{
    M2d_make_translation(z, -P[0], -P[1]);
    M2d_make_translation(b, P[0], P[1]);
  }

  M2d_make_rotation(r, da);
  M2d_make_rotation(r2, -da);

  M2d_make_scaling(f, 1, -1);

  M2d_mat_mult(m, r, z);
  M2d_mat_mult(m, f, m);
  M2d_mat_mult(m, r2, m);
  M2d_mat_mult(m, b, m);
  
  M2d_mat_mult_points(xp, yp, m, xp, yp,n);

  G_rgb(1,0,1) ;
  G_fill_polygon(xp,yp,n) ;
  

  while (G_wait_key() != 'q') ;
}
