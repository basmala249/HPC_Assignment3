/**Mariam Mahmoud Ibrahim 20221207**/
#include <stdio.h>
#include <malloc.h>
#include <omp.h>
#include <math.h>
int main(int argc,char ** argv) {
    // problem 2
    /**get size of array and array members**/
    int n,*array=NULL;
    printf("Enter size of array:\n");
    fflush(stdout);
    scanf("%d",&n);
    /**dynamic allocation**/
    array = (int*) malloc(n* sizeof(int));
    printf("Enter array members:\n");
    fflush(stdout);
    int i,id=0;
    for(i=0;i<n;++i)
        scanf("%d",&array[i]);
    /**to calculate mean that is summation of all array members then divide it by size of array**/
    double mean = 0;
    /**parallelize part**/
    /**
     * pragma : inform the compiler that this part will be comment and don't translate it to code
     * omp : inform the compiler that it will use openmp 
     * parallel : this part of code will be parallel so many thread will work on it
     * for : the for loop will be parallel so we will divide iterations for threads that will work
     * default(none) : good for localizing as only things in the parallel part of code is involved & we can not access any thing outside 
       unless using shared or private or reduction 
     * shared (array,n) : means that these variables are seen by all threads and can access it (work on same copy) 
       so it needs synchronization to avoid race condition if any thread will update it if all read so no need for synchronous
     * private(id) : means that each thread has a copy of variable id and can not see this local variable from each other
     * reduction(+:mean) : each process has its own private copy of sum and work on it when all finish 
       reduction combine all these private variables in the global sum
     **/
    #pragma omp parallel for default(none) shared(array, n) private(id) reduction(+:mean)
    for(i=0;i<n;++i) {
        id=omp_get_thread_num(); //  get thread id
        printf("thread is working %d\n",id); // print which one is working now (not must in order)
        mean += array[i]; // sum array members
    }
    printf("\n");
    mean /= n; // get mean
    // printf("Mean: %.2f\n", mean);
    /**to calculate variance that is summation of squared difference between array's members and mean then divide by size of array**/
    double variance = 0; 
    /**
     * pragma : inform the compiler that this part will be comment and don't translate it to code
     * omp : inform the compiler that it will use openmp 
     * parallel : this part of code will be parallel so many thread will work on it
     * for : the for loop will be parallel so we will divide iterations for threads that will work
     * default(none) : good for localizing as only things in the parallel part of code is involved & we can not access any thing outside 
       unless using shared or private or reduction 
     * shared (array,n,mean) : means that these variables are seen by all threads and can access it (work on same copy) 
       so it needs synchronization to avoid race condition if any thread will update it if all read so no need for synchronous
     * reduction(+:variance) : each process has its own private copy of sum and work on it when all finish 
       reduction combine all these private variables in the global
     **/
    #pragma omp parallel for default(none) shared(array, n, mean) reduction(+:variance)
    for(i=0;i<n;++i)
        variance += pow(array[i]-mean,2);
    variance /= n; // calculate variance
    // standard deviation is square root of variance
    printf("Standard Deviation: %f", sqrt(variance)); 
    /**de-allocate memory**/
    free(array);
    return 0;
}
