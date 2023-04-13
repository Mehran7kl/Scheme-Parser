#pragma once


#include <unordered_map>
namespace mr
{
struct DJB2_str_hash
{
	std::size_t operator()(char const * str)const
	{
		std::size_t hash = 5381;
		int c;

		while ((c = *str++))
			hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

		return hash;
	}
};
struct cstr_comp_trait
{
	typedef  char const* String;
	bool operator()(String const& str, String const& str2)const
	{
		return ! strcmp(str,str2);
	}
};
template <typename ValT>
struct str_hash_map :public std::unordered_map<std::string, ValT> 
{};

//Prior implemation

//template <typename ValueT, size_t table_size = 2 << 16, typename int_type = uint32_t>
//class hash_map
//{
//	constexpr static int max_str_size = 31;
//
//	struct bucket_t
//	{
//		bucket_t(char const *str, ValueT v)
//		{
//			strcpy(src, str);
//			val = v;
//		}
//		~bucket_t()
//		{
//
//			delete next;
//		}
//		bucket_t* next{};
//		char src[max_str_size + 1]{};
//		ValueT val{};
//	};
//	std::vector<bucket_t*> hash_table{table_size};
//	std::vector<bucket_t *> address_heap{};

//  public:
//	inline int_type hash(char const *str)
//	{
//		int_type hash = 5381;
//		int c;

//		while ((c = *str++))
//			hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

//		return hash;
//	}
//	inline void put(char const *str, ValueT val)
//	{
//		size_t index = hash(str) % table_size;
//		bucket_t* bucket{hash_table[index]};
//		if (!bucket)
//		{
//			hash_table[index] = new bucket_t{str, val};
//			bucket = hash_table[index];
//			address_heap.push_back(bucket);
//			return;
//		}

//		for(; bucket->next!=nullptr; bucket=bucket->next)
//		{
//			if (!strcmp(bucket->src, str))
//			{
//				bucket->val = val;
//				return;
//			}
//
//		}
//		if(!strcmp(bucket->src,str)){
//			bucket->val=val;
//			return;
//		}
//		bucket->next = new bucket_t{str, val};
//	}
//	inline bool has(char const *str)
//	{
//		size_t index = hash(str) % table_size;
//		bucket_t *bucket = hash_table[index];

//		while (bucket != nullptr)
//		{
//			if (!strcmp(bucket->src, str))
//			{
//				return true;
//			}
//			bucket = bucket->next;
//		}
//		return false;
//	}
//	bool get(char const *str, ValueT &val)
//	{
//		size_t index = hash(str) % table_size;
//		bucket_t *bucket = hash_table[index];

//		while (bucket != nullptr)
//		{
//			if (!strcmp(bucket->src, str))
//			{
//				val = bucket->val;
//				return true;
//			}
//			bucket = bucket->next;
//		}
//		return false;
//	}
//	//DJB2 hash function

//	inline ~hash_map()
//	{
//		for (auto a : address_heap)
//			delete a;
//	}
//};

} // namespace mr