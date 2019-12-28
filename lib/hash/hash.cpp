#include "hash.h"

//警告 C4700	使用了未初始化的局部变量“value”
//警告 C4172	返回局部变量或临时变量的地址: value
#pragma warning(disable:4700)
#pragma warning(disable:4172)

template <class T> 
THash<T>::THash()
{
	this->m_numHashTable = NULL;
	this->m_strHashTable = NULL;
	this->m_loadFactor = 0.8;
	this->m_initialSize = 8;
}

template <class T> 
THash<T>::THash(uint32 initialSize)
{
	this->m_numHashTable = NULL;
	this->m_strHashTable = NULL;
	this->m_loadFactor = 0.8;
	this->m_initialSize = initialSize;
}

template <class T> 
THash<T>::THash(uint32 initialSize, double loadFactor)
{
	this->m_numHashTable = NULL;
	this->m_strHashTable = NULL;
	this->m_loadFactor = (loadFactor <= 0 || loadFactor > 1) ? 0.8: loadFactor;
	this->m_initialSize = initialSize;
}

template <class T> 
THash<T>::~THash()
{
	this->destroy(this->m_numHashTable);
	this->destroy(this->m_strHashTable);
}

//
// 增加一个项目到Hash表
//
template <class T>
uint32 THash<T>::add(uint32 key, T value)
{
	uint32 pos = 0;

	if (this->m_numHashTable == NULL)
		this->m_numHashTable = this->create(this->m_initialSize, HashType::htNumber);

	// Do not let the table become overloaded
	if (this->m_numHashTable->Count > this->m_numHashTable->MaxLoad)
		this->m_numHashTable = this->expand(this->m_numHashTable);
 
	// Search for an empty slot
	if (this->findSlot(this->m_numHashTable, key, FindType::ftEmpty, &pos))
	{
		// Insert the data into the table.
		this->m_numHashTable->Items[pos].State = ItemStatus::isInUse;
		this->m_numHashTable->Items[pos].Key  = key;
		this->m_numHashTable->Items[pos].Value = value;
		this->m_numHashTable->Count++;
		return pos;
	}
	else
	{
		// The entry already exists!
		return pos;
	}
}

template <class T>
uint32 THash<T>::add(tchar* key, T value)
{
	uint32 pos = 0;

	if (this->m_strHashTable == NULL)
		 this->m_strHashTable = this->create(this->m_initialSize, HashType::htString);

	// Do not let the table become overloaded
	if (this->m_strHashTable->Count > this->m_strHashTable->MaxLoad)
		this->m_numHashTable = this->expand(this->m_strHashTable);
 
	// Search for an empty slot
	if (this->findSlot(this->m_strHashTable, key, FindType::ftEmpty, &pos))
	{
		// Insert the data into the table.
		this->m_strHashTable->Items[pos].State = ItemStatus::isInUse;
		this->m_strHashTable->Items[pos].Key  = (uint32)key;
		this->m_strHashTable->Items[pos].Value = value;
		this->m_strHashTable->Count++;
		return pos;
	}
	else
	{
		// The entry already exists!
		return pos;
	}
}

// 从Hash表中删除一个项目
template <class T>
T THash<T>::remove(uint32 key) 
{
	uint32 pos = 0;
	T value = NULL;

	if (this->m_numHashTable == NULL)
		return value;
 
	if (this->findSlot(this->m_numHashTable, key, FindType::ftExisting, &pos))
	{
		// Found the item, now mark it as deleted.
		T = this->m_numHashTable->Items[pos].Value;
		this->m_numHashTable->Items[pos].Key = 0;
		this->m_numHashTable->Items[pos].State = ItemStatus::isDeleted;
		this->m_numHashTable->Items[pos].Value = NULL;
		this->m_numHashTable->Count--;
		return value;
	}
	else
	{
		// Couldn't find the item to remove it.
		return value;
	}
}

