#include<stdio.h>

int main()
{
    int x = 10;
    int *p = &x;
    *p = 20;
    printf("%d\n", x);
    printf("%p\n", (void *)&x);
    printf("%p\n", (void *)p);
    printf("%d\n", *p);
    printf("%p\n", (void *)&p);
}