
/*

	ģ�飺USNͨ�ã��������������б��ֵ�
	���ߣ�denglf
	���ڣ�2019-04-24
	˵�������������dictionary.hģ���ֵ䣬�����洢USN��־�ļ���Ϣ��Ŀ�����ṩ�������
*/

#ifndef __USNCOMMON_H_
#define __USNCOMMON_H_

#include <windows.h>
#include "..\utility\utility.h"
#include "..\string\string.h"
#include "..\dictionary\HashHelpers.h"
#include "..\dictionary\VMemAlloc.h"

//�ļ���Ŀ
typedef struct
{
	uint64	 pfrn;		// ���ļ����պ�ParentFileReferenceNumber
	TString  name;		// �ļ�����
} FileItem;

//usn�����б�
class UsnIndex
{
public:

	// Ĭ�Ϲ��캯��
	UsnIndex()
	{
		//��ʼ������
		this->m_itemCount = 0;
		this->m_totalCount = 0;
		this->m_increase = 0;
		this->m_data = NULL;

		//��ʼ������
		this->Initialize(defaultCount, 0);
	}

	// ���캯��
	// count: ��ʼ����
	UsnIndex(uint32 count)
	{
		//��ʼ������
		this->m_itemCount = 0;
		this->m_totalCount = 0;
		this->m_increase = 0;
		this->m_data = NULL;

		//��ʼ������
		this->Initialize(count, 0);
	}

	// ���캯��
	// count: ��ʼ����
	// increase: ÿ������ʱ�������������Ϊ0���Զ�������
	UsnIndex(uint32 count, uint32 increase)
	{
		this->Initialize(count, increase);
	}

	// ��������
	virtual ~UsnIndex()
	{
		if (this->m_data != NULL)
		{
			m_mem.Free(this->m_data);
			this->m_data = NULL;
		}
	}

public:

	// ������Ŀ
	_INLINE_ void Add(uint64* item)
	{
		//Ԫ����������������
		if (this->m_totalCount == this->m_itemCount)
		{
			//���·���Ԫ������
			uint32 newCount = 0;
			if (this->m_increase > 0)
			{
				newCount = (this->m_itemCount == 0) ? defaultCount : this->m_itemCount + this->m_increase;//����ָ��ֵ����
				if (newCount > maxCount) newCount = maxCount; //�������
			}
			else
			{
				//newCount = (this->m_itemCount == 0) ? defaultCount : this->m_itemCount * 2;//��������������2��������΢���㷨��
				newCount = (this->m_itemCount == 0) ? defaultCount : this->GetCapacity();//��������������΢���ֵ��㷨��
				if (newCount > maxCount) newCount = maxCount; //�������
			}

			//���·����ڴ��С
			//ע��: new��malloc��������new������ڴ�ᱻ��ʼ��(�൱��calloc)��malloc�򲻻ᡣ
			uint64** newData = (uint64**)m_mem.Alloc(newCount * sizeof(uint64*));
			if (newCount == NULL)
			{
				return;
			}

			//����ԭ����Ԫ�ص��µ�Ԫ��������
			uint32 count = this->m_itemCount;
			for (uint32 i = 0; i < count; i++)
			{
				newData[i] = this->m_data[i];
			}

			m_mem.Free(this->m_data);//ɾ���������ݣ���ֹ�ڴ�й©
			this->m_data = newData;//ָ���µ��ڴ��ַ
			this->m_totalCount = newCount;//�����µ�����
		}

		//������Ԫ��
		this->m_data[this->m_itemCount] = item;
		this->m_itemCount++;
	}

	// ��������������Ŀ
	_INLINE_ uint64* Item(uint32 index) const
	{
		if (index < 0 || index >= this->m_itemCount)
		{
			return NULL;
		}
		else
		{
			return this->m_data[index];
		}
	}

	// ����[]������,�����±귵����Ŀ
	_INLINE_  uint64* operator[](uint32 index)
	{
		if (index < 0 || index >= this->m_itemCount)
		{
			return NULL;
		}
		else
		{
			return this->m_data[index];
		}
	}

