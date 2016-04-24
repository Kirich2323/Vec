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