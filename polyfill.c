// left to do, sort array, and only draw every other


#include  "FPToolkit.c"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int swidth, sheight ;



double bisect(double p[], double q[], double k){

   

   if((p[1] <  k && k  <= q[1])||(p[1] >=  k && k > q[1])){
     //G_rgb(1,0,0);
     //G_fill_circle(x, k,2);
     double x = (k - p[1])*(q[0] - p[0]) / (q[1] - p[1]) + p[0];
     
     return x;
   }
   return -1;
   
}
void sort2(double x[], int m){
  int s,i,k;
  double temp;

  for (k = 0; k <m;k++){
       s = k;
       for(i = k+1; i < m; i++){
	 if(x[i]<x[s]){s = i;}
       }
    temp =x[k];
    x[k] = x[s];
    x[s] = temp;
  }
}

void sort(double arr[], int n) 
{ 
    int i, key, j; 
    for (i = 1; i < n; i++)
    { 
        key = arr[i]; 
        j = i - 1; 
  
        // Move elements of arr[0..i-1],  
        // that are greater than key, to one 
        // position ahead of their 
        // current position
        while (j >= 0 && arr[j] > key)
        { 
            arr[j + 1] = arr[j]; 
            j = j - 1; 
        }   
        arr[j + 1] = key; 
    } 
}


void my_fill_polygon(double x[], double y[], int num_points){
  // for(int i = 0; i < num_points; i++){
  //  printf("%lf \n",x[i]);
  // }

  double min = y[0];
  for(int t = 0; t < num_points;t++){
    if(y[t] < min) min = y[t];
  }
  double p[2],q[2];
  int i;

  for(double k = min+0.1; k <= sheight; k++){
    double xb[500];
    
    //double k = 300;
    int n = 0;
    for(i = 0; i < num_points; i++){
      int j = (i+1)%num_points;
      p[0] = x[i];
      p[1] = y[i];
      q[0] = x[j];
      q[1] = y[j];
      double temp = bisect(p, q, k);
      Bool write = True;
      if (temp > -1){
	if(temp != xb[0]){
	   xb[n] = temp;
	}
	}
      else{ n--;}
      n ++;

    }

    // sort algo

    sort(xb, n);
    printf("Points: ");
    for(int e = 0; e < n; e++){
      printf("%lf ", xb[e]);
    }
    printf("\n");

    
    for(int m = 0; m < n-1; m++){
	//printf("%lf \n", xb[m]);
	if(m%2 == 0){
	G_rgb(0,1,0);
	G_line(xb[m], k, xb[m+1], k);
      }
    }
    G_wait_key() ;

    }

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
    //x[n] = floor((x[n]+10)/20) * 20;
    //y[n] = floor((y[n]+10)/20) * 20;
    
    
    G_fill_circle(x[n],y[n],2);
    // draw lines from point to point except first
    if( n > 0){
      G_line(x[n-1], y[n-1], x[n], y[n]);
    }
 
    n++;
  }

  
  my_fill_polygon(x, y, n);
  // fill 
  return n;
  

}

int test()
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
  //G_rgb(0.3,0.2,0.4);
  //for(int x = 0; x < swidth; x+= 20){
  //  G_line(x, 0, x ,sheight);
  //}
  //for(int y = 0; y < sheight; y+= 20){
  //  G_line(0, y, swidth, y);
  //}

  
  G_rgb(1,0,0) ;
  m = click_and_save(xp,yp) ;


  G_wait_key() ;

   //   G_save_image_to_file("demo.xwd") ;
   G_save_to_bmp_file("polyfill.bmp") ;
	     
}


int main()
{
   
   test();
   

}