	// ɾ��������Ŀ
	_INLINE_ void RemoveAll()
	{
		if (this->m_data != NULL)
		{
			m_mem.Free(this->m_data);
			this->m_data = NULL;
		}
		this->Initialize(defaultCount, this->m_increase);
	}

	// ������Ŀ����
	_INLINE_ uint32 Count() const
	{
		return this->m_itemCount;
	}

	// ��Ŀ�Ƿ����
	_INLINE_ tbool Exists(uint64* item) const
	{
		for (uint32 i = 0; i < this->m_itemCount; i++)
		{
			if (this->m_data[i] == item)
			{
				return TRUE;
			}
		}
		return FALSE;
	}

	//���һ����Ч����������
	_INLINE_ int32 GetCapacity()
	{
		//ȡ�������ֽ���
		int32 newCapacity = HashHelpers::ExpandPrime(this->m_itemCount);//��������
		int32 blocks = sizeof(FileItem); //ÿ����Ŀռ�õ��ֽ���
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
			uint32 capacity = (uint32)((ullAvailVirtual * 0.5) / blocks);
			//���¼�����������(���һ��>=m_capacity ����С����)
			capacity = HashHelpers::CalcPrime(capacity);
			if (capacity > this->m_itemCount)
			{
				newCapacity = capacity;
			}
		}

		//����
		return newCapacity;
	}

private:
	const uint32 defaultCount = 3;			//Ĭ������
	const uint32 defaultIncrease = 1024;	//Ĭ����������
	const uint32 maxCount = 0x7FEFFFFF;		//������� =2146435071=21�ڣ��Ѿ��㹻����

	VMemAlloc m_mem;		//�ڴ����
	uint64** m_data;		//��Ŀ����(���uint64��Ŀ�ĵ�ַ)
	uint32 m_totalCount;	//�ܵ�����(=�б���ʵ����������Ԫ������+�б���δʹ�õ�����)
	uint32 m_itemCount;		//��Ŀ����(=�б���ʵ����������Ԫ������)
	uint32 m_increase;		//��������

	// ��ʼ���б�
	void Initialize(uint32 count, uint32 increase)
	{
		this->m_itemCount = 0;
		this->m_totalCount = count > 0 ? count : defaultCount;
		this->m_increase = increase > 0 ? increase : 0;
		this->m_data = (uint64**)m_mem.Alloc(this->m_totalCount * sizeof(uint64*));
	}
};

//usn��Ŀ�б�
class UsnList
{
public:

	// Ĭ�Ϲ��캯��
	UsnList()
	{
		//��ʼ������
		this->m_itemCount = 0;
		this->m_totalCount = 0;
		this->m_increase = 0;
		this->m_data = NULL;

		//��ʼ������
		this->Initialize(defaultCount, 0);
	}

	// ���캯��
	// count: ��ʼ����
	UsnList(uint32 count)
	{
		//��ʼ������
		this->m_itemCount = 0;
		this->m_totalCount = 0;
		this->m_increase = 0;
		this->m_data = NULL;

		//��ʼ������
		this->Initialize(count, 0);
	}

	// ���캯��
	// count: ��ʼ����
	// increase: ÿ������ʱ�������������Ϊ0���Զ�������
	UsnList(uint32 count, uint32 increase)
	{
		this->Initialize(count, increase);
	}

	// ��������
	virtual ~UsnList()
	{
		if (this->m_data != NULL)
		{
			m_mem.Free(this->m_data);
			this->m_data = NULL;
		}
	}

public:

