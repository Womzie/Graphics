#include "FPToolkit.c"
int swidth, sheight ;
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

  cx = centerX(x, n);
  cy = centerY(y,n);
  return n ;
}





int good_bad(double a, double b, double c, double P[]){
  double valC, valP;
  valC = (a * cx) + (b * cy) + c;
  valP = (a * P[0]) + (b * P[1]) + c;
  printf("%lf %lf  \n", valC, valP);

  if(valP * valC < 0){
    return 0;
  }
  return 1;
    
}





int in_out (double x[], double y[], int n, double P[2])
// return 1 if point P is inside the convex polygon
// else return 0
{
  double a[n],b[n],c[n];


  printf("Center Points: %lf, %lf \n", cx, cy);
  G_rgb(1,1,1);
  G_fill_circle(cx,cy,5);
  
  int flag = 1;
  for(int i = 0; i < n; i++){
    int j = (i+1)%n;
   
    a[i] = y[i] - y[j];
    b[i] = x[j] - x[i];
    c[i] = (x[i] * y[j]) - (x[j] * y[i]);

    if(good_bad(a[i],b[i],c[i], P) == 0){
      flag = 0;
      break;
      }
    
    }
    return flag;
}



int main()
{
  double xp[1000],yp[1000] ;
  int n,q ;
  double P[2] ;


  swidth = 700 ; sheight = 700 ;
  G_init_graphics(swidth, sheight) ;
  G_rgb(0,0,0) ;
  G_clear() ;

  G_rgb(1,0,0) ;
  n = click_and_save(xp,yp) ;
  G_rgb(0,1,0) ;
  G_fill_polygon(xp,yp,n) ;


  while(True){
    G_wait_click(P) ;
    int res = in_out(xp,yp,n,P);
    G_rgb(0,1-res,1) ;
    G_fill_circle(P[0],P[1],2) ;
    
  }
  q = G_wait_key() ;
}

  
