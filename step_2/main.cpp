#include "uint256.h"

#include <CL/sycl.hpp>
#ifdef __CPU_OPTIMIZED__
#include "crypto/sha256.h"
#endif

#include <vector>
#include <iostream>
#include <chrono>

uint256 GenerateHashFromInt(uint16_t in)
{
#ifdef __CPU_OPTIMIZED__
    CSHA256 hasher{};
    hasher.Write((unsigned char*)&in, 2);
    unsigned char hash[hasher.OUTPUT_SIZE];
    hasher.Finalize(&hash[0]);
    std::vector<unsigned char> bytes{&hash[0], &hash[0]+hasher.OUTPUT_SIZE};
    return uint256{bytes};
#else
    // do the sha256 stuff on the GPU
#endif
}

int main(int argc, char** argv)
{
#ifdef __CPU_OPTIMIZED__
    std::cout << "Device: Plain old CPU\n";
#else
    sycl::queue q;
    std::cout << "Device: " << q.get_device().get_info<sycl::info::device::name>() << std::endl;
#endif

    std::vector<uint256> ids;
    auto begin = std::chrono::high_resolution_clock::now();
    for(uint16_t i = 0; i < 12800; ++i)
    {
        ids.push_back(GenerateHashFromInt(i));
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
    std::cout << duration << "ns total, average : " << duration / 12800 << "ns." << std::endl;
}

