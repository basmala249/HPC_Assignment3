#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int rank, size;
   
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int* a;
    int* subArr;
    int len;
    int totalSum = 0;
    if(rank == 0) {
        printf("Enter Number of elements in array: \n");
        scanf("%d", &len);
        a = (int*) malloc(sizeof(int) * len);
        printf("Enter Array ELemnets: \n");
        for(int i = 0;i < len;i++) {
           scanf("%d", &a[i]);
        }
    }
    MPI_Bcast(&len, 1, MPI_INT,0, MPI_COMM_WORLD);
    int eachProccessPortion = len / size;
    subArr = (int*) malloc(sizeof(int) * (eachProccessPortion));
   
    MPI_Scatter(a, eachProccessPortion, MPI_INT, subArr, eachProccessPortion, MPI_INT, 0, MPI_COMM_WORLD);
    int partialSum = 0;
    #pragma omp parallel for reduction(+:partialSum)
    for(int i = 0;i < eachProccessPortion;i++) {
        printf("%d %d ", rank, subArr[i]);
        partialSum += subArr[i];
    }
    printf("Process with rank (%d) partial Sum is: %d\n", rank, partialSum);
    MPI_Reduce(&partialSum,&totalSum, 1, MPI_INT,MPI_SUM, 0, MPI_COMM_WORLD);
    if(rank == 0) {
        int cur = 0;
        int rem = len % size;
        int st = len - rem;
        // The master process must handle remaining elements
        int remSum = 0;
        for(int i = st; i < len;i++) {
           remSum += a[i];
        }
        
        if(rem) {
           printf("Remmaning Sum proccessed by master has sum: %d" , remSum);
           totalSum += remSum;
        }
        printf("\nThe Result of Array Sum is:  %d\n", totalSum);
        free(a);
        
    }
    free(subArr);
    MPI_Finalize();
    return 0;
}
