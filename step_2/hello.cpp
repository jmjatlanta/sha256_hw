#include <CL/sycl.hpp>

static const int N = 8;

int verify_results(int* data)
{
    int expected[N] = {0, 2, 4, 6, 8, 10, 12, 14};
    for(int i = 0; i < N; ++i)
    {
        if (data[i] != expected[i])
        {
            std::cerr << "data[" << std::to_string(i) << "] != expected[" << std::to_string(i) << "]: "
                    << std::to_string(data[i]) << " != " << std::to_string(expected[i]) << "\n";
            return i;
        }
    }
    return -1;
}

int main(){
    // list possible devices:
    for (auto device : sycl::device::get_devices(sycl::info::device_type::all))
    {
        std::cout << "  Device: "
	        << device.get_info<sycl::info::device::name>()
	        << std::endl;
    }

    sycl::queue q;
    std::cout << "Device: " << q.get_device().get_info<sycl::info::device::name>() << std::endl;

    int *data = sycl::malloc_shared<int>(N, q);
    for(int i=0; i<N; i++) data[i] = i;

    q.parallel_for(sycl::range<1>(N), [=] (sycl::id<1> i){
        data[i] *= 2;
    }).wait();

    for(int i=0; i<N; i++)
        std::cout << data[i] << std::endl;

    if (verify_results(data) != -1)
        std::cerr << "Results invalid.\n";
    free(data, q);
    return 0;
}
