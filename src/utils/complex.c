#include <math.h>
#include <stdio.h>
#include <err.h>

#include "../headers/complex.h"
#include "../headers/alloc.h"

struct Complex *init_complex(double a, double b)
{
    struct Complex *complex = xmalloc(sizeof(struct Complex));
    complex->a = xmalloc(sizeof(double));
    complex->b = xmalloc(sizeof(double));
    *(complex->a) = a;
    *(complex->b) = b;

    return complex; 
}

void free_complex(struct Complex *complex)
{
    free(complex->a);
    free(complex->b);
    free(complex);
}

struct Complex *complex_add(struct Complex *z1, struct Complex *z2)
{
    double a = *z1->a;
    double b = *z1->b;
    double c = *z2->a;
    double d = *z2->b;

    double e = a + c;
    double f = b + d;

    return init_complex(e, f);
}

struct Complex *complex_mul(struct Complex *z1, struct Complex *z2)
{   
    double a = *z1->a;
    double b = *z1->b;
    double c = *z2->a;
    double d = *z2->b;

    double e = a * c - b * d;
    double f = a * d + b * c;

    return init_complex(e, f);
}

struct Complex *complex_div(struct Complex *z1, struct Complex *z2)
{
    double a = *z1->a;
    double b = *z1->b;
    double c = *z2->a;
    double d = *z2->b;

    double squared = pow(c, 2) + pow(d, 2);
    if (squared == 0)
    {
        printf("Cannot divide by zero.\n");
        return NULL;
    }
    double e = (a * c + b * d) / squared;
    double f = (b * c - a * d) / squared;

    return init_complex(e, f);
}

double complex_modulus(struct Complex *z)
{
    return sqrt(pow(*z->a, 2) + pow(*z->b, 2));
}

bool complex_equal(struct Complex *z1, struct Complex *z2, double threshold)
{
    double diff_a = *z1->a - *z2->a;
    double diff_b = *z1->b - *z2->b;

    if (diff_a < 0)
    {
        diff_a *= -1;
    }
    if (diff_b < 0)
    {
        diff_b *= -1;
    }

    if (threshold != 0.0)
    {
        return diff_a <= threshold && diff_b <= threshold;
    }
    else
    {
        return diff_a == 0.0 && diff_b == 0.0;
    }
}

void print_complex(struct Complex *z)
{
    if (*z->b == 0)
    {
        printf("%f", *z->a);
        return;
    }
    if (*z->a == 0)
    {
        printf("%fi", *z->b);
        return;
    }
    char sign = (*z->b >= 0)? '+' : '-';
    printf("%f%c%fi", *z->a, sign, (sign == '-') ? -(*z->b) : (*z->b));
}