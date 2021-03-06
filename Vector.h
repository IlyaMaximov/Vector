#ifndef VECTOR_H
#define VECTOR_H

#include <cassert>
#include <stdexcept>

template <class T, class Alloc = std::allocator<T>>
class Vector;


// Arithmetic operations for Implementing Vector
template <class T, class Alloc>
bool operator==(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs) {
    return lhs.compare(rhs) == 0;
}

template <class T, class Alloc>
bool operator!=(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs) {
    return !(lhs == rhs);
}

template <class T, class Alloc>
bool operator<(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs) {
    return lhs.compare(rhs) < 0;
}

template <class T, class Alloc>
bool operator<=(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs) {
    return lhs.compare(rhs) <= 0;
}

template <class T, class Alloc>
bool operator>(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs) {
    return rhs < lhs;
}

template <class T, class Alloc>
bool operator>=(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs) {
    return rhs <= lhs;
}


// Arithmetic operations for Bidirectional Iterator
template <class RandomIterator>
bool operator==(const RandomIterator& lhs, const RandomIterator& rhs) {
    return rhs.ptr_ == lhs.ptr_;
}

template <class RandomIterator>
bool operator!=(const RandomIterator& lhs, const RandomIterator& rhs) {
    return rhs.ptr_ != lhs.ptr_;
}


// Arithmetic operations for Random Access Iterator
template <class RandomIterator>
RandomIterator operator+(const RandomIterator& iter, const int offset) {
    return RandomIterator(iter.ptr_ + offset);
}

template <class RandomIterator>
RandomIterator operator+(const int offset, const RandomIterator& iter) {
    return RandomIterator(iter.ptr + offset);
}

template <class RandomIterator>
RandomIterator operator-(const RandomIterator& iter, const int offset) {
    return RandomIterator(iter.ptr_ - offset);
}

template <class RandomIterator>
int operator-(const RandomIterator& lhs, const RandomIterator& rhs) {
    return (lhs.ptr_ - rhs.ptr_);
}

template <class RandomIterator>
bool operator<(const RandomIterator& lhs, const RandomIterator& rhs) {
    return lhs.ptr_ < rhs.ptr_;
}

template <class RandomIterator>
bool operator>(const RandomIterator& lhs, const RandomIterator& rhs) {
    return rhs < lhs;
}

template <class RandomIterator>
bool operator<=(const RandomIterator& lhs, const RandomIterator& rhs) {
    return lhs.ptr_ <= rhs.ptr_;
}

template <class RandomIterator>
bool operator>=(const RandomIterator& lhs, const RandomIterator& rhs) {
    return rhs <= lhs;
}

//////////////////////////////////////////
//////////////////////////////////////////

template <class T, class Alloc>
class Vector {
public:
    template <bool is_const>
    class BaseIterator;

    using ConstIterator = BaseIterator<true>;
    using Iterator = BaseIterator<false>;


    explicit Vector(const Alloc& = Alloc());
    explicit Vector(size_t , const T& = T(), const Alloc& = Alloc());
    ~Vector();

    Vector(const Vector&);
    Vector(Vector&&) noexcept;
    Vector& operator=(const Vector&) &;
    Vector& operator=(Vector&&) & noexcept;

    void push_back(const T& );
    void push_back(T&& );
    template <class... Args>
    void emplace_back(Args&&... args);
    void pop_back();

    void clear();
    void reserve(size_t );
    void shrink_to_fit();
    void resize(size_t , const T& = T());

    T& operator [](size_t );
    T& at(size_t );
    Iterator begin() noexcept;
    Iterator end() noexcept;
    T& front() noexcept;
    T& back() noexcept;
    T* data() noexcept;

    const T& operator [](size_t ) const;
    const T& at(size_t ) const;
    ConstIterator cbegin() const noexcept;
    ConstIterator cend() const noexcept;
    ConstIterator begin() const noexcept;
    ConstIterator end() const noexcept;
    const T& front() const noexcept;
    const T& back() const noexcept;
    const T* data() const noexcept;

