
/*
	ģ�飺�ֵ�ʵ��
	���ߣ�denglf
	���ڣ�2019-04-24
	˵�����������д��΢��.NET FrameworksԴ�롣
	΢����ֵ��㷨���ÿռ任ʱ��ķ��������Ժܿ죬�����������ʱ�ǳ����ã�
	������Ӧ�����ֵ����ݿ��У��ֵ����ݿ⴦����ǳ־û����ݡ�
*/

#ifndef __DICTIONARY_H_
#define __DICTIONARY_H_

#include <windows.h>
#include "..\utility\utility.h"
#include "..\list\list.h"
#include "HashHelpers.h"
#include "VMemAlloc.h"

template <class TKey, class TValue>
class TDictionary
{
private:

	//�ֵ���Ŀ,΢��VB6�ֵ��Ϊ��ֵ��keyitem_pair
	//ռ��4+4+16*2=40�ֽ�
    struct dicitem
    {
        int32	hash;		// ��ϣ��, -1��ʾ��Ŀ�Ѿ���ɾ��
        int32	next;		// ��һ����Ŀ������, -1��ʾ��ǰ�Ѿ������һ����Ŀ
		TKey	key;		// ��Ŀkey
		TValue  value;		// ��Ŀvalue
    };

	int32	*m_buckets;		// ���д���
	dicitem	*m_items;		// ������Ŀ

	int32	m_capacity;		// �ֵ����������ϣ���Ԫ��������������������
	int32	m_count;		// �ֵ�����Ŀ������
	int32	m_delIndex;		// ��ɾ����Ŀ������
	int32	m_delCount;		// ��ɾ����Ŀ������
	
	VMemAlloc m_mem;		// �ڴ����

public:

	TDictionary()
	{
		//�鿴�ֵ���Ŀռ�õĿռ�
		/*
		int32 size = 0;
		size = sizeof(int32);
		size = sizeof(TKey);
		size = sizeof(TValue);
		size = sizeof(dicitem);
		*/
		
		//��ʼ����Ա����
		m_buckets	= NULL;
		m_items		= NULL;
		m_capacity	= 0;
		m_count		= 0;
		m_delIndex	= 0;
		m_delCount	= 0;

		//��ʼ��������Ϣ
		this->Initialize(DefaultHashTable);
	}

	//capacity: �ֵ�����
	TDictionary(int32 capacity)
    {
        if (capacity < 0)
        {
			capacity = DefaultHashTable;
        }
            
        if (capacity > 0)
        {
            this->Initialize(capacity);
        }
    }

	//����������Դ
	virtual ~TDictionary()
	{
		this->Destory();
	}

