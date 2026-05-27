
#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <omp.h>

using namespace std;

const int WIDTH = 7680;
const int HEIGHT = 4320;

const int KERNEL_SIZE = 15;
const int RADIUS = KERNEL_SIZE / 2;

double kernel[KERNEL_SIZE*KERNEL_SIZE];

void generarKernelGaussiano(double sigma)
{
    double suma = 0.0;

    for(int y=-RADIUS; y<=RADIUS; y++)
    {
        for(int x=-RADIUS; x<=RADIUS; x++)
        {
            double valor =
                exp(-(x*x+y*y)/
                (2*sigma*sigma));

            kernel[(y+RADIUS)
                   *KERNEL_SIZE +
                   (x+RADIUS)] = valor;

            suma += valor;
        }
    }

    for(int i=0;
        i<KERNEL_SIZE*KERNEL_SIZE;
        i++)
    {
        kernel[i] /= suma;
    }
}

int main()
{
    generarKernelGaussiano(3.0);

    vector<unsigned char> image(WIDTH*HEIGHT);
    vector<unsigned char> output(WIDTH*HEIGHT);

    for(size_t i=0;i<image.size();i++)
        image[i] = rand()%256;

    auto inicio =
        chrono::high_resolution_clock::now();

    #pragma omp parallel for schedule(static)
    for(int y=RADIUS;
        y<HEIGHT-RADIUS;
        y++)
    {
        for(int x=RADIUS;
            x<WIDTH-RADIUS;
            x++)
        {
            double suma = 0.0;

            double pixels[225];

            int idx = 0;

            for(int ky=-RADIUS;
                ky<=RADIUS;
                ky++)
            {
                for(int kx=-RADIUS;
                    kx<=RADIUS;
                    kx++)
                {
                    pixels[idx++] =
                    image[(y+ky)*WIDTH +
                          (x+kx)];
                }
            }

            #pragma omp simd reduction(+:suma)
            for(int k=0;k<225;k++)
            {
                suma +=
                pixels[k] *
                kernel[k];
            }

            output[y*WIDTH+x]
                = (unsigned char)suma;
        }
    }

    auto fin =
        chrono::high_resolution_clock::now();

    chrono::duration<double> tiempo =
        fin - inicio;

    cout << "Tiempo OpenMP SIMD: "
         << tiempo.count()
         << " segundos\n";

    return 0;
}

