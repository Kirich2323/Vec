#pragma once
#include"allocator.h"

template<typename T, typename Allocator = std::allocator<T>>
class Vec
{
public:
	typedef T value_type;
	typedef Allocator allocator_type;
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

	//constructors
	//default constructor
	explicit Vec(const allocator_type& alloc = allocator_type())
	{ _alloc = alloc; }

	//Fill constructor
	explicit Vec(size_type n)
	{
		allocate(n); 
		construct(_head, _end, value_type());
	}

	Vec(size_type n, value_type& val)
	{
		allocate(n);
		construct(_head, _end, val);
	}

	//Range constructor
	template <class InputIterator>
	Vec(InputIterator first, InputIterator last,
		const allocator_type& alloc = allocator_type())
	{
		_alloc = alloc;
		range<is_iterator<InputIterator>::value> is_range;
		_constructor(is_range, first, last);
	}

	//Copy constructor
	Vec(const Vec& x)
	{
		destroy(_head, _end);
		_alloc = x._alloc;
		if (x.size() > capacity())
		{
			allocate(x.size());
			copy(x._head, _head, size());
		}
	}

	Vec(const Vec& x, const allocator_type& alloc)
	{
		destroy(_head, _end);
		_alloc = alloc;
		if (x.size() > _capacity)
		{
			_alloc.deallocate(_head, _capacity);
			allocate(x.size());
			copy(x._head, _head, _size);
		}
	}

	//Move constructor
	Vec(Vec&& x)
	{
		destroy(_head, _end);
		_alloc.deallocate(_head, capacity());

		_alloc = x._alloc;
		_head = x._head;
		_memory_end = x._memory_end;
		_end = x._end;
		x._end = nullptr;
		x._head = nullptr;
	}

	Vec(Vec&& x, const allocator_type& alloc)
	{
		_alloc = alloc;
		destroy(_head, _end);
		if (_capacity < x._size)
		{
			allocate(x._size);
			copy(x._head, _head, _size);
		}

		x._size = 0;
		x._capacity = 0;
		x._end = nullptr;
		x._head = nullptr;
	}

	//Initializer list constructor
	Vec(std::initializer_list<value_type> il,
		const allocator_type& alloc = allocator_type())
	{
		_alloc = alloc;
		allocate(il.size());
		pointer ptr = _head;
		for (std::initializer_list<value_type>::iterator it = il.begin(); it != il.end(); ++it)
			_alloc.construct(ptr++, *it);
	}

	//destructor
	~Vec()
	{
		if (empty())
			return;
		destroy(_head, _end);
		_alloc.deallocate(_head, capacity());
	}

	//operators
	reference operator[](size_type ind) 
	{ return *(_head + ind); }

	const_reference operator[](size_type ind) const	
	{ return *(_head + ind); }

	Vec& operator=(const Vec& x)
	{
		destroy(_head, _end);
		_alloc = x._alloc;
		if (x.size() > capacity())
		{
			allocate(x.size());
			copy(x._head, _head, size());
		}
		return *this;
	}

	Vec& operator=(Vec&& x)
	{
		destroy(_head, _end);
		_alloc = x._alloc;
		_alloc = x._alloc;
		_head = x._head;
		_memory_end = x._memory_end;
		_end = x._end;
		x._end = nullptr;
		x._head = nullptr;
		return *this;
	}

	Vec& operator=(std::initializer_list<value_type> il)
	{
		destroy(_head, _end);
		if (il.size() > capacity())
		{
			allocate(il.size());
			pointer ptr = _head;
			for (auto member : il)
				_alloc.construct(ptr++, member);
		}
		return *this;
	}

	//iterators
	iterator begin() noexcept
	{ return _head; }

	const_iterator begin() const noexcept
	{ return _head; }

	iterator end() noexcept
	{ return _end;}

	const_iterator end() const  noexcept
	{ return _end; }

	reverse_iterator rbegin() noexcept
	{ return (reverse_iterator)(_end); }

	reverse_iterator rbegin() const noexcept
	{ return (reverse_iterator)(_end); }

	reverse_iterator rend() noexcept
	{ return (reverse_iterator)_head; }

	reverse_iterator rend() const noexcept
	{ return (reverse_iterator)_head; }

	//capacity
	size_type capacity() const noexcept
	{ return _memory_end - _head; }

	size_type size() const noexcept
	{ return _end - _head; }

	size_type max_sie() const noexcept
	{ return _alloc.max_size(); }

	void resize(size_type n)
	{
		if (capacity() < n)
			reallocate(n);
		else if (size() > n)
			destroy(_head + n, _end);

		_end = _head + n;
	}

	void resize(size_type n, const value_type& val)
	{
		size_type size = this->size();
		if (capacity() < n)
			reallocate(n);
		else if (this->size() > n)
			destroy(_head + n, _end);

		_end = _head + n;
		while(_head + size < _end)
			_alloc.construct(_head + size++, val);
	}

	bool empty() const noexcept
	{ return _head == _end; }

	void reserve(size_type n)
	{
		if (capacity() < n)
			reallocate(size(), n - size());
	}

	void shrink_to_fit()
	{ reallocate(size()); }

	//element access
	reference at(size_type n)
	{
		if (n >= size())
			throw out_of_range("out of range");

		return *(_head + n);
	}

	const_reference at(size_type n) const
	{
		if (n >= size())
			throw out_of_range("out of range");

		return *(_head + n);
	}

	reference front()
	{ return *_head; }
	const_reference front() const
	{ return *_head; }

	reference back()
	{ return *(_end - 1); }

	const_reference back() const
	{ return *(_end - 1); }

	value_type* data() noexcept
	{ return _head; }

