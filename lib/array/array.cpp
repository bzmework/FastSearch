#ifndef __ARRAY_T__
#define __ARRAY_T__

#include "array.h"

//
// Ĭ�Ϲ��캯��
//
template <class T>
TArray<T>::TArray()
{
	this->m_data = NULL;
	this->m_itemCount = 0;
	this->m_refCount = NULL;
}

//
// count: ��ʼ����
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
// oSrcArray: ԭ����
// ���Ӻ�����������ʱ����������ô˹��캯���Կ���������
// ����ʵ�����ü����ĺ��ļ�����
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
// ��������������Ŀ
//
template <class T>
T& TArray<T>::Item(int32 index) const
{
	return this->m_data[index];
}

//
// ���������Ŀ��һ�����������Ŀ�����鲻����ʹ��
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
// ������Ŀ����
//
template <class T>
int32 TArray<T>::Count() const
{
	return this->m_itemCount;
}

//
// ��Ŀ�Ƿ����
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
// ����[]������,�����±귵����Ŀ
//
template <class T>
T& TArray<T>::operator[](int32 index)
{
	return this->m_data[index];
}

//
// ����=������, ���鸳ֵ
//
template <class T>
_INLINE_ TArray<T>& TArray<T>::operator =(const TArray<T>& oSrcArray)
{
	this->ReleaseRef(); //����ǰ�����ͷ�
	this->m_data = oSrcArray.m_data;
	this->m_itemCount = oSrcArray.m_itemCount;
	this->m_refCount = oSrcArray.m_refCount;
	this->AddRef();
	return *this;
}

//
// ����==������, �Ƚ������Ƿ����
//
template <class T>
_INLINE_ bool TArray<T>::operator ==(const TArray<T>& oSrcArray) const
{
	return this->m_data == oSrcArray.m_data;
}

//
// ����==������, �Ƚ������Ƿ񲻵�
//
template <class T>
_INLINE_ bool TArray<T>::operator !=(const TArray<T>& oSrcArray) const
{
	return this->m_data != oSrcArray.m_data;
}

//
// ����==������, �Ƚ������Ƿ����
//
template <class T>
_INLINE_ bool TArray<T>::operator ==(const TArray<T>* poSrcArray) const
{
	return this->m_data == poSrcArray->m_data;
}

//
// ����==������, �Ƚ������Ƿ񲻵�
//
template <class T>
_INLINE_ bool TArray<T>::operator !=(const TArray<T>* poSrcArray) const
{
	return this->m_data != poSrcArray->m_data;
}

//
// ��������
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
// �ͷ�����
//
template <class T>
void TArray<T>::ReleaseRef()
{
	if (this->m_refCount != NULL)
	{
		(*this->m_refCount)--;
		if (*this->m_refCount < 1) //����ǰ�������ͷ��Լ�
		{
			delete this->m_refCount;
			this->m_refCount = NULL;
			if (this->m_itemCount > 0 && this->m_data != NULL)
			{
				delete[] this->m_data;
				this->m_data = NULL;
			}
		}
		else //�����ã����ͷ�����
		{
			this->m_refCount = NULL;
			this->m_data = NULL;
		}
		this->m_itemCount = 0;
	}
}

#endif //__ARRAY_T__