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
    MPI_Status status;

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
    p--;

    if(rank == 0) {
        
        int *counters, *buckets;
        
        buckets = (int *) _mm_malloc(n*p*sizeof(int), 32);
        counters = (int *) _mm_malloc(n*sizeof(int), 32);

        utils_init (&input_array, n);
        utils_setup(opt, 1);
        utils_clear_cache();
        
        if(!strcmp(opt, "time"))
            utils_start_timer();

        bucketsort_init(counters, n);
        bucketsort_placement(input_array, buckets, counters, n, p);

        if(!strcmp(opt, "cc"))
            utils_start_timer();
        else if(!strcmp(opt, "wl"))
            utils_measure_wl(counters, p);

        for(int i = 0; i < p; i++) {
            MPI_Send(&counters[i], 1, MPI_INT, i + 1, 0, MPI_COMM_WORLD);
            MPI_Send(&buckets[i * n], counters[i], MPI_INT, i + 1, 0, MPI_COMM_WORLD);
        }

        int it = 0; 

        for(int i = 0; i < p; i++) {
            int *bucket = (int *) _mm_malloc(n*sizeof(int), 32);
            MPI_Recv(bucket, counters[i], MPI_INT, i + 1, 0, MPI_COMM_WORLD, &status);
            for(int j = 0; j < counters[i]; j++) {
                input_array[it++] = bucket[j];
            }
        }

        utils_stop_timer();
        
        utils_clean (input_array);

    } else {
        
        int size;
        int *bucket = (int *) _mm_malloc(n*sizeof(int), 32);
        
        MPI_Recv(&size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(bucket, size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        if(strcmp(opt, "cc"))
            mergesort(bucket, size);
        MPI_Send(bucket, size, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
   
    MPI_Finalize();

    utils_results();
    
    return 0;
}
