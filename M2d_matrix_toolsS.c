#include <stdio.h>
#include <math.h>

// STUDENTS NEED TO FILL IN SOME OF THESE

/*

 ( x')          (x)
 ( y')  =   M * (y)  
 ( 1 )          (1)

instead of (x',y',1) = (x,y,1) * M  

*/



int M2d_print_mat (double a[3][3])
{
  int r,c ;
  for (r = 0 ; r < 3 ; r++ ) {
      for (c = 0 ; c < 3 ; c++ ) {
           printf(" %12.4lf ",a[r][c]) ;
      }
      printf("\n") ;
  }

  return 1 ;
} 





int M2d_copy_mat (double a[3][3], double b[3][3])
// a = b
{
  int r,c ;
  for (r = 0 ; r < 3 ; r++ ) {
      for (c = 0 ; c < 3 ; c++ ) {
           a[r][c] = b[r][c] ;
      }
  }

  return 1 ;
} 





int M2d_make_identity (double a[3][3])
// a = I
{
  int r,c ;
  for (r = 0 ; r < 3 ; r++ ) {
      for (c = 0 ; c < 3 ; c++ ) {
           if (r == c) a[r][c] = 1.0 ;
               else    a[r][c] = 0.0 ;
      }
  }

  return 1 ;
} 





int M2d_make_translation (double a[3][3], double dx, double dy)
{
  M2d_make_identity(a) ;
  a[0][2] =  dx ;  a[1][2] = dy ;  
  return 1 ;
}





int M2d_make_scaling (double a[3][3], double sx, double sy)
{
  // YOU NEED TO FILL THIS IN
  M2d_make_identity(a) ;
  a[0][0] = sx;
  a[1][1] = sy;
  
  return 1 ;
}





int M2d_make_rotation_cs (double a[3][3], double cs, double sn)
// this assumes cosine and sine are already known
{
  M2d_make_identity(a) ;

  a[0][0] =   cs ;  a[0][1] = -sn ;
  a[1][0] =   sn ;  a[1][1] =  cs ;

  return 1 ;
}



int M2d_make_rotation (double a[3][3], double radians)
{
  return M2d_make_rotation_cs(a, cos(radians), sin(radians)) ;
}






// WORKS 
int M2d_mat_mult (double res[3][3], double a[3][3], double b[3][3])
// res = a * b
// this is SAFE, i.e. the user can make a call such as 
// M2d_mat_mult(p,  p,q) or M2d_mat_mult(p,  q,p) or  M2d_mat_mult(p, p,p)
{
  // YOU NEED TO FILL THIS IN
  double temp[3][3];

  // by row
  for(int i = 0; i < 3; i++){
    //by column
    for(int j = 0; j < 3; j++){
      double sum  = 0;
      for(int k = 0; k < 3; k++){
	sum += a[i][k] * b[k][j];
      }

      temp[i][j] = sum;
    }
  }

	

  M2d_copy_mat(res, temp);
  return 1 ;
}





int M2d_mat_mult_pt (double P[2],   double m[3][3], double Q[2])
// P = m*Q
// SAFE, user may make a call like M2d_mat_mult_pt (W, m,W) ;
{
  double u,v ;

  u = m[0][0]*Q[0] + m[0][1]*Q[1] + m[0][2] ;
  v = m[1][0]*Q[0] + m[1][1]*Q[1] + m[1][2] ;

  P[0] = u ;
  P[1] = v ;
  
  return 1 ;
}




// WORKS
int M2d_mat_mult_points (double X[], double Y[],
                         double m[3][3],
                         double x[], double y[], int numpoints)
// |X0 X1 X2 ...|       |x0 x1 x2 ...|
// |Y0 Y1 Y2 ...| = m * |y0 y1 y2 ...|
// | 1  1  1 ...|       | 1  1  1 ...|

// SAFE, user may make a call like M2d_mat_mult_points (x,y, m, x,y, n) ;
{
  // YOU NEED TO FILL THIS IN
  double tempXY[2], tempX[numpoints], tempY[numpoints];
  for(int i = 0; i < numpoints; i++){
    tempXY[0] = x[i];
    tempXY[1] = y[i];
    M2d_mat_mult_pt(tempXY, m, tempXY);
    tempX[i] = tempXY[0];
    tempY[i] = tempXY[1];
  }
  

  for(int k = 0; k < numpoints; k++){
      X[k] = tempX[k];
      Y[k] = tempY[k];
  }

  return 1 ;
}



void main(){
  double a[3][3], b[3][3];
  a[0][0] = 3; a[0][1] = 2; a[0][2] = 1;
  a[1][0] = 1; a[1][1] = 3; a[1][2] = 2;
  a[2][0] = 2; a[2][1] = 1; a[2][2] = 3;

  b[0][0] = -0.5; b[0][1] = 0; b[0][2] = -2;
  b[1][0] = 0; b[1][1] = -0.5; b[1][2] = -2;
  b[2][0] = 0; b[2][1] = 0; b[2][2] = 1;
  //M2d_make_identity(b);

  double x[5] = {2,6,8,6,2};
  double y[5] = {0,0,0,-2,-2};
  //printf("%d\n", M2d_print_mat(a));
  printf("%d\n", M2d_print_mat(b));
  for(int i = 0; i < 5; i++){
    printf("%12.4lf ",x[i]);
  }
  printf("\n");
  for(int i = 0; i < 5; i++){
    printf("%12.4lf ",y[i]);
  }
  printf("\n");
  
  //M2d_mat_mult(b, a,b);
  //printf("%d\n", M2d_print_mat(b));
  printf("%d \n", M2d_mat_mult_points(x, y, b, x, y, 5));

  for(int i = 0; i < 5; i++){
    printf("%12.4lf ",x[i]);
  }
  printf("\n");
  for(int i = 0; i < 5; i++){
    printf("%12.4lf ",y[i]);
  }
  printf("\n");

  double s[3][3];
  printf("%d \n",M2d_print_mat(a));
  M2d_make_scaling(s, 2, 3);
  M2d_mat_mult(a, s, a);
  printf("%d \n",M2d_print_mat(a));
  
}


