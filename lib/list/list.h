#ifndef __LIST_H__
#define __LIST_H__

#include <malloc.h>
#include "../typedef.h"

/*
	模块：列表
	作者：denglf
	日期：2019-04-24
	说明：列表的实质是一个数组。
	本代码参考了微软的.NET Frameworks list.cs源码。
*/

template <class T> class TList
{
public:
	TList();
    TList(uint32 count);
    TList(uint32 count, uint32 increase);
    virtual ~TList();

public:

	//增加项目
	_INLINE_ void Add(T &item);

	//根据索引返回项目
	_INLINE_ T Item(uint32 index) const;

	//根据下标返回项目
	_INLINE_ T operator[](uint32 index);

	//删除项目
	_INLINE_ T Remove(T &item);

	//根据索引删除项目
	_INLINE_ T RemoveAt(uint32 index);

	//删除所有项目
	_INLINE_ void RemoveAll();

	//返回项目数量
	_INLINE_ uint32 Count() const;

	//项目是否存在
	_INLINE_ bool Exists(T item) const;

private:
	const uint32 defaultCount = 3;			//默认数量
	const uint32 defaultIncrease = 1024;	//默认增长数量
	const uint32 maxCount = 0x7FEFFFFF;		//最大数量 =2146435071=21亿，已经足够大了

    T* m_data;				//项目数组
    uint32 m_totalCount;	//总的数量(=列表中实际已填满的元素数量+列表中未使用的数量)
    uint32 m_itemCount;		//项目数量(=列表中实际已填满的元素数量)
    uint32 m_increase;		//增长数量

    void Initialize(uint32 count, uint32 increase); //初始化列表
};

#endif //__LIST_H__
