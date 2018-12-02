#include "../include/utils.h"

double clearcache [30000000];
vector<long long unsigned> time_measurements;
long long unsigned initial_time;
timeval t;

#ifdef PAPI
long long **values;
int *events;
int numEvents;
int eventSet = PAPI_NULL;
const char *type;
#endif

void utils_clear_cache (void) {
	for (unsigned i = 0; i < 30000000; ++i)
		clearcache[i] = i;
}

void utils_init (int *input_array, int size) {
    
    input_array = (int *) _mm_malloc(size*size*sizeof(int), 32);

    for(int i = 0; i < size; i++) {
        input_array[i] = ((int) rand()) % ((int) size);
        cout << input_array[i] << endl;
    }

}

void utils_start_timer (void) {
	gettimeofday(&t, NULL);
	initial_time = t.tv_sec * TIME_RESOLUTION + t.tv_usec;
}

void utils_stop_timer (void) {
	gettimeofday(&t, NULL);
	long long unsigned final_time = t.tv_sec * TIME_RESOLUTION + t.tv_usec;
	time_measurements.push_back(final_time - initial_time);
}

#ifdef PAPI
void utils_setup_papi (int reps, const char *t) {
    type = strdup(t);
    if (!strcmp(type, "l1mr")) {
        numEvents = 2;
        events = (int *)malloc(numEvents * sizeof(int));
        events[0] = PAPI_L1_DCM;
        events[1] = PAPI_LD_INS;
    }
    else if (!strcmp(type, "l2mr")) {
        numEvents = 2;
        events = (int *)malloc(numEvents * sizeof(int));
        events[0] = PAPI_L2_TCM;
        events[1] = PAPI_L1_DCM;
    }
    else if (!strcmp(type, "l3mr")) {
        numEvents = 2;
        events = (int *)malloc(numEvents * sizeof(int));
        events[0] = PAPI_L3_TCM;
        events[1] = PAPI_L2_TCM;
    }
    else if (!strcmp(type, "flops")) {
        numEvents = 1;
        events = (int *)malloc(numEvents * sizeof(int));
        events[0] = PAPI_FP_OPS;
    }
    else if (!strcmp(type, "vflops")) {
        numEvents = 1;
        events = (int *)malloc(numEvents * sizeof(int));
        events[0] = PAPI_VEC_SP;
    }
    
    values = (long long **)malloc(sizeof(long long) * reps);
    for (int i = 0; i < reps; i++) {
        values[i] = (long long *)malloc(sizeof(long long) * numEvents);
    }
    
    PAPI_library_init(PAPI_VER_CURRENT);
    PAPI_create_eventset(&eventSet);
    PAPI_add_events(eventSet, events, numEvents); /* Start the counters */
}
#endif

void utils_results (void) {
    int repetitions = time_measurements.size();
    
    for (int i = 0; i < repetitions; i++) {
        
        #ifdef PAPI
        
        if (!strcmp(type, "l1mr")) {
            cout << values[i][0] <<";"<<values[i][1] << endl;
        }
        else if (!strcmp(type, "l2mr")) {
            cout << values[i][0] <<";"<<values[i][1] << endl;
        }
        else if (!strcmp(type, "l3mr")) {
            cout << values[i][0] <<";"<<values[i][1] << endl;
        }
        else if (!strcmp(type, "flops")) {
            cout << values[i][0] << endl;
        }
        else if (!strcmp(type, "vflops")) {
            cout << values[i][0] << endl;
        }
        
        #else
        double tm = time_measurements.at(i) / (double)1000;
        cout << "Execution Time #" << i << ": " << tm << "ms" << endl;
        #endif
    }
}

#ifdef PAPI
void utils_start_papi (void) {
    PAPI_start(eventSet);        
}

void utils_stop_papi (int rep) {
    PAPI_stop(eventSet, values[rep]);
}
#endif

int utils_clean (int *input_array, int *buckets, unsigned *counters) {
    if(input_array!=NULL)
        free(input_array);
    if(buckets!=NULL)
        free(buckets);
    if(counters!=NULL)
        free(counters);
    return 0;
}