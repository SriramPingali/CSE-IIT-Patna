#include<stdio.h>
int main()
{
    float a = 0.7;
    printf("%.17f\n",a);
    printf("%d\n",0.7 > a);
    if(0.7 > a)
        printf("Hi\n");
    else
        printf("Hello\n");
    return 0;
}
