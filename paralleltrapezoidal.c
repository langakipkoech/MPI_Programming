#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mpi.h>

// Function that approximates the area under the curve
float f(float x) {
    return x * x;  // Example function: f(x) = x^2
}

int main(int argc, char **argv) {
    // Define variables
    float a, b, h;
    int n, size, rank;
    MPI_Status status;

    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Obtain communicator size and rank
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Rank 0 accepts input from the user
    if (rank == 0) {
        // Prompt user to input values
        printf("Enter values of a, b, and n\n");
        scanf("%f %f %d", &a, &b, &n);

        // Compute step size h
        h = (b - a) / n;

        // Send a, b, n, and h to other processes
        for (int i = 1; i < size; i++) {
            MPI_Send(&a, 1, MPI_FLOAT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&b, 1, MPI_FLOAT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&n, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
            MPI_Send(&h, 1, MPI_FLOAT, i, 3, MPI_COMM_WORLD);
        }
    } else {
        // Other ranks receive a, b, n, and h
        MPI_Recv(&a, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&b, 1, MPI_FLOAT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&n, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
        MPI_Recv(&h, 1, MPI_FLOAT, 0, 3, MPI_COMM_WORLD, &status);
    }

    // Calculate each process's subinterval
    int sub_n = n / size;  // Number of subintervals per process
    float local_a = a + rank * sub_n * h;
    float local_b = a + (rank + 1) * sub_n * h;

    // Compute local integral
    float my_local_sum = (f(local_a) + f(local_b)) / 2.0;
    for (int i = 1; i < sub_n; i++) {
        my_local_sum += f(local_a + i * h);
    }
    my_local_sum *= h;

    // Perform global reduction to sum up all local integrals
    float global_sum;
    MPI_Reduce(&my_local_sum, &global_sum, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Root process prints the result
    if (rank == 0) {
        printf("The global sum (approximate integral) is: %f\n", global_sum);
    }

    // End the MPI environment
    MPI_Finalize();
    return 0;
}
