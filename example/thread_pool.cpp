#include <msr/thread.hpp>

int main(int argc, const char *argv[]) {
    using namespace msr;
    using namespace std::this_thread;
    using namespace std::chrono;
    try {
        for (std::size_t i = 1; i <= 2 * msr::thread::hardware_concurrency(); ++i) {
            thread_pool pool(i);
            auto begin = std::chrono::high_resolution_clock::now();
            for (std::size_t j = 0; j < 1000; ++j) {
                pool.post([]() {
                    for (std::size_t i = 0; i < 1000000; ++i);
                });
            }
            pool.wait();
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = end - begin;
            printf("%lu threads: Finished in %ldms.\n", i, std::chrono::duration_cast<milliseconds>(duration).count());
        }
    } catch (thread_pool::exception &e) {
        fprintf(stderr, "%s\n", e.what());
    } catch (std::exception &e) {
        fprintf(stderr, "%s\n", e.what());
    }
    return EXIT_SUCCESS;
}
