/* Serial Trapezoid Rule Program                 */
/* [f(x0)/2 + f(xn)/2 + f(x1) + ... + f(xn-1)]*h */

#include <stdio.h>
#include <math.h>

double f(double x){
  return exp(x*x);
}

int main(){
  double integral;     /*definite integral result*/
  const double a=0.0;  /*left end point*/
  const double b=1.0;  /*right end point*/
  const int N=100000; /*number of subdivisions*/
  double h;            /*base width of subdivision*/
  double x;
  int i;

  h = (b-a)/N;
  integral = (f(a)+f(b))/2.0;
  x = a;

  for(i = 1; i <= N-1; i++){
      x = x+h;
      integral = integral + f(x);
  }

  integral = integral*h;

  printf("%s%d%s%f\n", "WITH N=", N, " TRAPEZOIDS, INTEGRAL=", integral);

  return 0;
}


