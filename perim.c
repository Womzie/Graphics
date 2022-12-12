#include <stdio.h>
#include <math.h>
#include <stdlib.h>

double distance(double x1, double y1, double x2, double y2){
  return sqrt(pow(x1 - x2,2) + pow(y1 - y2, 2));
}

double perimeter(double x[], double y[], int n){
  double perim = 0;
  int i;
  for(i = 0; i < n-1; i++){
    // or int j = (i+1)%n;
    // perim += distance(x[i],[y[i], x[j], y[j]);
    perim +=  distance(x[i], y[i], x[i+1], y[i+1]);
  }
  perim = perim + distance(x[0], y[0], x[n-1], y[n-1]);
  return perim;
}

int main()
{
  double u[3] = {100, 300, 400};
  double v[3] = {200, 300, 100};

  double a[4] = {500, 700, 600, 400};
  double b[4] = {100, 100, 400, 600};

  double puv, pab;
  puv = perimeter(u,v,3);
  pab = perimeter(a,b,4);
  printf("%lf %lf", puv, pab);
}

// mod of double = fmod(a,b)
// mod of int = a%b