	//���Ӽ�ֵ
	bool Add(TKey& key, TValue& value)
    {
		//���hashcode
		int32 hash = HashHelpers::GetHashCode(key); //����ʱ�����������TKey�������Զ��󶨵����õ�GetHashCode���غ�����

		//����hashcode��ô���λ��
		int32 targetBucket = hash % this->m_capacity;

		//�Ӵ����в�ѯÿ����Ŀ:
		//�����ǰ�����״β���,������Ϊδʹ��(-1),��˲������ѭ��;
		//�����ǰ�������Ѿ�����Ŀ�������ѭ��
		if (this->m_buckets[targetBucket] >= 0)
		{
			for (int32 i = this->m_buckets[targetBucket]; i >= 0; i = this->m_items[i].next)
			{
				//У����Ŀ��hash�Ƿ�һ�£��Ƚ���Ŀ��key�Ƿ���ͬ
				if ((m_items[i].hash == hash) && (this->m_items[i].key == key))
				{
					//key���ڵĻ�����ֵ�滻
					m_items[i].value = value;
					return true;
				}
			}
		}

		int32 index = 0;
		if (m_delCount > 0)
		{
			//��ɾ����Ŀ�л���һ��
			//������ɾ��ʱ��û���ͷſռ䣬��˿ռ�ɷ������ã�Ҳ�ӿ��˲�����ٶ�
			index = this->m_delIndex;//���һ��ɾ����Ŀ������
			this->m_delIndex = this->m_items[index].next;//ָ����һ����Ŀ������
			this->m_delCount--;//ɾ����������
		}
		else
		{
			//�����Ŀ�����Ѿ�������Ŀ����(��items�Ѿ�������)��������buckets��items����
			//countΪ��ǰ�������Ŀ������items.LengthΪ�����������=���۵�������
			//����״β�������count=0��items.Length>0����˲�����룬ֻ�е�����������Ѿ���������itemsʱ�Ż����
			if (this->m_count == this->m_capacity)
			{
				//���ݣ�ͨ�����ݿ��Կ�����������½��ֵ�ʱ��ָ����capacity�Ĵ�С�������ؼ������ݵĴ�����
				//�������capacity���ù����ڴ治��ʱ������ʧ�ܣ����capacityӦ����Ĭ�ϴ�С��
				if (this->Resize())
				{
					//���ݳɹ������¶�λ���µĴ��ۣ�ԭ��hash % m_capacity��������Ĵ���λ���Ѿ�����ȷ
					targetBucket = hash % this->m_capacity;
				}
				else
				{
					//����ʧ�ܣ����ڴ治���
					return false;
				}
			}

			//��¼���һ������/ɾ������Ŀ���������״�����ʱcount=0
			index = this->m_count;

			//����Ŀ����+1
			this->m_count++;
		}

		//����ֵ��������
		//ע�⣺���ڶ������ͣ�����TString��Ӧ����=���������ʵ��ֵ�������ܡ�
		this->m_items[index].key = key;
		this->m_items[index].value = value;

		//����hashcode
		this->m_items[index].hash = hash;

		//����һ��(next)��Ŀ�����óɵ�ǰ�����е���Ŀ����,
		//�����������۷�����ײ��ʱ���ϴι�������Ŀ�������ͳ��˵�ǰ��Ŀ����һ����Ŀ��
		//��ѯʱ���ܹ���ȷ����ѭ�����е�������Ŀ
		this->m_items[index].next = this->m_buckets[targetBucket];
		
		//���ڽ���Ŀ������λ�ü�¼�ڴ����У����ڸ��ݴ����ҵ���������ӵ���Ŀ
		this->m_buckets[targetBucket] = index;

		//����
		return true;
    }

	//ɾ����Ŀ
    bool Remove(TKey& key)
    {
        if (this->m_buckets != NULL)
        {
			//���hashcode
			int32 hash = HashHelpers::GetHashCode(key);

			//����hashcode��ô���λ��
			int32 bucket = hash % this->m_capacity;

			//ɾ��
			if (this->m_buckets[bucket] >= 0)
			{
				int last = -1;
				for (int32 i = this->m_buckets[bucket]; i >= 0; last = i, i = this->m_items[i].next)
				{
					if ((this->m_items[i].hash == hash) && (key == m_items[i].key))
					{
						//������ɾ������Ŀ��������������Add��ʱ�����������
						if (last < 0)
						{
							this->m_buckets[bucket] = this->m_items[i].next;
						}
						else
						{
							this->m_items[last].next = this->m_items[i].next;
						}

						//ע��: ��ɾ��ʱ�����ͷ���Ŀռ�õ��ڴ�
						this->m_items[i].hash = -1;//����
						this->m_items[i].next = m_delIndex;//ָ����һ����Ŀ
						this->m_items[i].key = (TKey)NULL;//�������ͨ������NULL�����Ŀռ�õ��ڴ棬ע���������Ӧ������=�������ʵ��NULL������ݹ��ܡ�
						this->m_items[i].value = (TValue)NULL;//���ֵ��ͨ������NULL�����Ŀռ�õ��ڴ棬ע���������Ӧ������=�������ʵ��NULL������ݹ��ܡ�
						this->m_delIndex = i; //ָ�����һ����Ŀ
						this->m_delCount++;
						return true;
					}
				}
			}
        }

        return false;
    }

