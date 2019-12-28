#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <malloc.h>
#include "../typedef.h"

/*
	ģ�飺����
	���ߣ�denglf
	���ڣ�2019-04-24
	˵�������б�һ���������ڴ���ʱ����ָ���������������ݡ�
*/

template <class T> class TArray
{
public:
	TArray();
	TArray(int32 count);
	TArray(const TArray<T>& array);
	virtual ~TArray();

public:

	//��������������Ŀ
	_INLINE_ T& Item(int32 index) const;

	//ɾ��������Ŀ
	_INLINE_ void Clear();

	//������Ŀ����
	_INLINE_ int32 Count() const;

	//��Ŀ�Ƿ����
	_INLINE_ bool Exists(T item) const;

	//�����±귵����Ŀ
	_INLINE_ T& operator[](int32 index);

	//���鸳ֵ
	_INLINE_ TArray<T>& operator =(const TArray<T>& oSrcArray);   //������

	//����Ƚ�
	_INLINE_ bool operator ==(const TArray<T>& oSrcArray) const;  //������
	_INLINE_ bool operator !=(const TArray<T>& oSrcArray) const;
	_INLINE_ bool operator ==(const TArray<T>* poSrcArray) const; //����ַ
	_INLINE_ bool operator !=(const TArray<T>* poSrcArray) const;

protected:
	T* m_data;				//��Ŀ����
	int32 m_itemCount;		//��Ŀ����
	int32* m_refCount;		//���ô���

private:
	const int32 maxCount = 0x7FEFFFFF;	//������� =2146435071=21�ڣ��Ѿ��㹻����
	void AddRef();			//��������
	void ReleaseRef();		//�ͷ�����
};

#endif //__ARRAY_H__
