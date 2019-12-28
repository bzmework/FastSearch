#ifndef __ARRAY_T__
#define __ARRAY_T__

#include "array.h"

//
// 默认构造函数
//
template <class T>
TArray<T>::TArray()
{
	this->m_data = NULL;
	this->m_itemCount = 0;
	this->m_refCount = NULL;
}

//
// count: 初始数量
//
template <class T>
TArray<T>::TArray(int32 count)
{
	this->m_data = NULL;
	this->m_itemCount = 0;
	this->m_refCount = NULL;
	if (count > 0)
	{
		if (count > maxCount) count = maxCount;
		this->m_data = new T[count];
		this->m_itemCount = count;
		this->m_refCount = new int32(1);
	}
}

//
// oSrcArray: 原数组
// 当从函数返回数组时编译器会调用此构造函数以拷贝副本，
// 这是实现引用计数的核心技术。
//
template <class T>
TArray<T>::TArray(const TArray<T>& oSrcArray)
{
	this->m_data = NULL;
	this->m_itemCount = 0;
	this->m_refCount = NULL;
	this->m_data = oSrcArray.m_data;
	this->m_itemCount = oSrcArray.m_itemCount;
	this->m_refCount = oSrcArray.m_refCount;
	this->AddRef();
}

template <class T>
TArray<T>::~TArray()
{
	this->ReleaseRef();
}

//
// 根据索引返回项目
//
template <class T>
T& TArray<T>::Item(int32 index) const
{
	return this->m_data[index];
}

//
// 清除所有项目，一旦清除所有项目，数组不能再使用
//
template <class T>
void TArray<T>::Clear()
{
	if (this->m_itemCount > 0)
	{
		if (this->m_data != NULL)
		{
			delete[] this->m_data;
			this->m_data = NULL;
		}
		this->m_itemCount = 0;
		this->m_refCount = NULL;
	}
}

//
// 返回项目数量
//
template <class T>
int32 TArray<T>::Count() const
{
	return this->m_itemCount;
}

//
// 项目是否存在
//
template <class T>
bool TArray<T>::Exists(T item) const
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
// 重载[]操作符,根据下标返回项目
//
template <class T>
T& TArray<T>::operator[](int32 index)
{
	return this->m_data[index];
}

//
// 重载=操作符, 数组赋值
//
template <class T>
_INLINE_ TArray<T>& TArray<T>::operator =(const TArray<T>& oSrcArray)
{
	this->ReleaseRef(); //引用前必须释放
	this->m_data = oSrcArray.m_data;
	this->m_itemCount = oSrcArray.m_itemCount;
	this->m_refCount = oSrcArray.m_refCount;
	this->AddRef();
	return *this;
}

//
// 重载==操作符, 比较数组是否相等
//
template <class T>
_INLINE_ bool TArray<T>::operator ==(const TArray<T>& oSrcArray) const
{
	return this->m_data == oSrcArray.m_data;
}

//
// 重载==操作符, 比较数组是否不等
//
template <class T>
_INLINE_ bool TArray<T>::operator !=(const TArray<T>& oSrcArray) const
{
	return this->m_data != oSrcArray.m_data;
}

//
// 重载==操作符, 比较数组是否相等
//
template <class T>
_INLINE_ bool TArray<T>::operator ==(const TArray<T>* poSrcArray) const
{
	return this->m_data == poSrcArray->m_data;
}

//
// 重载==操作符, 比较数组是否不等
//
template <class T>
_INLINE_ bool TArray<T>::operator !=(const TArray<T>* poSrcArray) const
{
	return this->m_data != poSrcArray->m_data;
}

//
// 增加引用
//
template <class T>
void TArray<T>::AddRef()
{
	if (this->m_refCount != NULL)
	{
		(*this->m_refCount)++;
	}
}

//
// 释放引用
//
template <class T>
void TArray<T>::ReleaseRef()
{
	if (this->m_refCount != NULL)
	{
		(*this->m_refCount)--;
		if (*this->m_refCount < 1) //引用前，必须释放自己
		{
			delete this->m_refCount;
			this->m_refCount = NULL;
			if (this->m_itemCount > 0 && this->m_data != NULL)
			{
				delete[] this->m_data;
				this->m_data = NULL;
			}
		}
		else //已引用，则释放引用
		{
			this->m_refCount = NULL;
			this->m_data = NULL;
		}
		this->m_itemCount = 0;
	}
}

#endif //__ARRAY_T__