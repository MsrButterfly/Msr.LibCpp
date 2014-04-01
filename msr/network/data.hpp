#ifndef MSR_NETWORK_DATA_HPP_INCLUDED
#define MSR_NETWORK_DATA_HPP_INCLUDED

#include <array>
#include <vector>
#include <string>
#include <typeinfo>
#include <memory>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <msr/network/data_exception.hpp>
#include <boost/lexical_cast.hpp>

namespace msr {
    namespace network {
        class data {
        public:
            using self = data;
            using exception = data_exception;
            using unit_t = std::uint8_t;
            using size_t = std::size_t;
        public:
            data(): size_(0), data_(nullptr) {}
            data(const self &another): size_(another.size_) {
                allocate(data_, size_);
                copy(data_, another.data_, size_);
            }
            data(self &&another): size_(another.size_) {
                allocate(data_, size_);
                data_ = another.data_;
                another.size_ = 0;
                another.data_ = nullptr;
            }
            data(const size_t &size): size_(size) {
                allocate(data_, size_);
            }
            template <class T>
            data(const T &value): size_(sizeof(T) / sizeof(unit_t)) {
                allocate(data_, size_);
                copy(data_, &value, size_);
            }
            template <class T, size_t N>
            data(const T (&a)[N]): size_(sizeof(T) * N / sizeof(unit_t)) {
                allocate(data_, size_);
                copy(data_, a, size_);
            }
            template <class T>
            data(const T *a, const size_t &size): size_(sizeof(T) * size / sizeof(unit_t)) {
                allocate(data_, size_);
                copy(data_, a, size_);
            }
            template <class T, size_t N>
            data(const std::array<T, N> &a): size_(sizeof(T) * N / sizeof(unit_t)) {
                allocate(data_, size_);
                copy(data_, a.data(), size_);
            }
            template <class T, size_t N>
            data(const boost::array<T, N> &a): size_(sizeof(T) * N / sizeof(unit_t)) {
                allocate(data_, size_);
                copy(data_, a.c_array(), size_);
            }
            template <class T>
            data(const std::vector<T> &v): size_(sizeof(T) * v.size() / sizeof(unit_t)) {
                allocate(data_, size_);
                copy(data_, v.data(), size_);
            }
            template <class C>
            data(const std::basic_string<C> &s): size_(sizeof(C) * s.size() / sizeof(unit_t)) {
                allocate(data_, size_);
                copy(data_, s.data(), size_);
            }
        public:
            self &operator=(const self &another) {
                resize(another.size_);
                copy(data_, another.data_, size_);
                return *this;
            }
            self &operator=(self &&another) {
                deallocate(data_);
                size_ = another.size_;
                data_ = another.data_;
                another.size_ = 0;
                another.data_ = nullptr;
                return *this;
            }
            template <class T>
            self &operator=(const T &value) {
                resize(sizeof(T) / sizeof(unit_t));
                copy(data_, &value, size_);
                return *this;
            }
            template <class T, size_t N>
            self &operator=(const T (&a)[N]) {
                resize(sizeof(T) * N / sizeof(unit_t));
                copy(data_, a, size_);
                return *this;
            }
            template <class T, size_t N>\
            self &operator=(const std::array<T, N> &a) {
                resize(sizeof(T) * N / sizeof(unit_t));
                copy(data_, a.data(), size_);
                return *this;
            }
            template <class T, size_t N>
            self &operator=(const boost::array<T, N> &a) {
                resize(sizeof(T) * N / sizeof(unit_t));
                copy(data_, a.data(), size_);
                return *this;
            }
            template <class T>
            self &operator=(const std::vector<T> &v) {
                resize(sizeof(T) * v.size() / sizeof(unit_t));
                copy(data_, v.data(), size_);
                return *this;
            }
            template <class C>
            self &operator=(const std::basic_string<C> &s) {
                resize(sizeof(C) * s.size() / sizeof(unit_t));
                copy(data_, s.data(), size_);
                return *this;
            }
        public:
            const size_t &size() const {
                return size_;
            }
            void resize(const size_t &size) {
                size_ = size;
                reallocate(data_, size);
            }
            bool empty() const {
                return !data_;
            }
            void clear() {
                deallocate(data_);
            }
            template <class T>
            T get() const {
                auto t_size = sizeof(T) / sizeof(unit_t);
                if (t_size != size_) {
                    throw exception(std::string("get<T>(): Size not matched. (sizeof(T) = ") +
                                    boost::lexical_cast<std::string>(t_size) + ", size = " +
                                    boost::lexical_cast<std::string>(size_));
                }
                return *static_cast<T *>(static_cast<void *>(data_));
            }
            template <class T, size_t N>
            std::array<T, N> get() const {
                if (sizeof(T) * N / sizeof(unit_t) != size_) {
                    throw exception(std::string("get<T, N>(): Size not matched. (N = ") +
                    boost::lexical_cast<std::string>(N) + ", size = " +
                    boost::lexical_cast<std::string>(size_));
                }
                std::array<T, N> a;
                auto data__ = static_cast<T *>(static_cast<void *>(data_));
                for (size_t i = 0; i < N; ++i) {
                    a[i] = data__[i];
                }
                return a;
            }
            inline boost::asio::const_buffers_1 const_buffer() const {
                return boost::asio::buffer(const_cast<const unit_t *>(data_), size_);
            }
            inline boost::asio::mutable_buffers_1 mutable_buffer() const {
                return boost::asio::buffer(data_, size_);
            }
        public:
            ~data() {
                deallocate(data_);
            }
        private:
            static inline void allocate(unit_t *&data_, const size_t &size) {
                data_ = new unit_t[size];
            }
            static inline void deallocate(unit_t *&data_) {
                if (data_) {
                    delete []data_;
                    data_ = nullptr;
                }
            }
            static inline void reallocate(unit_t *&data_, const size_t &size) {
                deallocate(data_);
                allocate(data_, size);
            }
            static inline void copy(void *data_d, const void *data_s, size_t size) {
                std::memcpy(data_d, data_s, size);
            }
        private:
            unit_t *data_;
            size_t size_;
        };
    }
}

#endif

