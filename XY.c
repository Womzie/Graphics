
#include  "FPToolkit.c"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int swidth, sheight ;

int numpoints;
double x[59000], y[59000];

int numpolys;
int psize[57500];
int con[57500][20];

double reds[57500], greens[57500], blues[57500];
  


void reader(){
  
  FILE *w;
  char fname[100];
  printf("Please enter the file name: \n"); scanf("%s",fname);
  
  w = fopen(fname, "r");
  if(w == NULL) {printf("UNABLE TO OPEN FILE\n"); exit(0);}
 
  fscanf(w,"%d", &numpoints);
  
  //printf("numpoints = %d\n", numpoints);

  int i;
  int j;
  for(i = 0; i < numpoints; i++){
    fscanf(w,"%lf %lf", &x[i], &y[i]);
  }

  fscanf(w,"%d", &numpolys);

  for(i = 0; i < numpolys; i++){
    fscanf(w, "%d", &psize[i]);

    for(j =0; j < psize[i]; j++){
      fscanf(w,"%d",&con[i][j]);
    }
    
  }
  
  for(i = 0; i < numpolys; i++){
    //for(j = 0; j < psize[i];j++){
      //printf("%d  ",con[i][j]);    }
    //printf("\n");

    fscanf(w,"%lf", &reds[i]);
    fscanf(w,"%lf", &greens[i]);
    fscanf(w,"%lf", &blues[i]);

  }

  
}




int main()
{
  // read file
  reader();
  swidth = 800 ; sheight = 800 ;
  G_init_graphics(swidth, sheight) ;
  G_rgb(0,0,0) ;
  G_clear() ;



  //display points
  G_rgb(1,0,0);
  int i;
  int j;
  for(i = 0; i < numpoints; i++){
    G_fill_circle(x[i],y[i],2);
  }




  for(i = 0; i < numpolys; i++){
    double a[1000], b[1000];
    G_rgb (reds[i], greens[i], blues[i]);
    for(j = 0; j < psize[i]; j++){
      int n = con[i][j];
      a[j] = x[n];
      b[j] = y[n];
      //printf("%lf", a[j]);
    }
    G_fill_polygon(a,b,psize[i]);
  }

  
  G_wait_key();
  
}

