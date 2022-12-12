#include "FPToolkit.c"
#include "M2d_matrix_tools.c"
#include <math.h>



// move to 0
// scale
// rotate
// move to midpoint of a,b


int main()
{
  // rocket
  double rx[8] = {0, 16,  7,  7,  0, -7, -7, -16 } ;
  double ry[8] = {0,  0, 15, 35, 50, 35, 15,   0 } ;
  double a[2], b[2];
  double dist;
  double midx,midy;
  G_init_graphics(700,700) ;  

  G_rgb(0,0,0) ;
  G_clear() ;
  G_rgb(0,1,0) ;
  G_fill_polygon(rx,ry,8) ;

  double maxX = rx[0];
  double maxY = ry[0];
  double minX = rx[0];
  double minY = ry[0];

  for(int i = 1; i < 8; i++){
    if(rx[i] > maxX){maxX = rx[i];}
    if(ry[i] > maxY){maxY = ry[i];}
    if(rx[i] < minX){minX = rx[i];}
    if(ry[i] < minY){minY = ry[i];}
  }
  midx = (minX + maxX) / 2;
  midy = (minY + maxY) / 2;

  double h = maxY - minY;

  G_wait_click(a);
  G_fill_circle(a[0],a[1],3);

  G_wait_click(b);
  G_fill_circle(b[0],b[1],3);

  G_rgb(1,0,0);
  G_fill_circle(midx, midy, 5);

  double dx = b[0] - a[0];
  double dy = b[1] - a[1];
  dist = sqrt(pow(b[0] - a[0], 2) + pow(b[1] - a[1], 2));
  double fx = (a[0] + b[0])/2;
  double fy = (a[1] + b[1])/2;

  
  double z[3][3], s[3][3], r[3][3], f[3][3], m[3][3];
  M2d_make_translation(z, -midx, -midy);

  M2d_make_scaling(s, dist/h, dist/h);

  double da = atan2(dy,dx) - M_PI/2;
  
  M2d_make_rotation(r,da);
  M2d_make_translation(f, fx, fy);
  M2d_mat_mult(m, s, z);
  M2d_mat_mult(m, r, m);
  M2d_mat_mult(m, f, m);

  M2d_mat_mult_points(rx, ry, m, rx, ry, 8);

  G_rgb(0,0,0) ;
  G_clear() ;
  
  G_rgb(0,1,0) ;
  G_fill_circle(a[0],a[1],3);
  G_fill_circle(b[0],b[1],3);
  G_fill_polygon(rx,ry,8) ;
  G_wait_key() ;
}