    bool empty() const noexcept;
    size_t capacity() const noexcept;
    size_t size() const noexcept;

    int compare(const Vector&) const;
    friend bool operator == <T, Alloc>(const Vector&, const Vector&);
    friend bool operator != <T, Alloc>(const Vector&, const Vector&);
    friend bool operator < <T, Alloc>(const Vector&, const Vector&);
    friend bool operator <= <T, Alloc>(const Vector&, const Vector&);
    friend bool operator > <T, Alloc>(const Vector&, const Vector&);
    friend bool operator >= <T, Alloc>(const Vector&, const Vector&);


    template <bool is_const>
    class BaseIterator: public std::iterator<std::random_access_iterator_tag,
                                             typename std::conditional<is_const, const T, T>::type> {
    public:
        BaseIterator() = default;
        BaseIterator(const BaseIterator&) = default;
        explicit BaseIterator(typename BaseIterator::pointer);
        BaseIterator& operator=(const BaseIterator&) & = default;
        ~BaseIterator() = default;

        BaseIterator& operator++() &;
        BaseIterator& operator--() &;
        BaseIterator operator++(int) &;
        BaseIterator operator--(int) &;


        typename BaseIterator::reference operator*();
        typename BaseIterator::pointer operator->();

        friend bool operator== <BaseIterator>(const BaseIterator&, const BaseIterator&);
        friend bool operator!= <BaseIterator>(const BaseIterator&, const BaseIterator&);

        // Еxclusively Random Iterator
        friend BaseIterator operator+ <BaseIterator>(const BaseIterator&, int offset);
        friend BaseIterator operator+ <BaseIterator>(int offset, const BaseIterator&);
        friend BaseIterator operator- <BaseIterator>(const BaseIterator&, int offset);
        friend int operator- <BaseIterator>(const BaseIterator&, const BaseIterator&);

        friend bool operator< <BaseIterator>(const BaseIterator&, const BaseIterator&);
        friend bool operator> <BaseIterator>(const BaseIterator&t, const BaseIterator&);
        friend bool operator<= <BaseIterator>(const BaseIterator&, const BaseIterator&);
        friend bool operator>= <BaseIterator>(const BaseIterator&, const BaseIterator&);

        BaseIterator& operator+= (int offset);
        BaseIterator& operator-= (int offset);

        typename BaseIterator::reference operator[] (int offset);

    private:
        typename BaseIterator::pointer ptr_ = nullptr;
    };


private:
    size_t size_ = 0u, capacity_ = 0;
    Alloc alloc_ = Alloc();
    T* arr_ = nullptr;
    using traits = std::allocator_traits<Alloc>;
};


//////////////////////////////////////////
//////////////////////////////////////////


template<class T, class Alloc>
template<bool is_const>
Vector<T, Alloc>::BaseIterator<is_const>::BaseIterator(typename BaseIterator<is_const>::pointer ptr) {
    ptr_ = ptr;
}

template<class T, class Alloc>
template<bool is_const>
typename Vector<T, Alloc>::template BaseIterator<is_const>&
        Vector<T, Alloc>::BaseIterator<is_const>::operator++() & {

    ++ptr_;
    return (*this);
}

template<class T, class Alloc>
template<bool is_const>
typename Vector<T, Alloc>::template BaseIterator<is_const>&
        Vector<T, Alloc>::BaseIterator<is_const>::operator--() & {

    --ptr_;
    return (*this);
}

template<class T, class Alloc>
template<bool is_const>
typename Vector<T, Alloc>::template BaseIterator<is_const>
        Vector<T, Alloc>::BaseIterator<is_const>::operator++(int) & {

    BaseIterator tmp(*this);
    ++ptr_;
    return tmp;
}

template<class T, class Alloc>
template<bool is_const>
typename Vector<T, Alloc>::template BaseIterator<is_const>
        Vector<T, Alloc>::BaseIterator<is_const>::operator--(int) & {

    BaseIterator tmp(*this);
    ++ptr_;
    return tmp;
}