	//ɾ��������Ŀ
    void RemoveAll()
    {
        if (this->m_count > 0)
        {
            //�������д���
			for (int32 i = 0; i < this->m_capacity; i++)
			{
				this->m_buckets[i] = -1;
			}

			//�����Ŀ����
			for (int32 i = 0; i < this->m_count; i++)
			{
				this->m_items[i].next = -1;
				this->m_items[i].hash = -1;
				this->m_items[i].key = (TKey)NULL;//�������ͨ������NULL�����Ŀռ�õ��ڴ棬ע���������Ӧ������=�������ʵ��NULL������ݹ��ܡ�
				this->m_items[i].value = (TValue)NULL;//���ֵ��ͨ������NULL�����Ŀռ�õ��ڴ棬ע���������Ӧ������=�������ʵ��NULL������ݹ��ܡ�
			}

			//������Ŀ��Ϣ
			this->m_delIndex = -1;
			this->m_delCount = 0;
			this->m_count = 0;
        }
    }

	//��ȡֵ
    bool GetValue(TKey& key, TValue* value)
    {
        //����value
        int32 i = this->FindItem(key);
		if (i >= 0)
		{
		    *value = this->m_items[i].value; //��ֵ������ȥ
			return true;
		}

        //û���ҵ�����ȱʡֵ
		*value = (TValue)NULL;
        return false;
    }

	//key�Ƿ����
    bool ContainsKey(TKey& key)
    {
        int32 i = this->FindItem(key);
        if (i >= 0)
        {
            return true;
        }
        return false;
    }

	//ֵ�Ƿ����
    bool ContainsValue(TKey& value)
    {
		if (value == NULL)
		{
			for (int32 i = 0; i < this->m_count; i++)
			{
				if (this->m_items[i].hash >= 0 && (this->m_items[i].value == NULL))
				{
					return true;
				}
			}
		}
		else
		{
			for (int32 i = 0; i < this->m_count; i++)
			{
				if ((this->m_items[i].hash >= 0) && (this->m_items[i].value == value))
				{
					return true;
				}
			}
		}
		return false;
    }

	//��������Keys
	TList<TKey>& Keys()
	{
		//�����б�
		TList<TKey> *list = new TList<TKey>();
		
		//ȡ�����������б���
		for (int32 i = 0; i < this->m_count; i++)
		{
			if (m_items[i].hash >= 0)
			{
				list->Add(m_items[i].key);
			}
		}
		
		//����
		return *list;
	}

	//��������Values
	TList<TValue>& Values()
	{
		//�����б�
		TList<TValue>* list = new TList<TValue>();

		//ȡ��ֵ�������б���
		for (int32 i = 0; i < this->m_count; i++)
		{
			if (m_items[i].hash >= 0)
			{
				list->Add(m_items[i].value);
			}
		}

		//����
		return *list;
	}

	//��ѯֵ��֧��ͨ���*��?
	TList<TValue>& Query(tchar* filter)
	{
		//�����б�
		TList<TValue>* list = new TList<TValue>();

		//ȡ��ֵ�������б���
		for (int32 i = 0; i < this->m_count; i++)
		{
			if ((this->m_items[i].hash >= 0) && qstrmatch(this->m_items[i].value, filter))
			{
				list->Add(this->m_items[i].value);
			}
		}

		//����
		return *list;
	}

	//������Ŀ����
	int32 Count()
	{
		return this->m_count;
	}

private:

	//����������Ŀ
	void Destory()
	{
		//�������
		this->RemoveAll();

		//���ٴ��۵�
		if (this->m_buckets != NULL)
		{
			this->m_mem.Free(m_buckets);
			this->m_buckets = NULL;
		}
		if (this->m_items != NULL)
		{
			this->m_mem.Free(m_items);
			this->m_items = NULL;
		}
		this->m_capacity = 0;
	}

