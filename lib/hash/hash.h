#ifndef __HASH_H__
#define __HASH_H__

#include "../typedef.h"
#include "../utility/utility.h"

#pragma warning(disable:4482)

//
// Hash表类
//
template <class T> class THash
{
public:	
	//查找类型
	enum FindType 
	{
		ftEmpty		= 0x0, 
		ftExisting	= 0x1
	};

	//项目状态
	enum ItemStatus 
	{
		isEmpty		= 0x0, 
		isInUse		= 0x1,
		isDeleted	= 0x2
	};

	//Hash表类型
	enum HashType
	{
		htNumber	= 0x0,
		htString	= 0x1
	};

	//Hash表项目
	typedef struct Item
	{
		ItemStatus	State;
		uint32		Key;
		T			Value;
	} Item;

	//Hash表
	typedef struct Hash
	{
		HashType	Type;
		uint32		MaxLoad;
		uint32		MaxItems;
		uint32		Count;	//项目数量
		Item*		Items;	//项目集合
	} Hash;

public:
	THash();
	THash(uint32 initialSize);
	THash(uint32 initialSize, double loadFactor);
	virtual ~THash();

	uint32 add(uint32 key, T value);
	uint32 add(tchar* key, T value);

	T remove(uint32 key); 
	T remove(tchar* key);

	T lookup(uint32 key);
	T lookup(tchar* key);
	
	bool isExist(uint32 key);
	bool isExist(tchar* key);

	uint32 count() const;

	T& operator [](uint32 key);
	T& operator [](tchar* key);

private:
	double m_loadFactor;
	uint32 m_initialSize;

	Hash* m_numHashTable;//数字为关键字的HASH表
	Hash* m_strHashTable;//字符串为关键字的HASH表

	uint32 nextPrime(uint32 numberDesired);

	uint32 hash1(uint32 key);
	uint32 hash1(tchar* key);

	uint32 hash2(uint32 key);
	uint32 hash2(tchar* key);

	bool findSlot(Hash* table, uint32 key, FindType findMethod, uint32* slot);
	bool findSlot(Hash* table, tchar* key, FindType findMethod, uint32* slot);

private:
	//
	// 创建新的Hash表 
	//
	Hash* create(uint32 initialSize, HashType type)
	{
		Hash* newTable;

		// 分配一个空的Hash表
		newTable = new Hash[sizeof(*newTable)];
		Utility::memset(newTable, 0, sizeof(*newTable));

		// 设置最小的项目尺寸
		if (initialSize < 8) initialSize = this->m_initialSize;

		// 分配足够大的容量，但使其不超过80%的容量。
		initialSize = (uint32)(initialSize * 1.25);

		// 在Hash表中分配一个空的数据行
		initialSize = this->nextPrime(initialSize);
		newTable->Type = type;
		newTable->MaxLoad = (uint32)(initialSize * this->m_loadFactor);
		newTable->MaxItems = initialSize;
		newTable->Items = new Item[initialSize * sizeof(struct Item)];
		Utility::memset(newTable->Items, 0, initialSize * sizeof(struct Item));
		
		// 返回新的Hash表
		return newTable;
	}

	//
	// 扩展Hash表以容纳更多的项目
	//
	Hash* THash<T>::expand(Hash* oldTable)
	{
		// 创建一新的临时Hash表
		Hash* newTable = NULL;
		newTable = this->create(oldTable->MaxItems * 2, oldTable->Type);

		// 将旧的项目数据拷贝到新的Hash表中
		Utility::memcpy(newTable->Items, oldTable->Items, oldTable->MaxItems * sizeof(struct Item));
		newTable->Count = oldTable->Count;

		// 销毁旧的项目数据
		Utility::memset(oldTable->Items, 0, oldTable->MaxItems * sizeof(struct Item));
		delete[] oldTable->Items;

		// 销毁旧的Hash表
		Utility::memset(oldTable, 0, sizeof(*oldTable));
		delete[] oldTable;

		// 返回新的Hash表
		return newTable;
	}

	//
	// 销毁Hash表
	//
	void destroy(Hash* table)
	{
		// 确保Hash表有效 
		if (table != NULL)
		{
			// 释放表中的项目
			qmemset(table->Items, 0, table->MaxItems * sizeof(struct Item));
			delete[] table->Items;
			table->Items = NULL;

			// 销毁Hash表
			qmemset(table, 0, sizeof(*table));
			delete[] table;
			table = NULL;
		}
	}
};

#endif //__HASH_H__