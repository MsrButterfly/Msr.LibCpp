#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>
#include <msr/thread.hpp>

int main(int argc, const char *argv[]) {
    using namespace std;
    using namespace this_thread;
    using namespace chrono;
    using namespace msr;
    try {
        for (size_t i = 1; i <= 2 * thread::hardware_concurrency(); ++i) {
            thread_pool pool(i);
            auto begin = high_resolution_clock::now();
            for (size_t j = 0; j < 1000; ++j) {
                pool.post([]() {
                    for (size_t i = 0; i < 1000000; ++i);
                });
            }
            pool.wait();
            auto end = high_resolution_clock::now();
            auto duration = end - begin;
            printf("%zd threads: Finished in %lldms.\n", i, duration_cast<milliseconds>(duration).count());
        }
    } catch (reverse_control &e) {
        stringstream ss;
        ss << e;
        fprintf(stderr, "%s\n", ss.str().c_str());
    } catch (std::exception &e) {
        fprintf(stderr, "%s\n", e.what());
    }
    return EXIT_SUCCESS;
}
