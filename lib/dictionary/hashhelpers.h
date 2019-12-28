
/*
	模块：哈希表(HashTable)帮助
	作者：denglf
	日期：2019-04-24
	说明：本代码参考自微软.NET Frameworks源码
*/

#ifndef __HASHHELPERS_H_
#define __HASHHELPERS_H_

#include <math.h>
#include "..\typedef.h"
#include "..\utility\utility.h"
#include "..\string\string.h"
//#include "FastHash.h"
//#include "SuperFastHash.h"
//#include "MurmurHash3.h"
//#include "BlizzardHash.h"

#define CP2UL( p ) ((unsigned long)(unsigned long) (p))
#define CP2L( p )  ((long)(long) (p))
#define CP2UI( p ) ((unsigned int)(unsigned int) (p))
#define CP2I( p )  ((int)(int) (p))

//素数表
// Table of prime numbers to use as hash table sizes. 
// 用作哈希表大小的素数表。

// A typical resize algorithm would pick the smallest prime number in this array
// that is larger than twice the previous capacity. 
// 一个典型的调整大小算法会选择这个数组中最小的素数这比之前容量的两倍还要大。

// Suppose our Hashtable currently has capacity x and enough elements are added 
// such that a resize needs to occur. Resizing first computes 2x then finds the 
// first prime in the table greater than 2x, i.e. if primes are ordered 
// p_1, p_2, ..., p_i, ..., it finds p_n such that p_n-1 < 2x < p_n. 
// 假设我们的Hashtable当前具有容量x，并且添加了足够的元素因此需要进行大小调整。
// 调整大小首先计算2x，然后找到表中大于2x的第一个素数，即如果素数是有序的
// p_1、p_2……,p_i,…，它发现p_n使得p_n-1 < 2x < p_n。

// Doubling is important for preserving the asymptotic complexity of the 
// hashtable operations such as add.  Having a prime guarantees that double 
// hashing does not lead to infinite loops.  IE, your hash function will be 
// h1(key) + i*h2(key), 0 <= i < size.  h2 and the size must be relatively prime.
//对偶对于保持函数的渐近复杂度很重要
//哈希表操作(如add.)的质数保证了该值的两倍哈希不会导致无限循环。
//也就是说，你的哈希函数是：h1(key) + i*h2(key)， 0 <= i < size。h2的大小必须是质数。

//说明：为什么HashTable要保持素数大小增长？
//因为经过研究，采用素数，能在[0, buckets-1]区间取得均匀分布的bucket
//注：bucket指HashTable上的储槽, 故为了形象化，在字典设计中不再称HashTable而改称buckets
static int32 primes[] = 
{
	3, 7, 11, 17, 23, 29, 37, 47, 59, 71, 89, 107, 131, 163, 197, 239, 293, 353, 431, 521, 631, 761, 919,
	1103, 1327, 1597, 1931, 2333, 2801, 3371, 4049, 4861, 5839, 7013, 8419, 10103, 12143, 14591, 17519, 
	21023, 25229, 30293, 36353, 43627, 52361, 62851, 75431, 90523, 108631, 130363, 156437,
	187751, 225307, 270371, 324449, 389357, 467237, 560689, 672827, 807403, 968897, 1162687, 1395263,
	1674319, 2009191, 2411033, 2893249, 3471899, 4166287, 4999559, 5999471, 7199369
};

//HashTable最大值: Int32.MaxValue = 0x7FEFFFFD(2147483647)
//21亿,已经足够大了,如果想要更大,则为int64_t了
static int32 MaxHashTable = 0x7FEFFFFD;

//缺省HashTable容量: 百万数据级
//素数设置参考表:
//-------------------------------------------------------------------------------------------------------------
//项目				十亿级数据		千万级数据		百万级数据		十万级数据		万级数据(微软VB6字典默认值)
//					1亿-9亿			1000万-9999万	100万-999万		10万-99万		0-5万
//BUCKET_COUNT		100000007		10000019		1000003			100043			509
//HASH_MAX_RANGE	999999937		99999989		9999991			999983			1201
//-------------------------------------------------------------------------------------------------------------
//微软VB6字典默认素数设置为万级数据，因此一旦字典要容纳百万级数据后,字典的存取速度就会非常慢,
//因为哈希表(HashTable)碰撞太频繁,每个bucket(储槽)上的数据太多,所以要增大哈希表的容量，降低碰撞率。
static int32 DefaultHashTable = 3;// 0xF4243;//0xF4243 = 1000003 注：设置太大HashTable容量会增长过快过大，超过千万后，8G内存容易申请失败。

