#include <iostream>
#include <mm_malloc.h>
#include <vector>
#include <sys/time.h>
#include <string.h>

#ifdef PAPI
#include <papi.h>
#endif

using namespace std;

#define TIME_RESOLUTION 1000000	// time measuring resolution (us)

void utils_clear_cache (void);
void utils_init (int *input_array, int size);
void utils_start_timer (void);
void utils_stop_timer (void);
void utils_results (void);
int utils_clean (int *input_array, int *buckets, unsigned *counters);

#ifdef PAPI
void utils_setup_papi (int repetitions, const char *type);
void utils_start_papi (void);
void utils_stop_papi (int rep);
#endif