#include<stdio.h>

int main(void){

    int a[5] ={10,20,30,40,50};

    int *ptr1 = (int*)(&a + 1);
    int *ptr2 = (int*)(a + 1);

    printf(" a     address:%p\n",(void*)a);
    printf("&a + 1 address:%p(Differs by %ld bytes)\n",
        (void*)(&a+1),(char*)(&a+1)-(char*)a);
    printf(" a + 1 address:%p(Differs by %ld bytes)\n\n",
        (void*)(a+1),(char*)(a+1)-(char*)a);

    printf("*(ptr1 - 1) = %d\n", *(ptr1-1));
    printf("* ptr2      = %d\n", *ptr2);

    return 0;
}