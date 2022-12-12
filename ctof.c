#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main()
{
  double c,f;
  printf("enter degrees celsius: ");
  scanf("%lf",&c);
  f = 1.8 * c + 32;
  printf("%lf degrees celsius equals %0.2lf degrees farenheit",c,f);
}
