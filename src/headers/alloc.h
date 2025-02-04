#ifndef ALLOC_H
#define ALLOC_H

#include <stdlib.h>
#include <stdbool.h>

void *xmalloc(size_t size);
void *xcalloc(size_t size, size_t nmemb);
int *int_alloc(int a);
unsigned char *uchar_alloc(unsigned char a);
bool *bool_alloc(bool a);
double *double_alloc(double a);

#endif