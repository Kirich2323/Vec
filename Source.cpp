#include<iostream>
#include"MemoryPool.h"
#include"Vec.h"
#include<vector>
#include <windows.h>
#include"gtest\gtest.h"
#include<random>
#include"Sort.h"
#include<string>
#include<set>

using namespace std;

double get_time()
{
	LARGE_INTEGER t, f;
	QueryPerformanceCounter(&t);
	QueryPerformanceFrequency(&f);
	return (double)t.QuadPart / (double)f.QuadPart;
}

random_device rd;
mt19937 mt(rd());
uniform_int_distribution<int> dist(-1e6, 1e6);

class VecTest : public ::testing::Test
{
protected:
	template<typename T>
	void compare_size(Vec<T>* my, vector<T>* stl)
	{
		ASSERT_EQ(my->size(), stl->size());
	}

	template<typename T>
	void compare_elements(Vec<T>* my, vector<T>* stl)
	{
		for (size_t i = 0; i < my->size(); i++)
			ASSERT_EQ((*my)[i], (*stl)[i]);
	}

	template<typename T>
	void compare_capacity(Vec<T>* my, vector<T>* stl)
	{
		ASSERT_EQ(my->capacity(), stl->capacity());
	}

	template<typename T>
	void compare_vecs(Vec<T>* my, vector<T>* stl)
	{
		compare_size<T>(my, stl);
		compare_elements<T>(my, stl);
	}
	template<typename T>
	void compare_bounds(Vec<T>* my, vector<T>* stl)
	{
		compare_size<T>(my, stl);
	}
};

TEST_F(VecTest, default_constructor)
{
	Vec<int> my_vec;
	vector<int> stl_vec;
	compare_bounds<int>(&my_vec, &stl_vec);
}

TEST_F(VecTest, default_constructor_string)
{
	Vec<string> my_vec;
	vector<string> stl_vec;
	compare_bounds<string>(&my_vec, &stl_vec);
}

TEST_F(VecTest, explicit_fill_constructor)
{
	Vec<int> my_vec(10);
	vector<int> stl_vec(10);	
	compare_bounds<int>(&my_vec, &stl_vec);
}

TEST_F(VecTest, explicit_fill_constructor_string)
{
	Vec<string> my_vec(10);
	vector<string> stl_vec(10);
	compare_bounds<string>(&my_vec, &stl_vec);
}

TEST_F(VecTest, operator_access)
{
	Vec<int> my_vec(10);
	vector<int> stl_vec(10);
	for (int i = 0; i < my_vec.size(); ++i)
	{
		int tmp = dist(mt);
		my_vec[i] = tmp;
		stl_vec[i] = tmp;
	}
	compare_vecs<int>(&my_vec, &stl_vec);
}

TEST_F(VecTest, operator_access_string)
{
	Vec<string> my_vec(2);
	vector<string> stl_vec(2);
	my_vec[0] = "ababaca";
	my_vec[1] = "acacaba";
	stl_vec[0] = "ababaca";
	stl_vec[1] = "acacaba";
	compare_vecs<string>(&my_vec, &stl_vec);
}

TEST_F(VecTest, fill_constructor)
{
	Vec<int> my_vec(10000, 1);
	vector<int> stl_vec(10000, 1);
	compare_vecs<int>(&my_vec, &stl_vec);
}

TEST_F(VecTest, fill_constructor_string)
{
	string str = "papapo";
	Vec<string> my_vec(10000, str);
	vector<string> stl_vec(10000, str);
	compare_vecs<string>(&my_vec, &stl_vec);
}

TEST_F(VecTest, range_constructor)
{
	vector<int> source = { 1, 2, 3, 4, 5, -1, -2, 3, 1, 4, 1, 5, 9, 99999999 };
	Vec<int> my_vec(source.begin(), source.end());
	vector<int> stl_vec(source.begin(), source.end());
	compare_vecs<int>(&my_vec, &stl_vec);
}

