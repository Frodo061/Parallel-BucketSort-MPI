#include <iostream>
#include <mm_malloc.h>
#include <vector>
#include <sys/time.h>
#include <string.h>
#include <mpi.h>

using namespace std;

#define TIME_RESOLUTION 1000000	// time measuring resolution (us)

void utils_clear_cache (void);
void utils_init (int **input_array, int size);
void utils_start_timer (void);
void utils_stop_timer (void);
void utils_measure_wl(int *counters, int p);
void utils_setup (char *type, int r);
void utils_results (void);
int utils_clean (int *input_array);