template<class T, class Alloc>
template<bool is_const>
typename Vector<T, Alloc>::template BaseIterator<is_const>::reference
        Vector<T, Alloc>::BaseIterator<is_const>::operator*() {

    return *ptr_;
}

template<class T, class Alloc>
template<bool is_const>
typename Vector<T, Alloc>::template BaseIterator<is_const>::pointer
        Vector<T, Alloc>::BaseIterator<is_const>::operator->() {

    return ptr_;
}

template<class T, class Alloc>
template<bool is_const>
typename Vector<T, Alloc>::template BaseIterator<is_const>&
        Vector<T, Alloc>::BaseIterator<is_const>::operator+=(const int offset) {

    ptr_ += offset;
    return (*this);
}

template<class T, class Alloc>
template<bool is_const>
typename Vector<T, Alloc>::template BaseIterator<is_const>&
        Vector<T, Alloc>::BaseIterator<is_const>::operator-=(const int offset) {

    ptr_ -= offset;
    return (*this);
}

template<class T, class Alloc>
template<bool is_const>
typename Vector<T, Alloc>::template BaseIterator<is_const>::reference
        Vector<T, Alloc>::BaseIterator<is_const>::operator[](const int offset) {

    return *(ptr_ + offset);
}


//////////////////////////////////////////
//////////////////////////////////////////


template<class T, class Alloc>
Vector<T, Alloc>::Vector(const Alloc &init_alloc) :
    size_(0),
    capacity_(0),
    alloc_(init_alloc),
    arr_(nullptr) {}

template<class T, class Alloc>
Vector<T, Alloc>::Vector(size_t init_size, const T& init_value, const Alloc& init_alloc) :
    size_(init_size),
    capacity_(init_size),
    alloc_(init_alloc),
    arr_(traits::allocate(alloc_, capacity_)) {

    for (size_t i = 0; i < size_; ++i) {
        traits::construct(alloc_, arr_ + i, init_value);
    }
}

template<class T, class Alloc>
Vector<T, Alloc>::~Vector() {
    this->clear();
}

template<class T, class Alloc>
void Vector<T, Alloc>::clear() {
    for (size_t i = 0; i < size_; ++i) {
        traits::destroy(alloc_, arr_ + i);
    }
    traits::deallocate(alloc_, arr_, capacity_);

    arr_ = nullptr;
    size_ = capacity_ = 0;
}


template<class T, class Alloc>
Vector<T, Alloc>::Vector(const Vector& other_vector) :
    size_(other_vector.size_),
    capacity_(other_vector.capacity_),
    alloc_(traits::select_on_container_copy_construction(other_vector.alloc_)),
    arr_(traits::allocate(alloc_, capacity_)) {

    for (size_t i = 0; i < size_; ++i) {
        traits::construct(alloc_, arr_ + i, other_vector.arr_[i]);
    }
}

template<class T, class Alloc>
Vector<T, Alloc>::Vector(Vector&& other_vector) noexcept :
    size_(other_vector.size_),
    capacity_(other_vector.capacity_),
    alloc_(std::move(other_vector.alloc_)),
    arr_(other_vector.arr_) {

    other_vector.size_ = other_vector.capacity_ = 0;
    other_vector.arr_ = nullptr;
}

template<class T, class Alloc>
Vector<T, Alloc>& Vector<T, Alloc>::operator=(const Vector& other_vector) & {
    if (this != &other_vector) {
        for (size_t i = 0; i < size_; ++i) {
            traits::destroy(alloc_, arr_ + i);
        }
        bool alloc_copy_req = traits::propagate_on_container_copy_assignment::value;
        bool realloc_req = (capacity_ < other_vector.size_) ||
                (other_vector.size_ <= capacity_ / 4) || (alloc_copy_req && alloc_ != other_vector.alloc_);

        if (realloc_req) {
            traits::deallocate(alloc_, arr_, capacity_);
        }
        if (alloc_copy_req) {
            alloc_ = other_vector.alloc_;
        }
        if (realloc_req) {
            capacity_ = other_vector.size_;
            arr_ = traits::allocate(alloc_, capacity_);
        }

        size_ = other_vector.size_;
        for (size_t i = 0; i < size_; ++i) {
            traits::construct(alloc_, arr_ + i, other_vector[i]);
        }
    }
    return (*this);
}

