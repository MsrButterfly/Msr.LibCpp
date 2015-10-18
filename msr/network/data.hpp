#ifndef MSR_NETWORK_DATA_HPP_INCLUDED
#define MSR_NETWORK_DATA_HPP_INCLUDED

#include <array>
#include <cstdlib>
#include <string>
#include <vector>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <msr/memory.hpp>
#include <msr/utility.hpp>

namespace msr {
    namespace network {
        
        class data {
            MSR_CLASS_TYPE_DEFINATIONS(data);
        public:
            data();
            data(const self_type &another);
            data(self_type &&another);
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
            self_type &operator=(const self_type &another);
            self_type &operator=(self_type &&another);
            template <class T>
            self_type &operator=(const T &value);
            template <class T, size_t N>
            self_type &operator=(const T (&a)[N]);
            template <class T, size_t N>
            self_type &operator=(const std::array<T, N> &a);
            template <class T, size_t N>
            self_type &operator=(const boost::array<T, N> &a);
            template <class T>
            self_type &operator=(const std::vector<T> &v);
            template <class C>
            self_type &operator=(const std::basic_string<C> &s);
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
            static inline void allocate(byte *&data_, const size_t &size);
            static inline void deallocate(byte *&data_);
            static inline void reallocate(byte *&data_, const size_t &size);
            static inline void copy(void *data_d, const void *data_s, size_t size);
        private:
            byte *data_;
            size_t size_;
        };
        
    }
}

#include <msr/network/detail/data.ipp>

#endif