template <class T>
T THash<T>::remove(tchar* key) 
{
	uint32 pos = 0;
	T value = NULL;

	if (this->m_strHashTable == NULL)
		return value;
 
	if (this->findSlot(this->m_strHashTable, key, FindType::ftExisting, &pos))
	{
		// Found the item, now mark it as deleted.
		value = this->m_strHashTable->Items[pos].Value;
		this->m_strHashTable->Items[pos].Key = 0;
		this->m_strHashTable->Items[pos].State = ItemStatus::isDeleted;
		this->m_strHashTable->Items[pos].Value = NULL;
		this->m_strHashTable->Count--;
		return value;
	}
	else
	{
		// Couldn't find the item to remove it.
		return value;
	}
}

//
// 从Hash表中查找一个项目
//
template <class T>
T THash<T>::lookup(uint32 key) 
{
	uint32 pos = 0;
	T value = NULL;

	if (this->m_numHashTable == NULL)
		return value;

	if (this->findSlot(this->m_numHashTable, key, FindType::ftExisting, &pos))
		return this->m_numHashTable->Items[pos].Value;
	else
		return value;
}

template <class T>
T THash<T>::lookup(tchar* key) 
{
	uint32 pos = 0;
	T value = NULL;

	if (this->m_strHashTable == NULL)
		return value;

	if (this->findSlot(this->m_strHashTable, key, FindType::ftExisting, &pos))
		return this->m_strHashTable->Items[pos].Value;
	else
		return value;
}

template <class T>
bool THash<T>::isExist(uint32 key)
{
	uint32 pos = 0;

	if (this->m_numHashTable == NULL) return false;
	return this->findSlot(this->m_numHashTable, key, FindType::ftExisting, &pos);
}

template <class T>
bool THash<T>::isExist(tchar* key)
{
	uint32 pos = 0;

	if (this->m_strHashTable == NULL) return false;
	return this->findSlot(this->m_strHashTable, key, FindType::ftExisting, &pos);
}

//
// 返回Hash表项目数量
//
template <class T>
uint32 THash<T>::count() const
{
	if (this->m_numHashTable != NULL && this->m_strHashTable != NULL)
	{
		return this->m_numHashTable->Count + this->m_strHashTable->Count;
	}
	else if (this->m_numHashTable != NULL )
	{
		return this->m_numHashTable->Count ;
	}
	else if (this->m_strHashTable != NULL)
	{
		return this->m_strHashTable->Count;
	}
	else
	{
		return 0;
	}
}

//
// 计算Hash值
//
template <class T>
inline uint32 THash<T>::hash1(uint32 key)
{
	return key;
}

/* FYI: This is the "One-at-a-Time" algorithm by Bob Jenkins */
/* from requirements by Colin Plumb. */
/* (http://burtleburtle.net/bob/hash/doobs.html) */
/* adapted from Perl sources ( hv.h ) */
template <class T>
inline uint32 THash<T>::hash1(tchar* key)
{
	/*utchar *s = (utchar*)key;
	uint32 hash = 0;
	while(*s) 
		hash = hash * 31UL + *s++;
	return hash;*/

	uint32 hash = 0;

    while(*key)
    {
        hash += *key++;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);

    return hash + (hash << 15);
}

//
// 计算下一个Hash值
// 
template <class T>
inline uint32 THash<T>::hash2(uint32 key)
{
	return this->hash1(key) >> 3;
}

template <class T>
inline uint32 THash<T>::hash2(tchar* key)
{
	return this->hash1(key) >> 3;
}

//
// 获取下一个质数
//
template <class T>
uint32 THash<T>::nextPrime(uint32 numberDesired)
{
	uint32 i;
	uint32 halfwayPoint;
	bool isDivisible;

	do
	{
		numberDesired++;
		isDivisible = false;
		halfwayPoint = numberDesired / 2;
		for (i = 2; i <= halfwayPoint; i++)
		{
			if (numberDesired % i == 0)
			{
				isDivisible = true;
				break;
			}
		}
	} while(isDivisible);

	return numberDesired;
}

