#pragma once
#include<random>

static std::random_device rd1;
static std::mt19937 mt1(rd1());
static std::uniform_int_distribution<size_t> dist1(0, LONG_MAX);

template<class RandIterator>
bool less(RandIterator left, RandIterator right)
{
	return *left < *right;
}

template<class RandIterator>
void Sort(RandIterator first, RandIterator last)
{
	if (first == last)
		return;
	RandIterator left = first;
	RandIterator right = last;
	RandIterator pivot = left++;

	while (left != right)
		if (*left < *pivot) 
			++left;
		else 
		{
			while ((left != --right) &&  (*pivot < *right))
				;
			std::iter_swap(left, right);
		}
	--left;
	std::iter_swap(first, left);
	Sort(first, left);
	Sort(right, last);
}