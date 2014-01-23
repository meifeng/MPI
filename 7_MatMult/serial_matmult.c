#include <stdlib.h>
#include <stdio.h>

void matMult(int Arows, int Acols, int Bcols, double *A, double *B, double *C) {
  int i,j,k;
  for (i = 0; i < Arows; i++) {
    for (j = 0; j < Bcols; j++) {
      C[i*Bcols+j] = 0.0;
      for (k = 0; k < Acols; k++) {
        C[i*Bcols+j] += A[i*Acols+k] * B[k*Bcols+j];
      }
    }
  }
}

void printMat(int NR, int NC, double *A) {
  int i,j;
  for (i = 0; i < NR; i++) {
    for (j = 0; j < NC; j++) {
      printf("%8.1f ",A[i*NC+j]);
    }
    printf("\n");
  }
}

int main(int argc, char *argv[]) {
  int NRA = 8;
  int NCA = 8;
  int NCB = 8;
  int NRB = NCA;
  int i,j;

  double *A, *B, *C;
  A = (double *) malloc(NRA * NCA * sizeof(double));
  B = (double *) malloc(NRB * NCB * sizeof(double));
  C = (double *) malloc(NRA * NCB * sizeof(double));
  
  //Initialize the matrices
  for (i = 0; i < NRA; i++) {
    for (j = 0; j < NCA; j++) {
      A[i*NCA+j] = i + j;
    }
  }
  for (i = 0; i < NRB; i++) {
    for (j = 0; j < NCB; j++) {
      B[i*NCB+j] = i - j;
    }
  }

  printf("A=\n"); printMat(NRA, NCA, A);
  printf("B=\n"); printMat(NRB, NCB, B);

  matMult(NRA, NCA, NCB, A, B, C);
  printf("C=\n"); printMat(NRA,NCB, C);
  
  free(C);
  free(B);
  free(A);
}

  //
