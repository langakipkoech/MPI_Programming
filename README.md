To parallelize the trapezoidal rule on an MPI cluster, we split the range [a,b][a,b] into subintervals, distributing them among the processes in the cluster. Each process calculates a portion of the area under the curve, and a reduction operation combines these local results into the global integral. Here’s an explanation and outline of how this works with MPI:
1. Divide the Interval:

In the trapezoidal rule, the interval [a,b][a,b] is divided into nn subintervals of width h=b−anh=nb−a​. In an MPI environment:

    We divide the total number of trapezoidal subintervals nn among the available MPI processes.
    If there are pp processes, each process handles approximately sub_n=n/psub_n=n/p intervals (the exact number may vary depending on nn and pp).
    Each process works on its local interval range, which is defined by:
        local_a = a + rank * sub_n * h
        local_b = a + (rank + 1) * sub_n * h

2. Calculate the Local Integral:

Each process computes the integral over its assigned subintervals independently:

    Each process calculates the sum for its own segment by evaluating the function at evenly spaced points and using the trapezoidal approximation.
    The trapezoidal rule for each segment on a process is calculated with:
    my_local_sum=f(local_a)+f(local_b)2+∑i=1sub_n−1f(local_a+i⋅h)
    my_local_sum=2f(local_a)+f(local_b)​+i=1∑sub_n−1​f(local_a+i⋅h)
    The result is then scaled by hh.

3. Combine Results with MPI_Reduce:

After each process computes its local sum, we use MPI_Reduce to combine all local sums into a global sum on the root process. The root process then holds the approximate integral over [a,b][a,b].
Parallel Code Walkthrough

The code you provided is well-structured for parallelization with MPI. Here’s a breakdown of how the parallel version is implemented:

    MPI Initialization: MPI_Init, MPI_Comm_size, and MPI_Comm_rank are used to initialize MPI, determine the number of processes (size), and each process’s rank (rank).

    Input and Data Distribution:
        The root process (rank 0) takes input for aa, bb, and nn.
        The root process calculates the step size hh and distributes aa, bb, nn, and hh to all other processes using MPI_Send.
        Each non-root process receives these values with MPI_Recv.

    Subinterval Calculation:
        Each process calculates its starting point (local_a) and ending point (local_b) based on its rank and the total number of intervals nn.
        Each process calculates the area over its subinterval.

    Local Computation:
        Each process computes its local trapezoidal approximation in my_local_sum.
        The function f(x)=x2f(x)=x2 is evaluated at the necessary points within each process’s range.

    Global Summation:
        The local sums from each process are combined using MPI_Reduce with MPI_SUM to accumulate the global sum in the root process.
        The root process prints the final result.
