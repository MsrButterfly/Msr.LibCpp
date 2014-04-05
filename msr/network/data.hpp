#ifndef MSR_NETWORK_DATA_HPP_INCLUDED
#define MSR_NETWORK_DATA_HPP_INCLUDED

#include <array>
#include <cstdlib>
#include <string>
#include <vector>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <msr/network/data_exception.hpp>

namespace msr {
    namespace network {
        class data {
        public:
            using self = data;
            using exception = data_exception;
            using unit_t = std::uint8_t;
            using size_t = std::size_t;
        public:
            data();
            data(const self &another);
            data(self &&another);
            data(const size_t &size);
            template <class T>
            data(const T &value);
            template <class T, size_t N>
            data(const T (&a)[N]);
            template <class T>
            data(const T *a, const size_t &size);
            template <class T, size_t N>
            data(const std::array<T, N> &a);
            template <class T, size_t N>
            data(const boost::array<T, N> &a);
            template <class T>
            data(const std::vector<T> &v);
            template <class C>
            data(const std::basic_string<C> &s);
        public:
            self &operator=(const self &another);
            self &operator=(self &&another);
            template <class T>
            self &operator=(const T &value);
            template <class T, size_t N>
            self &operator=(const T (&a)[N]);
            template <class T, size_t N>
            self &operator=(const std::array<T, N> &a);
            template <class T, size_t N>
            self &operator=(const boost::array<T, N> &a);
            template <class T>
            self &operator=(const std::vector<T> &v);
            template <class C>
            self &operator=(const std::basic_string<C> &s);
        public:
            const size_t &size() const;
            void resize(const size_t &size);
            bool empty() const;
            void clear();
            template <class T>
            T get() const;
            template <class T, size_t N>
            std::array<T, N> get() const;
            boost::asio::const_buffers_1 const_buffer() const;
            boost::asio::mutable_buffers_1 mutable_buffer() const;
        public:
            ~data();
        private:
            static inline void allocate(unit_t *&data_, const size_t &size);
            static inline void deallocate(unit_t *&data_);
            static inline void reallocate(unit_t *&data_, const size_t &size);
            static inline void copy(void *data_d, const void *data_s, size_t size);
        private:
            unit_t *data_;
            size_t size_;
        };
    }
}

#include <msr/network/detail/data.ipp>

#endif

