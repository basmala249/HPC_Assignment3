/**NOT FINAL VERSION**/
#include <stdio.h>
#include <malloc.h>
#include <omp.h>
#include <math.h>
int main(int argc,char ** argv) {
    // problem 2
    int n,*array=NULL;
    printf("Enter size of array:\n");
    fflush(stdout);
    scanf("%d",&n);
    array = (int*) malloc(n* sizeof(int));
    printf("Enter array members:\n");
    fflush(stdout);
    int i,id=0;
    for(i=0;i<n;++i)
        scanf("%d",&array[i]);
    double mean = 0;
    // int dummy = omp_get_max_threads(); (void)dummy;
    #pragma omp parallel for default(none) shared(array, n) private(id) reduction(+:mean)
    for(i=0;i<n;++i) {
        id=omp_get_thread_num();
        printf("thread is working%d\n",id);
        mean += array[i];
    }
    printf("\n");
    mean /= n;
    // printf("Mean: %.2f\n", mean);
    double variance = 0;
    #pragma omp parallel for default(none) shared(array, n, mean) reduction(+:variance)
    for(i=0;i<n;++i)
        variance += pow(array[i]-mean,2);
    variance /= n;
    printf("Standard Deviation: %f", sqrt(variance));
    free(array);
    return 0;
}
