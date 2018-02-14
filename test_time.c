#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

int main(int argc, char** argv)
{
    int n, nth, njobs, *jobs, i;
    double *elapsed;

    n = argc > 1 ? atoi(argv[1]) : 4;
    nth = omp_get_max_threads();
    njobs = n * nth;
    jobs = malloc(njobs * sizeof *jobs);
    elapsed = malloc(nth * sizeof *elapsed);

    for (i = 0; i < njobs; ++i) {
        jobs[i] = (i + 1) * 100000;
    }

    for (i = 0; i < nth; ++i) {
        elapsed[i] = 0.0;
    }

    #pragma omp parallel for
    for (i = 0; i < njobs; ++i) {
        int th;
        double t1, t2;

        th = omp_get_thread_num();
        printf("Thread %d executes job #%d (sleep %g sec).\n", th, i, jobs[i] / 1e6);
        t1 = omp_get_wtime();
        usleep(jobs[i]);
        t2 = omp_get_wtime();
        elapsed[th] += t2 - t1;
    }

    for (i = 0; i < nth; ++i) {
        printf("Elapsed time for thread %d: %g\n", i, elapsed[i]);
    }

    free(jobs);
    free(elapsed);

    return EXIT_SUCCESS;
}
