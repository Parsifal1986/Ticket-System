#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "../Exceptions/exceptions.hpp"

namespace sjtu 
{
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector
{
public:
	/**
	 * TODO
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
	class const_iterator;
	class iterator 
	{
	// The following code is written for the C++ type_traits library.
	// Type traits is a C++ feature for describing certain properties of a type.
	// For instance, for an iterator, iterator::value_type is the type that the 
	// iterator points to. 
	// STL algorithms and containers may use these type_traits (e.g. the following 
	// typedef) to work properly. In particular, without the following code, 
	// @code{std::sort(iter, iter1);} would not compile.
	// See these websites for more information:
	// https://en.cppreference.com/w/cpp/header/type_traits
	// About value_type: https://blog.csdn.net/u014299153/article/details/72419713
	// About iterator_category: https://en.cppreference.com/w/cpp/iterator
	friend vector;
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		 int p;
		 void *v;
		 int size;
	public:
		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		iterator(int pos, void *vec, int used_size) {
			v = vec;
			p = pos;
			size = used_size;
		}

		iterator(iterator &other) {
			p = other.p;
			v = other.v;
			size = other.size;
		}

		iterator operator+(const int &n) const 
		{
			return iterator(p + n, v, size);
		}
		iterator operator-(const int &n) const 
		{
			return iterator(p - n, v, size);
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const 
		{
			if (v != rhs.v) {
				throw invalid_iterator();
			} else {
				return p - rhs.p;
			}
		}
		iterator& operator+=(const int &n) 
		{
			p += n;
			return *this;
		}
		iterator& operator-=(const int &n) 
		{
			p -= n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			iterator tmp(*this);
			++p;
			return tmp;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
			return (*this += 1);
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			iterator tmp(*this);
			--p;
			return tmp;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
			return (*this -= 1);
		}
		/**
		 * TODO *it
		 */
		T& operator*() const{
			if (p >= size || p < 0) {
				throw invalid_iterator();
			}
			return ((T*)v)[p];
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {
			return (rhs.v == v) && (rhs.p == p);
		}
		bool operator==(const const_iterator &rhs) const {
			return (rhs.v == v) && (rhs.p) == p;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return !(rhs == *this);
		}
		bool operator!=(const const_iterator &rhs) const {
			return !(rhs == *this);
		}
	};
	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator 
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;
		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		const_iterator(int pos, void *vec, int used_size) {
			v = vec;
			p = pos;
			size = used_size;
		}

		const_iterator(const_iterator &other) {
			p = other.p;
			v = other.v;
			size = other.size;
		}

		const_iterator operator+(const int &n) const 
		{
			return iterator(p + n, v, size);
		}
		const_iterator operator-(const int &n) const 
		{
			return iterator(p - n, v, size);
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const const_iterator &rhs) const 
		{
			if (v != rhs.v) {
				throw invalid_iterator();
			} else {
				return p - rhs.p;
			}
		}
		const_iterator& operator+=(const int &n) 
		{
			p += n;
			return *this;
		}
		const_iterator& operator-=(const int &n) 
		{
			p -= n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		const_iterator operator++(int) {
			const_iterator tmp(*this);
			++p;
			return tmp;
		}
		/**
		 * TODO ++iter
		 */
		const_iterator& operator++() {
			return (*this += 1);
		}
		/**
		 * TODO iter--
		 */
		const_iterator operator--(int) {
			const_iterator tmp(*this);
			--p;
			return tmp;
		}
		/**
		 * TODO --iter
		 */
		const_iterator& operator--() {
			return (*this -= 1);
		}
		/**
		 * TODO *it
		 */
		const T& operator*() const{
			if (p > size) {
				throw invalid_iterator();
			}
			return ((T*)v)[p];
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {
			return (rhs.v == v) && (rhs.p == p);
		}
		bool operator==(const const_iterator &rhs) const {
			return (rhs.v == v) && (rhs.p) == p;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return !(rhs == *this);
		}
		bool operator!=(const const_iterator &rhs) const {
			return !(rhs == *this);
		}
	
	private:
		/*TODO*/
		int p;
		void *v;
		int size;
	};
	/**
	 * TODO Constructs
	 * At least two: default constructor, copy constructor
	 */
	vector() {
		data = operator new [] (sizeof(T) * 2);
		used_size = 0;
		total_size = 2;
	}

	vector(const vector &other) {
		data = new T[other.used_size];
		for (int i = 0; i < other.used_size; i++) {
			((T*)data)[i] = ((T*)other.data)[i];
		}
		total_size = other.total_size;
		used_size = other.used_size;
	}
	/**
	 * TODO Destructor
	 */
	~vector() {
		Delete();
		// delete [] (T*)data;
		// ::operator delete [] (data);
	}
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other) {
		if (other.data == data) {
			return *this;
		}
		Delete();
		// delete [] (T*)data;
		// ::operator delete [] (data);
		data = new T[other.used_size];
		for (int i = 0; i < other.used_size; i++) {
			*((T*)data + i) = *((T*)other.data + i);
		}
		total_size = other.total_size;
		used_size = other.used_size;
		return *this;
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos) {
		if (pos >= used_size || pos < 0) {
			throw index_out_of_bound();
		} else {
			return ((T*)data)[pos];
		}
	}
	const T & at(const size_t &pos) const {
		if (pos >= used_size || pos < 0) {
			throw index_out_of_bound();
		} else {
			return ((T*)data)[pos];
		}
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) {
		if (pos >= used_size || pos < 0) {
			throw index_out_of_bound();
		} else {
			return ((T*)data)[pos];
		}
	}
	const T & operator[](const size_t &pos) const {
		if (pos >= used_size || pos < 0) {
			throw index_out_of_bound();
		} else {
			return ((T*)data)[pos];
		}
	}
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const T & front() const {
		if (used_size == 0) {
			throw container_is_empty();
		}
		return ((T*)data)[0];
	}
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const {
		if (used_size == 0) {
			throw container_is_empty();
		}
		return ((T*)data)[used_size - 1];
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
		return iterator(0, data, used_size);
	}
	const_iterator cbegin() const {
		return const_iterator(0, data, used_size);
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
		return iterator(used_size, data, used_size);
	}
	const_iterator cend() const {
		return const_iterator(used_size, data, used_size);
	}
	/**
	 * checks whether the container is empty
	 */
	bool empty() const {
		return used_size == 0;
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const {
		return used_size;
	}
	/**
	 * clears the contents
	 */
	void clear() {
		Delete();
		// delete [] (T*)data;
		// ::operator delete [] (data);
		data = operator new [] (sizeof(T) * 2);
		total_size = 2;
		used_size = 0;
	}
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const T &value) {
		used_size++;
		check_volume();
		for (int p = used_size; p != pos.p; p--) {
			new(((T*)data + p)) T(((T*)data)[p - 1]);
		}
		new(((T*)data + pos.p)) T(value);
		return iterator(pos.p, data, used_size);
	}
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value) {
		if (ind > used_size) {
			throw index_out_of_bound();
		}
		used_size++;
		check_volume();
		for (int p = used_size; p != ind; p--) {
			new((T*)data + p) T(((T*)data)[p - 1]);
		}
		new((T*)data + ind) T(value);
		return iterator(ind, data, used_size);
	}
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos) {
		for (int p = pos.p; p < used_size; p++) {
			new((T*)data + p) T(((T*)data)[p + 1]);
		}
		used_size--;
		((T*)data + used_size)->~T();
		check_volume();
		return pos;
	}
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind) {
		if (ind >= used_size || ind < 0) {
			throw index_out_of_bound();
		}
		for (int p = ind; p != used_size; p++) {
			new((T*)data + p) T(((T*)data)[p + 1]);
		}
		used_size--;
		((T*)data + used_size)->~T();
		check_volume();
		return ((T*)data)[ind];
	}
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value) {
		new(((T*)data) + used_size) T(value);
		used_size++;
		check_volume();
		return;
	}
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back() {
		if (used_size == 0) {
			throw container_is_empty();
		}
		used_size--;
		((T*)data + used_size)->~T();
		check_volume();
		return;
	}

	void check_volume() {
		if (used_size >= total_size) {
			void *tmp = operator new [] (sizeof(T) * (total_size - 1) * 2 + sizeof(T));
			for (int i = 0; i < used_size; i++) {
				new((T*)tmp + i) T(((T*)data)[i]);
			}
			// delete [] (T*)data;
			// ::operator delete [] (data);
			Delete();
			data = tmp;
			total_size = (total_size - 1) * 2 + 1;
		} else if (used_size < total_size / 4) {
			void *tmp = operator new [] (sizeof(T) * (total_size - 1) / 2 + sizeof(T));
			for (int i = 0; i < used_size; i++) {
				new((T*)tmp + i) T(((T*)data)[i]);
			}
			// delete [] (T*)data;
			// ::operator delete [] (data);
			Delete();
			data = tmp;
			total_size = (total_size - 1) / 2 + 1;
		}
	}

	inline void Delete() {
		for (int i = 0; i < used_size; i++) {
			((T*)data + i)->~T();
		}
		operator delete [] (data);
		// for (int i = 0; i < total_size; i++) {
		// 	if (i < used_size) {
		// 		delete ((T*)data + i); 
		// 	} else {
		// 		::operator delete ((T*)data + i);
		// 	}
		// }
	}

private:
	void *data;
	int total_size, used_size;
};


}

#endif
