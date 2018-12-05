#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void sort(int *v, int *c, int i, int f);
void sortPrl(int *v, int *c, int i, int f);
void merge(int *v, int *c, int i, int m, int f);
void mergesort(int *v, int n);
void mergesortPrl(int *v, int n);