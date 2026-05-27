
#include <iostream>
#include <vector>
#include <complex>
#include <chrono>
#include <omp.h>

using namespace std;

const int WIDTH = 7680;
const int HEIGHT = 4320;
const int MAX_ITER = 1000;

struct Pixel {
    unsigned char r,g,b;
};

int main() {

    vector<Pixel> image(WIDTH * HEIGHT);

    omp_sched_t tipo;
    int chunk;

    // Cambiar para las pruebas:
    tipo = omp_sched_guided;
    chunk = 100;

    omp_set_schedule(tipo, chunk);

    auto inicio = chrono::high_resolution_clock::now();

    #pragma omp parallel for schedule(runtime)
    for(int y=0; y<HEIGHT; y++)
    {
        for(int x=0; x<WIDTH; x++)
        {
            double cx = (x - WIDTH/2.0) * 4.0 / WIDTH;
            double cy = (y - HEIGHT/2.0) * 4.0 / WIDTH;

            complex<double> c(cx,cy);
            complex<double> z(0,0);

            int iter=0;

            while(abs(z)<2.0 && iter<MAX_ITER)
            {
                z = z*z + c;
                iter++;
            }

            image[y*WIDTH+x].r = iter % 256;
            image[y*WIDTH+x].g = (iter*5)%256;
            image[y*WIDTH+x].b = (iter*10)%256;
        }
    }

    auto fin = chrono::high_resolution_clock::now();

    chrono::duration<double> tiempo = fin - inicio;

    cout << "Tiempo: "
         << tiempo.count()
         << " segundos\n";

    return 0;
}

