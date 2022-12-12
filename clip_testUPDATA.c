
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
      double wx[],  double wy[], int wsize)
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
	  printf("BAD \n");
	}

      }//end of else
     
      
    }//end of inner for

    for(int w = 0; w < s; w++){
      rx[w] = tempx[w];
      ry[w] = tempy[w];
    }
    rsize = s;
    r += 50;
  }  
  // fill this in with your code
  return rsize ;
}







int main()
// this tests clipping of polygon to convex window
{
  int pn, wn ;

  double px[100] = {  0,100,200, 100} ;
  double py[100] = { 0, 100,0, -100} ;
  pn = 4 ;

  double wx[100] = { 70, 120, 65} ;
  double wy[100] = {100, 150, 25} ;
  wn = 3 ;



  srand48(100) ;

  G_init_graphics (700, 700) ;
  G_rgb (0,0,0) ;
  G_clear() ;

  G_rgb (1,0,0) ;
  G_polygon(wx,wy,wn) ;

  G_rgb (0,0,1) ;
  G_polygon(px,py,pn) ;


  G_wait_key() ;


  pn =  Clip_Polygon_Against_Convex_Window (px, py, pn,
                                            wx, wy, wn) ;

  for(int i = 0; i < pn; i++){
    printf("%lf  %lf \n",px[i],py[i]); 
  }

  G_rgb (1,1,0) ;
  G_fill_polygon(px,py,pn) ;
  G_wait_key() ;
}




