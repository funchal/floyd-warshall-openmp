#include <iostream>
#include <random>
#include <chrono>

#include <omp.h>

// https://en.wikipedia.org/wiki/Floyd%E2%80%93Warshall_algorithm
// mat=square 2d matrix n x n
// assumes concurrent writes to neighbouring elements in mat is safe
// threads=0 means automatic number of threads
template<typename T>
void floydwarshall(T* mat, size_t n, int threads = 0)
{
   for (int k = 0; k < n; ++k) {
#pragma omp parallel for schedule(static) num_threads(threads)
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                T val = mat[i*n + k] + mat[k*n+j];
                if (mat[i*n + j] > val) {
                    mat[i*n + j] = val;
                }
            }
        }
    }
}

class stopwatch {
    typedef std::chrono::steady_clock clock;
    clock::time_point start;
public:
    stopwatch()
        : start(clock::now())
    {}

    ~stopwatch()
    {
        auto end = std::chrono::steady_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us";
    }
};

void test(size_t n)
{
    float* mat = new float[n*n];
    float* ref = new float[n*n];
    std::mt19937 gen;
    std::uniform_real_distribution<float> dis(0, 1);
    for (int i = 0; i != n*n; ++i) {
        ref[i] = dis(gen);
    }

    std::cout << "matrix " << n << "x" << n << std::endl;
    int max_threads = omp_get_max_threads() * 2;
    for (int threads = 0; threads <= max_threads; ++threads)
    {
        std::cout << threads << " threads" << std::endl;
        for (int repeat = 0; repeat != 4; ++repeat)
        {
            for (int i = 0; i != n*n; ++i) {
                mat[i] = ref[i];
            }

            {
                stopwatch s;
                floydwarshall(mat, n, threads);
            }

            float t = 0;
            for (int i = 0; i != n*n; ++i) {
                t += i*mat[i] / (n*n);
            }
            std::cout << " checksum: " << t << std::endl;
        }
    }

    delete[] mat;
    delete[] ref;
}

int main(int, char*[])
{
    test(100);
    test(200);
    test(1000);
    return 0;
}
