# Msr.Thread

Msr.Thread just contains thread pool now, I will add more in the next few months.

## Msr.Thread.ThreadPool

###### A simple c++ thread pool based on boost::thread. (header only)

Tested on msvc-12.0, llvm-5.1(3.4svn)/clang-503.0.38 with boost 1.55.0(32/64bit).

There is a small bug in boost 1.55.0 on clang, which can be fixed with this:

https://github.com/boostorg/atomic/commit/6bb71fdd8f7cc346d90fb14beb38b7297fc1ffd9

(You can just remove all assignments to the variable of storage_type. I think it's safe to ignore memset initialization.)
