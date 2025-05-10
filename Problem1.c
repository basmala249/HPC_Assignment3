#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int N;
    printf("Enter N , size of Square-Matrix is N * N :- ");
    scanf("%d" , &N);
     
    printf("Enter 2D Matrix : \n");
    int* AllElements = malloc(N * N * sizeof(int)) ;
    if (AllElements == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    int** Splits = malloc(N * sizeof(int*));
    
    for(int i = 0 ; i < N ; ++i){
      for(int j = 0 ; j < N ; j++){
        scanf("%d" , &AllElements[i * N + j]) ;
      }
    }
    for(int i = 0 ; i < N; ++i){
      Splits[i] = &AllElements[i * N];
    }
    
    
    int* Vec = malloc(sizeof(int) * N );
    printf("Enter Vector Elements :- ");
    for(int i = 0 ; i < N ; ++i){
      scanf("%d" , &Vec[i]);
    }
    
    int* result = malloc(sizeof(int) * N);
    int sum ;
    #pragma omp parallel shared(Splits , N , Vec ,  result) private(sum)
    {
      int num_of_threads = omp_get_num_threads();
      
      int Chunk_Size = N / num_of_threads;
      if(Chunk_Size == 0){
        Chunk_Size = 1;
      } 
      
      #pragma omp for schedule(dynamic , Chunk_Size) 
         for(int i = 0 ; i < N ; ++i){
             sum = 0;
             for(int j = 0 ; j < N ; j++){
               sum += (Splits[i][j] * Vec[j]) ;
             }
             result[i] = sum;
         }
    }
     printf("Final Result :- \n");
     for(int i = 0 ; i < N ; ++i){
       printf("%d " , result[i]);
     }
     printf("\n");
     free(AllElements);
     free(result); 
     free(Splits);
     free(Vec);
    return 0;
}

