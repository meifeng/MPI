#include <stdio.h>
#include <stdlib.h>

void matvec (int M, int N, int ** mat, int * vec, int * result) {
  int i, j;
  for ( i = 0; i < M; i++ ) {
    result[i] = 0.0;
    for ( j = 0; j < N; j++ ) {
      result[i] += mat[i][j] * vec[j];
    }
  }
}


int main (int argc, char * argv[]) {

  int nrows = 64;
  int ncols = 32;
  int i,j;

  int **A = (int **)malloc(nrows*sizeof(int*));
  for(i = 0; i < nrows; i++) {
    A[i] = (int *) malloc(ncols*sizeof(int));
  }

  printf("A = \n");
  for (i = 0; i < nrows; i++ ) {
    for (j = 0; j < ncols; j++ ) {
      A[i][j] = i + j;
      printf("%2d ", A[i][j]);
    }
    printf("\n");
  }
  
  int x[ncols];
  printf("x = \n");
  for (j = 0; j < ncols; j++) {
    x[j] = j;
    printf("%d\n",x[j]);
  }
  
  int result[nrows];
  matvec(nrows, ncols, A, x, result);
  printf("A x = \n");
  for (i = 0; i < nrows; i++) {
    printf("%d\n", result[i]);  
  }
  for(i = 0; i < nrows; i++) {
    free(A[i]);
  } 
 free(A);
}
