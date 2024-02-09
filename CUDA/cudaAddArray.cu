#include <thrust/device_vector.h>
#include <thrust/for_each.h>
#include <thrust/iterator/counting_iterator.h>
#include <thrust/async/copy.h>
#include <thrust/async/transform.h>
#include <thrust/async/for_each.h>

#include "poTimer.h"

__global__ void vector_add(thrust::device_ptr<float> A, thrust::device_ptr<float> B, thrust::device_ptr<float> C , int N)
{
    int id = threadIdx.x + blockDim.x *blockIdx.x;
    if(id >= N)return;
    C[id] = A[id] + B[id];
}

int main(int argc, char** argv)
{
    int N = 10000000;
    thrust::device_vector<float> A(N,10.0f);
    std::cout << A[0] << std::endl;
    thrust::device_vector<float> B(N,20.0f);
    thrust::device_vector<float> C(N);

    std::vector<float> hA(N,10.0f);
    std::vector<float> hB(N,20.0f);
    std::vector<float> hC(N);

    auto a = A.data();
    auto b = B.data();
    auto c = C.data();
    {
        po::simpleTimer timer("for_each");
        for(int i = 0; i < 1000 ;i++)
        {
            thrust::for_each(thrust::counting_iterator<int>(0),thrust::counting_iterator<int>(N),[a,b,c]__device__(int id)
            {
                c[id] = a[id] + b[id];
                // C[id] = A[id] + B[id];
            }
            );
        }
        std::cout << "Out" << std::endl;

    }
    thrust::device_event e(thrust::new_stream);
    {
        po::simpleTimer timer("for_each_async");

        for(int i = 0; i < 1000; i++)
        {
            e = thrust::async::for_each(
                thrust::device.after(e),thrust::counting_iterator<int>(0),thrust::counting_iterator<int>(N),
                [a,b,c]__device__(int id){
                    c[id] = a[id] + b[id];
                }
            );
        }
        e.wait();
        std::cout << "Out" << std::endl;
    }

    {
        po::simpleTimer timer("global");
        int nThread = 512;
        int nBlocks = N/nThread + 1;
        vector_add<<<nBlocks,nThread>>>(a,b,c,N);
        std::cout << "Out" << std::endl;
        cudaDeviceSynchronize();
    }

    {
        po::simpleTimer timer("CPU 1core");
            for(int i = 0; i < N;i++)
            {
                hC[i] = hA[i] + hB[i];
            }
    }

}