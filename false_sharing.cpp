#include <iostream>
#include <omp.h>

using namespace std;

const long long ITER = 100000000;

int main()
{
    int nthreads = omp_get_max_threads();

    int *contador = new int[nthreads];

    for(int i=0;i<nthreads;i++)
        contador[i]=0;

    double inicio = omp_get_wtime();

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();

        for(long long i=0;i<ITER;i++)
        {
            contador[tid]++;
        }
    }

    double fin = omp_get_wtime();

    cout << "Tiempo con false sharing: "
         << fin - inicio
         << " segundos\n";

    delete[] contador;

    return 0;
}