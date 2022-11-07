#include "uint256.h"

#include <CL/sycl.hpp>
#include "sycl_hash.hpp"
#include "tools/sycl_queue_helpers.hpp"
#include "crypto/sha256.h"
#include "hash_functions/sha256.hpp"
#include <vector>
#include <iostream>
#include <chrono>

template<typename T>
unsigned char* to_array(T in, unsigned char* out)
{
    memcpy(out, &in, sizeof(T));
    return out;
}
/***
 * @brief generate a simple hash based on an incoming int
 * @note: just used to generate fake ids, uses CPU only
 * @return the id
 */
template<typename T>
uint256 GenerateHashCPU(T in)
{
    CSHA256 hasher{};
    unsigned char arr[sizeof(T)];
    to_array(in, arr);
    hasher.Write(arr, sizeof(T));
    unsigned char hash[hasher.OUTPUT_SIZE];
    hasher.Finalize(&hash[0]);
    std::vector<unsigned char> bytes{&hash[0], &hash[0]+hasher.OUTPUT_SIZE};
    return uint256{bytes};
}

bool compare_collections(const std::vector<uint256>& a, const std::vector<uint256>& b)
{
    for( size_t i = 0; i < a.size(); ++i)
    {
        if ( a[i] != b[i] )
        {
            std::cerr << "Mismatch at element " << std::to_string(i) << ": " 
                    << a[i].GetHex() << " vs " << b[i].GetHex() << "\n";
            return false;
        }
    }
    return true;
}

uint256 arr_to_uint256(unsigned char* in)
{
    CSHA256 hasher{};
    std::vector<unsigned char> vec{in, in + hasher.OUTPUT_SIZE};
    return uint256{vec};
}

void compare(const std::string& prefix, uint256 lhs, uint256 rhs)
{
    std::string result = " == ";
    if (lhs != rhs)
        result = " != ";
    std::cout << prefix << lhs.GetHex() << result << rhs.GetHex() << "\n";
}

/***
 * Verify that hashing works as we expect
 */
void test_sha256()
{
    // test the non-optimized version
    uint256 expected = uint256S("6f988221368a68284b0af7c781dd9131a5c39e80906267afb0d52bbb2160fddf");
    CSHA256 hasher{};
    const char* str = "Hello, World!";
    hasher.Write((const unsigned char*)&str[0], strlen(str));
    unsigned char hash[hasher.OUTPUT_SIZE];
    hasher.Finalize(&hash[0]);
    uint256 result = arr_to_uint256(hash);
    compare("normal ", result, expected);
}

/***
 * Verify that hashing works as we expect
 */
void test_sha256_compute()
{
    uint256 expected = uint256S("6f988221368a68284b0af7c781dd9131a5c39e80906267afb0d52bbb2160fddf");
    sycl::queue q;

    constexpr int hash_size = 32;
    const char* input = "Hello, World!";
    int item_size = strlen(input);

    unsigned char output[hash_size]; // reserve space for the output

    hash::compute<hash::method::sha256>(q, (const unsigned char*)input, item_size, output, 1);

    uint256 result = arr_to_uint256(output);
    compare("compute ", result, expected);
}

/**
 * With a collection of needed "nonces", hash the data, then compare if below the threshold
 */
int main(int argc, char** argv)
{
    //test_sha256();
    //test_sha256_compute();
    sycl::queue q;
    std::cout << "Device: " << q.get_device().get_info<sycl::info::device::name>() << std::endl;
    static uint16_t N = 12800;

    std::vector<uint256> ids;
    auto begin = std::chrono::high_resolution_clock::now();
    for(uint32_t i = 0; i < N; ++i)
    {
        ids.push_back(GenerateHashCPU(i));
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
    std::cout << "Building CPU IDs: " << duration << "ns total, average : " << duration / 12800 << "ns." << std::endl;

    ////////
    // Test 1: Do the same thing as above, but with SYCL
    ////////

    // make an array of 4 byte integers
    char inputs[N][4];
    for(uint32_t i = 0; i < N; ++i)
    {
        to_array(i, (unsigned char*)inputs[i]);
    }
    unsigned char output[N][32];
    // make the call
    begin = std::chrono::high_resolution_clock::now();
    hash::compute<hash::method::sha256>(q, (unsigned char*)inputs, 4, (unsigned char*)output, N);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
    std::cout << "Building GPU IDs: " << duration << "ns total, average : " << duration / 12800 << "ns." << std::endl;

    // the two collections should be the same
    std::vector<uint256> gpuids;
    for(int i = 0; i < N; ++i)
    {
        uint256 new_key = arr_to_uint256(output[i]);
        gpuids.push_back( new_key );
    }

    if(compare_collections(ids, gpuids))
        std::cout << "Collections match\n";
    else
        std::cout << "Collections do not match.\n";
}

