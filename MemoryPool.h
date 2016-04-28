#pragma once
#define MBYTE 1048576
#include <map>
#include <algorithm>
#include <vector>

struct Index {
	size_t size;
	size_t ptr_counter;
	std::map<size_t, std::map <size_t, Index>::iterator>::iterator free_memory_blocks_iterator;
	Index(size_t size = 0, size_t ptr_counter = 0) :
		size(size), ptr_counter(ptr_counter) {};
};

class MemoryPool {
public:
	MemoryPool(size_t size)
	{
		auto tmp = memory_blocks.insert(std::make_pair(0, Index(size, 0))).first;
		tmp->second.free_memory_blocks_iterator = free_memory_blocks.insert(std::make_pair(size, tmp)).first;
		mem = new char[size];
	}

	~MemoryPool() 
	{
		delete[] mem;
	}

	void* Allocate(size_t n) 
	{
		auto it = free_memory_blocks.lower_bound(n);

		if (it == free_memory_blocks.end()) 
			throw "Fragment not found";
		else 
		{
			auto it2 = it->second;
			free_memory_blocks.erase(it);
			it2->second.ptr_counter = 1;
			if (it2->second.size > n) 
			{
				size_t tmpIndex = it2->first + n;
				size_t tmpSize = it2->second.size - n;
				auto tmp = memory_blocks.insert(std::make_pair(tmpIndex, Index(tmpSize, 1))).first;
				tmp->second.free_memory_blocks_iterator = free_memory_blocks.insert(std::make_pair(tmpSize, tmp)).first;
			}
			it2->second.size = n;
			return &mem[it2->first];
		}
		return nullptr;
	}

	void Deallocate(void* ptr) 
	{
		size_t index = (char*)ptr - &mem[0];
		auto cur = memory_blocks.find(index);
		if (cur == memory_blocks.end())
			return;
		if (--cur->second.ptr_counter != 0)
			return;

		auto it = cur;
		if (it != memory_blocks.begin())
			if ((--it)->second.ptr_counter == 0) 
				cur = Merge(cur, it);

		it = cur;

		if (++it != memory_blocks.end()) 
			if (it->second.ptr_counter == 0) 
				Merge(it, cur);

		cur->second.free_memory_blocks_iterator = free_memory_blocks.insert(std::make_pair(cur->second.size, cur)).first;
	}

	void Increment(void* ptr) 
	{
		size_t index = (char*)ptr - &mem[0];
		memory_blocks[index].ptr_counter++;
	}

private:
	std::map <size_t, Index>::iterator& Merge(std::map <size_t, Index>::iterator& it,
		                                      std::map <size_t, Index>::iterator& cur)
	{
		auto il = free_memory_blocks.begin();
		auto range = free_memory_blocks.equal_range(cur->second.size);
		for (auto i = range.first; i != range.second; i++)
			if (i->second == cur) {
				free_memory_blocks.erase(i);
				break;
			}
		cur->second.size += it->second.size;
		free_memory_blocks.erase(it->second.free_memory_blocks_iterator);
		memory_blocks.erase(it);
		return cur;
	}

	std::map <size_t, Index> memory_blocks;
	std::map <size_t, std::map <size_t, Index>::iterator> free_memory_blocks;
	char* mem;
};