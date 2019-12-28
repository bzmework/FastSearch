
/*

	模块：USN通用，包括：索引，列表，字典
	作者：denglf
	日期：2019-04-24
	说明：本代码基于dictionary.h模板字典，用来存储USN日志文件信息，目的是提供最佳性能
*/

#ifndef __USNCOMMON_H_
#define __USNCOMMON_H_

#include <windows.h>
#include "..\utility\utility.h"
#include "..\string\string.h"
#include "..\dictionary\HashHelpers.h"
#include "..\dictionary\VMemAlloc.h"

//文件项目
typedef struct
{
	uint64	 pfrn;		// 父文件参照号ParentFileReferenceNumber
	TString  name;		// 文件名称
} FileItem;

//usn索引列表
class UsnIndex
{
public:

	// 默认构造函数
	UsnIndex()
	{
		//初始化变量
		this->m_itemCount = 0;
		this->m_totalCount = 0;
		this->m_increase = 0;
		this->m_data = NULL;

		//初始化数量
		this->Initialize(defaultCount, 0);
	}

	// 构造函数
	// count: 初始数量
	UsnIndex(uint32 count)
	{
		//初始化变量
		this->m_itemCount = 0;
		this->m_totalCount = 0;
		this->m_increase = 0;
		this->m_data = NULL;

		//初始化数量
		this->Initialize(count, 0);
	}

	// 构造函数
	// count: 初始数量
	// increase: 每次扩容时增长数量，如果为0则自动增长。
	UsnIndex(uint32 count, uint32 increase)
	{
		this->Initialize(count, increase);
	}

	// 析构函数
	virtual ~UsnIndex()
	{
		if (this->m_data != NULL)
		{
			m_mem.Free(this->m_data);
			this->m_data = NULL;
		}
	}

public:

	// 增加项目
	_INLINE_ void Add(uint64* item)
	{
		//元素数组已满则扩容
		if (this->m_totalCount == this->m_itemCount)
		{
			//重新分配元素数组
			uint32 newCount = 0;
			if (this->m_increase > 0)
			{
				newCount = (this->m_itemCount == 0) ? defaultCount : this->m_itemCount + this->m_increase;//按照指定值增长
				if (newCount > maxCount) newCount = maxCount; //最大容量
			}
			else
			{
				//newCount = (this->m_itemCount == 0) ? defaultCount : this->m_itemCount * 2;//按照已有数量的2倍增长（微软算法）
				newCount = (this->m_itemCount == 0) ? defaultCount : this->GetCapacity();//按照质数增长（微软字典算法）
				if (newCount > maxCount) newCount = maxCount; //最大容量
			}

			//重新分配内存大小
			//注意: new和malloc的区别是new分配的内存会被初始化(相当于calloc)，malloc则不会。
			uint64** newData = (uint64**)m_mem.Alloc(newCount * sizeof(uint64*));
			if (newCount == NULL)
			{
				return;
			}

			//复制原来的元素到新的元素数组中
			uint32 count = this->m_itemCount;
			for (uint32 i = 0; i < count; i++)
			{
				newData[i] = this->m_data[i];
			}

			m_mem.Free(this->m_data);//删除已有数据，防止内存泄漏
			this->m_data = newData;//指向新的内存地址
			this->m_totalCount = newCount;//保持新的数量
		}

		//增加新元素
		this->m_data[this->m_itemCount] = item;
		this->m_itemCount++;
	}

	// 根据索引返回项目
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

	// 重载[]操作符,根据下标返回项目
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

	// 删除所有项目
	_INLINE_ void RemoveAll()
	{
		if (this->m_data != NULL)
		{
			m_mem.Free(this->m_data);
			this->m_data = NULL;
		}
		this->Initialize(defaultCount, this->m_increase);
	}

	// 返回项目数量
	_INLINE_ uint32 Count() const
	{
		return this->m_itemCount;
	}

	// 项目是否存在
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