//缺省的HashTable素数值
static int32 DefaultHashPrime = 101;

class HashHelpers
{

public:

	//判断一个数是否为素数
	//素数又称质数。指整数在一个大于1的自然数中，除了1和此整数自身外，没法被其他自然数整除的数。
	//换句话说，只有两个正因数(1和自己)的自然数即为素数。
	//比1大但不是素数的数称为合数。
	//1和0既非素数也非合数。
    static _INLINE_ bool IsPrime(int32 candidate)
    {
        if ((candidate & 1) != 0)
        {
			//如果是合数，必定是两个数的乘积，这两个数肯定一个大于根号n，一个小于根号n
            int32 limit = (int32)sqrt((float)candidate);

			//如果能整除就不是素数,偶数不再素数范围
            for (int32 divisor = 3; divisor <= limit; divisor += 2)
            {
                if ((candidate % divisor) == 0)
                    return false;
            }
            return true;
        }

		//2是素数
        return (candidate == 2);
    }

	//获得素数
    static _INLINE_ int GetPrime(int min)
    {
		//获得数组元素的数量
		int32 count = (sizeof(primes) / sizeof(primes[0]));

        //未设置(0)，则从primes表中取出一个 >= min的最小素数
        for (int32 i = 0; i < count; i++)
        {
            int prime = primes[i];
            if (prime >= min) return prime;
        }

        //超出primes的最大值，则运算一个
        //outside of our predefined table. compute the hard way. 
        for (int32 i = (min | 1); i < MaxHashTable; i += 2)
        {
            if (IsPrime(i) && ((i - 1) % DefaultHashPrime != 0))
                return i;
        }
        return min;
    }

	//计算素数
	static _INLINE_ int CalcPrime(int min)
	{
		for (int32 i = (min | 1); i < MaxHashTable; i += 2)
		{
			if (IsPrime(i) && ((i - 1) % DefaultHashPrime != 0))
				return i;
		}
		return min;
	}

	//获取最小的素数
    static _INLINE_ int32 GetMinPrime()
    {
        return primes[0];
    }

	// 根据HashTable原先的大小(oldSize)，返回要扩容的HashTable的大小。
    static _INLINE_ int32 ExpandPrime(int32 oldSize)
    {
		
		//扩充成原来的2倍
        int32 newSize = 2 * oldSize;

		//
        // Allow the hashtables to grow to maximum possible size (~2G elements) before encoutering capacity overflow.
        // Note that this check works even when _items.Length overflowed thanks to the (uint32_t) cast
		// 允许哈希表在容量溢出之前增长到最大可能的大小(~2G元素)。
		// 注意，这个检查即使在_items时也可以工作。由于(uint32_t)强制转换，长度溢出
        if (newSize > MaxHashTable && MaxHashTable > oldSize)
        {
            return MaxHashTable;//注意: MaxHashTable = int32_t.MaxValue = 2147483647 正好是一个素数
        }

		//返回扩容后HashTable大小(一个素数)
        return GetPrime(newSize);
	}

	//获得字符类型Hash
	//FYI: This is the "One-at-a-Time" algorithm by Bob Jenkins
	//from requirements by Colin Plumb. 
	//http://burtleburtle.net/bob/hash/doobs.html
	//adapted from Perl sources ( hv.h ) 
	static _INLINE_ int32 GetHashCode(tchar* key)
	{
		/*
		utchar *s = (utchar*)key;
		uint32 hash = 0;
		while(*s)
			hash = hash * 31UL + *s++;
		return hash;
		*/

		uint32 hash = 0;

		while (*key)
		{
			hash += *key++;
			hash += (hash << 10);
			hash ^= (hash >> 6);
		}
		hash += (hash << 3);
		hash ^= (hash >> 11);
		hash += (hash << 15);
		
		//hash = hash & 0x7FFFFFFF; //0x7FFFFFFF=2147483647 &操作是将负数转换成正数
		hash = hash % MaxHashTable;//取模是将hash限定在MaxHashTable范围

		return hash;
	}

	//获得短整型类型Hash
	static _INLINE_ int32 GetHashCode(short &key)
	{
		uint32 hash = key & 0x7FFFFFFF; //0x7FFFFFFF=2147483647 &操作是将负数转换成正数
		hash = hash % MaxHashTable;//取模是将hash限定在MaxHashTable范围

		return hash;
	}

