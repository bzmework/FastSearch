/*
	ģ�飺�ַ���
	���ߣ�denglf
	���ڣ�2019-04-24
	˵����
	�ο���MonaOS��https://github.com/higepon/mona https://sourceforge.net/projects/monaos/
*/

#ifndef __STRING_H__
#define __STRING_H__

#include "../typedef.h"
#include "../utility/utility.h"
#include "../array/array.h"
#include "../array/array.cpp"

class TString
{
public:
    
	//Ĭ�Ϲ��캯��
	TString();

	//�Զ����캯��
	//text ��ʼ���ı�
	//length �ı����ȣ���ѡ��-1��ʾ����text�е������ַ���>=0��ֻ����text��length���ȵ��ַ�
    TString(const tchar* text, int32 length = -1);

	//���ÿ���
    TString(const TString& text);

	//�����������ͷ���Դ
    virtual ~TString();

	//�����ַ����ĳ��ȣ��ַ�������
    int32 getLength() const { return this->length; }

	//�����ַ����Ĵ�С���ֽ�������
    int32 getSize() const { return this->length * sizeof(tchar); }

	//[]���������أ�֧�ַ����ַ����еĵ����ַ�
	tchar operator [](int32 index) const;

	//char*���أ�֧�ַ���char*���Ը�ֵ������char*
	operator const tchar*() const { return this->buffer; }

	//==���������أ�֧��������char*���ͽ��бȽ�
	tbool operator ==(const tchar* text) const;

	//==���������أ�֧��������TString���ͽ��бȽ�
	tbool operator ==(const TString& text) const;

	//!=���������أ�֧��������char*���ͽ��бȽ�
	inline tbool operator !=(const tchar* text) const { return !this->operator ==(text); }
	
	//!=���������أ�֧��������TString���ͽ��бȽ�
	inline tbool operator !=(const TString& text) const { return !this->operator ==(text); }

	//=���������أ�֧������char*���ͽ��и�ֵ
	TString& operator =(const tchar* text);

	//=���������أ�֧������TString���ͽ��и�ֵ
	TString& operator =(const TString& text);

	//+=���������أ�֧��������tchar*���ͽ�������
	void operator +=(const tchar* text);

	//+=���������أ�֧��������TString���ͽ�������
	void operator +=(const TString& text);

	//+=���������أ�֧��������tchar���ͽ�������
	void operator +=(tchar ch);

	//+���������أ�֧��������tchar*���ͽ�������
	TString operator +(const tchar* text) const;

	//+���������أ�֧��������TString���ͽ�������
	TString operator +(const TString& text) const;

	//����ַ����Ƿ���ָ����ǰ׺��ʼ
	tbool startsWith(const TString& value) const;

	//����ַ����Ƿ���ָ���ĺ�׺����
	tbool endsWith(const TString& value) const;

	//����ĳ���ַ���λ�á��൱��VB6��InStr������
	//ch Ҫ���ҵ��ַ������磺'a'
	//from ��ָ��λ�ÿ�ʼ����
	int32 indexOf(tchar ch, int32 from = 0) const;

	//����ĳ���ַ�����λ�á��൱��VB6��InStr������
	//value Ҫ���ҵ��ַ��������磺"abc"
	//from ��ָ��λ�ÿ�ʼ����
	int32 indexOf(const TString& value, int32 from = 0) const;

	//�����λ�ÿ�ʼ����ĳ���ַ���λ�á��൱��VB6��InStrRev������
	//ch Ҫ���ҵ��ַ������磺'a'
	//from ��ָ��λ�ÿ�ʼ����
	int32 lastIndexOf(tchar ch, int32 from = -1) const;

	//�����λ�ÿ�ʼ����ĳ���ַ�����λ�á��൱��VB6��InStrRev������
	//value Ҫ���ҵ��ַ��������磺"abc"
	//from ��ָ��λ�ÿ�ʼ����
	int32 lastIndexOf(const TString& value, int32 from = -1) const;

	//��ָ��λ�ÿ�ʼ����ָ������(�ַ�����)���ַ������൱��VB6��Mid������
	//start ��ʼλ��
	//length ����(����)
	TString substr(int32 start, int32 length) const;

	//�����ַ������ָ������(�ַ�����)���ַ������൱��VB6��Left������
	TString left(int32 length);

	//�����ַ����ұ�ָ������(�ַ�����)���ַ������൱��VB6��Right������
	TString right(int32 length);

	//������ǰ���ո���ַ������൱��VB6��LTrim������
	TString ltrim();

	//�����޺�׺�ո���ַ������൱��VB6��RTrim������
	TString rtrim();

	//������ǰ���ո�ͺ�׺�ո���ַ������൱��VB6��Trim������
	TString trim();

	//���ַ��ָ��ַ������൱��VB6��Split������
	TArray<TString> split(tchar ch) const;

	//���ַ����ָ��ַ������൱��VB6��Split������
	TArray<TString> split(const TString& value) const;

	//����ת����Сд���ַ������൱��VB6��LCase������
	TString toLower() const;

	//����ת���ɴ�д���ַ������൱��VB6��Ucase������
	TString toUpper() const;
	
	//���ص�ǰ�ַ���ָ��(�����޸�)
	tchar* toString() const;

	//��ת�ַ���
	void reserve();

	//���ã�������ַ���
	int32 reset();

	//�����ַ���
	int32 insert(int32 start, const TString& text);

	//ɾ��ָ�����ȵ��ַ���
	int32 remove(int32 start, int32 length);
	
	//�Ƿ���filterָ����ͨ���ƥ�䣬֧��*��?ͨ�����
	tbool match(const tchar* filter);

	//��ʽ�����
	tchar* format(const tchar* fmt, ...);

protected:
    tchar* buffer;
    int32 length;

};

extern TString operator +(const tchar* text1, const TString& text2);

#endif  //__STRING_H__
