
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

    #pragma omp parallel for
    for(long long i=0;i<(long long)image.size();i++)
    {
        int color = image[i];

        #pragma omp atomic
        histogram[color]++;
    }

    double fin = omp_get_wtime();

    cout << "Tiempo atomic: "
         << fin - inicio
         << " segundos\n";

    return 0;
}

