#ifndef VECTOR_H
#define VECTOR_H
#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <stdexcept>

using namespace std;

template <typename T>
class Vector {
 public:
  class ConstIterator;
  class Iterator;
  using value_type = T;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using iterator = Vector::Iterator;
  using const_iterator = Vector::ConstIterator;

 private:
  size_type max_sz;
  size_type sz;
  pointer values;

 public:
  Vector() {
    max_sz = 0;
    sz = 0;
    values = nullptr;
  }

  Vector(size_type n) {
    max_sz = n;
    values = new value_type[max_sz];
    sz = 0;
  }

  Vector(const initializer_list<value_type>& initializer_list) {
    sz = 0;
    max_sz = initializer_list.size();
    values = new value_type[max_sz];

    for (const auto& i : initializer_list) {
      values[sz++] = i;
    }
  }

  ~Vector(){
    delete[] values;
  }

  Vector(const Vector& src) {
    max_sz = src.max_sz;
    sz = src.sz;
    values = new value_type[max_sz];

    for (size_type i{0}; i < sz; i++) {
      values[i] = src.values[i];
    }
  }

  Vector& operator=(Vector src) {
    swap(sz, src.sz);
    swap(max_sz, src.max_sz);
    swap(values, src.values);
    return *this;
  }

  size_type size() const {
    return sz;
  }

  bool empty() const {
    return !sz;
  }

  void clear() {
    sz = 0;
  }

  void reserve(size_type n) {
    if (n > max_sz) {
      value_type* new_values = new value_type[n];

      for (size_type i{0}; i < sz; i++) {
        new_values[i] = values[i];
      }

      max_sz = n;
      delete[] values;
      values = new_values;
    }
  }

  void shrink_to_fit() {
    if (sz < max_sz) {
      value_type* new_values = new value_type[sz];

      for (size_type i{0}; i < sz; i++) {
        new_values[i] = values[i];
      }

      max_sz = sz;
      delete[] values;
      values = new_values;
    }
  }

  void push_back(const_reference val) {
    if (sz >= max_sz){
      reserve(2 * max_sz + 1);
    }
    values[sz++] = val;
  }

  void pop_back() {
    if (size() <= 0) {
      throw runtime_error("Vector is already empty!\n");
    }
    else {
      sz--;
    }
  }

  reference operator[](size_type index) {
    if (!(index >= 0 && index < sz)) {
      throw runtime_error("Index out of range!\n");
    }
    return values[index];
  }

  const_reference operator[](size_type index) const {
    if (!(index >= 0 && index < sz)) {
      throw runtime_error("Index out of range!\n");
    }
    return values[index];
  }

  size_type capacity() const{
    return max_sz;
  }

  ostream& print(ostream& o) const {
    bool first{true};

    o << "[";

    for (size_type i{0}; i < sz; i++) {
      if (first) {
        first = false;
      }
      else {
        o << ", ";
      }
      o << values[i];
    }
    
    o << "]";
    return o;
  }

  ostream& print(ostream& o) {
    for (size_type i{0}; i < sz; i++){
      o << values[i] << "\n";
    }
    return o;
  }

  iterator begin() {
    if (empty()) {
      return end();
    }
    return iterator(values, values + sz);
  }

  iterator end(){
    return iterator(values + sz, values + sz);
  }

  const_iterator begin() const {
    if (empty()) {
      return end();
    }
    return const_iterator(values, values + sz);
  }

  const_iterator end() const{
    return const_iterator(values + sz, values + sz);
  }

  iterator insert(const_iterator pos, const_reference val) {
    auto diff = pos - begin();

    if (diff < 0 || static_cast<size_type>(diff) > sz) {
      throw runtime_error("Iterator out of bounds");
    }
    
    size_type current{static_cast<size_type>(diff)};

    if (sz >= max_sz) {
      reserve((max_sz + 1) * 2);  // Achtung Sonderfall, wenn keine Mindesgröße definiert ist
    }

    for (auto i{sz}; i-- > current;) {
      values[i + 1] = values[i];
    }

    values[current] = val;
    ++sz;
    return iterator{values + current};
  }

