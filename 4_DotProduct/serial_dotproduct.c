/* Serial dot product program */

#include <stdio.h>

const int N=2000;

double dotProduct(double *x, double *y, int n) {
  int i;
  double prod = 0.0;
  for (i = 0; i < n; i++) {
    prod += x[i]*y[i];
  }
  return prod;
}

int main(int argc, char *argv[]) {
  double x[N];
  double y[N];
  int i;
  for(i = 0; i < N; i++) {
    x[i] = 0.01 * i;
    y[i] = 0.03 * i;
  }

  double prod;
  prod = dotProduct(x,y,N);
  printf("dotProduct = %f\n", prod);

  return 0;
}



