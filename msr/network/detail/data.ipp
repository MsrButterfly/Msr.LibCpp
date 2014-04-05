#ifndef MSR_NETWORK_DETAIL_DATA_IPP_INCLUDED
#define MSR_NETWORK_DETAIL_DATA_IPP_INCLUDED

#include <boost/lexical_cast.hpp>

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
        data::data(const T &value): size_(sizeof(T) / sizeof(unit_t)) {
            allocate(data_, size_);
            copy(data_, &value, size_);
        }
        template <class T, size_t N>
        data::data(const T (&a)[N]): size_(sizeof(T) * N / sizeof(unit_t)) {
            allocate(data_, size_);
            copy(data_, a, size_);
        }
        template <class T>
        data::data(const T *a, const size_t &size): size_(sizeof(T) * size / sizeof(unit_t)) {
            allocate(data_, size_);
            copy(data_, a, size_);
        }
        template <class T, size_t N>
        data::data(const std::array<T, N> &a): size_(sizeof(T) * N / sizeof(unit_t)) {
            allocate(data_, size_);
            copy(data_, a.data(), size_);
        }
        template <class T, size_t N>
        data::data(const boost::array<T, N> &a): size_(sizeof(T) * N / sizeof(unit_t)) {
            allocate(data_, size_);
            copy(data_, a.c_array(), size_);
        }
        template <class T>
        data::data(const std::vector<T> &v): size_(sizeof(T) * v.size() / sizeof(unit_t)) {
            allocate(data_, size_);
            copy(data_, v.data(), size_);
        }
        template <class C>
        data::data(const std::basic_string<C> &s): size_(sizeof(C) * s.size() / sizeof(unit_t)) {
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
            resize(sizeof(T) / sizeof(unit_t));
            copy(data_, &value, size_);
            return *this;
        }
        template <class T, size_t N>
        data &data::operator=(const T (&a)[N]) {
            resize(sizeof(T) * N / sizeof(unit_t));
            copy(data_, a, size_);
            return *this;
        }
        template <class T, size_t N>\
        data &data::operator=(const std::array<T, N> &a) {
            resize(sizeof(T) * N / sizeof(unit_t));
            copy(data_, a.data(), size_);
            return *this;
        }
        template <class T, size_t N>
        data &data::operator=(const boost::array<T, N> &a) {
            resize(sizeof(T) * N / sizeof(unit_t));
            copy(data_, a.data(), size_);
            return *this;
        }
        template <class T>
        data &data::operator=(const std::vector<T> &v) {
            resize(sizeof(T) * v.size() / sizeof(unit_t));
            copy(data_, v.data(), size_);
            return *this;
        }
        template <class C>
        data &data::operator=(const std::basic_string<C> &s) {
            resize(sizeof(C) * s.size() / sizeof(unit_t));
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
            auto t_size = sizeof(T) / sizeof(unit_t);
            if (t_size != size_) {
                throw exception(1,
                                (std::string("get<T>(): Size not matched. (sizeof(T) = ") +
                                 boost::lexical_cast<std::string>(t_size) + ", size = " +
                                 boost::lexical_cast<std::string>(size_)).c_str());
            }
            return *static_cast<T *>(static_cast<void *>(data_));
        }
        template <class T, size_t N>
        std::array<T, N> data::get() const {
            if (sizeof(T) * N / sizeof(unit_t) != size_) {
                throw exception(2,
                                (std::string("get<T, N>(): Size not matched. (N = ") +
                                 boost::lexical_cast<std::string>(N) + ", size = " +
                                 boost::lexical_cast<std::string>(size_)).c_str());
            }
            std::array<T, N> a;
            auto data__ = static_cast<T *>(static_cast<void *>(data_));
            for (size_t i = 0; i < N; ++i) {
                a[i] = data__[i];
            }
            return a;
        }
        boost::asio::const_buffers_1 data::const_buffer() const {
            return boost::asio::buffer(const_cast<const unit_t *>(data_), size_);
        }
        boost::asio::mutable_buffers_1 data::mutable_buffer() const {
            return boost::asio::buffer(data_, size_);
        }
        data::~data() {
            deallocate(data_);
        }
        inline void data::allocate(unit_t *&data_, const size_t &size) {
            data_ = new unit_t[size];
        }
        inline void data::deallocate(unit_t *&data_) {
            if (data_) {
                delete []data_;
                data_ = nullptr;
            }
        }
        inline void data::reallocate(unit_t *&data_, const size_t &size) {
            deallocate(data_);
            allocate(data_, size);
        }
        inline void data::copy(void *data_d, const void *data_s, size_t size) {
            std::memcpy(data_d, data_s, size);
        }
    }
}

#endif
