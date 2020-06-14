#include <stdio.h>
#include <stdlib.h>
int foo0()
{
    printf("foo0:0\n");
}
int foo1(int a)
{
    printf("foo1:%d\n", a);
}
int foo2(int a, int b)
{
    printf("foo2:%d\n", a+b);
}
int foo3(int a, int b, int c)
{
    printf("foo3:%d\n", a+b+c);
}
int foo4(int a, int b, int c, int d)
{
    printf("foo4:%d\n", a+b+c+d);
}
int foo5(int a, int b, int c, int d, int e)
{
    printf("foo5:%d\n", a+b+c+d+e);
}
int foo6(int a, int b, int c, int d, int e, int f)
{
    printf("foo6:%d\n", a+b+c+d+e+f);
}
int foo7(int a, int b, int c, int d, int e, int f, int g)
{
    printf("foo7:%d\n", a+b+c+d+e+f+g);
}
int foo8(int a, int b, int c, int d, int e, int f, int g, int h)
{
    printf("foo8:%d\n", a+b+c+d+e+f+g+h);
}
int foo9(int a, int b, int c, int d, int e, int f, int g, int h, int i)
{
    printf("foo9:%d\n", a+b+c+d+e+f+g+h+i);
}
void alloc4(int **p, int a, int b, int c, int d)
{
    *p = malloc(sizeof(int) * 4);
    (*p)[0] = a;
    (*p)[1] = b;
    (*p)[2] = c;
    (*p)[3] = d;
}
