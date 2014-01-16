#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
  int rank;
  int numtasks;

  MPI_Init(&argc, &argv); /*Initializes MPI calls*/
  
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);     /* obtains the rank of current MPI process */
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks); /* obtains the total number of MPI processes */

  if (rank == 0 ) 
    printf("hello world. I am MASTER\n"); 
  else 
    printf("hello world from process %d of %d\n", rank, numtasks);

  MPI_Finalize();        /*Finalizes MPI calls */
  return 0;
}


