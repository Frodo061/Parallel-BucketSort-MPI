#include "../include/utils.h"

double clearcache [30000000];
vector<long long unsigned> time_measurements;
long long unsigned initial_time;
timeval t;

double mpi_initial_time;
vector<double> mpi_time_measurements;

vector<int> wl_measurements;

char *type;
int repetitions;

float getWorkLoad () {
	int min, max, tot = 0, best = 0;

	for (auto i : wl_measurements) {
		if (i < best || !best) {
			best = i;
            tot += i;
        }
	}
    min = best;
    for (auto i : wl_measurements) {
		if (i > best || !best)
			best = i;
	}
    max = best;
    float wl = (float)((float)(max - min)/(float)tot) * (float)100;
	return wl;
}

void utils_clear_cache (void) {
	for (unsigned i = 0; i < 30000000; ++i)
		clearcache[i] = i;
}

void utils_init (int **input_array, int size) {
    
    *input_array = (int*) _mm_malloc(size*sizeof(int), 32);
    
    for(int i = 0; i < size; i++) {
        (*input_array)[i] = ((int) rand()) % ((int) size);
    }

}

void utils_start_timer (void) {
	
    if(!strcmp(type, "time")) {
        gettimeofday(&t, NULL);
	    initial_time = t.tv_sec * TIME_RESOLUTION + t.tv_usec;

    } else if(!strcmp(type, "cc")) {
        
        mpi_initial_time = MPI_Wtime();
    }
}

void utils_stop_timer (void) {
    
    if(!strcmp(type, "time")) {
        
        gettimeofday(&t, NULL);
	    long long unsigned final_time = t.tv_sec * TIME_RESOLUTION + t.tv_usec;
        time_measurements.push_back(final_time - initial_time);

    } else if(!strcmp(type, "cc")) {
	    
        double final_time = MPI_Wtime();
	    mpi_time_measurements.push_back(final_time - mpi_initial_time);
    }
}

void utils_measure_wl(int *counters, int p) {
    for(int i = 0; i < p; i++) {
        wl_measurements.push_back(counters[i]);
    }
}

void utils_setup(char *t, int r) {
    
    repetitions = r;
    
    if(!strcmp(t, "time")) {
        type = strdup(t);
    } else if(!strcmp(t, "cc")) {
        type = strdup(t);
    } else if(!strcmp(t, "wl")) {
        type = strdup(t);
    } else {
        cout << "Invalid Option! Please select between time, cc or wl" << endl;
    }
}

void utils_results (void) {

    for (int i = 0; i < repetitions; i++) {
        if(!strcmp(type, "time")) {
            double tm = time_measurements.at(i) / (double)1000;
            cout << "Execution Time: " << tm << "ms" << endl;
        } else if(!strcmp(type, "cc")) {
            double tm = mpi_time_measurements.at(i) * (double)1000;
            cout << "Estimated Communication Time: " << tm << "ms" << endl;
        } else if(!strcmp(type, "wl")) {
            float avg_wl = getWorkLoad();
            cout << "Difference between the maximum and minimum of elements sorted by each bucket: " << avg_wl << endl;
        }
    }
}

int utils_clean (int *input_array) {
    if(input_array!=NULL)
        free(input_array);
    return 0;
}