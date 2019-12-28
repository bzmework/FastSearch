#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <malloc.h>
#include "../typedef.h"

/*
	模块：数组
	作者：denglf
	日期：2019-04-24
	说明：与列表不一样，数组在创建时必须指定容量，不能扩容。
*/

template <class T> class TArray
{
public:
	TArray();
	TArray(int32 count);
	TArray(const TArray<T>& array);
	virtual ~TArray();

public:

	//根据索引返回项目
	_INLINE_ T& Item(int32 index) const;

	//删除所有项目
	_INLINE_ void Clear();

	//返回项目数量
	_INLINE_ int32 Count() const;

	//项目是否存在
	_INLINE_ bool Exists(T item) const;

	//根据下标返回项目
	_INLINE_ T& operator[](int32 index);

	//数组赋值
	_INLINE_ TArray<T>& operator =(const TArray<T>& oSrcArray);   //按引用

	//数组比较
	_INLINE_ bool operator ==(const TArray<T>& oSrcArray) const;  //按引用
	_INLINE_ bool operator !=(const TArray<T>& oSrcArray) const;
	_INLINE_ bool operator ==(const TArray<T>* poSrcArray) const; //按地址
	_INLINE_ bool operator !=(const TArray<T>* poSrcArray) const;

protected:
	T* m_data;				//项目数组
	int32 m_itemCount;		//项目数量
	int32* m_refCount;		//引用次数

private:
	const int32 maxCount = 0x7FEFFFFF;	//最大数量 =2146435071=21亿，已经足够大了
	void AddRef();			//增加引用
	void ReleaseRef();		//释放引用
};

#endif //__ARRAY_H__
