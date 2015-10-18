#ifndef MSR_NETWORK_DETAIL_DATA_IPP_INCLUDED
#define MSR_NETWORK_DETAIL_DATA_IPP_INCLUDED

#include <string>
#include <msr/exception.hpp>
#include <msr/utility.hpp>

namespace msr {
    namespace network {
    
        data::data(): size_(0), data_(nullptr) {}
        
        data::data(const data &another): size_(another.size_) {
            allocate(data_, size_);
            copy(data_, another.data_, size_);
        }
        
        data::data(data &&another): size_(another.size_) {
            allocate(data_, size_);
            data_ = another.data_;
            another.size_ = 0;
            another.data_ = nullptr;
        }
        
        data::data(const size_t &size): size_(size) {
            allocate(data_, size_);
        }
        
        template <class T>
        data::data(const T &value): size_(sizeof(T)) {
            allocate(data_, size_);
            copy(data_, &value, size_);
        }
        
        template <class T, size_t N>
        data::data(const T (&a)[N]): size_(sizeof(T) * N) {
            allocate(data_, size_);
            copy(data_, a, size_);
        }
        
        template <class T>
        data::data(const T *a, const size_t &size): size_(sizeof(T) * size) {
            allocate(data_, size_);
            copy(data_, a, size_);
        }
        
        template <class T, size_t N>
        data::data(const std::array<T, N> &a): size_(sizeof(T) * N) {
            allocate(data_, size_);
            copy(data_, a.data(), size_);
        }
        
        template <class T, size_t N>
        data::data(const boost::array<T, N> &a): size_(sizeof(T) * N) {
            allocate(data_, size_);
            copy(data_, a.c_array(), size_);
        }
        
        template <class T>
        data::data(const std::vector<T> &v): size_(sizeof(T) * v.size()) {
            allocate(data_, size_);
            copy(data_, v.data(), size_);
        }
        
        template <class C>
        data::data(const std::basic_string<C> &s): size_(sizeof(C) * s.size()) {
            allocate(data_, size_);
            copy(data_, s.data(), size_);
        }
        
        data &data::operator=(const data &another) {
            resize(another.size_);
            copy(data_, another.data_, size_);
            return *this;
        }
        
        data &data::operator=(data &&another) {
            deallocate(data_);
            size_ = another.size_;
            data_ = another.data_;
            another.size_ = 0;
            another.data_ = nullptr;
            return *this;
        }
        
        template <class T>
        data &data::operator=(const T &value) {
            resize(sizeof(T));
            copy(data_, &value, size_);
            return *this;
        }
        
        template <class T, size_t N>
        data &data::operator=(const T (&a)[N]) {
            resize(sizeof(T) * N);
            copy(data_, a, size_);
            return *this;
        }
        
        template <class T, size_t N>\
        data &data::operator=(const std::array<T, N> &a) {
            resize(sizeof(T) * N);
            copy(data_, a.data(), size_);
            return *this;
        }
        
        template <class T, size_t N>
        data &data::operator=(const boost::array<T, N> &a) {
            resize(sizeof(T) * N);
            copy(data_, a.data(), size_);
            return *this;
        }
        
        template <class T>
        data &data::operator=(const std::vector<T> &v) {
            resize(sizeof(T) * v.size());
            copy(data_, v.data(), size_);
            return *this;
        }
        
        template <class C>
        data &data::operator=(const std::basic_string<C> &s) {
            resize(sizeof(C) * s.size());
            copy(data_, s.data(), size_);
            return *this;
        }
        
        const size_t &data::size() const {
            return size_;
        }
        
        void data::resize(const size_t &size) {
            size_ = size;
            reallocate(data_, size);
        }
        
        bool data::empty() const {
            return !data_;
        }
        
        void data::clear() {
            deallocate(data_);
        }
        
        template <class T>
        T data::get() const {
            using namespace std;
            if (sizeof(T) != size_) {
                throw type_mismatch(MSR_FUNCTION_SIGNATURE,
                    string("sizeof(") + typeid(T).name() + ") = " +
                    to_string(sizeof(T)) + ", size() = " + to_string(size_));
            }
            return *static_cast<T *>(static_cast<void *>(data_));
        }
        
        template <class T, size_t N>
        std::array<T, N> data::get() const {
            using namespace std;
            if (sizeof(T) * N != size_) {
                throw type_mismatch(MSR_FUNCTION_SIGNATURE,
                    string("sizeof(") + typeid(T).name() + ") * " +
                    to_string(N) + " = " + to_string(sizeof(T)) +
                    ", size() = " + to_string(size_));
            }
            std::array<T, N> a;
            auto data__ = static_cast<T *>(static_cast<void *>(data_));
            for (size_t i = 0; i < N; ++i) {
                a[i] = data__[i];
            }
            return a;
        }
        
        boost::asio::const_buffers_1 data::const_buffer() const {
            return boost::asio::buffer(const_cast<const byte *>(data_), size_);
        }
        
        boost::asio::mutable_buffers_1 data::mutable_buffer() const {
            return boost::asio::buffer(data_, size_);
        }
        
        data::~data() {
            deallocate(data_);
        }
        
        inline void data::allocate(byte *&data_, const size_t &size) {
            data_ = new byte[size];
        }
        
        inline void data::deallocate(byte *&data_) {
            if (data_) {
                delete []data_;
                data_ = nullptr;
            }
        }
        
        inline void data::reallocate(byte *&data_, const size_t &size) {
            deallocate(data_);
            allocate(data_, size);
        }
        
        inline void data::copy(void *data_d, const void *data_s, size_t size) {
            std::memcpy(data_d, data_s, size);
        }
        
    }
}

#endif
