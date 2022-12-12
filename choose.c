#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int fac(int n)
{
  int i,r;
  r = 1;
  for (i = 1;i<=n;i++){
    r = r*i;
  }
  return r;
}


int main()
{
  int numfriends, numinvites, numchoices;
  printf("how many friends do you have? ");
  scanf("%d", &numfriends);
  printf("how many invitations to your party? ");
  scanf("%d", &numinvites);
  
  numchoices = fac(numfriends)/(fac(numinvites) * fac(numfriends - numinvites));
  printf("%d\n",numchoices);
}