template<class T, class Alloc>
Vector<T, Alloc>& Vector<T, Alloc>::operator=(Vector&& other_vector) & noexcept {
    if (this != &other_vector) {
        if (alloc_ != other_vector.alloc_ && !traits::propagate_on_container_move_assignment::value) {
            for (size_t i = 0; i < size_; ++i) {
                traits::destroy(alloc_, arr_ + i);
            }
            if (capacity_ < other_vector.size_ || other_vector.size_ <= capacity_ / 4) {
                traits::deallocate(alloc_, arr_, capacity_);
                arr_ = traits::allocate(alloc_, other_vector.size_);
                capacity_ = other_vector.size_;
            }
            size_ = other_vector.size_;
            for (size_t i = 0; i < size_; ++i) {
                traits::construct(alloc_, arr_ + i, std::move_if_noexcept(other_vector[i]));
            }
            other_vector.clear();
        } else {
            this->clear();
            if (traits::propagate_on_container_move_assignment::value) {
                alloc_ = std::move(other_vector.alloc_);
            }
            size_ = other_vector.size_;
            capacity_ = other_vector.capacity_;
            arr_ = other_vector.arr_;

            other_vector.size_ = other_vector.capacity_ = 0;
            other_vector.arr_ = nullptr;
        }
    }
    return (*this);
}



#define pushBack(method_argument_transmission) { \
    if (size_ < capacity_) { \
        traits::construct(alloc_, arr_ + size_, method_argument_transmission); \
    } else if (capacity_ == 0) { \
        ++capacity_; \
        arr_ = traits::allocate(alloc_, capacity_); \
        traits::construct(alloc_, arr_, method_argument_transmission); \
    } else { \
        assert(size_ == capacity_); \
        capacity_ *= 2; \
        T* new_arr = traits::allocate(alloc_, capacity_); \
        traits::construct(alloc_, new_arr + size_, method_argument_transmission); \
        for (size_t i = 0; i < size_; ++i) { \
            traits::construct(alloc_, new_arr + i, std::move_if_noexcept(arr_[i])); \
            traits::destroy(alloc_, arr_ + i); \
        } \
        traits::deallocate(alloc_, arr_, size_); \
        arr_ = new_arr; \
    } \
    ++size_; \
}

template<class T, class Alloc>
void Vector<T, Alloc>::push_back(const T& value) {
    pushBack(value)
}

template<class T, class Alloc>
void Vector<T, Alloc>::push_back(T&& value) {
    pushBack(std::move(value))
}

template<class T, class Alloc>
template<class... Args>
void Vector<T, Alloc>::emplace_back(Args &&... args) {
    pushBack(std::forward<Args>(args)...)
}

#undef pushBack


#define ReallockIf(condition, new_capacity) { \
    if (condition) { \
        T* new_arr = traits::allocate(alloc_, new_capacity); \
        for (size_t i = 0; i < size_; ++i) { \
            traits::construct(alloc_, new_arr + i, std::move_if_noexcept(arr_[i])); \
            traits::destroy(alloc_, arr_ + i); \
        } \
        traits::deallocate(alloc_, arr_, capacity_); \
        arr_ = new_arr; \
        capacity_ = new_capacity; \
    } \
}

template<class T, class Alloc>
void Vector<T, Alloc>::pop_back() {
    if (this->empty()) {
        throw std::logic_error("deleting from empty array");
    }
    --size_;
    traits::destroy(alloc_, arr_ + size_);
    if (this->empty()) {
        this->clear();
    } else {
        ReallockIf(size_ <= capacity_ / 4, capacity_ / 2)
    }
}

template<class T, class Alloc>
void Vector<T, Alloc>::reserve(size_t new_capacity) {
    ReallockIf(new_capacity > capacity_, new_capacity)
}