	//获得一个有效的增长容量
	_INLINE_ int32 GetCapacity()
	{
		//取得新扩字节数
		int32 newCapacity = HashHelpers::ExpandPrime(this->m_itemCount);//新扩容量
		int32 blocks = sizeof(FileItem); //每个项目占用的字节数
		DWORDLONG newSize = static_cast<DWORDLONG>(blocks) * static_cast<DWORDLONG>(newCapacity);//字节数

		//取得内存可用空间
		MEMORYSTATUSEX ms;
		ms.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&ms);
		DWORDLONG ullAvailPhys = ms.ullAvailPhys; //字节数
		DWORDLONG ullAvailVirtual = ms.ullAvailVirtual; //字节数

		//已经超出了可用的虚拟内存，但还没有超过可用的物理内存，按可用虚拟内存的50%申请内存
		if (newSize >= ullAvailVirtual && newSize < ullAvailPhys)
		{
			//还可以申请的容量
			uint32 capacity = (uint32)((ullAvailVirtual * 0.5) / blocks);
			//重新计算新括容量(获得一个>=m_capacity 的最小素数)
			capacity = HashHelpers::CalcPrime(capacity);
			if (capacity > this->m_itemCount)
			{
				newCapacity = capacity;
			}
		}

		//返回
		return newCapacity;
	}

private:
	const uint32 defaultCount = 3;			//默认数量
	const uint32 defaultIncrease = 1024;	//默认增长数量
	const uint32 maxCount = 0x7FEFFFFF;		//最大数量 =2146435071=21亿，已经足够大了

	VMemAlloc m_mem;		//内存管理
	uint64** m_data;		//项目数组(存放uint64项目的地址)
	uint32 m_totalCount;	//总的数量(=列表中实际已填满的元素数量+列表中未使用的数量)
	uint32 m_itemCount;		//项目数量(=列表中实际已填满的元素数量)
	uint32 m_increase;		//增长数量

	// 初始化列表
	void Initialize(uint32 count, uint32 increase)
	{
		this->m_itemCount = 0;
		this->m_totalCount = count > 0 ? count : defaultCount;
		this->m_increase = increase > 0 ? increase : 0;
		this->m_data = (uint64**)m_mem.Alloc(this->m_totalCount * sizeof(uint64*));
	}
};

//usn项目列表
class UsnList
{
public:

	// 默认构造函数
	UsnList()
	{
		//初始化变量
		this->m_itemCount = 0;
		this->m_totalCount = 0;
		this->m_increase = 0;
		this->m_data = NULL;

		//初始化数量
		this->Initialize(defaultCount, 0);
	}

	// 构造函数
	// count: 初始数量
	UsnList(uint32 count)
	{
		//初始化变量
		this->m_itemCount = 0;
		this->m_totalCount = 0;
		this->m_increase = 0;
		this->m_data = NULL;

		//初始化数量
		this->Initialize(count, 0);
	}

	// 构造函数
	// count: 初始数量
	// increase: 每次扩容时增长数量，如果为0则自动增长。
	UsnList(uint32 count, uint32 increase)
	{
		this->Initialize(count, increase);
	}

	// 析构函数
	virtual ~UsnList()
	{
		if (this->m_data != NULL)
		{
			m_mem.Free(this->m_data);
			this->m_data = NULL;
		}
	}

public:

	// 增加项目
	_INLINE_ void Add(FileItem* item)
	{
		//元素数组已满则扩容
		if (this->m_totalCount == this->m_itemCount)
		{
			//重新分配元素数组
			uint32 newCount = 0;
			if (this->m_increase > 0)
			{
				newCount = (this->m_itemCount == 0) ? defaultCount : this->m_itemCount + this->m_increase;//按照指定值增长
				if (newCount > maxCount) newCount = maxCount; //最大容量
			}
			else
			{
				//newCount = (this->m_itemCount == 0) ? defaultCount : this->m_itemCount * 2;//按照已有数量的2倍增长（微软算法）
				newCount = (this->m_itemCount == 0) ? defaultCount : this->GetCapacity();//按照质数增长（微软字典算法）
				if (newCount > maxCount) newCount = maxCount; //最大容量
			}

			//重新分配内存大小
			//注意: new和malloc的区别是new分配的内存会被初始化(相当于calloc)，malloc则不会。
			FileItem** newData = (FileItem**)m_mem.Alloc(newCount * sizeof(FileItem*)); 
			if (newCount == NULL)
			{
				return;
			}

			//复制原来的元素到新的元素数组中
			uint32 count = this->m_itemCount;
			for (uint32 i = 0; i < count; i++)
			{
				newData[i] = this->m_data[i];
			}

			m_mem.Free(this->m_data);//删除已有数据，防止内存泄漏
			this->m_data = newData;//指向新的内存地址
			this->m_totalCount = newCount;//保持新的数量
		}

		//增加新元素
		this->m_data[this->m_itemCount] = item;
		this->m_itemCount++;
	}