	// ������Ŀ
	_INLINE_ void Add(FileItem* item)
	{
		//Ԫ����������������
		if (this->m_totalCount == this->m_itemCount)
		{
			//���·���Ԫ������
			uint32 newCount = 0;
			if (this->m_increase > 0)
			{
				newCount = (this->m_itemCount == 0) ? defaultCount : this->m_itemCount + this->m_increase;//����ָ��ֵ����
				if (newCount > maxCount) newCount = maxCount; //�������
			}
			else
			{
				//newCount = (this->m_itemCount == 0) ? defaultCount : this->m_itemCount * 2;//��������������2��������΢���㷨��
				newCount = (this->m_itemCount == 0) ? defaultCount : this->GetCapacity();//��������������΢���ֵ��㷨��
				if (newCount > maxCount) newCount = maxCount; //�������
			}

			//���·����ڴ��С
			//ע��: new��malloc��������new������ڴ�ᱻ��ʼ��(�൱��calloc)��malloc�򲻻ᡣ
			FileItem** newData = (FileItem**)m_mem.Alloc(newCount * sizeof(FileItem*)); 
			if (newCount == NULL)
			{
				return;
			}

			//����ԭ����Ԫ�ص��µ�Ԫ��������
			uint32 count = this->m_itemCount;
			for (uint32 i = 0; i < count; i++)
			{
				newData[i] = this->m_data[i];
			}

			m_mem.Free(this->m_data);//ɾ���������ݣ���ֹ�ڴ�й©
			this->m_data = newData;//ָ���µ��ڴ��ַ
			this->m_totalCount = newCount;//�����µ�����
		}

		//������Ԫ��
		this->m_data[this->m_itemCount] = item;
		this->m_itemCount++;
	}

	// ��������������Ŀ
	_INLINE_ FileItem* Item(uint32 index) const
	{
		if (index < 0 || index >= this->m_itemCount)
		{
			return NULL;
		}
		else
		{
			return this->m_data[index];
		}
	}

	// ����[]������,�����±귵����Ŀ
	_INLINE_  FileItem* operator[](uint32 index)
	{
		if (index < 0 || index >= this->m_itemCount)
		{
			return NULL;
		}
		else
		{
			return this->m_data[index];
		}
	}

	// ɾ��������Ŀ
	_INLINE_ void RemoveAll()
	{
		if (this->m_data != NULL)
		{
			m_mem.Free(this->m_data);
			this->m_data = NULL;
		}
		this->Initialize(defaultCount, this->m_increase);
	}

	// ������Ŀ����
	_INLINE_ uint32 Count() const
	{
		return this->m_itemCount;
	}

	// ��Ŀ�Ƿ����
	_INLINE_ tbool Exists(FileItem* item) const
	{
		for (uint32 i = 0; i < this->m_itemCount; i++)
		{
			if (this->m_data[i] == item)
			{
				return TRUE;
			}
		}
		return FALSE;
	}

	//���һ����Ч����������
	_INLINE_ int32 GetCapacity()
	{
		//ȡ�������ֽ���
		int32 newCapacity = HashHelpers::ExpandPrime(this->m_itemCount);//��������
		int32 blocks = sizeof(FileItem); //ÿ����Ŀռ�õ��ֽ���
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
			uint32 capacity = (uint32)((ullAvailVirtual * 0.5) / blocks);
			//���¼�����������(���һ��>=m_capacity ����С����)
			capacity = HashHelpers::CalcPrime(capacity);
			if (capacity > this->m_itemCount)
			{
				newCapacity = capacity;
			}
		}

		//����
		return newCapacity;
	}

private:
	const uint32 defaultCount = 3;			//Ĭ������
	const uint32 defaultIncrease = 1024;	//Ĭ����������
	const uint32 maxCount = 0x7FEFFFFF;		//������� =2146435071=21�ڣ��Ѿ��㹻����

	VMemAlloc m_mem;		//�ڴ����
	FileItem** m_data;		//��Ŀ����(���FileItem��Ŀ�ĵ�ַ)
	uint32 m_totalCount;	//�ܵ�����(=�б���ʵ����������Ԫ������+�б���δʹ�õ�����)
	uint32 m_itemCount;		//��Ŀ����(=�б���ʵ����������Ԫ������)
	uint32 m_increase;		//��������
	
	// ��ʼ���б�
	void Initialize(uint32 count, uint32 increase)
	{
		this->m_itemCount = 0;
		this->m_totalCount = count > 0 ? count : defaultCount;
		this->m_increase = increase > 0 ? increase : 0;
		this->m_data = (FileItem**)m_mem.Alloc(this->m_totalCount * sizeof(FileItem*));
	}
};

//usn�ֵ�
class UsnDictionary
{
private:

