#include <iostream>
#include <fstream>
#include <vector>
#include <complex>
#include <chrono>
#include <omp.h>

using namespace std;

const int WIDTH = 7680;
const int HEIGHT = 4320;
const int MAX_ITER = 1000;

struct Pixel {
    unsigned char r, g, b;
};

int main() {

    auto inicio = chrono::high_resolution_clock::now();

    vector<Pixel> image(WIDTH * HEIGHT);

    #pragma omp parallel for schedule(static)
    for(int y = 0; y < HEIGHT; y++) {

        for(int x = 0; x < WIDTH; x++) {

            double cx = (x - WIDTH/2.0) * 4.0 / WIDTH;
            double cy = (y - HEIGHT/2.0) * 4.0 / WIDTH;

            complex<double> c(cx, cy);
            complex<double> z(0,0);

            int iter = 0;

            while(abs(z) < 2.0 && iter < MAX_ITER) {
                z = z*z + c;
                iter++;
            }

            Pixel p;

            if(iter == MAX_ITER) {
                p.r = p.g = p.b = 0;
            }
            else {
                p.r = iter % 256;
                p.g = (iter * 5) % 256;
                p.b = (iter * 10) % 256;
            }

            image[y*WIDTH+x] = p;
        }
    }

    auto fin = chrono::high_resolution_clock::now();

    chrono::duration<double> tiempo = fin - inicio;

    cout << "Tiempo OpenMP: "
         << tiempo.count()
         << " segundos\n";

    return 0;
}