	// 根据索引返回项目
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

	// 重载[]操作符,根据下标返回项目
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

	// 删除所有项目
	_INLINE_ void RemoveAll()
	{
		if (this->m_data != NULL)
		{
			m_mem.Free(this->m_data);
			this->m_data = NULL;
		}
		this->Initialize(defaultCount, this->m_increase);
	}

	// 返回项目数量
	_INLINE_ uint32 Count() const
	{
		return this->m_itemCount;
	}

	// 项目是否存在
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

	//获得一个有效的增长容量
	_INLINE_ int32 GetCapacity()
	{
		//取得新扩字节数
		int32 newCapacity = HashHelpers::ExpandPrime(this->m_itemCount);//新扩容量
		int32 blocks = sizeof(FileItem); //每个项目占用的字节数
		DWORDLONG newSize = static_cast<DWORDLONG>(blocks) * static_cast<DWORDLONG>(newCapacity);//字节数

		//取得内存可用空间
		MEMORYSTATUSEX ms;
		ms.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&ms);
		DWORDLONG ullAvailPhys = ms.ullAvailPhys; //字节数
		DWORDLONG ullAvailVirtual = ms.ullAvailVirtual; //字节数

		//已经超出了可用的虚拟内存，但还没有超过可用的物理内存，按可用虚拟内存的50%申请内存
		if (newSize >= ullAvailVirtual && newSize < ullAvailPhys)
		{
			//还可以申请的容量
			uint32 capacity = (uint32)((ullAvailVirtual * 0.5) / blocks);
			//重新计算新括容量(获得一个>=m_capacity 的最小素数)
			capacity = HashHelpers::CalcPrime(capacity);
			if (capacity > this->m_itemCount)
			{
				newCapacity = capacity;
			}
		}

		//返回
		return newCapacity;
	}

private:
	const uint32 defaultCount = 3;			//默认数量
	const uint32 defaultIncrease = 1024;	//默认增长数量
	const uint32 maxCount = 0x7FEFFFFF;		//最大数量 =2146435071=21亿，已经足够大了

	VMemAlloc m_mem;		//内存管理
	FileItem** m_data;		//项目数组(存放FileItem项目的地址)
	uint32 m_totalCount;	//总的数量(=列表中实际已填满的元素数量+列表中未使用的数量)
	uint32 m_itemCount;		//项目数量(=列表中实际已填满的元素数量)
	uint32 m_increase;		//增长数量
	
	// 初始化列表
	void Initialize(uint32 count, uint32 increase)
	{
		this->m_itemCount = 0;
		this->m_totalCount = count > 0 ? count : defaultCount;
		this->m_increase = increase > 0 ? increase : 0;
		this->m_data = (FileItem**)m_mem.Alloc(this->m_totalCount * sizeof(FileItem*));
	}
};

//usn字典
class UsnDictionary
{
private:

	//字典项目,微软VB6字典称为键值对keyitem_pair
	//占用4+4+16*2=40字节
	struct dicitem
	{
		int32	 next;		// 下一个项目的索引, -1表示当前已经是最后一个项目
		uint64	 hash;		// 哈希码, -1表示项目已经被删除
		uint64	 key;		// 项目key
		FileItem value;		// 项目value
	};

	int32* m_buckets;		// 所有储槽
	dicitem* m_items;		// 所有项目

