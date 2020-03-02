#ifndef VECTOR_H
#define VECTOR_H

#include <cassert>
#include <stdexcept>

template <class T, class Alloc = std::allocator<T>>
class Vector;


template <class T, class Alloc>
bool operator==(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs) {
    if (&lhs == &rhs) {
        return true;
    } else if (lhs.size() != rhs.size()) {
        return false;
    }
    for (size_t i = 0; i < lhs.size(); ++i) {
        if (lhs[i] != rhs[i]) {
            return false;
        }
    }
    return true;
}

template <class T, class Alloc>
bool operator!=(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs) {
    return !(lhs == rhs);
}



template <class T, class Alloc>
class Vector {
public:
    explicit Vector(const Alloc& = Alloc());
    explicit Vector(size_t , const T& = T(), const Alloc& = Alloc());
    ~Vector();

    Vector(const Vector&);
    Vector(Vector&&) noexcept;
    Vector& operator=(const Vector&) &;
    Vector& operator=(Vector&&) & noexcept ;

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
    T* begin();
    T* end();
    T& front();
    T& back();

    const T& operator [](size_t ) const;
    const T& at(size_t ) const;
    const T* begin() const;
    const T* end() const;
    const T& front() const;
    const T& back() const ;

    bool empty() const;
    size_t capacity() const;
    size_t size() const;

    friend bool operator == <T, Alloc>(const Vector&, const Vector&);
    friend bool operator != <T, Alloc>(const Vector&, const Vector&);

private:
    size_t size_ = 0u, capacity_ = 0;
    Alloc alloc_ = Alloc();
    T* arr_ = nullptr;
    using traits = std::allocator_traits<Alloc>;
};


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
T* Vector<T, Alloc>::begin() {
    return arr_;
}
template<class T, class Alloc>
T* Vector<T, Alloc>::end() {
    return arr_ + size_;
}
template<class T, class Alloc>
const T& Vector<T, Alloc>::at(size_t ind) const {
    if (ind >= size_) {
        throw std::out_of_range("Accessing a nonexistent array element");
    }
    return arr_[ind];
}
template<class T, class Alloc>
T& Vector<T, Alloc>::front() {
    return arr_[0];
}
template<class T, class Alloc>
T& Vector<T, Alloc>::back() {
    return arr_[size_ - 1];
}


template<class T, class Alloc>
const T& Vector<T, Alloc>::operator[](size_t ind) const {
    return arr_[ind];
}
template<class T, class Alloc>
const T* Vector<T, Alloc>::begin() const {
    return arr_;
}
template<class T, class Alloc>
const T* Vector<T, Alloc>::end() const {
    return arr_ + size_;
}
template<class T, class Alloc>
const T& Vector<T, Alloc>::front() const {
    return arr_[0];
}
template<class T, class Alloc>
const T& Vector<T, Alloc>::back() const {
    return arr_[size_ - 1];
}
template<class T, class Alloc>
bool Vector<T, Alloc>::empty() const {
    return size_ == 0;
}
template<class T, class Alloc>
size_t Vector<T, Alloc>::capacity() const {
    return capacity_;
}
template<class T, class Alloc>
size_t Vector<T, Alloc>::size() const {
    return size_;
}

#endif //VECTOR_H