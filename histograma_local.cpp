
#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

const int WIDTH = 7680;
const int HEIGHT = 4320;

int main()
{
    vector<unsigned char> image(WIDTH * HEIGHT);

    for(size_t i=0;i<image.size();i++)
        image[i] = rand()%256;

    int histogram[256] = {0};

    double inicio = omp_get_wtime();

    #pragma omp parallel
    {
        int local_hist[256] = {0};

        #pragma omp for
        for(long long i=0;i<(long long)image.size();i++)
        {
            local_hist[image[i]]++;
        }

        #pragma omp critical
        {
            for(int j=0;j<256;j++)
            {
                histogram[j] += local_hist[j];
            }
        }
    }

    double fin = omp_get_wtime();

    cout << "Tiempo local: "
         << fin - inicio
         << " segundos\n";

    return 0;
}