TEST_F(VecTest, range_constructor_string)
{
	vector<string> source = {"a", "b,c", "apo", "zzzzzz", "some string here", "praise the sun" };
	Vec<string> my_vec(source.begin(), source.end());
	vector<string> stl_vec(source.begin(), source.end());
	compare_vecs<string>(&my_vec, &stl_vec);
}

TEST_F(VecTest, copy_constructor)
{
	Vec<int> my_vec(Vec<int>(10000, 1));
	vector<int> stl_vec(vector<int>(10000, 1));
	compare_vecs<int>(&my_vec, &stl_vec);
}

TEST_F(VecTest, copy_constructor_2)
{
	Vec<int> my_vec(Vec<int>(0, -1));
	vector<int> stl_vec(vector<int>(0, -1));
	compare_vecs<int>(&my_vec, &stl_vec);
}

TEST_F(VecTest, opeator_equal)
{
	Vec<int> my_tmp(100);
	vector<int> stl_tmp(100);
	for (int i = 0; i < my_tmp.size(); ++i)
	{
		int tmp = dist(mt);
		my_tmp[i] = tmp;
		stl_tmp[i] = tmp;
	}
	Vec<int> my_vec;
	my_vec = my_tmp;
	vector<int> stl_vec;
	stl_vec = stl_tmp;
	compare_vecs<int>(&my_tmp, &stl_tmp);
	compare_vecs<int>(&my_vec, &stl_vec);
}

TEST_F(VecTest, move_constructor)
{
	Vec<int> my_tmp(1, 1);
	Vec<int> my_vec(std::move(my_tmp));
	vector<int> stl_tmp(1, 1);
	vector<int> stl_vec(std::move(stl_tmp));
	compare_vecs<int>(&my_tmp, &stl_tmp);
	compare_vecs<int>(&my_vec, &stl_vec);
}

TEST_F(VecTest, initializer_list_constructor)
{
	Vec<int> my_vec({ 10, 20, 30, 40, -10, -4, 1024, 55 });
	vector<int> stl_vec({ 10, 20, 30, 40, -10, -4, 1024, 55 });
	compare_vecs<int>(&my_vec, &stl_vec);
}

TEST_F(VecTest, operator_equal_to_initializer_list)
{
	Vec<int> my_vec;
	my_vec = { 5, 10, 15, 6, 13, -2, -2, -2, 11, 9999999 };
	vector<int> stl_vec;
	stl_vec = { 5, 10, 15, 6, 13, -2, -2, -2, 11, 9999999 };
	compare_vecs<int>(&my_vec, &stl_vec);
}

TEST_F(VecTest, operator_equal_to_move)
{
	Vec<int> my_tmp(1, 1);
	Vec<int> my_vec;
	my_vec = std::move(my_tmp);
	vector<int> stl_tmp(1, 1);
	vector<int> stl_vec;
	stl_vec = std::move(stl_tmp);
	compare_vecs<int>(&my_tmp, &stl_tmp);
	compare_vecs<int>(&my_vec, &stl_vec);
}

TEST_F(VecTest, push_back)
{
	Vec<int> my_vec;
	vector<int> stl_vec;
	for (int i = 0; i < 1e3; ++i)
	{
		int tmp = dist(mt);
		my_vec.push_back(tmp);
		stl_vec.push_back(tmp);
		compare_vecs(&my_vec, &stl_vec);
	}
}

TEST_F(VecTest, iterator_begin)
{
	for (int i = 0; i < 1e3; ++i)
	{
		Vec<int>my_vec(10);
		vector<int>stl_vec(10);
		for (int j = 0; j < 10; j++)
		{
			int tmp = dist(mt);
			my_vec[j] = tmp;
			stl_vec[j] = tmp;
		}
		ASSERT_EQ(*stl_vec.begin(), *my_vec.begin());
		auto second = ++stl_vec.begin();
		int* my_second = my_vec.begin();
		my_second++;
		ASSERT_EQ(*second, *my_second);
	}
}

