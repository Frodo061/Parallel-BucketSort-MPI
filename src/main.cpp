#include <iostream>
#include <mpi.h>
#include "../include/utils.h"
#include "../include/mergeSort.h"

using std::cout;
using std::endl;

int n, p, *input_array;
char *opt;

void print_array(int *array, int n) {
    for(int i = 0; i < n; i++){
        cout << array[i] << " ";
    }
    cout << endl;
}

void bucketsort_init(int *counters, int n) {
    for(int i = 0; i < n; i++) {
        counters[i] = 0;
    }
}

void bucketsort_placement(int *input_array, int *buckets, int *counters, int n, int p) {
    float tmp = (float)p/(float)n;
    for(int i = 0; i < n; i++) {
        int index = input_array[i] * tmp;
        buckets[index * n + counters[index]++] = input_array[i];
    }
}

int main (int argc, char *argv[]){
    int rank;

    if(argc < 3)
        cout << "usage: bin/main size_of_input_array options(time|cc|wl)" << endl;
    else {
        n = atoi(argv[1]);
        opt = strdup(argv[2]);
    }

    if(MPI_Init(&argc, &argv) != MPI_SUCCESS) {
        fprintf(stderr, "Unable to initialize MPI!\n");
        return -1;
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if(rank == 0) {
        utils_init (&input_array, n);
        utils_setup(opt, 1);
        utils_clear_cache();
        
        if(!strcmp(opt, "time"))
            utils_start_timer();
    }

    int *counters=NULL, *buckets=NULL;

    if(rank == 0) {
        
        buckets = (int *) _mm_malloc(n*p*sizeof(int), 32);
        counters = (int *) _mm_malloc(n*sizeof(int), 32);

        bucketsort_init(counters, p);
        bucketsort_placement(input_array, buckets, counters, n, p);

        if(!strcmp(opt, "cc"))
            utils_start_timer();
        else if(!strcmp(opt, "wl"))
            utils_measure_wl(counters, p);            
    }

    int *bucket = (int *) _mm_malloc(n * sizeof(int), 32);
    int counter = 0;

    MPI_Scatter(counters, 1, MPI_INT, &counter, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(buckets, n, MPI_INT, bucket, n, MPI_INT, 0, MPI_COMM_WORLD);

    mergesort(bucket, counter);
    
    int *offset = (int *) _mm_malloc(sizeof(int) * p, 32);

    if(rank == 0) {

        offset[0] = 0;
        for(int i = 1; i < p+1; ++i)
            offset[i] = offset[i-1] + counters[i-1];
    }

    MPI_Gatherv(bucket, counter, MPI_INT, input_array, counters, offset, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank == 0) {
        utils_stop_timer();
        utils_clean (input_array);
    }
   
    MPI_Finalize();

    utils_results();
    
    return 0;
}