	//��ʼ��
	_INLINE_ bool Initialize(int32 capacity)
    {
   
		//�����ֵ������
        //ʵ����������hashtable�Ĵ�С�����Ĵ�С����������ײ��Ƶ��
        //Խ������Ƶ�ʾ�Խ�ͣ�����Ӧ��ռ���ڴ�ͻ�����
        //���������ľ����ǣ�����capacity��prime���в���һ������capacity����С����
		int32 size = HashHelpers::GetPrime(capacity);

		//�ȷ�����
		//�������ɼ�ֵ����ĿsΪhashtable�Ĵ�С������ʼ��������Ŀ
		//dicitem* newItems = new dicitem[size];
		//dicitem* newItems = (dicitem*)malloc(sizeof(dicitem) * size);
		dicitem* newItems = (dicitem*)m_mem.Alloc(sizeof(dicitem) * size);
		if (newItems == NULL)
		{
			return 0; //�ڴ����ʧ��
		}
		else
		{
			for (int32 i = 0; i < size; i++)
			{
				newItems[i].hash = -1;
				newItems[i].next = -1;
				newItems[i].key = (TKey)NULL;
				newItems[i].value = (TValue)NULL;
			}
		}

        //��������s(��hashtable)������ʼ�����д���Ϊδʹ��(-1)״̬
		//int32* newBuckets = new int32[size]; //new��ʵ����malloc
		//int32* newBuckets = (int32*)malloc(sizeof(int32) * size);
		int32* newBuckets = (int32*)m_mem.Alloc(sizeof(int32) * size);
		if (newBuckets == NULL)
		{
			//�ͷ�newItemsռ�õ��ڴ��ֹ�ڴ�й©
			if (newItems != NULL)
			{
				m_mem.Free(newItems);
			}
			return 0; //�ڴ����ʧ��
		}
		else
		{
			for (int32 i = 0; i < size; i++)
			{
				newBuckets[i] = -1;
			}
		}
        
		//ָ��buckets��items
		this->m_buckets = newBuckets;
		this->m_items = newItems;

		//��ʼ��������Ϣ
		this->m_delIndex = -1;		
		this->m_delCount = 0;	
		this->m_count = 0;

		//���뱣���ֵ䳤��
		this->m_capacity = size;

		//����
		return 1;
    }

	//����
	//�����٣�������2*m_count����С������
	_INLINE_ bool Resize()
	{
		//��������
		int32 newSize = this->GetCapacity();

		//���������������
		//�����µ���Ŀ(����=newBuckets.Length)������ʼ��������Ŀ��
		//΢����ֵ��㷨�����Ȼ�������Ŀ죬�������˼�����ÿռ任ʱ�䣬����
		//ǧ������: ��50000000 * 40��/ 1048576 ��1907 MB�����ڴ治�����������£�100%�����ڴ��ʧ�ܡ�
		//dicitem *newItems = new dicitem[newSize];
		//dicitem *newItems = (dicitem*)malloc(sizeof(dicitem) * newSize);
		dicitem *newItems = (dicitem*)m_mem.Alloc(sizeof(dicitem) * newSize);
		if (newItems == NULL)
		{
			return 0; //�ڴ����ʧ��
		}
		else
		{
			//��ʼ������Ŀ
			for (int32 i = 0; i < newSize; i++)
			{
				newItems[i].hash = -1;
				newItems[i].next = -1;
				newItems[i].key = (TKey)NULL;
				newItems[i].value = (TValue)NULL;
			}
		}

		//�����µĴ��ۣ�����ʼ��Ϊδʹ��
		//int32 *newBuckets = new int32[newSize];
		//int32 *newBuckets = (int32*)malloc(sizeof(int32) * newSize);
		int32 *newBuckets = (int32*)m_mem.Alloc(sizeof(int32) * newSize);
		if (newBuckets == NULL)
		{
			//�ͷ�newItemsռ�õ��ڴ��ֹ�ڴ�й©
			if (newItems != NULL)
			{
				m_mem.Free(newItems);
			}
			return 0; //�ڴ����ʧ��
		}
		else
		{
			//���³�ʼ��
			for (int32 i = 0; i < newSize; i++)
			{
				newBuckets[i] = -1;
			}
		}

		//���ڽ����оɵ���Ŀ�����䵽�½���Ŀ��
		memcpy(newItems, this->m_items, sizeof(dicitem) * this->m_count);

		//Ȼ�����½���Ŀ(entry)�봢��(���й���)��ΪʲôҪ��ô����
		//��Ϊhashcode % newSize����hashcode % newBuckets.Length���Ժ��Ѿ�����ԭ���Ĵ����ˣ�
		//���Ա������½��й�����
		for (int32 i = 0; i < this->m_count; i++)
		{
			if (newItems[i].hash >= 0)
			{
				int32 bucket = newItems[i].hash % newSize;//���¼���bucket

				//������һ����Ŀ
				//�����۷�����ײ������ͬkeyͨ��hash�����õ���ͬһ���ۣ���ʱ��,
				//����������Ѿ���������Ŀ����������ʱ�ͽ��ѹ�����Ŀ��Ϊ������Ŀ����һ����Ŀ��
				//��ȷ��Ӧ���ǳ�Ϊ��ǰ������Ŀ����һ����Ŀ����ʹ˳�򱻴����˵��Ⲣ����Ҫ��
				//��Ϊ����m_items���������ʼ�ն���ö�ٳ�˳�����ӵ�ÿһ����Ŀ��
				if (newBuckets[bucket] >= 0) //�ѷ�������(��������ײ)
				{
					newItems[i].next = newBuckets[bucket];//��Ϊ��ǰ��Ŀ����һ����Ŀ
				}
				else //����ײ
				{
					newItems[i].next = -1; //�Ѿ������һ����Ŀ
				}

				//����ǰ��Ŀ�������µ�bucket���۹���������ͨ��bucket�ܿ����ҵ��Լ�
				newBuckets[bucket] = i;
			}
		}

		//�ͷ�ԭ��ռ�õ��ڴ�,��ֹ�ڴ�й©
		m_mem.Free(this->m_buckets);
		m_mem.Free(this->m_items);
		
		//ָ���µ�buckets��items
		this->m_buckets = newBuckets;
		this->m_items = newItems;

		//���뱣���ֵ䳤��
		this->m_capacity = newSize;

		//����
		return true;
	}
	 