TEST_F(VecTest, iterator_rbegin)
{
	for (int i = 0; i < 1e3; ++i)
	{
		Vec<int>my_vec(10);
		vector<int>stl_vec(10);
		for (int j = 0; j < 10; j++)
		{
			int tmp = dist(mt);
			my_vec[j] = tmp;
			stl_vec[j] = tmp;
		}
		ASSERT_EQ(*stl_vec.rbegin(), *my_vec.rbegin());
	}
}

TEST_F(VecTest, const_iterator_rbegin)
{
	for (int i = 0; i < 1e3; ++i)
	{
		Vec<int>buff(10);
		for (int j = 0; j < 10; j++)
			buff[j] = dist(mt);

		const Vec<int> my_vec(buff.begin(), buff.end());
		const vector<int>stl_vec(buff.begin(), buff.end());
		ASSERT_EQ(*stl_vec.rbegin(), *my_vec.rbegin());
	}
}

TEST_F(VecTest, iterator_end)
{
	for (int i = 1; i < 1e3; ++i)
	{
		Vec<int>my_vec(i);
		vector<int>stl_vec(i);
		for (int j = 0; j < i; j++)
		{
			int tmp = dist(mt);
			my_vec[j] = tmp;
			stl_vec[j] = tmp;
		}
		ASSERT_EQ(*(--stl_vec.end()), *(my_vec.end() - 1));
	}
}

TEST_F(VecTest, iterator_rend)
{
	for (int i = 1; i < 1e3; ++i)
	{
		Vec<int>my_vec(i);
		vector<int>stl_vec(i);
		for (int j = 0; j < i; j++)
		{
			int tmp = dist(mt);
			my_vec[j] = tmp;
			stl_vec[j] = tmp;
		}
		ASSERT_EQ(*(--stl_vec.rend()), *(my_vec.rend() - 1));
	}
}

TEST_F(VecTest, const_iterator_rend)
{
	for (int i = 1; i < 1e3; ++i)
	{
		Vec<int>buff(i);
		for (int j = 0; j < i; j++)
			buff[j] = dist(mt);
		const Vec<int>my_vec(buff.begin(), buff.end());
		const vector<int>stl_vec(buff.begin(), buff.end());
		ASSERT_EQ(*(--stl_vec.rend()), *(my_vec.rend() - 1));
	}
}

TEST_F(VecTest, max_size)
{
	Vec<int>my_vec;
	vector<int>stl_vec;
	ASSERT_EQ(stl_vec.max_size(), my_vec.max_sie());
}

TEST_F(VecTest, resize)
{
	Vec<int>my_vec;
	vector<int>stl_vec;
	for (int i = 0; i < 1e3; ++i)
	{
		int new_size = abs(dist(mt)) % 1272340;
		my_vec.resize(new_size);
		stl_vec.resize(new_size);
		compare_bounds(&my_vec, &stl_vec);
	}
}

TEST_F(VecTest, resize_2)
{
	Vec<int>my_vec(1000, 1);
	vector<int>stl_vec(1000, 1);
	for (int i = 0; i < 1e3; ++i)
	{
		int new_size = abs(dist(mt)) % 1272340;
		my_vec.resize(new_size);
		stl_vec.resize(new_size);
		compare_bounds(&my_vec, &stl_vec);
	}
}

TEST_F(VecTest, resize_with_fill)
{
	Vec<int>my_vec;
	vector<int>stl_vec;
	for (int i = 0; i < 1e3; ++i)
	{
		int new_size = abs(dist(mt)) % 1272340;
		int tmp = dist(mt);
		my_vec.resize(new_size, tmp);
		stl_vec.resize(new_size, tmp);
		compare_bounds(&my_vec, &stl_vec);
	}
}

TEST_F(VecTest, empty)
{
	Vec<int>my_vec(1000, 1);
	vector<int>stl_vec(1000, 1);
	ASSERT_EQ(stl_vec.empty(), my_vec.empty());
}

TEST_F(VecTest, empty_2)
{
	Vec<int>my_vec;
	vector<int>stl_vec;
	ASSERT_EQ(stl_vec.empty(), my_vec.empty());
}

