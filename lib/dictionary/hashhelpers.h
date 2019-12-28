
/*
	ģ�飺��ϣ��(HashTable)����
	���ߣ�denglf
	���ڣ�2019-04-24
	˵����������ο���΢��.NET FrameworksԴ��
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

//������
// Table of prime numbers to use as hash table sizes. 
// ������ϣ���С��������

// A typical resize algorithm would pick the smallest prime number in this array
// that is larger than twice the previous capacity. 
// һ�����͵ĵ�����С�㷨��ѡ�������������С���������֮ǰ������������Ҫ��

// Suppose our Hashtable currently has capacity x and enough elements are added 
// such that a resize needs to occur. Resizing first computes 2x then finds the 
// first prime in the table greater than 2x, i.e. if primes are ordered 
// p_1, p_2, ..., p_i, ..., it finds p_n such that p_n-1 < 2x < p_n. 
// �������ǵ�Hashtable��ǰ��������x������������㹻��Ԫ�������Ҫ���д�С������
// ������С���ȼ���2x��Ȼ���ҵ����д���2x�ĵ�һ������������������������
// p_1��p_2����,p_i,����������p_nʹ��p_n-1 < 2x < p_n��

// Doubling is important for preserving the asymptotic complexity of the 
// hashtable operations such as add.  Having a prime guarantees that double 
// hashing does not lead to infinite loops.  IE, your hash function will be 
// h1(key) + i*h2(key), 0 <= i < size.  h2 and the size must be relatively prime.
//��ż���ڱ��ֺ����Ľ������ӶȺ���Ҫ
//��ϣ�����(��add.)��������֤�˸�ֵ��������ϣ���ᵼ������ѭ����
//Ҳ����˵����Ĺ�ϣ�����ǣ�h1(key) + i*h2(key)�� 0 <= i < size��h2�Ĵ�С������������

//˵����ΪʲôHashTableҪ����������С������
//��Ϊ�����о�����������������[0, buckets-1]����ȡ�þ��ȷֲ���bucket
//ע��bucketָHashTable�ϵĴ���, ��Ϊ�����󻯣����ֵ�����в��ٳ�HashTable���ĳ�buckets
static int32 primes[] = 
{
	3, 7, 11, 17, 23, 29, 37, 47, 59, 71, 89, 107, 131, 163, 197, 239, 293, 353, 431, 521, 631, 761, 919,
	1103, 1327, 1597, 1931, 2333, 2801, 3371, 4049, 4861, 5839, 7013, 8419, 10103, 12143, 14591, 17519, 
	21023, 25229, 30293, 36353, 43627, 52361, 62851, 75431, 90523, 108631, 130363, 156437,
	187751, 225307, 270371, 324449, 389357, 467237, 560689, 672827, 807403, 968897, 1162687, 1395263,
	1674319, 2009191, 2411033, 2893249, 3471899, 4166287, 4999559, 5999471, 7199369
};

//HashTable���ֵ: Int32.MaxValue = 0x7FEFFFFD(2147483647)
//21��,�Ѿ��㹻����,�����Ҫ����,��Ϊint64_t��
static int32 MaxHashTable = 0x7FEFFFFD;

//ȱʡHashTable����: �������ݼ�
//�������òο���:
//-------------------------------------------------------------------------------------------------------------
//��Ŀ				ʮ�ڼ�����		ǧ������		��������		ʮ������		������(΢��VB6�ֵ�Ĭ��ֵ)
//					1��-9��			1000��-9999��	100��-999��		10��-99��		0-5��
//BUCKET_COUNT		100000007		10000019		1000003			100043			509
//HASH_MAX_RANGE	999999937		99999989		9999991			999983			1201
//-------------------------------------------------------------------------------------------------------------
//΢��VB6�ֵ�Ĭ����������Ϊ�����ݣ����һ���ֵ�Ҫ���ɰ������ݺ�,�ֵ�Ĵ�ȡ�ٶȾͻ�ǳ���,
//��Ϊ��ϣ��(HashTable)��ײ̫Ƶ��,ÿ��bucket(����)�ϵ�����̫��,����Ҫ�����ϣ���������������ײ�ʡ�
static int32 DefaultHashTable = 3;// 0xF4243;//0xF4243 = 1000003 ע������̫��HashTable����������������󣬳���ǧ���8G�ڴ���������ʧ�ܡ�

//ȱʡ��HashTable����ֵ
static int32 DefaultHashPrime = 101;

class HashHelpers
{

public:

	//�ж�һ�����Ƿ�Ϊ����
	//�����ֳ�������ָ������һ������1����Ȼ���У�����1�ʹ����������⣬û����������Ȼ������������
	//���仰˵��ֻ������������(1���Լ�)����Ȼ����Ϊ������
	//��1�󵫲�������������Ϊ������
	//1��0�ȷ�����Ҳ�Ǻ�����
    static _INLINE_ bool IsPrime(int32 candidate)
    {
        if ((candidate & 1) != 0)
        {
			//����Ǻ������ض����������ĳ˻������������϶�һ�����ڸ���n��һ��С�ڸ���n
            int32 limit = (int32)sqrt((float)candidate);

			//����������Ͳ�������,ż������������Χ
            for (int32 divisor = 3; divisor <= limit; divisor += 2)
            {
                if ((candidate % divisor) == 0)
                    return false;
            }
            return true;
        }

		//2������
        return (candidate == 2);
    }

	//�������
    static _INLINE_ int GetPrime(int min)
    {
		//�������Ԫ�ص�����
		int32 count = (sizeof(primes) / sizeof(primes[0]));

        //δ����(0)�����primes����ȡ��һ�� >= min����С����
        for (int32 i = 0; i < count; i++)
        {
            int prime = primes[i];
            if (prime >= min) return prime;
        }

        //����primes�����ֵ��������һ��
        //outside of our predefined table. compute the hard way. 
        for (int32 i = (min | 1); i < MaxHashTable; i += 2)
        {
            if (IsPrime(i) && ((i - 1) % DefaultHashPrime != 0))
                return i;
        }
        return min;
    }

	//��������
	static _INLINE_ int CalcPrime(int min)
	{
		for (int32 i = (min | 1); i < MaxHashTable; i += 2)
		{
			if (IsPrime(i) && ((i - 1) % DefaultHashPrime != 0))
				return i;
		}
		return min;
	}

	//��ȡ��С������
    static _INLINE_ int32 GetMinPrime()
    {
        return primes[0];
    }

	// ����HashTableԭ�ȵĴ�С(oldSize)������Ҫ���ݵ�HashTable�Ĵ�С��
    static _INLINE_ int32 ExpandPrime(int32 oldSize)
    {
		
		//�����ԭ����2��
        int32 newSize = 2 * oldSize;

		//
        // Allow the hashtables to grow to maximum possible size (~2G elements) before encoutering capacity overflow.
        // Note that this check works even when _items.Length overflowed thanks to the (uint32_t) cast
		// �����ϣ�����������֮ǰ�����������ܵĴ�С(~2GԪ��)��
		// ע�⣬�����鼴ʹ��_itemsʱҲ���Թ���������(uint32_t)ǿ��ת�����������
        if (newSize > MaxHashTable && MaxHashTable > oldSize)
        {
            return MaxHashTable;//ע��: MaxHashTable = int32_t.MaxValue = 2147483647 ������һ������
        }

		//�������ݺ�HashTable��С(һ������)
        return GetPrime(newSize);
	}

	//����ַ�����Hash
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
		
		//hash = hash & 0x7FFFFFFF; //0x7FFFFFFF=2147483647 &�����ǽ�����ת��������
		hash = hash % MaxHashTable;//ȡģ�ǽ�hash�޶���MaxHashTable��Χ

		return hash;
	}

	//��ö���������Hash
	static _INLINE_ int32 GetHashCode(short &key)
	{
		uint32 hash = key & 0x7FFFFFFF; //0x7FFFFFFF=2147483647 &�����ǽ�����ת��������
		hash = hash % MaxHashTable;//ȡģ�ǽ�hash�޶���MaxHashTable��Χ

		return hash;
	}

	//�����������Hash
	static _INLINE_ int32 GetHashCode(int &key)
	{
		uint32 hash = key & 0x7FFFFFFF; //0x7FFFFFFF=2147483647 &�����ǽ�����ת��������
		hash = hash % MaxHashTable;//ȡģ�ǽ�hash�޶���MaxHashTable��Χ
	
		return hash;
	}

	//��ó���������Hash
	static _INLINE_ int32 GetHashCode(long &key)
	{
		uint32 hash = key & 0x7FFFFFFF; //0x7FFFFFFF=2147483647 &�����ǽ�����ת��������
		hash = hash % MaxHashTable;//ȡģ�ǽ�hash�޶���MaxHashTable��Χ

		return hash;
	}

	//��ó���������Hash
	static _INLINE_ int32 GetHashCode(uint64& key)
	{
		uint32 hash = key & 0x7FFFFFFF; //0x7FFFFFFF=2147483647 &�����ǽ�����ת��������
		hash = hash % MaxHashTable;//ȡģ�ǽ�hash�޶���MaxHashTable��Χ

		return hash;
	}

	//�������͵�keyû������

	//��ø�������Hash
	//static _INLINE_ int32 GetHashCode(float &key)
	//{
	//	uint32 hash = (*((int32*)&key)) & 0x7FFFFFFF; //0x7FFFFFFF=2147483647 &�����ǽ�����ת��������
	//	hash = hash % MaxHashTable; //ȡģ�ǽ�hash�޶���MaxHashTable��Χ

	//	return hash;
	//}

	//��ø�������Hash
	//static _INLINE_ int32 GetHashCode(double &key)
	//{
	//	uint32 hash = (*((int32*)&key)) & 0x7FFFFFFF; //0x7FFFFFFF=2147483647 &�����ǽ�����ת��������
	//	hash = hash % MaxHashTable; //ȡģ�ǽ�hash�޶���MaxHashTable��Χ

	//	return hash;
	//}

	//��õ�ַ����Hash
	static _INLINE_ int32 GetHashCode(void* key)
	{
		uint32 hash = CP2UL(key) & 0x7FFFFFFF; //0x7FFFFFFF=2147483647 &�����ǽ�����ת��������
		hash = hash % MaxHashTable; //ȡģ�ǽ�hash�޶���MaxHashTable��Χ

		return hash;
	}

	//���TString����Hash
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

		//hash = hash & 0x7FFFFFFF; //0x7FFFFFFF=2147483647 &�����ǽ�����ת��������
		hash = hash % MaxHashTable;//ȡģ�ǽ�hash�޶���MaxHashTable��Χ

		return hash;
	}

	//Hash��ײ����
	static _INLINE_ void TestHashCollision()
	{
		uint32 hash = 0;
		int32 capacity = 1000000; //������������HashTable��С
		int32 *buckets = new int32[capacity]; //����
		int32 cols = 0; //��ײ����
		int32 hits = 0; //��������
		int32 depth = 0; //��ѯ���
		
		//Utility::memset(buckets, 0, capacity);
		for (int32 i = 0; i < capacity; i++)
		{
			buckets[i] = 0;
		}

		for (int32 i = 0; i < capacity; i++)
		{
			//����ؼ���
			tchar keyc[30] = { NUL };
			format(keyc, __T("�ؼ���%d"), i);

			//����Hash

			/*
			΢��VB6�ֵ� Hash�㷨��
			100000�����ݣ�������ײ����43734��δ��������43734������ѯ���5��
			1000000�����ݣ�������ײ����558898��δ��������558898������ѯ���8��
			
			hash = 0;
			tchar* key = keyc;
			while (*key)
			{
				hash += (hash << 6) + *key++;
			}
			hash = hash % MaxHashTable;//ȡģ�ǽ�hash�޶���MaxHashTable��Χ
			*/

			/*
			BobHash�� http://burtleburtle.net/bob/hash/doobs.html �㷨:
			100000�����ݣ�������ײ����36734��δ��������36734������ѯ���7��
			1000000�����ݣ�������ײ����367849��δ��������367849������ѯ���8��
			
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
			hash = hash % MaxHashTable;//ȡģ�ǽ�hash�޶���MaxHashTable��Χ
			*/

			/*
			FastHash�� https://code.google.com/archive/p/fast-hash/ �㷨: 
			100000�����ݣ�������ײ����36890��δ��������36890������ѯ���7��
			1000000�����ݣ�������ײ����367601��δ��������367601������ѯ���11��
			
			hash = 0;
			tchar* key = keyc;
			hash = hash_fast32(key, 256, 0);
			hash = hash % MaxHashTable;//ȡģ�ǽ�hash�޶���MaxHashTable��Χ
			*/

			/*
			SuperFastHash�� http://www.azillionmonkeys.com/qed/hash.html �㷨: 
			100000�����ݣ�������ײ����36887��δ��������36887������ѯ���8
			1000000�����ݣ�������ײ����368074��δ��������368074������ѯ���8
			
			hash = 0;
			tchar* key = keyc;
			hash = SuperFastHash((char*)key, 256);
			hash = hash % MaxHashTable;//ȡģ�ǽ�hash�޶���MaxHashTable��Χ
			*/

			/*
			MurmurHash3�� https://github.com/aappleby/smhasher �㷨: 
			100000�����ݣ�������ײ����36685��δ��������36685������ѯ���8
			1000000�����ݣ�������ײ����368162��δ��������368162������ѯ���9

			hash = 0;
			tchar* key = keyc;
			MurmurHash3_x86_32(key, 256, 0, &hash);
			hash = hash % MaxHashTable;//ȡģ�ǽ�hash�޶���MaxHashTable��Χ
			*/
			
			/*
			BlizzardHash����ѩ���㷨:
			100000�����ݣ�������ײ����36572��δ��������36572������ѯ���8
			1000000�����ݣ�������ײ����367876��δ��������367876������ѯ���8
			
			PrepareCryptTable(); //ע������HashǰҪ���������

			hash = 0;
			tchar* key = keyc;
			hash = BlizzardHash(key, 0);//��ѩHash��֧��Unicode
			hash = hash % MaxHashTable;//ȡģ�ǽ�hash�޶���MaxHashTable��Χ

			*/

			/*
			С�᣺
			��������Ĳ��Կ��Կ������κ�һ�������hash�㷨��һ��������С��Χ�� hash % capacity;������ض�������ײ��
			��ײ��ζ�Ŵ����˷ѣ�����С�ڵ���HashTable������������key�Ǳ��ⷢ����ײ�ʹ����˷ѵ���Ч�ֶΡ�
			����Ҫ������ײ����ôHashTable�����㹻������������������Hash������int32��hash,��HashTable��Ĵ�С������2147483647��
			ռ�õ��ڴ����ǣ�(2147483647 * 4) / 1048576 = 8192MB �ڴ档��Ȼ�������㹻����ڴ棬��������ô���HashTable�ǲ����ܵġ�
			��Ȼ���ܱ���hash���ݷ�����ײ����ô��ײ�ǲ��ɱ���ġ�
			�Ա������hash�㷨��������ײ�����������ַ���key�����ֵ��в���BobHash���ɡ�
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
			hash = hash % MaxHashTable;//ȡģ�ǽ�hash�޶���MaxHashTable��Χ

			//���㴢��λ��
			int32 pos = hash % capacity; 

			//ͳ����ײ
			if (buckets[pos] > 0)
			{
				buckets[pos] += 1;
				cols++;
			}
			else
			{
				buckets[pos] = 1;
			}

			//�����ײ���
			if (depth < buckets[pos])
			{
				depth = buckets[pos];
			}
		}
		
		//ͳ��δ����������δ������ζ�Ŵ��۵��˷�
		for (int32 i = 0; i < capacity; i++)
		{
			if (buckets[i] == 0)
			{
				hits++;
			}
		}

		delete[] buckets;

		tchar ret[120] = { NUL };
		format(ret, __T("%d�����ݣ�������ײ����%d��δ��������%d������ѯ���%d"), capacity, cols, hits, depth);

	}
	
};

#endif //__HASHHELPERS_H_
