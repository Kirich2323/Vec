#include <cstddef>
#include "MemoryPool.h"

template <typename T>
struct Allocator {
	typedef T                 value_type;
	typedef std::size_t       size_type;
	typedef std::ptrdiff_t    difference_type;
	typedef       value_type* pointer;
	typedef       value_type& reference;
	typedef const value_type* const_pointer;
	typedef const value_type& const_reference;

	Allocator()
	{
		//_pool = &MemoryPool(1024);
	}

	Allocator(MemoryPool* pool) 
	{
		_pool = pool;
	}

	~Allocator() throw() {}

	template <typename U> 
	Allocator(const Allocator<U>& other) 
	{
		_pool = other._pool;
	}

	value_type* allocate(size_type n)
	{
		//return static_cast<pointer>(operator new(n * sizeof(T)));
		return (pointer)_pool->Allocate(sizeof(value_type) * n);
	}

	void deallocate(value_type* p, size_type n)
	{
		//operator delete(p);
		_pool->Deallocate(p);
	}

	template<class U, class... Args>
	void construct(U* p, Args&&... args) {
		new((void *)p) U(std::forward<Args>(args)...);
	}

	template<class U>
	void destroy(U* p) {
		p->~U();
	}

	MemoryPool* _pool;

private:
};

template <class T, class U>
bool operator==(const Allocator<T>&, const Allocator<U>&) throw() {
	return true;
};
template <class T, class U>
bool operator!=(const Allocator<T>&, const Allocator<U>&) throw() {
	return false;
};