TEST_F(VecTest, reserve)
{
	Vec<int>my_vec;
	my_vec.push_back(1);
	vector<int>stl_vec;
	stl_vec.push_back(1);
	my_vec.reserve(1000);
	stl_vec.reserve(1000);
	compare_capacity(&my_vec, &stl_vec);
}

TEST_F(VecTest, shrink_to_fit)
{
	Vec<int>my_vec;
	my_vec.push_back(1);
	vector<int>stl_vec;
	stl_vec.push_back(1);
	my_vec.reserve(1000);
	stl_vec.reserve(1000);
	my_vec.shrink_to_fit();
	stl_vec.shrink_to_fit();
	compare_capacity(&my_vec, &stl_vec);
}

TEST_F(VecTest, at)
{
	Vec<int>my_vec(1000);
	vector<int>stl_vec(1000);
	for (int i = 0; i < my_vec.size(); i++)
	{
		int tmp = dist(mt);
		my_vec[i] = tmp;
		stl_vec[i] = tmp;
	}

	for (int i = 0; i < my_vec.size(); i++)
		ASSERT_EQ(stl_vec.at(i), my_vec.at(i));
}

TEST_F(VecTest, at_2)
{
	Vec<int>my_vec(1000);
	vector<int>stl_vec(1000);
	for (int i = 0; i < my_vec.size(); i++)
	{
		int tmp = dist(mt);
		my_vec.at(i) = tmp;
		stl_vec.at(i) = tmp;
	}

	for (int i = 0; i < my_vec.size(); i++)
		ASSERT_EQ(stl_vec.at(i), my_vec.at(i));
}

TEST_F(VecTest, const_at)
{
	Vec<int>my_tmp(1000);
	vector<int>stl_tmp(1000);
	for (int i = 0; i < my_tmp.size(); i++)
	{
		int tmp = dist(mt);
		my_tmp.at(i) = tmp;
		stl_tmp.at(i) = tmp;
	}

	const Vec<int>my_vec(my_tmp);
	const vector<int>stl_vec(stl_tmp);
	for (int i = 0; i < my_vec.size(); i++)
		ASSERT_EQ(stl_vec.at(i), my_vec.at(i));
}

TEST_F(VecTest, at_exception)
{
	for (int i = 1; i < 1e3; ++i)
	{
		Vec<int>my_vec(i);
		vector<int>stl_vec(i);
		EXPECT_THROW(my_vec.at(-1), out_of_range);
		EXPECT_THROW(my_vec.at(i), out_of_range);
		EXPECT_THROW(my_vec.at(i*i), out_of_range);

		EXPECT_THROW(stl_vec.at(-1), out_of_range);
		EXPECT_THROW(stl_vec.at(i), out_of_range);
		EXPECT_THROW(stl_vec.at(i*i), out_of_range);
	}
}

TEST_F(VecTest, const_at_exception)
{
	for (int i = 1; i < 1e3; ++i)
	{
		const Vec<int>my_vec(i);
		const vector<int>stl_vec(i);
		EXPECT_THROW(my_vec.at(-1), out_of_range);
		EXPECT_THROW(my_vec.at(i), out_of_range);
		EXPECT_THROW(my_vec.at(i*i), out_of_range);

		EXPECT_THROW(stl_vec.at(-1), out_of_range);
		EXPECT_THROW(stl_vec.at(i), out_of_range);
		EXPECT_THROW(stl_vec.at(i*i), out_of_range);
	}
}


TEST_F(VecTest, front)
{
	for (int i = 1; i < 1e3; ++i)
	{
		Vec<int>my_vec(i);
		vector<int>stl_vec(i);
		for (int j = 0; j < i; j++)
		{
			int tmp = dist(mt);
			my_vec[j] = tmp;
			stl_vec[j] = tmp;
		}
		ASSERT_EQ(stl_vec.front(), my_vec.front());
	}
}

