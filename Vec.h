#pragma once
template<typename T>
class Vec
{
public:
	class iterator;
	Vec(size_t n = 0);
	Vec(const Vec<T>& vec);
	~Vec();
	size_t size() { return _size; }
	T& operator[](int ind);
	Vec<T>& operator=(const Vec<T>& right);
	iterator begin() { return iterator(this, 0); }
	iterator end() { return iterator(this, _size - 1); }
private:
	size_t _size;
	T* _head;
	iterator _rear;
};

template<typename T>
class Vec<T>::iterator
{
public:
	iterator(Vec<T>* source, size_t start_pos);
	iterator() {}
	T& operator*() { return (*_vec)[_pos]; }
	T& operator++();
	T& operator--();
	bool operator==(iterator& r);
	bool operator!=(iterator& r);
	Vec<T>* vec;
	size_t pos;
private:
	Vec<T>* _vec;
	size_t _pos;
};

template<typename T>
Vec<T>::iterator::iterator(Vec<T>* source, size_t start_pos)
{
	_vec = source;
	_pos = start_pos;
}

#include"Vec.h"
template<typename T>
Vec<T>::Vec(size_t n)
{
	_head = new T[n];
	_size = n;
}

template<typename T>
inline Vec<T>::Vec(const Vec<T>& vec)
{
	_size = vec._size;
	_rear = vec._rear;
	_head = vec._head;
}

template<typename T>
Vec<T>::~Vec()
{

}

template<typename T>
T& Vec<T>::operator[](int ind)
{
	return *(_head + ind);
}

template<typename T>
inline Vec<T>& Vec<T>::operator=(const Vec<T>& right)
{
	delete[] _head;
	_size = right._size;
	_rear = right._rear;
	_head = right._head;
	return *this;
}

template<typename T>
T& Vec<T>::iterator::operator++()
{
	if (_pos == _vec->size())
		throw(runtime_error("Iterator is not incrementable"));
	else
		++_pos;
}

template<typename T>
T& Vec<T>::iterator::operator--()
{
	if (_pos == 0)
		throw(runtime_error("Iterator is not decrementable"));
	else
		--_pos;
}

template<typename T>
bool Vec<T>::iterator::operator==(iterator& r)
{
	return r._vec == _vec && r._pos == _pos;
}

template<typename T>
bool Vec<T>::iterator::operator!=(iterator& r)
{
	return r._vec != _vec || r._pos != _pos;
}