	int32	m_capacity;		// 字典的容量（哈希表的元素总数，即储槽总数）
	int32	m_count;		// 字典中项目的数量
	int32	m_delIndex;		// 被删除项目的索引
	int32	m_delCount;		// 被删除项目的数量

	VMemAlloc m_mem;		// 内存管理
	UsnIndex* m_index;		// Usn索引
	UsnList* m_list;		// Usn列表
	
public:

	UsnDictionary()
	{
		//初始化成员变量
		m_buckets = NULL;
		m_items = NULL;
		m_capacity = 0;
		m_count = 0;
		m_delIndex = 0;
		m_delCount = 0;

		//初始化其它信息
		this->Initialize(DefaultHashTable);
	}

	//capacity: 字典容量
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

	//销毁所有资源
	virtual ~UsnDictionary()
	{
		this->Destory();
	}

	//增加键值
	void Add(uint64& key, uint64& pfrn, tchar* name)
	{
		//获得hashcode
		//uint64 hash = HashHelpers::GetHashCode(key);
		uint64 hash = key;

		//根据hashcode获得储槽位置
		int32 targetBucket = hash % this->m_capacity;

		//从储槽中查询每个项目:
		//如果当前储槽首次插入,由于其为未使用(-1),因此不会进入循环;
		//如果当前储槽中已经有项目，则进入循环
		if (this->m_buckets[targetBucket] >= 0)
		{
			for (int32 i = this->m_buckets[targetBucket]; i >= 0; i = this->m_items[i].next)
			{
				//校验项目的hash是否一致，比较项目的key是否相同
				if ((m_items[i].hash == hash) && (this->m_items[i].key == key))
				{
					//key存在的话将其值替换
					m_items[i].value.pfrn = pfrn;
					m_items[i].value.name = name;
					return;
				}
			}
		}

		int32 index = 0;
		if (m_delCount > 0)
		{
			//从删除项目中回收一个
			//由于在删除时并没有释放空间，因此空间可反复利用，也加快了插入的速度
			index = this->m_delIndex;//最后一个删除项目的索引
			this->m_delIndex = this->m_items[index].next;//指向下一个项目的索引
			this->m_delCount--;//删除数量减少
		}
		else
		{
			//如果项目数量已经等于项目总数(即items已经被填满)，则必须对buckets和items扩容
			//count为当前插入的项目总数，items.Length为分配的数量（=储槽的数量）
			//如果首次插入由于count=0，items.Length>0，因此不会进入，只有当插入的数量已经填满所有items时才会进入
			if (this->m_count == this->m_capacity)
			{
				//扩容，通过扩容可以看出，如果在新建字典时，指定了capacity的大小，则会大大地减少扩容的次数，
				//但是如果capacity设置过大，内存不足时会扩容失败，因此capacity应采用默认大小。
				if (this->Resize())
				{
					//扩容成功，重新定位到新的储槽，原来hash % m_capacity计算出来的储槽位置已经不正确
					targetBucket = hash % this->m_capacity;
				}
				else
				{
					//扩容失败，报内存不足错。
					return;
				}
			}

			//记录最后一次增加/删除后项目的总数，首次增加时count=0
			index = this->m_count;

			//将项目总数+1
			this->m_count++;
		}

		//将键值拷贝进来
		//注意：对于对象类型，例如TString，应重载=号运算符，实现值拷贝功能。
		this->m_items[index].key = key;
		this->m_items[index].value.pfrn = pfrn;
		this->m_items[index].value.name = name;

		//设置hashcode
		this->m_items[index].hash = hash;

		//将下一个(next)项目，设置成当前储槽中的项目索引,
		//这样，当储槽发生碰撞的时候，上次关联的项目索引，就成了当前项目的下一个项目，
		//查询时就能够正确地轮循储槽中的所有项目
		this->m_items[index].next = this->m_buckets[targetBucket];

		//现在将项目的索引位置记录在储槽中，便于根据储槽找到最后新增加的项目
		this->m_buckets[targetBucket] = index;
	}