TEST_F(VecTest, front_2)
{
	for (int i = 1; i < 1e3; ++i)
	{
		Vec<int>my_vec(i);
		vector<int>stl_vec(i);
		for (int j = 0; j < i; j++)
		{
			int tmp = dist(mt);
			my_vec[j] = tmp;
			stl_vec[j] = tmp;
		}
		int tmp = dist(mt);
		stl_vec.front() = tmp;
		my_vec.front() = tmp;
		ASSERT_EQ(stl_vec.front(), my_vec.front());
	}
}

TEST_F(VecTest, front_3)
{
	for (int i = 1; i < 1e3; ++i)
	{
		int tmp = dist(mt);
		const Vec<int>my_vec(i, tmp);
		const vector<int>stl_vec(i, tmp);
		ASSERT_EQ(stl_vec.front(), my_vec.front());
	}
}

TEST_F(VecTest, back)
{
	for (int i = 1; i < 1e3; ++i)
	{
		Vec<int>my_vec(i);
		vector<int>stl_vec(i);
		for (int j = 0; j < i; j++)
		{
			int tmp = dist(mt);
			my_vec[j] = tmp;
			stl_vec[j] = tmp;
		}
		ASSERT_EQ(stl_vec.back(), my_vec.back());
	}
}

TEST_F(VecTest, back_2)
{
	for (int i = 1; i < 1e3; ++i)
	{
		Vec<int>my_vec(i);
		vector<int>stl_vec(i);
		for (int j = 0; j < i; j++)
		{
			int tmp = dist(mt);
			my_vec[j] = tmp;
			stl_vec[j] = tmp;
		}
		int tmp = dist(mt);
		stl_vec.back() = tmp;
		my_vec.back() = tmp;
		ASSERT_EQ(stl_vec.back(), my_vec.back());
	}
}

TEST_F(VecTest, back_3)
{
	for (int i = 1; i < 1e3; ++i)
	{
		int tmp = dist(mt);
		const Vec<int>my_vec(i, tmp);
		const vector<int>stl_vec(i, tmp);
		ASSERT_EQ(stl_vec.back(), my_vec.back());
	}
}

TEST_F(VecTest, data)
{
	for (int i = 1; i < 1e3; ++i)
	{
		Vec<int>my_vec(i);
		vector<int>stl_vec(i);
		for (int j = 0; j < i; j++)
		{
			int tmp = dist(mt);
			my_vec[j] = tmp;
			stl_vec[j] = tmp;
		}
		ASSERT_EQ(*stl_vec.data(), *my_vec.data());
	}
}

TEST_F(VecTest, data_2)
{
	for (int i = 1; i < 1e3; ++i)
	{
		const Vec<int>my_vec(i, 0);
		const vector<int>stl_vec(i, 0);
		ASSERT_EQ(*stl_vec.data(), *my_vec.data());
	}
}

TEST_F(VecTest, assign)
{
	for (int i = 1; i < 1e3; ++i)
	{
		Vec<int>my_vec(i);
		vector<int>stl_vec(i);
		for (int j = 0; j < i; j++)
		{
			int tmp = dist(mt);
			my_vec[j] = tmp;
			stl_vec[j] = tmp;
		}
		int new_size = abs(dist(mt)) % 1000;
		int new_value = dist(mt);
		my_vec.assign(new_size, new_value);
		stl_vec.assign(new_size, new_value);
		compare_vecs(&my_vec, &stl_vec);
	}
}

TEST_F(VecTest, assign_initializer_list)
{
	for (int i = 1; i < 1e3; ++i)
	{
		Vec<int>my_vec(i);
		vector<int>stl_vec(i);
		for (int j = 0; j < i; j++)
		{
			int tmp = dist(mt);
			my_vec[j] = tmp;
			stl_vec[j] = tmp;
		}
		initializer_list<int> il = { 1, 3, 2, 10, 88, 13 };
		my_vec.assign(il);
		stl_vec.assign(il);
		compare_vecs(&my_vec, &stl_vec);
	}
}