  iterator erase(const_iterator pos) {
    auto diff = pos - begin();

    if (diff < 0 || static_cast<size_type>(diff) >= sz) {
      throw runtime_error("Iterator out of bounds");
    }

    size_type current{static_cast<size_type>(diff)};

    for (auto i{current}; i < sz - 1; ++i) {
      values[i] = values[i + 1];
    }

    --sz;
    return iterator{values + current};
  }

  class Iterator {
   public:
    using value_type = Vector::value_type;
    using reference = Vector::reference;
    using pointer = Vector::pointer;
    using difference_type = Vector::difference_type;
    using iterator_category = forward_iterator_tag;
    
   private:
    pointer ptr;
    pointer endptr;
    
   public:
    Iterator() {
      ptr = nullptr;
      endptr = nullptr;
    }

    Iterator(pointer ptr){
      this->ptr = ptr;
    }

    Iterator(pointer begin, pointer end) {
      this->ptr = begin;
      this->endptr = end;
    }

    reference operator*() {
      if (ptr == endptr) {
        throw runtime_error("Iterator Dereferenzierung nicht moeglich");
      }
      return (*ptr);
    }

    pointer operator->() {
      if (ptr == endptr) {
        throw runtime_error("Iterator Arrow-Operator nicht moeglich");
      }
      return ptr;
    }

    iterator& operator++() {
      if (ptr != endptr) {
        ptr++;
      }
      return *this;
    }

    iterator operator++(int) {
      iterator it = (*this);

      if (ptr != endptr) {
        ptr++;
      }
      return it;
    }

    operator const_iterator() const{
      return ConstIterator(this->ptr);
    }

    friend bool operator==(const Vector::Iterator& lop, const Vector::Iterator& rop) {
      return lop.ptr == rop.ptr;
    }

    friend bool operator!=(const Vector::Iterator& lop, const Vector::Iterator& rop) {
      return !(lop.ptr == rop.ptr);
    }
  };

  class ConstIterator {
   public:
    using value_type = Vector::value_type;
    using reference = Vector::const_reference;
    using pointer = Vector::const_pointer;
    using difference_type = Vector::difference_type;
    using iterator_category = forward_iterator_tag;

   private:
    const_pointer ptr;
    const_pointer endptr;

   public:
    ConstIterator() {
      ptr = nullptr;
      endptr = nullptr;
    }

    ConstIterator(pointer ptr){
      this->ptr = ptr;
    }

    ConstIterator(pointer begin, pointer end) {
      this->ptr = begin;
      this->endptr = end;
    }

    reference operator*() const {
      if (ptr == endptr) {
        throw runtime_error("ConstIterator Dereferenzierung nicht moeglich");
      }
      return (*ptr);
    }

    pointer operator->() const {
      if (ptr == endptr) {
        throw runtime_error("ConstIterator Arrow-Operator nicht moeglich");
      }
      return ptr;
    }

    const_iterator& operator++() {
      if (ptr != endptr) {
        ptr++;
      }
      return *this;
    }

    const_iterator operator++(int) {
      const_iterator it = (*this);

      if (ptr != endptr) {
        ptr++;
      }
      return it;
    }

    friend Vector::difference_type operator-(const Vector::ConstIterator& lop, const Vector::ConstIterator& rop) {
      return lop.ptr - rop.ptr;
    }

    friend bool operator==(const Vector::ConstIterator& lop, const Vector::ConstIterator& rop) {
      return lop.ptr == rop.ptr;
    }

    friend bool operator!=(const Vector::ConstIterator& lop, const Vector::ConstIterator& rop) {
      return !(lop.ptr == rop.ptr);
    }
  };
    
};

template <typename T>
ostream& operator<<(ostream& o, const Vector<T>& vec) {
  return vec.print(o);
}

#endif