	const value_type* data() const noexcept
	{ return _head; }

	//modifier 
	template <class InputIterator>
	void assign(InputIterator first, InputIterator last)
	{
		range<is_iterator<InputIterator>::value> is_range;
		_assign(is_range, first, last);
	}

	void assign(std::initializer_list<value_type> il)
	{ assign(il.begin(), il.end()); }

	void push_back(const value_type& val)
	{
		if (++_end > _memory_end)
		{
			size_type size = _end - _head;
			reallocate(size, size);
		}
		_alloc.construct<value_type>((_end - 1), val);
	}

	void push_back(value_type&& val)
	{
		if (++_end > _memory_end)
		{
			size_type size = this->size();
			reallocate(size, size);
		}
		_alloc.construct((_end - 1), val);
	}

	void pop_back()
	{ destroy(_end-- - 1, _end); }

	iterator insert(const_iterator position, const value_type& val)
	{ return insert(position, 1, val); }

	template <class InputIterator>
	iterator insert(const_iterator position, InputIterator first, InputIterator last)
	{
		range<is_iterator<InputIterator>::value> is_range;
		return _insert(is_range, position, first, last);
	}

	iterator insert(const_iterator position, value_type&& val)
	{ return insert(position, val);	}

	iterator insert(const_iterator position, std::initializer_list<value_type> il)
	{ return insert(position, il.begin(), il.end()); }

	iterator erase(const_iterator position)
	{ return erase(position, position + 1); }

	iterator erase(const_iterator first, const_iterator last)
	{
		pointer first_ptr = pointer(first);
		pointer last_ptr = pointer(last);
		destroy(first_ptr, last_ptr);
		copy(last_ptr, first_ptr, _end - last_ptr);
		_end -= last_ptr - first_ptr;
		return first_ptr;
	}

	void swap(Vec& x)
	{
		pointer tmp_head = x._head;
		pointer tmp_end = x._end;
		pointer tmp_memory_end = x._memory_end;
		allocator_type tmp_alloc = x._alloc;

		x._head = _head;
		x._end = _end;
		x._memory_end = _memory_end;
		x._alloc = _alloc;

		_head = tmp_head;
		_end = tmp_end;
		_memory_end = tmp_memory_end;
		_alloc = tmp_alloc;
	}

	void clear() noexcept
	{ Vec<T>().swap(*this); }

	template <typename... Args>
	iterator emplace(const_iterator position, Args&&... args)
	{ return insert(position, value_type(args...)); }

	template <typename... Args>
	void emplace_back(Args&&... args)
	{ insert(end(), value_type(args...)); }

	allocator_type get_allocator() const noexcept
	{ return _alloc; }

private:
	allocator_type _alloc;
	pointer _head = nullptr;
	pointer _end = nullptr;
	pointer _memory_end = nullptr;

	template<typename I>
	struct is_iterator 
	{
	private:  
		static int detect(...);

		template<typename U>
		static decltype(std::declval<std::iterator_traits<U>::iterator_category>()) detect(const U&);
	public:
		static const bool value = !std::is_same<int, decltype(detect(I()))>::value;
	};

	template<bool B>
	struct range {};

	// Range constructor
	template<class U>
	void _constructor(range<true>, U first, U last) 
	{
		for (U i = first; i != last; i++)
			push_back(*i);
	}

	// Fill constructor
	template<class U>
	void _constructor(range<false>, U n, U val) 
	{
		allocate(n);
		construct(_head, _end, val);
	}

	//range assign
	template<class U>
	void _assign(range<true>, U first, U last)
	{
		destroy(_head, _end);
		_end = _head;
		for (U i = first; i != last; i++)
			push_back(*i);
	}

	//size assign
	template<class U>
	void _assign(range<false>, U n, U val)
	{
		destroy(_head, _end);
		if (n > capacity())
			reallocate(n);

		_end = _head + n;
		for (pointer ptr = _head; ptr < _end; ++ptr)
			_alloc.construct<value_type>(ptr, val);
	}

	//fill insert
	iterator _insert(range<false>, const_iterator position, size_type n, value_type& val)
	{
		size_type dist = position - begin();
		size_type old_size = size();
		_end += n;
		if (_end > _memory_end)
		{
			size_type size = this->size();
			reallocate(size, size);
		}

		pointer edge = _head + old_size;
		pointer iter = _head + dist;
		pointer right_iter = iter + n;
		pointer right_end = _end;

		while (iter != edge)
		{
			_alloc.construct(--right_end, *(--edge));
			destroy(edge, edge + 1);
		}

		construct(iter, right_iter, val);

		return _head + dist;
	}

	//range insert
	template<class U>
	iterator _insert(range<true>, const_iterator position, U first, U last)
	{
		size_type count = 0;
		for (U i = first; i != last; ++i, ++count)
			position = insert(position, *i) + 1; 
		return _head + count;
	}

	pointer allocate(size_type size, size_type additional_capacity = 0)
	{
		if (size > 0)
		{
			size_type capacity = size + additional_capacity;
			_head = _alloc.allocate(capacity);
			_end = _head + size;
			_memory_end = _head + capacity;
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
			_alloc.construct<value_type>(to++, *from++);
	}

	void reallocate(size_type n, size_type additional_capacity = 0)
	{
		pointer old_head = _head;
		size_type old_capacity = capacity();
		allocate(n, additional_capacity);
		copy(old_head, _head, min(old_capacity, capacity()));
		destroy(old_head, old_head + old_capacity);
		_alloc.deallocate(old_head, old_capacity);
	}

	void construct(pointer left, pointer right, const value_type& val)
	{
		while(left != right)
			_alloc.construct<value_type>(left++, val);
	}
};