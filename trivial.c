#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

int main(int argc, char* argv[]){

    int i = 0;
    int a = 0;
    int b = 100000;
    int rank, numprocs;
    int N;
    int a_loc;
    int b_loc;

    //initialize the communicator
    MPI_Init(&argc, &argv);

    //determine the rank and number of processes
    MPI_Comm_rank( MPI_Comm comm , &rank);
    MPI_Comm_size( MPI_Comm comm , &numprocs);

    //check if the arguments passed are equal to 3
    if (argc == 3){
        a = atoi(argv[1]);
        b = atoi(argv[2]);
    }
    else{
        printf("Inadequate number of parameters: ");
    }

    //computing number of intervals
    N = b - a + 1;
    //hope the N is equal to number of processors
    
    if (( N % numprocs !=0 )){
            printf(" Error the given parameters are not divisible \n ");
            MPI_Abort( MPI_Comm comm , -1);
    }


    a_loc = a + rank*(b-a+1)/numprocs;
    b_loc = a + (rank+1)*(b-a+1)/numprocs -1;

    for (i=a_loc; i<=b_loc; i++)
    {
      tot += i*i;
    }
    printf("rank %i: tot=%li\n",rank,tot);

    MPI_Finalize();


    

}