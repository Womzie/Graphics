#include <stdio.h>
#include <stdlib.h>


typedef
struct {
  int objnum ;
  int polynum ;
  double dist ;
}
THING ;


int n = 9 ;
THING x[9]  ;


void init_array()
{
  x[0].objnum  = 0 ;  x[0].polynum  = 0 ;  x[0].dist  = 8.6 ;
  x[1].objnum  = 0 ;  x[1].polynum  = 1 ;  x[1].dist  = 3.6 ;
  x[2].objnum  = 0 ;  x[2].polynum  = 2 ;  x[2].dist  = 4.6 ;

  x[3].objnum  = 1 ;  x[3].polynum  = 0 ;  x[3].dist  = 2.7 ;
  x[4].objnum  = 1 ;  x[4].polynum  = 1 ;  x[4].dist  = 5.6 ;
  x[5].objnum  = 1 ;  x[5].polynum  = 2 ;  x[5].dist  = 4.2 ;
  x[6].objnum  = 1 ;  x[6].polynum  = 3 ;  x[6].dist  = 3.9 ;

  x[7].objnum  = 2 ;  x[7].polynum  = 0 ;  x[7].dist  = 4.5 ;
  x[8].objnum  = 2 ;  x[8].polynum  = 1 ;  x[8].dist  = 6.5 ;

}




void print_array()
{
  int i ;
  for (i = 0 ; i < n ; i++) {
    printf("%d %d %lf\n",x[i].objnum, x[i].polynum, x[i].dist) ;
  }
  printf("\n") ;
}





/////////////////////////////////////////////////////////////

void selection_sort_doubles (double v[], int n) 
{
  int i,s,j ;
  double tmp ;

  for (i = 0 ; i < n ; i++) {
    s = i ;
    for (j = i+1 ; j < n ; j++) {
      if (v[j] < v[s]) { s = j ; }
    }
    tmp = v[i] ; v[i] = v[s] ; v[s] = tmp ;
  }

}


/////////////////////////////////////////////////////////////


void selection_sort_things (THING v[], int n) 
{
  int i,s,j ;
  THING tmp ;

  for (i = 0 ; i < n ; i++) {
    s = i ;
    for (j = i+1 ; j < n ; j++) {
      if (v[j].dist < v[s].dist) { s = j ; }
    }
    tmp = v[i] ; v[i] = v[s] ; v[s] = tmp ;
  }

}


/////////////////////////////////////////////////////////////



int main()
{
  double p[5] = { 12.5, 4.3, 2.7, 8.5, 6.2 } ;

  for (int i = 0 ; i < 5 ; i++) { printf("%6.3lf ",p[i]) ; } printf("\n") ;
  selection_sort_doubles(p,5)  ;
  for (int i = 0 ; i < 5 ; i++) { printf("%6.3lf ",p[i]) ; } printf("\n") ; 

  printf("\n") ;
  
  init_array() ;
  print_array() ;
  selection_sort_things(x,9) ;
  print_array() ;
}