	//获得整型类型Hash
	static _INLINE_ int32 GetHashCode(int &key)
	{
		uint32 hash = key & 0x7FFFFFFF; //0x7FFFFFFF=2147483647 &操作是将负数转换成正数
		hash = hash % MaxHashTable;//取模是将hash限定在MaxHashTable范围
	
		return hash;
	}

	//获得长整型类型Hash
	static _INLINE_ int32 GetHashCode(long &key)
	{
		uint32 hash = key & 0x7FFFFFFF; //0x7FFFFFFF=2147483647 &操作是将负数转换成正数
		hash = hash % MaxHashTable;//取模是将hash限定在MaxHashTable范围

		return hash;
	}

	//获得长整型类型Hash
	static _INLINE_ int32 GetHashCode(uint64& key)
	{
		uint32 hash = key & 0x7FFFFFFF; //0x7FFFFFFF=2147483647 &操作是将负数转换成正数
		hash = hash % MaxHashTable;//取模是将hash限定在MaxHashTable范围

		return hash;
	}

	//浮点类型的key没有意义

	//获得浮点类型Hash
	//static _INLINE_ int32 GetHashCode(float &key)
	//{
	//	uint32 hash = (*((int32*)&key)) & 0x7FFFFFFF; //0x7FFFFFFF=2147483647 &操作是将负数转换成正数
	//	hash = hash % MaxHashTable; //取模是将hash限定在MaxHashTable范围

	//	return hash;
	//}

	//获得浮点类型Hash
	//static _INLINE_ int32 GetHashCode(double &key)
	//{
	//	uint32 hash = (*((int32*)&key)) & 0x7FFFFFFF; //0x7FFFFFFF=2147483647 &操作是将负数转换成正数
	//	hash = hash % MaxHashTable; //取模是将hash限定在MaxHashTable范围

	//	return hash;
	//}

	//获得地址类型Hash
	static _INLINE_ int32 GetHashCode(void* key)
	{
		uint32 hash = CP2UL(key) & 0x7FFFFFFF; //0x7FFFFFFF=2147483647 &操作是将负数转换成正数
		hash = hash % MaxHashTable; //取模是将hash限定在MaxHashTable范围

		return hash;
	}

	//获得TString类型Hash
	static _INLINE_ int32 GetHashCode(TString& str)
	{
		uint32 hash = 0;

		tchar* key = str.toString();

		while (*key)
		{
			hash += *key++;
			hash += (hash << 10);
			hash ^= (hash >> 6);
		}
		hash += (hash << 3);
		hash ^= (hash >> 11);
		hash += (hash << 15);

		//hash = hash & 0x7FFFFFFF; //0x7FFFFFFF=2147483647 &操作是将负数转换成正数
		hash = hash % MaxHashTable;//取模是将hash限定在MaxHashTable范围

		return hash;
	}

