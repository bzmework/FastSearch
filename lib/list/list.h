#ifndef __LIST_H__
#define __LIST_H__

#include <malloc.h>
#include "../typedef.h"

/*
	ģ�飺�б�
	���ߣ�denglf
	���ڣ�2019-04-24
	˵�����б��ʵ����һ�����顣
	������ο���΢���.NET Frameworks list.csԴ�롣
*/

template <class T> class TList
{
public:
	TList();
    TList(uint32 count);
    TList(uint32 count, uint32 increase);
    virtual ~TList();

public:

	//������Ŀ
	_INLINE_ void Add(T &item);

	//��������������Ŀ
	_INLINE_ T Item(uint32 index) const;

	//�����±귵����Ŀ
	_INLINE_ T operator[](uint32 index);

	//ɾ����Ŀ
	_INLINE_ T Remove(T &item);

	//��������ɾ����Ŀ
	_INLINE_ T RemoveAt(uint32 index);

	//ɾ��������Ŀ
	_INLINE_ void RemoveAll();

	//������Ŀ����
	_INLINE_ uint32 Count() const;

	//��Ŀ�Ƿ����
	_INLINE_ bool Exists(T item) const;

private:
	const uint32 defaultCount = 3;			//Ĭ������
	const uint32 defaultIncrease = 1024;	//Ĭ����������
	const uint32 maxCount = 0x7FEFFFFF;		//������� =2146435071=21�ڣ��Ѿ��㹻����

    T* m_data;				//��Ŀ����
    uint32 m_totalCount;	//�ܵ�����(=�б���ʵ����������Ԫ������+�б���δʹ�õ�����)
    uint32 m_itemCount;		//��Ŀ����(=�б���ʵ����������Ԫ������)
    uint32 m_increase;		//��������

    void Initialize(uint32 count, uint32 increase); //��ʼ���б�
};

#endif //__LIST_H__