	//删除项目
	tbool Remove(uint64& key)
	{
		if (this->m_buckets != NULL)
		{
			//获得hashcode
			//uint64 hash = HashHelpers::GetHashCode(key);
			uint64 hash = key;

			//根据hashcode获得储槽位置
			int32 bucket = hash % this->m_capacity;

			//删除
			if (this->m_buckets[bucket] >= 0)
			{
				int last = -1;
				for (int32 i = this->m_buckets[bucket]; i >= 0; last = i, i = this->m_items[i].next)
				{
					if ((this->m_items[i].hash == hash) && (key == m_items[i].key))
					{
						//将各个删除的项目串联起来，便于Add的时候回收再利用
						if (last < 0)
						{
							this->m_buckets[bucket] = this->m_items[i].next;
						}
						else
						{
							this->m_items[last].next = this->m_items[i].next;
						}

						//注意: 在删除时必须释放项目占用的内存
						this->m_items[i].hash = -1;//重置
						this->m_items[i].next = m_delIndex;//指向下一个项目
						this->m_items[i].key = 0;//清除键，通过设置NULL清除项目占用的内存，注意对象类型应该重载=号运算符实现NULL清除数据功能。
						this->m_items[i].value.name = NULL;//清除值，通过设置NULL清除项目占用的内存，注意对象类型应该重载=号运算符实现NULL清除数据功能。
						this->m_items[i].value.pfrn = 0;
						this->m_delIndex = i; //指向最后一个项目
						this->m_delCount++;
						return TRUE;
					}
				}
			}
		}

		return FALSE;
	}

	//删除所有项目
	void RemoveAll()
	{
		if (this->m_count > 0)
		{
			//重置所有储槽
			for (int32 i = 0; i < this->m_capacity; i++)
			{
				this->m_buckets[i] = -1;
			}

			//清除项目数据
			for (int32 i = 0; i < this->m_count; i++)
			{
				this->m_items[i].next = -1;
				this->m_items[i].hash = -1;
				this->m_items[i].key = 0;//清除键，通过设置NULL清除项目占用的内存，注意对象类型应该重载=号运算符实现NULL清除数据功能。
				this->m_items[i].value.name = NULL;//清除值，通过设置NULL清除项目占用的内存，注意对象类型应该重载=号运算符实现NULL清除数据功能。
				this->m_items[i].value.pfrn = 0;
			}

			//重置项目信息
			this->m_delIndex = -1;
			this->m_delCount = 0;
			this->m_count = 0;
		}
	}

	//获取值
	tbool GetValue(uint64& key, FileItem& value)
	{
		//查找value
		int32 i = this->FindItem(key);
		if (i >= 0)
		{
			value = this->m_items[i].value; //将值拷贝出去
			return TRUE;
		}

		//没有找到返回缺省值
		value.name = NULL;
		value.pfrn = 0;
		return FALSE;
	}

	//key是否存在
	tbool ContainsKey(uint64& key)
	{
		int32 i = this->FindItem(key);
		if (i >= 0)
		{
			return TRUE;
		}
		return FALSE;
	}

	//值是否存在
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

	//返回所有Keys
	UsnIndex* Keys()
	{
		//删除列表所有项目
		this->m_index->RemoveAll();
		
		//取出键，放入列表中
		for (int32 i = 0; i < this->m_count; i++)
		{
			if (m_items[i].hash >= 0)
			{
				this->m_index->Add(&m_items[i].key);
			}
		}

		//返回
		return this->m_index;
	}

	//返回所有Values
	UsnList* Values()
	{
		//删除列表所有项目
		this->m_list->RemoveAll();

		//取出值，放入列表中
		for (int32 i = 0; i < this->m_count; i++)
		{
			if (m_items[i].hash >= 0)
			{
				this->m_list->Add(&m_items[i].value);
			}
		}

		//返回
		return this->m_list;
	}

	//查询值，支持通配符*和?
	UsnList* Query(tchar* filter)
	{
		//删除列表所有项目
		m_list->RemoveAll();

		//取出值，放入列表中
		for (int32 i = 0; i < this->m_count; i++)
		{
			if ((this->m_items[i].hash >= 0) && qstrmatch(this->m_items[i].value.name, filter))
			{
				m_list->Add(&this->m_items[i].value);
			}
		}

		//返回
		return m_list;
	}