template<class T, class Alloc>
void Vector<T, Alloc>::shrink_to_fit() {
    ReallockIf(size_ != capacity_, size_)
}

template<class T, class Alloc>
void Vector<T, Alloc>::resize(size_t new_size, const T& value) {
    if (new_size > size_) {
        this->reserve(new_size);
        while (size_ < new_size) {
            this->push_back(value);
        }
    } else if (new_size < size_) {
        for (size_t i = new_size; i < size_; ++i) {
            traits::destroy(alloc_, arr_ + i);
        }
        size_ = new_size;
        if (size_ <= capacity_ / 4) {
            this->shrink_to_fit();
        }
    }
}

#undef ReallockIf


template<class T, class Alloc>
T& Vector<T, Alloc>::operator[](size_t ind) {
    return arr_[ind];
}
template<class T, class Alloc>
T& Vector<T, Alloc>::at(size_t ind) {
    if (ind >= size_) {
        throw std::out_of_range("Accessing a nonexistent array element");
    }
    return arr_[ind];
}
template<class T, class Alloc>
typename Vector<T, Alloc>::Iterator Vector<T, Alloc>::begin() noexcept {
    return Iterator(arr_);
}
template<class T, class Alloc>
typename Vector<T, Alloc>::Iterator Vector<T, Alloc>::end() noexcept {
    return Iterator(arr_ + size_);
}
template<class T, class Alloc>
const T& Vector<T, Alloc>::at(size_t ind) const {
    if (ind >= size_) {
        throw std::out_of_range("Accessing a nonexistent array element");
    }
    return arr_[ind];
}
template<class T, class Alloc>
T& Vector<T, Alloc>::front() noexcept {
    return arr_[0];
}
template<class T, class Alloc>
T& Vector<T, Alloc>::back() noexcept {
    return arr_[size_ - 1];
}
template<class T, class Alloc>
T *Vector<T, Alloc>::data() noexcept {
    return arr_;
}


template<class T, class Alloc>
const T& Vector<T, Alloc>::operator[](size_t ind) const {
    return arr_[ind];
}
template<class T, class Alloc>
typename Vector<T, Alloc>::ConstIterator Vector<T, Alloc>::cbegin() const noexcept {
    return ConstIterator(arr_);
}
template<class T, class Alloc>
typename Vector<T, Alloc>::ConstIterator Vector<T, Alloc>::cend() const noexcept {
    return ConstIterator(arr_ + size_);
}
template<class T, class Alloc>
typename Vector<T, Alloc>::ConstIterator Vector<T, Alloc>::begin() const noexcept {
    return cbegin();
}
template<class T, class Alloc>
typename Vector<T, Alloc>::ConstIterator Vector<T, Alloc>::end() const noexcept {
    return cend();
}
template<class T, class Alloc>
const T& Vector<T, Alloc>::front() const noexcept {
    return arr_[0];
}
template<class T, class Alloc>
const T& Vector<T, Alloc>::back() const noexcept {
    return arr_[size_ - 1];
}
template<class T, class Alloc>
const T* Vector<T, Alloc>::data() const noexcept {
    return arr_;
}

template<class T, class Alloc>
bool Vector<T, Alloc>::empty() const noexcept {
    return size_ == 0;
}
template<class T, class Alloc>
size_t Vector<T, Alloc>::capacity() const noexcept {
    return capacity_;
}
template<class T, class Alloc>
size_t Vector<T, Alloc>::size() const noexcept {
    return size_;
}

template<class T, class Alloc>
int Vector<T, Alloc>::compare(const Vector& rhs) const {
    const Vector& lhs = *this;

    if (&lhs == &rhs) {
        return 0;
    }
    for (size_t i = 0; i < std::min(lhs.size(), rhs.size()); ++i) {
        if (lhs[i] < rhs[i]) {
            return -1;
        } else if (lhs[i] > rhs[i]) {
            return 1;
        }
    }
    return lhs.size() - static_cast<int>(rhs.size());
}


#endif //VECTOR_H