TEST_F(VecTest, pop_back)
{
	for (int i = 1; i < 1e3; ++i)
	{
		Vec<int>my_vec(i);
		vector<int>stl_vec(i);
		for (int j = 0; j < i; j++)
		{
			int tmp = dist(mt);
			my_vec[j] = tmp;
			stl_vec[j] = tmp;
		}
		for (int j = i; j > 1; --j)
		{
			my_vec.pop_back();
			stl_vec.pop_back();
			ASSERT_EQ(*(stl_vec.end() - 1), *(my_vec.end() - 1));
		}
	}
}

TEST_F(VecTest, insert_begin)
{
	Vec<int> my_vec;
	vector<int> stl_vec;
	for (int i = 0; i < 1e3; ++i)
	{
		int tmp = dist(mt);
		ASSERT_EQ(*stl_vec.insert(stl_vec.begin(), tmp), *my_vec.insert(my_vec.begin(), tmp));
		compare_vecs(&my_vec, &stl_vec);
	}
}

TEST_F(VecTest, insert_random_acces)
{
	Vec<int> my_vec(1, 1);
	vector<int> stl_vec(1, 1);
	for (int i = 0; i < 1e3; ++i)
	{
		int tmp = dist(mt);
		size_t offset = 0;
		if (my_vec.size() != 0)
			offset = abs(dist(mt)) % my_vec.size();
		ASSERT_EQ(*stl_vec.insert(stl_vec.begin() + offset, tmp), *my_vec.insert(my_vec.begin() + offset, tmp));
		compare_vecs(&my_vec, &stl_vec);
	}
}

TEST_F(VecTest, insert_end)
{
	Vec<int> my_vec;
	vector<int> stl_vec;
	for (int i = 0; i < 1e3; ++i)
	{
		int tmp = dist(mt);
		ASSERT_EQ(*stl_vec.insert(stl_vec.end(), tmp), *my_vec.insert(my_vec.end(), tmp));
		compare_vecs(&my_vec, &stl_vec);
	}
}

TEST_F(VecTest, insert_fill_begin)
{
	Vec<int> my_vec;
	vector<int> stl_vec;
	for (int i = 1; i < 1e3; ++i)
	{
		int tmp = dist(mt);
		ASSERT_EQ(*stl_vec.insert(stl_vec.begin(), max(2, i % 31), tmp), *my_vec.insert(my_vec.begin(), max(2, i % 31), tmp));
		compare_vecs(&my_vec, &stl_vec);
	}
}

TEST_F(VecTest, insert_fill_end)
{
	Vec<int> my_vec;
	vector<int> stl_vec;
	for (int i = 1; i < 1e3; ++i)
	{
		int tmp = dist(mt);
		ASSERT_EQ(*stl_vec.insert(stl_vec.end(), max(2, i % 31), tmp), *my_vec.insert(my_vec.end(), max(2, i % 31), tmp));
		compare_vecs(&my_vec, &stl_vec);
	}
}

TEST_F(VecTest, insert_fill_random_access)
{
	Vec<int> my_vec;
	vector<int> stl_vec;
	for (int i = 1; i < 1e3; ++i)
	{
		int tmp = dist(mt);
		size_t offset = 0;
		if (my_vec.size() != 0)
			offset = dist(mt) % my_vec.size();
		ASSERT_EQ(*stl_vec.insert(stl_vec.begin() + offset, max(2, i % 31), tmp), *my_vec.insert(my_vec.begin() + offset, max(2, i % 31), tmp));
		compare_vecs(&my_vec, &stl_vec);
	}
}

TEST_F(VecTest, insert_initialize_list_begin)
{
	initializer_list<int> il = { 1, 2, 10, -8, 121254, 88172, 127469, 12471, 127046, -124081, 10814, -1516, 0, -314159 };
	Vec<int> my_vec;
	vector<int> stl_vec;
	my_vec.insert(my_vec.begin(), il);
	stl_vec.insert(stl_vec.begin(), il);
	compare_vecs(&my_vec, &stl_vec);
}

