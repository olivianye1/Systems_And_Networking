#Jacob Waxman and Olivia Nye
# CMPS 2300 Systems and Networks
#Lab 4 bitwise.c


#include <stdio.h>
int prefix(unsigned int x, unsigned int n);
int suffix(unsigned int x, unsigned int n);
void dec2bin(unsigned int c);
double toggle(int x, int n);

int main(void) {
  unsigned int a =20;
  unsigned int b =3;
  int value = suffix(a,b);
  printf("Decimal value: %d\n",value);
  printf("Binary value: ");
  dec2bin(value);

  return 0;
}

void dec2bin(unsigned int c)
{
   int i = 0;
   for(i = 31; i >= 0; i--){
     if((c & (1 << i)) != 0){
       printf("1");
     }else{
       printf("0");
     } 
   }
}


int prefix(unsigned int x, unsigned int n){
  int d = (floor(log10(x)) + 1) - n;
  int y = 1 << dif;
    y = y-1;
  y = ~y;
  y = y & x;
  y = y  >> d;
  printf("%d\n" , y);
  return y;
}


int suffix(unsigned int x, unsigned int n)
{
  int suffix = (((1 << n) - 1) & (x >> ( 0 )));
  return suffix;


}

double toggle(int x, int n)
{
  int number = (x ^ (1 << (n-1)));
  return (double)number;

}