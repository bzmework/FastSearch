#ifndef __LIST_T__
#define __LIST_T__

#include "list.h"

//
// 默认构造函数
//
template <class T> 
TList<T>::TList() 
{
	//初始化变量
	this->m_itemCount = 0;
	this->m_totalCount = 0;
	this->m_increase = 0;
	this->m_data = NULL;

	//初始化数量
    this->Initialize(defaultCount, 0);
}

//
// count: 初始数量
//
template <class T> 
TList<T>::TList(uint32 count) 
{
	//初始化变量
	this->m_itemCount = 0;
	this->m_totalCount = 0;
	this->m_increase = 0;
	this->m_data = NULL;

	//初始化数量
    this->Initialize(count, 0);
}

//
// count: 初始数量
// increase: 每次扩容时增长数量，如果为0则自动增长。
//
template <class T> 
TList<T>::TList(uint32 count, uint32 increase) 
{
    this->Initialize(count, increase);
}

template <class T> 
TList<T>::~TList() 
{
    delete[] this->m_data;
}

//
// 增加项目
//
template <class T> 
void TList<T>::Add(T &item) 
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
			newCount = (this->m_itemCount == 0) ? defaultCount : this->m_itemCount * 2;//按照已有数量的2倍增长（微软算法）
			if (newCount > maxCount) newCount = maxCount; //最大容量
		}

        //重新分配内存大小
		T* newData = new T[newCount]; //注意: new和malloc的区别是new分配的内存会被初始化(相当于calloc)，malloc则不会。

        //复制原来的元素到新的元素数组中
		uint32 count = this->m_itemCount;
        for (uint32 i = 0; i < count; i++) 
		{
			newData[i] = this->m_data[i];
        }

        delete[] this->m_data;//删除已有数据，防止内存泄漏
		this->m_data = newData;//指向新的内存地址
		this->m_totalCount = newCount;//保持新的数量
    }

    //增加新元素
    this->m_data[this->m_itemCount] = item;
    this->m_itemCount++;
}

//
// 根据索引返回项目
//
template <class T> 
T TList<T>::Item(uint32 index) const 
{
    if (index < 0 || index >= this->m_itemCount) 
	{
        return (T)NULL;
    }
	else
	{
		return this->m_data[index];
	}
}

//
// 重载[]操作符,根据下标返回项目
//
template <class T> 
T TList<T>::operator[](uint32 index) 
{
    return this->Item(index);
}

//
// 删除项目
//
template <class T> 
T TList<T>::Remove(T &item) 
{
    uint32 count = this->m_itemCount;
    for (uint32 i = 0; i < count; i++) 
	{
        if (this->m_data[i] == item) 
		{
            return (this->RemoveAt(i));
        }
    }
    return (T)NULL;
}

//
// 根据索引删除指定项目
//
template <class T> 
T TList<T>::RemoveAt(uint32 index) 
{
    //检查索引是否越界
    if (index < 0 || index >= this->m_itemCount) 
	{
        return (T)NULL;
    }

    //拷贝待移除的元素
    T toRemove = this->m_data[index];

	//移除元素
	int count = this->m_itemCount;
    for (uint32 i = index; i < count - 1; i++) 
	{
        this->m_data[i] = this->m_data[i + 1];
    }
    this->m_itemCount--;

	//返回已被移除的元素
    return toRemove;
}

//
// 删除所有项目
//
template <class T> 
void TList<T>::RemoveAll() 
{
    if(this->m_data != NULL) delete[] this->m_data;
	this->Initialize(defaultCount, this->m_increase);
}

//
// 返回项目数量
//
template <class T> 
uint32 TList<T>::Count() const 
{
    return this->m_itemCount;
}

//
// 项目是否存在
//
template <class T> 
bool TList<T>::Exists(T item) const
{
    uint32 count = this->m_itemCount;
    for (uint32 i = 0; i < count; i++) 
	{
        if (this->m_data[i] == item)
		{
            return true;
        }
    }
    return false;
}

//
// 初始化列表
//
template <class T> 
void TList<T>::Initialize(uint32 count, uint32 increase)
{
    this->m_itemCount = 0;
    this->m_totalCount = count > 0 ? count : defaultCount;
    this->m_increase = increase > 0 ? increase : 0;
    this->m_data = new T[this->m_totalCount];
}

#endif //__LIST_T__