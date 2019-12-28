#ifndef __LIST_T__
#define __LIST_T__

#include "list.h"

//
// Ĭ�Ϲ��캯��
//
template <class T> 
TList<T>::TList() 
{
	//��ʼ������
	this->m_itemCount = 0;
	this->m_totalCount = 0;
	this->m_increase = 0;
	this->m_data = NULL;

	//��ʼ������
    this->Initialize(defaultCount, 0);
}

//
// count: ��ʼ����
//
template <class T> 
TList<T>::TList(uint32 count) 
{
	//��ʼ������
	this->m_itemCount = 0;
	this->m_totalCount = 0;
	this->m_increase = 0;
	this->m_data = NULL;

	//��ʼ������
    this->Initialize(count, 0);
}

//
// count: ��ʼ����
// increase: ÿ������ʱ�������������Ϊ0���Զ�������
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
// ������Ŀ
//
template <class T> 
void TList<T>::Add(T &item) 
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
			newCount = (this->m_itemCount == 0) ? defaultCount : this->m_itemCount * 2;//��������������2��������΢���㷨��
			if (newCount > maxCount) newCount = maxCount; //�������
		}

        //���·����ڴ��С
		T* newData = new T[newCount]; //ע��: new��malloc��������new������ڴ�ᱻ��ʼ��(�൱��calloc)��malloc�򲻻ᡣ

        //����ԭ����Ԫ�ص��µ�Ԫ��������
		uint32 count = this->m_itemCount;
        for (uint32 i = 0; i < count; i++) 
		{
			newData[i] = this->m_data[i];
        }

        delete[] this->m_data;//ɾ���������ݣ���ֹ�ڴ�й©
		this->m_data = newData;//ָ���µ��ڴ��ַ
		this->m_totalCount = newCount;//�����µ�����
    }

    //������Ԫ��
    this->m_data[this->m_itemCount] = item;
    this->m_itemCount++;
}

//
// ��������������Ŀ
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
// ����[]������,�����±귵����Ŀ
//
template <class T> 
T TList<T>::operator[](uint32 index) 
{
    return this->Item(index);
}

//
// ɾ����Ŀ
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
// ��������ɾ��ָ����Ŀ
//
template <class T> 
T TList<T>::RemoveAt(uint32 index) 
{
    //��������Ƿ�Խ��
    if (index < 0 || index >= this->m_itemCount) 
	{
        return (T)NULL;
    }

    //�������Ƴ���Ԫ��
    T toRemove = this->m_data[index];

	//�Ƴ�Ԫ��
	int count = this->m_itemCount;
    for (uint32 i = index; i < count - 1; i++) 
	{
        this->m_data[i] = this->m_data[i + 1];
    }
    this->m_itemCount--;

	//�����ѱ��Ƴ���Ԫ��
    return toRemove;
}

//
// ɾ��������Ŀ
//
template <class T> 
void TList<T>::RemoveAll() 
{
    if(this->m_data != NULL) delete[] this->m_data;
	this->Initialize(defaultCount, this->m_increase);
}

//
// ������Ŀ����
//
template <class T> 
uint32 TList<T>::Count() const 
{
    return this->m_itemCount;
}

//
// ��Ŀ�Ƿ����
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
// ��ʼ���б�
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