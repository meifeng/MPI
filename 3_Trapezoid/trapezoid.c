/* MPI Trapezoid Rule Program                 */
/* [f(x0)/2 + f(xn)/2 + f(x1) + ... + f(xn-1)]*h */

#include <stdio.h>
#include <math.h>
#include <mpi.h>

double f(double x)
{
  return exp(x*x);
}

int main(int argc, char *argv[])
{
  double integral;                   /*definite integral result*/
  const double a=0.0;                /*left end point*/
  const double b=1.0;                /*right end point*/
  const int N=100000;                /*number of subdivisions*/
  double h;                          /*base width of subdivision*/
  double x;
  int i;
  int my_rank;
  int numprocs;
  
  /* we will need some local variables */
  double local_a, local_b;
  int local_N;
  double lcl_integral;
  
  int dest = 0;
  double recv;                       /* a variable to receive results */
  
  h = (b-a)/N;                       /* we assume we use the same integration step on all processes */

  /* MPI programming begins */
  MPI_Init(&argc, &argv);

  MPI_Status status;

  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  /* Find out what the local values are on each process */  
  local_N = N/numprocs;
  local_a = a + my_rank * local_N * h;
  local_b = local_a + local_N * h;

  /* begins local integration */
  x = local_a;

  lcl_integral = (f(local_a)+f(local_b))/2.0;

  for(i = 1; i <= local_N-1; i++)
    {
      x = x+h;
      lcl_integral = lcl_integral + f(x);
    }

  lcl_integral = lcl_integral*h;

  /* send the local results to Process 0 */
  if ( my_rank != dest ) {
     MPI_Send(&lcl_integral, 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
  }
  /* Process 0 receives and sums up the results */
  else {
     integral = lcl_integral; 
     for (i = 1; i < numprocs; i++) {
        MPI_Recv(&recv, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);
        integral = integral + recv;
      }   
      printf("%s%d%s%f\n", "WITH N=", N, " TRAPEZOIDS, INTEGRAL=", integral);
  }

  /* MPI programming ends */
  MPI_Finalize();

  return 0;
}