	//�ֵ���Ŀ,΢��VB6�ֵ��Ϊ��ֵ��keyitem_pair
	//ռ��4+4+16*2=40�ֽ�
	struct dicitem
	{
		int32	 next;		// ��һ����Ŀ������, -1��ʾ��ǰ�Ѿ������һ����Ŀ
		uint64	 hash;		// ��ϣ��, -1��ʾ��Ŀ�Ѿ���ɾ��
		uint64	 key;		// ��Ŀkey
		FileItem value;		// ��Ŀvalue
	};

	int32* m_buckets;		// ���д���
	dicitem* m_items;		// ������Ŀ

	int32	m_capacity;		// �ֵ����������ϣ���Ԫ��������������������
	int32	m_count;		// �ֵ�����Ŀ������
	int32	m_delIndex;		// ��ɾ����Ŀ������
	int32	m_delCount;		// ��ɾ����Ŀ������

	VMemAlloc m_mem;		// �ڴ����
	UsnIndex* m_index;		// Usn����
	UsnList* m_list;		// Usn�б�
	
public:

	UsnDictionary()
	{
		//��ʼ����Ա����
		m_buckets = NULL;
		m_items = NULL;
		m_capacity = 0;
		m_count = 0;
		m_delIndex = 0;
		m_delCount = 0;

		//��ʼ��������Ϣ
		this->Initialize(DefaultHashTable);
	}

	//capacity: �ֵ�����
	UsnDictionary(int32 capacity)
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
	virtual ~UsnDictionary()
	{
		this->Destory();
	}

	//���Ӽ�ֵ
	void Add(uint64& key, uint64& pfrn, tchar* name)
	{
		//���hashcode
		//uint64 hash = HashHelpers::GetHashCode(key);
		uint64 hash = key;

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
					m_items[i].value.pfrn = pfrn;
					m_items[i].value.name = name;
					return;
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
					return;
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
		this->m_items[index].value.pfrn = pfrn;
		this->m_items[index].value.name = name;

		//����hashcode
		this->m_items[index].hash = hash;

		//����һ��(next)��Ŀ�����óɵ�ǰ�����е���Ŀ����,
		//�����������۷�����ײ��ʱ���ϴι�������Ŀ�������ͳ��˵�ǰ��Ŀ����һ����Ŀ��
		//��ѯʱ���ܹ���ȷ����ѭ�����е�������Ŀ
		this->m_items[index].next = this->m_buckets[targetBucket];

		//���ڽ���Ŀ������λ�ü�¼�ڴ����У����ڸ��ݴ����ҵ���������ӵ���Ŀ
		this->m_buckets[targetBucket] = index;
	}

