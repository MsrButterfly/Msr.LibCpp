#include <msr/thread.hpp>

int main(int argc, const char *argv[]) {
    using namespace msr::thread;
    using namespace std::this_thread;
    using namespace std::chrono;
    thread_pool pool;
    try {
        auto begin = std::chrono::high_resolution_clock::now();
        for (std::size_t i = 0; i < 2000; i++) {
            pool.post([]() {
                for (unsigned int j = 0; j < 1000000; j++);
            });
        }
        pool.wait();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = end - begin;
        pool.stop();
        printf("Finished in %lldms.\n", std::chrono::duration_cast<milliseconds>(duration).count());
    } catch (thread_pool::exception &e) {
        fprintf(stderr, "%s\n", e.what());
    } catch (std::exception &e) {
        fprintf(stderr, "%s\n", e.what());
    }
    return EXIT_SUCCESS;
}
