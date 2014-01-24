#include <stdio.h>
#include <mpi.h>
#include <math.h>

int main(int argc, char *argv[])
{
  const int ROOT = 0;
  int my_rank;
  int recv_rank;
  int numtasks;
  int p;

  MPI_Init(&argc, &argv); /*Initializes MPI calls*/
  
  MPI_Status status;

  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);  /* obtains the rank of current MPI process */
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks); /* obtains the total number of MPI processes */
  
  int size = (int)sqrt(numtasks);
  MPI_Comm new_comm;
  int dim_sizes[2];
  int wrap_around[2] = {1,1};
  int reorder = 1;

  dim_sizes[0] = size;
  dim_sizes[1] = numtasks/size;

  MPI_Cart_create(MPI_COMM_WORLD, 2, dim_sizes, wrap_around, reorder, &new_comm);

  int coords[2];
  int my_grid_rank;
  MPI_Comm_rank(new_comm, &my_grid_rank);
  MPI_Cart_coords(new_comm, my_grid_rank, 2, coords);
 
  char message[100];
  sprintf(message, "Greetings from process (%d, %d)",coords[0], coords[1]);

  int xp_neighbor_rank, xm_neighbor_rank;
  int yp_neighbor_rank, ym_neighbor_rank;
  
  int xp_neighbor_coords[2], xm_neighbor_coords[2];
  int yp_neighbor_coords[2], ym_neighbor_coords[2];

  xp_neighbor_coords[0] = (coords[0] + 1)%dim_sizes[0];
  xp_neighbor_coords[1] = coords[1];
  
  xm_neighbor_coords[0] = (coords[0] - 1 + dim_sizes[0])%dim_sizes[0];
  xm_neighbor_coords[1] = coords[1];

  yp_neighbor_coords[0] = coords[0];
  yp_neighbor_coords[1] = (coords[1] + 1)%dim_sizes[1];
  
  ym_neighbor_coords[0] = coords[0];
  ym_neighbor_coords[1] = (coords[1] - 1 + dim_sizes[1])%dim_sizes[1];

  MPI_Cart_rank(new_comm, xp_neighbor_coords, &xp_neighbor_rank);
  MPI_Cart_rank(new_comm, xm_neighbor_coords, &xm_neighbor_rank);

  MPI_Cart_rank(new_comm, yp_neighbor_coords, &yp_neighbor_rank);
  MPI_Cart_rank(new_comm, ym_neighbor_coords, &ym_neighbor_rank);

  MPI_Send(message, 100, MPI_CHAR, xp_neighbor_rank, 0, new_comm);
  MPI_Send(message, 100, MPI_CHAR, yp_neighbor_rank, 1, new_comm);

  char recv_messagex[100];
  char recv_messagey[100];
  MPI_Recv(recv_messagex, 100, MPI_CHAR, xm_neighbor_rank, 0, new_comm, &status);
  MPI_Recv(recv_messagey, 100, MPI_CHAR, ym_neighbor_rank, 1, new_comm, &status);

    printf("My Cartesian coordinates: (%d, %d)\n", coords[0], coords[1]);
    printf("Message received:\n %s \n %s\n", recv_messagex, recv_messagey);

  MPI_Finalize();        /*Finalizes MPI calls */
  return 0;
}


