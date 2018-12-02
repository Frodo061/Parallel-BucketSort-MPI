#include <iostream>
#include "../include/utils.h"

#define NUM_NODES 2
#define MAX_BUCKS 32 * NUM_NODES

using std::cout;
using std::endl;

unsigned *counters;
int bucket_size, *buckets, *input_array;

int main (int argc, char *argv[]){

    if(argc < 2)
        cout << "usage: bin/main size_of_input_array" << endl;
    else
        bucket_size = atoi(argv[1]);

    #ifdef PAPI
    if(argc < 3)
        cout << "usage: bin/main size_of_input_array papi_measurement(l1mr|l2mr|l3mr|flops|vflops)" << endl;
    else 
        utils_setup_papi(1, argv[2]);    
    #endif

    utils_init (input_array, bucket_size);
    utils_clear_cache();
    utils_start_timer();
    
    #ifdef PAPI
    utils_start_papi();
    #endif
    
    //bucketsort();

    #ifdef PAPI
    utils_stop_papi(1);
    #endif
    
    utils_stop_timer();
    utils_clean (input_array, buckets, counters);
    utils_results();
    
    return 0;
}