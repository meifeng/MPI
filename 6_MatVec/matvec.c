#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

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
  int my_rank;
  int nprocs;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  //assuming nrows and ncols are divisible by nprocs
  int local_nrows = nrows / nprocs; 
  int vsize = ncols;  
  int local_vsize = vsize / nprocs;

  if (nrows % nprocs != 0 || vsize % nprocs != 0) {
    printf("nrows or ncols cannot be divided by nprocs\n");
    MPI_Abort(MPI_COMM_WORLD,10);
  }

  int **localA = (int **)malloc(local_nrows*sizeof(int*));
  for(i = 0; i < local_nrows; i++) {
    localA[i] = (int *) malloc(ncols*sizeof(int));
  }

  for (i = 0; i < local_nrows; i++ ) {
    for (j = 0; j < ncols; j++ ) {
      localA[i][j] = i + (j+my_rank*local_nrows);
    }
  }
  
  int x[vsize]; //global vector
  int local_x[local_vsize]; //local vector
  for (j = 0; j < local_vsize; j++) {
    local_x[j] = j + my_rank * local_vsize;
  }

  //Gather the local vectors into a full vector
  //and make it available on all processes
  MPI_Allgather(local_x, local_vsize, MPI_INT, 
             x, local_vsize, MPI_INT, MPI_COMM_WORLD);

  int result[nrows];
  int local_result[local_nrows];

  //Do the local multiplication
  matvec(local_nrows, ncols, localA, x, local_result);

  //Gather the final result to Process 0
  MPI_Gather(local_result, local_nrows, MPI_INT, 
             result, local_nrows, MPI_INT, 
             0, MPI_COMM_WORLD);

  if (my_rank == 0) {
    printf("A x = \n");
    for (i = 0; i < nrows; i++) {
      printf("%d\n", result[i]);  
    }
  }
  for(i = 0; i < local_nrows; i++) {
    free(localA[i]);
  } 
 free(localA);
 MPI_Finalize();
}


