#pragma once
#include"allocator.h"

template<typename T, typename Allocator>
class Vec
{
public:
	typedef Allocator allocator_type;
	typedef typename allocator_type::value_type      value_type;
	typedef typename allocator_type::reference       reference;
	typedef typename allocator_type::const_reference const_reference;
	typedef typename allocator_type::size_type       size_type;
	typedef typename allocator_type::difference_type difference_type;
	typedef typename allocator_type::pointer         pointer;
	typedef typename allocator_type::const_pointer   const_pointer;
	typedef pointer       iterator;
	typedef const_pointer const_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

	explicit Vec(const allocator_type& alloc = allocator_type())
	{

	}

	Vec(size_type n = 0)
	{
		allocate(n);
		/*_head = _alloc.allocate(n);
		_size = n;*/
	}


	Vec(size_type n, allocator_type alloc)
	{
		_alloc = alloc;
		allocate(n);
		/*_size = n;
		if (_size > 0)
			_head = _alloc.allocate(n);*/
	}

	//Vec(const Vec& vec)
	//{
	//	destroy(_head, _end);
	//	_alloc = vec._alloc;
	//	if (vec.size() > this->_capacity)
	//	{
	//		allocate(2 * vec.size() * sizeof(value_type));
	//		copy(vec._head, _head, _size);
	//	}
	//	/*_size = vec._size;
	//	_end = vec._end;
	//	_head = vec._head;
	//	_alloc = vec._alloc;*/
	//}

	~Vec()
	{
		destroy(_head, _end);
		_alloc.deallocate(_head, _size);
	}

	iterator begin() 
	{
		return _head;
	}

	const_iterator begin() const 
	{
		return _head;
	}

	iterator end() 
	{
		return _end;
	}

	const_iterator end() const 
	{
		return _end;
	}

	void push_back(const value_type elem)
	{
		if (++_size > _capacity)
		{
			//pointer new_head = _alloc.allocate(_size * 2);
			pointer old_head = _head;
			size_type old_capacity = _capacity;
			allocate(_size * 2);
		/*	if (_capacity != 0)
			{*/
			copy(old_head, _head, sizeof(value_type) * old_capacity);
			_alloc.deallocate(old_head, old_capacity);
			//?}
		}

		*(_head + _size - 1) = elem;
	}

	size_type size() const 
	{
		return _size; 
	}

	bool empty() const
	{
		return !((bool)_size);
	}

	reference operator[](size_type ind)
	{
		return *(_head + ind);
	}

	Vec& operator=(const Vec& right)
	{
		destroy(_head, _end);
		_alloc = right._alloc;
		if (right.size() > _capacity)
		{
			allocate(2 * right.size() * sizeof(value_type));
			copy(right._head, _head, _size);
		}
		return *this;
	}

private:
	allocator_type _alloc;
	size_t _size;
	size_t _capacity;
	pointer _head;
	pointer _end;

	pointer allocate(size_type size)
	{
		if (_size > 0)
		{
			_capacity = 2 * size;
			_head = _alloc.allocate(_capacity);
			_end = _head + size;
			_size = size;
		}
		return _head;
	}

	void destroy(pointer begin, pointer end)
	{
		for (pointer it = begin; it < end; it++)
			//it->~value_type();
			_alloc.destroy(it);
	}

	void copy(pointer from, pointer to, size_type size)
	{
		for (size_type i = 0; i < size; ++i)
			*(to++) = *(from++);
	}
};