//
// 从Hash表里查找一个可用的槽
//
template <class T>
bool THash<T>::findSlot(Hash* table, uint32 key, FindType findType, uint32* slot)
{
	uint32 hash = 0;
	uint32 addlAmt = 0;
	uint32 i;
	 
	//获取Hash键
	hash = this->hash1(key) % table->MaxItems;

	// Perform the lookup a maximum of MaxItems times.
	for (i = 0; i < table->MaxItems; i++) 
	{
		// Are we supposed to find an empty slot or look for an existing key?
		if (findType == FindType::ftExisting)
		{
			if (table->Items[hash].State == ItemStatus::isEmpty)// Look for an existing key
			{
				return false;// Couldn't find the key
			}
			else if (table->Items[hash].State == ItemStatus::isInUse 
				&& table->Items[hash].Key == key)
			{
				*slot = hash;
				return true;// Found the key
			}
		}
		else
		{
			// Look for an empty slot to insert the new key into
			if (table->Items[hash].State != ItemStatus::isInUse)
			{
				*slot = hash;
				return true;// Found a spot
			}
			else if (table->Items[hash].Key == key)
			{
				return false;// Key already exists!
			}
		}

		// Rehash the hash and try again.
		if (addlAmt == 0)
			addlAmt =  this->hash2(key) % (table->MaxItems >> 3) + 1;
		hash = (hash + addlAmt) % table->MaxItems;
	}

	return false;
}

template <class T>
bool THash<T>::findSlot(Hash* table, tchar* key, FindType findType, uint32* slot)
{
	uint32 hash = 0;
	uint32 addlAmt = 0;
	ulong32 i;
	 
	//获取Hash键
	hash = this->hash1(key) % table->MaxItems;

	// Perform the lookup a maximum of MaxItems times.
	for (i = 0; i < table->MaxItems; i++) 
	{
		// Are we supposed to find an empty slot or look for an existing key?
		if (findType == ftExisting)
		{
			if (table->Items[hash].State == ItemStatus::isEmpty)// Look for an existing key
			{
				return false;// Couldn't find the key
			}
			else if (table->Items[hash].State == ItemStatus::isInUse 
				&& Utility::strcmp((tchar*)table->Items[hash].Key, key) == 0)
			{
				*slot = hash;
				return true;// Found the key
			}
		}
		else
		{
			// Look for an empty slot to insert the new key into
			if (table->Items[hash].State != ItemStatus::isInUse)
			{
				*slot = hash;
				return true;// Found a spot
			}
			else if(Utility::strcmp((tchar*)table->Items[hash].Key, key) == 0)
			{
				return false;// Key already exists!
			}
		}
		// Rehash the hash and try again.
		if (addlAmt == 0)
			addlAmt =  this->hash2(key) % (table->MaxItems >> 3) + 1;
		hash = (hash + addlAmt) % table->MaxItems;
	}

	return false;
}

template <class T>
T& THash<T>::operator [](uint32 key)
{
	uint32 pos = 0;
	T value;

	if (this->m_numHashTable != NULL)
	{
		if (this->findSlot(this->m_numHashTable, key, FindType::ftExisting, &pos))
			return this->m_numHashTable->Items[pos].Value;
	}
	
	pos = this->add(key, value);
	if (this->m_numHashTable->Count > 0 && pos > 0)
		return this->m_numHashTable->Items[pos].Value;
	else
		return value;
}

template <class T>
T& THash<T>::operator [](tchar* key)
{
	uint32 pos = 0;
	T value;

	if (this->m_strHashTable != NULL)
	{
		if (this->findSlot(this->m_strHashTable, key, FindType::ftExisting, &pos))
			return this->m_strHashTable->Items[pos].Value;
	}
	
	pos = this->add(key, value);
	if (this->m_strHashTable->Count > 0 && pos > 0)
		return this->m_strHashTable->Items[pos].Value;
	else
		return value;
}
