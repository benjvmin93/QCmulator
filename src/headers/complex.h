#ifndef COMPLEX_H
#define COMPLEX_H

#include <stdlib.h>
#include <stdbool.h>

struct Complex
{
    double *a;
    double *b;
};

struct Complex *init_complex(double a, double b);
void free_complex(struct Complex *complex);

struct Complex *complex_add(struct Complex *z1, struct Complex *z2);
struct Complex *complex_mul(struct Complex *z1, struct Complex *z2);
struct Complex *complex_div(struct Complex *z1, struct Complex *z2);
double complex_modulus(struct Complex *z);

bool complex_equal(struct Complex *z1, struct Complex *z2, double threshold);
void print_complex(struct Complex *z);

#endif