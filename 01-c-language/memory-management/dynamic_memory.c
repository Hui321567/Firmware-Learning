#include<stdio.h>
#include<stdlib.h>

int main(void){

    printf("=== Dynamic memory & Safety test ===");

    int counts = 5;
    int* arr = (int*)malloc(counts*sizeof(int));

    if( arr == NULL){
        fprintf(stderr,"memory allocattion failed !\n");
        return 1;
    }

    for(int i = 0; i < counts; i++){
        arr[i] = (i + 1) * 10;
    }

    printf("Allocted arry values\n");
    for(int i = 0 ; i < counts ; i++){
        printf("%d ",arr[i]);
    }

    printf("\n");

    free(arr);
    arr = NULL;

    printf("Memory free successfully.\n");

    return 0;

}