TEST_F(VecTest, insert_initialize_list_end)
{
	initializer_list<int> il = { 1, 2, 10, -8, 121254, 88172, 127469, 12471, 127046, -124081, 10814, -1516, 0, -314159 };
	Vec<int> my_vec;
	vector<int> stl_vec;
	my_vec.insert(my_vec.end(), il);
	stl_vec.insert(stl_vec.end(), il);
	compare_vecs(&my_vec, &stl_vec);
}

TEST_F(VecTest, erase_end)
{
	Vec<int> my_vec;
	vector<int>stl_vec;
	for (int i = 0; i < 1e3; ++i)
	{
		int tmp = dist(mt);
		my_vec.push_back(tmp);
		stl_vec.push_back(tmp);
	}

	for (int j = 1e3; j > 0; --j)
	{
		my_vec.erase(my_vec.end() - 1);
		stl_vec.erase(stl_vec.end() - 1);
		compare_vecs(&my_vec, &stl_vec);
	}
}

TEST_F(VecTest, erase_random)
{
	Vec<int> my_vec;
	vector<int>stl_vec;
	for (int k = 0; k < 50; ++k)
	{
		for (int i = 0; i < 1e3; ++i)
		{
			int tmp = dist(mt);
			my_vec.push_back(tmp);
			stl_vec.push_back(tmp);
		}

		for (int j = 1e3; j > 1; --j)
		{
			int iter_pos = abs(dist(mt)) % (my_vec.size() - 1);
			ASSERT_EQ(*stl_vec.erase(stl_vec.begin() + iter_pos), *my_vec.erase(my_vec.begin() + iter_pos));
			compare_vecs(&my_vec, &stl_vec);
		}
	}
}

TEST_F(VecTest, swap)
{
	Vec<int> my_vec;
	Vec<int> my_vec2;
	vector<int> stl_vec;
	vector<int> stl_vec2;
	for (int j = 0; j < 1e2; ++j)
	{
		for (int i = 0; i < 1e3; ++i)
		{
			int tmp = dist(mt);
			my_vec.push_back(tmp);
			stl_vec.push_back(tmp);
			tmp = dist(mt);
			my_vec2.push_back(tmp);
			stl_vec2.push_back(tmp);
		}
		my_vec.swap(my_vec2);
		stl_vec.swap(stl_vec2);
		compare_vecs(&my_vec, &stl_vec);
		compare_vecs(&my_vec2, &stl_vec2);
	}
}

TEST_F(VecTest, clear)
{
	Vec<int> my_vec;
	vector<int> stl_vec;
	for (int j = 0; j < 1e3; ++j)
	{
		for (int i = 0; i < 1e3; ++i)
		{
			int tmp = dist(mt);
			my_vec.push_back(tmp);
			stl_vec.push_back(tmp);
		}
		my_vec.clear();
		stl_vec.clear();
		compare_vecs(&my_vec, &stl_vec);
	}
}

TEST_F(VecTest, emplace)
{
	Vec<int> my_vec;
	vector<int> stl_vec;
	for (int i = 1; i < 1e3; ++i)
	{
		int tmp = dist(mt);
		ASSERT_EQ(*stl_vec.emplace(stl_vec.end(), tmp), *my_vec.emplace(my_vec.end(), tmp));
		compare_vecs(&my_vec, &stl_vec);
	}
}

TEST_F(VecTest, emplace_back)
{
	Vec<int> my_vec;
	vector<int> stl_vec;
	for (int i = 1; i < 1e3; ++i)
	{
		int tmp = dist(mt);
		stl_vec.emplace_back(tmp);
		my_vec.emplace_back(tmp);
		compare_vecs(&my_vec, &stl_vec);
	}
}


class AllocatorTest : public ::testing::Test
{
protected:
	void compare_vecs(Vec<int, Allocator<int>>& my_vec, vector<int, Allocator<int>>& stl_vec)
	{
		for (size_t i = 0; i < my_vec.size(); i++)
			ASSERT_EQ(my_vec[i], stl_vec[i]);
	}

