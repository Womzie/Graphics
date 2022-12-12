#include "FPToolkit.c"

int swidth, sheight ;


void print_poly(double x[], double y[], int n)
{
  int i ;
  printf("\n") ;
  printf("%d\n",n) ;
  for (i = 0 ; i < n ; i++) {
    printf("%7.2lf %7.2lf\n",x[i],y[i]) ;
  }
  printf("\n") ;
}


double polygon_perimeter (double x[], double y[], int n)
{
  double a,b,c,p ;
  int i,j ;
  
  p = 0.0 ;
  for (i = 0 ; i < n ; i++) {
    j = i+1 ; if (j == n) { j = 0 ; }
    a = x[j] - x[i] ; b = y[j] - y[i] ;
    c = sqrt(a*a + b*b) ;
    p += c ;
  }
  
  return p ;
}

int click_and_save (double x[], double y[]){
  double p[2], q[2];
  int n = 0;
  // red  rectangle
  G_rgb(1,0,0);
  
  G_fill_rectangle(0,0,40,20);
  
  G_rgb(1,0,0) ;


  
  while(True){
    G_wait_click(p) ;
    // if click green area, draw last line and exit
    if (p[1] <= 20 && p[0] <= 40){
      G_line(x[0], y[0], x[n-1], y[n-1]);
      break;
    }

    // save points in array
    x[n] = p[0];
    y[n] = p[1];

    // snap
    x[n] = floor((x[n]+10)/20) * 20;
    y[n] = floor((y[n]+10)/20) * 20;
    

    
    G_fill_circle(x[n],y[n],2);
    // draw lines from point to point except first
    if( n > 0){
      G_line(x[n-1], y[n-1], x[n], y[n]);
    }
 
    n++;
  }
  return n;
  

}




int test02()
{
  double xp[1000],yp[1000],p1 ;
  int m ;
  double xq[500], yq[500],p2 ;
  int n ;
  double P[2] ;

  swidth = 800 ; sheight = 800 ;
  G_init_graphics(swidth, sheight) ;
  G_rgb(0,0,0) ;
  G_clear() ;

  
  // grid
  G_rgb(0,0.7,0);
  for(int x = 0; x < swidth; x+= 20){
    G_line(x, 0, x ,sheight);
  }
  for(int y = 0; y < sheight; y+= 20){
    G_line(0, y, swidth, y);
  }
  
  
  G_rgb(1,0,0) ;
  m = click_and_save(xp,yp) ;

  G_rgb(1,0,0) ;
  n = click_and_save(xq,yq) ;

  p1 = polygon_perimeter (xp,yp,m) ;
  p2 = polygon_perimeter (xq,yq,n) ;
  printf("p1 = %lf  p2 = %lf\n",p1,p2) ;

  G_rgb(0,1,0) ;
  if (p1 > p2) {
    G_fill_polygon(xp,yp,m) ;
  } else if (p2 > p1) {
    G_fill_polygon(xq,yq,n) ;
  } else {
    G_fill_polygon(xp,yp,m) ;
    G_fill_polygon(xq,yq,n) ;    
  }
  
  
  G_wait_key() ;
  
  print_poly(xp,yp,m) ;
  print_poly(xq,yq,n) ;  
	     
}



int main()
{
  //test01() ;
  test02() ;
}