	//������Ŀ, �ҵ�������Ŀ������,���򷵻�-1
    _INLINE_ int32 FindItem(TKey& key)
    {
		//���hashcode
		int32 hash = HashHelpers::GetHashCode(key);

		//����hashcode��ô���λ��
		int32 bucket = hash % m_capacity;

        //ȡ�ô��۵�λ��i��������۴���(i >= 0)����ʼ����Ŀ��������Ŀ
		for (int32 i = this->m_buckets[bucket]; i >= 0; i = this->m_items[i].next)
		{
			//�Ƚ�hash��key�����Ƿ����
			if ((this->m_items[i].hash == hash) && (this->m_items[i].key == key))
			{
				return i;
			}
		}
        
        return -1;//û���ҵ�
    }
	
	//���һ����Ч����������
	_INLINE_ int32 GetCapacity()
	{
		//ȡ�������ֽ���
		int32 newCapacity = HashHelpers::ExpandPrime(this->m_capacity);//��������
		int32 blocks = sizeof(int32) + sizeof(dicitem); //ÿ�����ۺ���Ŀռ�õ��ֽ���
		DWORDLONG newSize = static_cast<DWORDLONG>(blocks) * static_cast<DWORDLONG>(newCapacity);//�ֽ���

		//ȡ���ڴ���ÿռ�
		MEMORYSTATUSEX ms;
		ms.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&ms);
		DWORDLONG ullAvailPhys = ms.ullAvailPhys; //�ֽ���
		DWORDLONG ullAvailVirtual = ms.ullAvailVirtual; //�ֽ���

		//�Ѿ������˿��õ������ڴ棬����û�г������õ������ڴ棬�����������ڴ��50%�����ڴ�
		if (newSize >= ullAvailVirtual && newSize < ullAvailPhys)
		{
			//���������������
			int32 capacity = (int32)((ullAvailVirtual * 0.5)/ blocks) ;
			//���¼�����������(���һ��>=m_capacity ����С����)
			capacity = HashHelpers::CalcPrime(capacity);
			if (capacity > this->m_capacity)
			{
				newCapacity = capacity;
			}
		}

		//����
		return newCapacity;
	}
};

#endif //__DICTIONARY_H_