	void compare_default_vecs(vector<int, Allocator<int>>& my_alloc_vec, vector<int>& stl_alloc_vec)
	{
		for (size_t i = 0; i < my_alloc_vec.size(); i++)
			ASSERT_EQ(my_alloc_vec[i], stl_alloc_vec[i]);
	}
};

TEST_F(AllocatorTest, constructor)
{
	vector<int, Allocator<int>> stl_vec(10000, 0,Allocator<int>());
	Vec<int, Allocator<int>> my_vec(10000, 0, Allocator<int>());
	compare_vecs(my_vec, stl_vec);
}

TEST_F(AllocatorTest, push_back)
{
	vector<int, Allocator<int>> stl_vec(1);
	Vec<int, Allocator<int>> my_vec(1);
	for (int i = 0; i < 1e4; ++i)
	{
		int tmp = dist(mt);
		stl_vec.push_back(tmp);
		my_vec.push_back(tmp);
	}
	compare_vecs(my_vec, stl_vec);
}

TEST_F(AllocatorTest, stl_push_back)
{
	vector<int> stl_vec(1);
	vector<int, Allocator<int>> my_vec(1);
	for (int i = 0; i < 1e4; ++i)
	{
		int tmp = dist(mt);
		stl_vec.push_back(tmp);
		my_vec.push_back(tmp);
	}
	compare_default_vecs(my_vec, stl_vec);
}

TEST(SortTest_myvec_vs_stlvec_standard_allocator)
{
	Vec<int> my_vec;
	vector<int> stl_vec;
	for (int i = 0; i < 1e6; ++i)
	{
		int tmp = dist(mt);
		my_vec.push_back(tmp);
		stl_vec.push_back(tmp);
	}
	double start = get_time();
	Sort(my_vec.begin(), my_vec.end());
	double end = get_time();
	cout << "My vec sort time: " << end - start << endl;
	start = get_time();
	Sort(stl_vec.begin(), stl_vec.end());
	end = get_time();
	cout << "STL vec sort time " << end - start << endl;
}

TEST(my_sort_vs_stl_sort_on_stl_vec_standard_allocator)
{
	vector<int> stl_vec1;
	vector<int> stl_vec2;
	for (int i = 0; i < 1e6; ++i)
	{
		int tmp = dist(mt);
		stl_vec1.push_back(tmp);
		stl_vec2.push_back(tmp);
	}
	double start = get_time();
	Sort(stl_vec1.begin(), stl_vec1.end());
	double end = get_time();
	cout << "My sort time: " << end - start << endl;
	start = get_time();
	sort(stl_vec2.begin(), stl_vec2.end());
	end = get_time();
	cout << "STL sort time " << end - start << endl;
}

TEST(my_sort_vs_stl_sort_on_my_vec_standard_allocator)
{
	Vec<int> my_vec1;
	Vec<int> my_vec2;
	for (int i = 0; i < 1e6; ++i)
	{
		int tmp = dist(mt);
		my_vec1.push_back(tmp);
		my_vec2.push_back(tmp);
	}
	double start = get_time();
	Sort(my_vec1.begin(), my_vec1.end());
	double end = get_time();
	cout << "My sort time: " << end - start << endl;
	start = get_time();
	sort(my_vec2.begin(), my_vec2.end());
	end = get_time();
	cout << "STL sort time " << end - start << endl;
}

TEST(my_vec_vs_stlvec_standard_allocator)
{
	Vec<int, Allocator<int>> my_vec;
	vector<int> stl_vec;
	for (int i = 0; i < 1e6; ++i)
	{
		int tmp = dist(mt);
		my_vec.push_back(tmp);
		stl_vec.push_back(tmp);
	}
	double start = get_time();
	Sort(my_vec.begin(), my_vec.end());
	double end = get_time();
	cout << "My vec sort time: " << end - start << endl;
	start = get_time();
	Sort(stl_vec.begin(), stl_vec.end());
	end = get_time();
	cout << "STL vec sort time " << end - start << endl;
}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
}