	//Hash碰撞测试
	static _INLINE_ void TestHashCollision()
	{
		uint32 hash = 0;
		int32 capacity = 1000000; //储槽数量，即HashTable大小
		int32 *buckets = new int32[capacity]; //储槽
		int32 cols = 0; //碰撞次数
		int32 hits = 0; //命中数量
		int32 depth = 0; //查询深度
		
		//Utility::memset(buckets, 0, capacity);
		for (int32 i = 0; i < capacity; i++)
		{
			buckets[i] = 0;
		}

		for (int32 i = 0; i < capacity; i++)
		{
			//构造关键字
			tchar keyc[30] = { NUL };
			format(keyc, __T("关键字%d"), i);

			//计算Hash

			/*
			微软VB6字典 Hash算法：
			100000条数据，发生碰撞次数43734，未命中数量43734，最大查询深度5。
			1000000条数据，发生碰撞次数558898，未命中数量558898，最大查询深度8。
			
			hash = 0;
			tchar* key = keyc;
			while (*key)
			{
				hash += (hash << 6) + *key++;
			}
			hash = hash % MaxHashTable;//取模是将hash限定在MaxHashTable范围
			*/

			/*
			BobHash： http://burtleburtle.net/bob/hash/doobs.html 算法:
			100000条数据，发生碰撞次数36734，未命中数量36734，最大查询深度7。
			1000000条数据，发生碰撞次数367849，未命中数量367849，最大查询深度8。
			
			hash = 0;
			tchar* key = keyc;
			while (*key)
			{
				hash += *key++;
				hash += (hash << 10);
				hash ^= (hash >> 6);
			}
			hash += (hash << 3);
			hash ^= (hash >> 11);
			hash += (hash << 15);
			hash = hash % MaxHashTable;//取模是将hash限定在MaxHashTable范围
			*/

			/*
			FastHash： https://code.google.com/archive/p/fast-hash/ 算法: 
			100000条数据，发生碰撞次数36890，未命中数量36890，最大查询深度7。
			1000000条数据，发生碰撞次数367601，未命中数量367601，最大查询深度11。
			
			hash = 0;
			tchar* key = keyc;
			hash = hash_fast32(key, 256, 0);
			hash = hash % MaxHashTable;//取模是将hash限定在MaxHashTable范围
			*/

			/*
			SuperFastHash： http://www.azillionmonkeys.com/qed/hash.html 算法: 
			100000条数据，发生碰撞次数36887，未命中数量36887，最大查询深度8
			1000000条数据，发生碰撞次数368074，未命中数量368074，最大查询深度8
			
			hash = 0;
			tchar* key = keyc;
			hash = SuperFastHash((char*)key, 256);
			hash = hash % MaxHashTable;//取模是将hash限定在MaxHashTable范围
			*/

			/*
			MurmurHash3： https://github.com/aappleby/smhasher 算法: 
			100000条数据，发生碰撞次数36685，未命中数量36685，最大查询深度8
			1000000条数据，发生碰撞次数368162，未命中数量368162，最大查询深度9

			hash = 0;
			tchar* key = keyc;
			MurmurHash3_x86_32(key, 256, 0, &hash);
			hash = hash % MaxHashTable;//取模是将hash限定在MaxHashTable范围
			*/
			
			/*
			BlizzardHash（暴雪）算法:
			100000条数据，发生碰撞次数36572，未命中数量36572，最大查询深度8
			1000000条数据，发生碰撞次数367876，未命中数量367876，最大查询深度8
			
			PrepareCryptTable(); //注意生成Hash前要生成密码表

			hash = 0;
			tchar* key = keyc;
			hash = BlizzardHash(key, 0);//暴雪Hash不支持Unicode
			hash = hash % MaxHashTable;//取模是将hash限定在MaxHashTable范围

			*/

			/*
			小结：
			经过上面的测试可以看出：任何一个优秀的hash算法，一旦将其缩小范围（ hash % capacity;），则必定发生碰撞，
			碰撞意味着储槽浪费，采用小于等于HashTable表容量的整型key是避免发生碰撞和储槽浪费的有效手段。
			假设要避免碰撞，那么HashTable必须足够大以至于能容纳所有Hash，例如int32型hash,则HashTable表的大小必须是2147483647大，
			占用的内存则是：(2147483647 * 4) / 1048576 = 8192MB 内存。显然除非有足够大的内存，否则建立这么大的HashTable是不可能的。
			既然不能避免hash缩容发生碰撞，那么碰撞是不可避免的。
			对比上面的hash算法发生的碰撞次数，对于字符串key，在字典中采用BobHash即可。
			*/
			hash = 0;
			tchar* key = keyc;
			while (*key)
			{
				hash += *key++;
				hash += (hash << 10);
				hash ^= (hash >> 6);
			}
			hash += (hash << 3);
			hash ^= (hash >> 11);
			hash += (hash << 15);
			hash = hash % MaxHashTable;//取模是将hash限定在MaxHashTable范围

			//计算储槽位置
			int32 pos = hash % capacity; 

			//统计碰撞
			if (buckets[pos] > 0)
			{
				buckets[pos] += 1;
				cols++;
			}
			else
			{
				buckets[pos] = 1;
			}

			//最大碰撞深度
			if (depth < buckets[pos])
			{
				depth = buckets[pos];
			}
		}
		
		//统计未命中数量，未命中意味着储槽的浪费
		for (int32 i = 0; i < capacity; i++)
		{
			if (buckets[i] == 0)
			{
				hits++;
			}
		}

		delete[] buckets;

		tchar ret[120] = { NUL };
		format(ret, __T("%d条数据，发生碰撞次数%d，未命中数量%d，最大查询深度%d"), capacity, cols, hits, depth);

	}
	
};

#endif //__HASHHELPERS_H_