	//返回项目数量
	int32 Count()
	{
		return this->m_count;
	}

private:

	//销毁所有项目
	void Destory()
	{
		//清除数据
		this->RemoveAll();

		//销毁储槽等
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

		//销毁索引
		if (this->m_index != NULL)
		{
			delete this->m_index;
			this->m_index = NULL;
		}

		//销毁列表
		if (this->m_list != NULL)
		{
			delete this->m_list;
			this->m_list = NULL;
		}
	}

	//初始化
	_INLINE_ tbool Initialize(int32 capacity)
	{

		//设置字典的容量
		//实际上是设置hashtable的大小，它的大小决定发生碰撞的频率
		//越大发生的频率就越低，但相应的占用内存就会增加
		//设置容量的诀窍是，根据capacity从prime表中查找一个大于capacity的最小素数
		int32 size = HashHelpers::GetPrime(capacity);

		//先分配大的
		//创建容纳键值的项目s为hashtable的大小，并初始化所有项目
		//dicitem* newItems = new dicitem[size];
		//dicitem* newItems = (dicitem*)malloc(sizeof(dicitem) * size);
		dicitem* newItems = (dicitem*)m_mem.Alloc(sizeof(dicitem) * size);
		if (newItems == NULL)
		{
			return FALSE; //内存分配失败
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

		//创建储槽s(即hashtable)，并初始化所有储槽为未使用(-1)状态
		//int32* newBuckets = new int32[size]; //new的实质是malloc
		//int32* newBuckets = (int32*)malloc(sizeof(int32) * size);
		int32* newBuckets = (int32*)m_mem.Alloc(sizeof(int32) * size);
		if (newBuckets == NULL)
		{
			//释放newItems占用的内存防止内存泄漏
			if (newItems != NULL)
			{
				m_mem.Free(newItems);
			}
			return FALSE; //内存分配失败
		}
		else
		{
			for (int32 i = 0; i < size; i++)
			{
				newBuckets[i] = -1;
			}
		}

		//指向buckets和items
		this->m_buckets = newBuckets;
		this->m_items = newItems;

		//初始化其它信息
		this->m_delIndex = -1;
		this->m_delCount = 0;
		this->m_count = 0;

		//必须保存字典长度
		this->m_capacity = size;

		//创建Usn索引
		this->m_index = new UsnIndex();

		//创建Usn列表
		this->m_list = new UsnList();

		//返回
		return TRUE;
	}

	//扩容
	//括多少？括大于2*m_count的最小素数个
	_INLINE_ bool Resize()
	{
		//新扩容量
		int32 newSize = this->GetCapacity();

		//数据量大的先扩容
		//创建新的项目(数量=newBuckets.Length)，并初始化所有项目。
		//微软的字典算法设计虽然比其它的快，但其设计思想是用空间换时间，下面
		//千万级数据: （50000000 * 40）/ 1048576 ＝1907 MB。在内存不够充足的情况下，100%分配内存会失败。
		//dicitem *newItems = new dicitem[newSize];
		//dicitem *newItems = (dicitem*)malloc(sizeof(dicitem) * newSize);
		dicitem* newItems = (dicitem*)m_mem.Alloc(sizeof(dicitem) * newSize);
		if (newItems == NULL)
		{
			return 0; //内存分配失败
		}
		else
		{
			//初始化新项目
			for (int32 i = 0; i < newSize; i++)
			{
				newItems[i].hash = -1;
				newItems[i].next = -1;
				newItems[i].key = 0;
				newItems[i].value.name = NULL;
			}
		}

		//创建新的储槽，并初始化为未使用
		//int32 *newBuckets = new int32[newSize];
		//int32 *newBuckets = (int32*)malloc(sizeof(int32) * newSize);
		int32* newBuckets = (int32*)m_mem.Alloc(sizeof(int32) * newSize);
		if (newBuckets == NULL)
		{
			//释放newItems占用的内存防止内存泄漏
			if (newItems != NULL)
			{
				m_mem.Free(newItems);
			}
			return 0; //内存分配失败
		}
		else
		{
			//重新初始化
			for (int32 i = 0; i < newSize; i++)
			{
				newBuckets[i] = -1;
			}
		}

		//现在将所有旧的项目拷贝其到新建项目，
		memcpy(newItems, this->m_items, sizeof(dicitem) * this->m_count);

		//然后重新将项目(entry)与储槽(进行关联)，为什么要这么做？
		//因为hashcode % newSize（或hashcode % newBuckets.Length）以后，已经不是原来的储槽了，
		//所以必须重新进行关联。
		for (int32 i = 0; i < this->m_count; i++)
		{
			if (newItems[i].hash >= 0)
			{
				int32 bucket = newItems[i].hash % newSize;//重新计算bucket

				//设置下一个项目
				//当储槽发生碰撞（即不同key通过hash运算后得到了同一储槽）的时候,
				//如果储槽上已经关联了项目的索引，此时就将已关联项目作为新增项目的下一个项目，
				//正确的应该是成为当前新增项目的上一个项目，即使顺序被打乱了但这并不重要，
				//因为根据m_items数组的索引始终都能枚举出顺序增加的每一个项目。
				if (newBuckets[bucket] >= 0) //已发生关联(即发生碰撞)
				{
					newItems[i].next = newBuckets[bucket];//成为当前项目的下一个项目
				}
				else //无碰撞
				{
					newItems[i].next = -1; //已经是最后一个项目
				}

				//将当前项目索引与新的bucket储槽关联，便于通过bucket能快速找到自己
				newBuckets[bucket] = i;
			}
		}

		//释放原先占用的内存,防止内存泄漏
		m_mem.Free(this->m_buckets);
		m_mem.Free(this->m_items);

		//指向新的buckets和items
		this->m_buckets = newBuckets;
		this->m_items = newItems;

		//必须保存字典长度
		this->m_capacity = newSize;

		//返回
		return true;
	}

	//查找项目, 找到返回项目的索引,否则返回-1
	_INLINE_ int32 FindItem(uint64& key)
	{
		//获得hashcode
		//uint64 hash = HashHelpers::GetHashCode(key);
		uint64 hash = key;

		//根据hashcode获得储槽位置
		int32 bucket = hash % m_capacity;

		//取得储槽的位置i，如果储槽存在(i >= 0)，则开始在项目中搜索项目
		for (int32 i = this->m_buckets[bucket]; i >= 0; i = this->m_items[i].next)
		{
			//比较hash和key，看是否符合
			if ((this->m_items[i].hash == hash) && (this->m_items[i].key == key))
			{
				return i;
			}
		}

		return -1;//没有找到
	}

	//获得一个有效的增长容量
	_INLINE_ int32 GetCapacity()
	{
		//取得新扩字节数
		int32 newCapacity = HashHelpers::ExpandPrime(this->m_capacity);//新扩容量
		int32 blocks = sizeof(int32) + sizeof(dicitem); //每个储槽和项目占用的字节数
		DWORDLONG newSize = static_cast<DWORDLONG>(blocks) * static_cast<DWORDLONG>(newCapacity);//字节数

		//取得内存可用空间
		MEMORYSTATUSEX ms;
		ms.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&ms);
		DWORDLONG ullAvailPhys = ms.ullAvailPhys; //字节数
		DWORDLONG ullAvailVirtual = ms.ullAvailVirtual; //字节数

		//已经超出了可用的虚拟内存，但还没有超过可用的物理内存，按可用虚拟内存的50%申请内存
		if (newSize >= ullAvailVirtual && newSize < ullAvailPhys)
		{
			//还可以申请的容量
			int32 capacity = (int32)((ullAvailVirtual * 0.5) / blocks);
			//重新计算新括容量(获得一个>=m_capacity 的最小素数)
			capacity = HashHelpers::CalcPrime(capacity);
			if (capacity > this->m_capacity)
			{
				newCapacity = capacity;
			}
		}

		//返回
		return newCapacity;
	}
};

#endif //__USNCOMMON_H_

