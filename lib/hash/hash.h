#ifndef __HASH_H__
#define __HASH_H__

#include "../typedef.h"
#include "../utility/utility.h"

#pragma warning(disable:4482)

//
// Hash����
//
template <class T> class THash
{
public:	
	//��������
	enum FindType 
	{
		ftEmpty		= 0x0, 
		ftExisting	= 0x1
	};

	//��Ŀ״̬
	enum ItemStatus 
	{
		isEmpty		= 0x0, 
		isInUse		= 0x1,
		isDeleted	= 0x2
	};

	//Hash������
	enum HashType
	{
		htNumber	= 0x0,
		htString	= 0x1
	};

	//Hash����Ŀ
	typedef struct Item
	{
		ItemStatus	State;
		uint32		Key;
		T			Value;
	} Item;

	//Hash��
	typedef struct Hash
	{
		HashType	Type;
		uint32		MaxLoad;
		uint32		MaxItems;
		uint32		Count;	//��Ŀ����
		Item*		Items;	//��Ŀ����
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

	Hash* m_numHashTable;//����Ϊ�ؼ��ֵ�HASH��
	Hash* m_strHashTable;//�ַ���Ϊ�ؼ��ֵ�HASH��

	uint32 nextPrime(uint32 numberDesired);

	uint32 hash1(uint32 key);
	uint32 hash1(tchar* key);

	uint32 hash2(uint32 key);
	uint32 hash2(tchar* key);

	bool findSlot(Hash* table, uint32 key, FindType findMethod, uint32* slot);
	bool findSlot(Hash* table, tchar* key, FindType findMethod, uint32* slot);

private:
	//
	// �����µ�Hash�� 
	//
	Hash* create(uint32 initialSize, HashType type)
	{
		Hash* newTable;

		// ����һ���յ�Hash��
		newTable = new Hash[sizeof(*newTable)];
		Utility::memset(newTable, 0, sizeof(*newTable));

		// ������С����Ŀ�ߴ�
		if (initialSize < 8) initialSize = this->m_initialSize;

		// �����㹻�����������ʹ�䲻����80%��������
		initialSize = (uint32)(initialSize * 1.25);

		// ��Hash���з���һ���յ�������
		initialSize = this->nextPrime(initialSize);
		newTable->Type = type;
		newTable->MaxLoad = (uint32)(initialSize * this->m_loadFactor);
		newTable->MaxItems = initialSize;
		newTable->Items = new Item[initialSize * sizeof(struct Item)];
		Utility::memset(newTable->Items, 0, initialSize * sizeof(struct Item));
		
		// �����µ�Hash��
		return newTable;
	}

	//
	// ��չHash�������ɸ������Ŀ
	//
	Hash* THash<T>::expand(Hash* oldTable)
	{
		// ����һ�µ���ʱHash��
		Hash* newTable = NULL;
		newTable = this->create(oldTable->MaxItems * 2, oldTable->Type);

		// ���ɵ���Ŀ���ݿ������µ�Hash����
		Utility::memcpy(newTable->Items, oldTable->Items, oldTable->MaxItems * sizeof(struct Item));
		newTable->Count = oldTable->Count;

		// ���پɵ���Ŀ����
		Utility::memset(oldTable->Items, 0, oldTable->MaxItems * sizeof(struct Item));
		delete[] oldTable->Items;

		// ���پɵ�Hash��
		Utility::memset(oldTable, 0, sizeof(*oldTable));
		delete[] oldTable;

		// �����µ�Hash��
		return newTable;
	}

	//
	// ����Hash��
	//
	void destroy(Hash* table)
	{
		// ȷ��Hash����Ч 
		if (table != NULL)
		{
			// �ͷű��е���Ŀ
			qmemset(table->Items, 0, table->MaxItems * sizeof(struct Item));
			delete[] table->Items;
			table->Items = NULL;

			// ����Hash��
			qmemset(table, 0, sizeof(*table));
			delete[] table;
			table = NULL;
		}
	}
};

#endif //__HASH_H__