	//ɾ����Ŀ
	tbool Remove(uint64& key)
	{
		if (this->m_buckets != NULL)
		{
			//���hashcode
			//uint64 hash = HashHelpers::GetHashCode(key);
			uint64 hash = key;

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
						this->m_items[i].key = 0;//�������ͨ������NULL�����Ŀռ�õ��ڴ棬ע���������Ӧ������=�������ʵ��NULL������ݹ��ܡ�
						this->m_items[i].value.name = NULL;//���ֵ��ͨ������NULL�����Ŀռ�õ��ڴ棬ע���������Ӧ������=�������ʵ��NULL������ݹ��ܡ�
						this->m_items[i].value.pfrn = 0;
						this->m_delIndex = i; //ָ�����һ����Ŀ
						this->m_delCount++;
						return TRUE;
					}
				}
			}
		}

		return FALSE;
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
				this->m_items[i].key = 0;//�������ͨ������NULL�����Ŀռ�õ��ڴ棬ע���������Ӧ������=�������ʵ��NULL������ݹ��ܡ�
				this->m_items[i].value.name = NULL;//���ֵ��ͨ������NULL�����Ŀռ�õ��ڴ棬ע���������Ӧ������=�������ʵ��NULL������ݹ��ܡ�
				this->m_items[i].value.pfrn = 0;
			}

			//������Ŀ��Ϣ
			this->m_delIndex = -1;
			this->m_delCount = 0;
			this->m_count = 0;
		}
	}

	//��ȡֵ
	tbool GetValue(uint64& key, FileItem& value)
	{
		//����value
		int32 i = this->FindItem(key);
		if (i >= 0)
		{
			value = this->m_items[i].value; //��ֵ������ȥ
			return TRUE;
		}

		//û���ҵ�����ȱʡֵ
		value.name = NULL;
		value.pfrn = 0;
		return FALSE;
	}

	//key�Ƿ����
	tbool ContainsKey(uint64& key)
	{
		int32 i = this->FindItem(key);
		if (i >= 0)
		{
			return TRUE;
		}
		return FALSE;
	}

	//ֵ�Ƿ����
	tbool ContainsValue(TString& value)
	{
		if (value == NULL)
		{
			for (int32 i = 0; i < this->m_count; i++)
			{
				if (this->m_items[i].hash >= 0 && (this->m_items[i].value.name == NULL))
				{
					return TRUE;
				}
			}
		}
		else
		{
			for (int32 i = 0; i < this->m_count; i++)
			{
				if ((this->m_items[i].hash >= 0) && (this->m_items[i].value.name == value))
				{
					return TRUE;
				}
			}
		}
		return FALSE;
	}

	//��������Keys
	UsnIndex* Keys()
	{
		//ɾ���б�������Ŀ
		this->m_index->RemoveAll();
		
		//ȡ�����������б���
		for (int32 i = 0; i < this->m_count; i++)
		{
			if (m_items[i].hash >= 0)
			{
				this->m_index->Add(&m_items[i].key);
			}
		}

		//����
		return this->m_index;
	}

	//��������Values
	UsnList* Values()
	{
		//ɾ���б�������Ŀ
		this->m_list->RemoveAll();

		//ȡ��ֵ�������б���
		for (int32 i = 0; i < this->m_count; i++)
		{
			if (m_items[i].hash >= 0)
			{
				this->m_list->Add(&m_items[i].value);
			}
		}

		//����
		return this->m_list;
	}

	//��ѯֵ��֧��ͨ���*��?
	UsnList* Query(tchar* filter)
	{
		//ɾ���б�������Ŀ
		m_list->RemoveAll();

		//ȡ��ֵ�������б���
		for (int32 i = 0; i < this->m_count; i++)
		{
			if ((this->m_items[i].hash >= 0) && qstrmatch(this->m_items[i].value.name, filter))
			{
				m_list->Add(&this->m_items[i].value);
			}
		}

		//����
		return m_list;
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

		//��������
		if (this->m_index != NULL)
		{
			delete this->m_index;
			this->m_index = NULL;
		}

		//�����б�
		if (this->m_list != NULL)
		{
			delete this->m_list;
			this->m_list = NULL;
		}
	}

	//��ʼ��
	_INLINE_ tbool Initialize(int32 capacity)
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
			return FALSE; //�ڴ����ʧ��
		}
		else
		{
			for (int32 i = 0; i < size; i++)
			{
				newItems[i].hash = -1;
				newItems[i].next = -1;
				newItems[i].key = 0;
				newItems[i].value.name = NULL;
				newItems[i].value.pfrn = 0;
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
			return FALSE; //�ڴ����ʧ��
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

		//����Usn����
		this->m_index = new UsnIndex();

		//����Usn�б�
		this->m_list = new UsnList();

		//����
		return TRUE;
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
		dicitem* newItems = (dicitem*)m_mem.Alloc(sizeof(dicitem) * newSize);
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
				newItems[i].key = 0;
				newItems[i].value.name = NULL;
			}
		}

		//�����µĴ��ۣ�����ʼ��Ϊδʹ��
		//int32 *newBuckets = new int32[newSize];
		//int32 *newBuckets = (int32*)malloc(sizeof(int32) * newSize);
		int32* newBuckets = (int32*)m_mem.Alloc(sizeof(int32) * newSize);
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
	_INLINE_ int32 FindItem(uint64& key)
	{
		//���hashcode
		//uint64 hash = HashHelpers::GetHashCode(key);
		uint64 hash = key;

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
			int32 capacity = (int32)((ullAvailVirtual * 0.5) / blocks);
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

#endif //__USNCOMMON_H_

