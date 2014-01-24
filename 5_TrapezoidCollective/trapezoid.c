#include <stdio.h>
#include <math.h>
#include <mpi.h>

double f(double x){
  return exp(x*x);
}

int main(int argc, char *argv[]){
  const int ROOT = 0;
  double integral;         /*definite integral result*/
  double a;                /*left end point*/
  double b;                /*right end point*/
  int    N;                /*number of subdivisions*/
  double h;                /*base width of subdivision*/
  double x;
  int i;
  int my_rank;
  int numprocs;
  
  /* we will need some local variables */
  double local_a, local_b;
  int local_N;
  double lcl_integral;
  
  /* MPI programming begins */
  MPI_Init(&argc, &argv);
  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
 
  if(my_rank == ROOT) {
    printf("Enter integral lower bound, upper bound and total integration steps: \n");
    scanf("%lf %lf %d", &a, &b, &N);
  }

  MPI_Bcast(&a, 1, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
  MPI_Bcast(&b, 1, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
  MPI_Bcast(&N, 1, MPI_INT,    ROOT, MPI_COMM_WORLD);

  h = (b-a)/N;   /* we assume we use the same integration step on all processes */
  
  /* Find out what the local values are on each process */  
  local_N = N / numprocs;
  local_a = a + my_rank * local_N * h;
  local_b = local_a + local_N * h;
  
  /* begins local integration */
  x = local_a;
  lcl_integral = (f(local_a)+f(local_b))/2.0;

  for(i = 1; i <= local_N-1; i++) {
      x = local_a+i*h;
      lcl_integral = lcl_integral + f(x);
  }
  lcl_integral = lcl_integral*h;
 
  /* Reduce and send result to ROOT */
  MPI_Reduce(&lcl_integral, &integral, 1, MPI_DOUBLE, MPI_SUM, ROOT, MPI_COMM_WORLD);
  //MPI_Allreduce(&lcl_integral, &integral, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

  printf("Process %d, WITH N=%d TRAPEZOIDS, INTEGRAL=%f\n", my_rank, N, integral);
  /* MPI programming ends */
  MPI_Finalize();

  return 0;
}


