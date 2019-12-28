/*
	ģ�飺�ַ���
	���ߣ�denglf
	���ڣ�2019-04-24
	˵����
	�ο���MonaOS��https://github.com/higepon/mona https://sourceforge.net/projects/monaos/
*/

#ifndef __WSTRING_H__
#define __WSTRING_H__

#include "../typedef.h"
#include "../utility/utility.h"
#include "../array/array.h"
#include "../array/array.cpp"

class WString
{
public:

	//Ĭ�Ϲ��캯��
	WString();

	//�Զ����캯��
	//text ��ʼ���ı�
	//length �ı����ȣ���ѡ��-1��ʾ����text�е������ַ���>=0��ֻ����text��length���ȵ��ַ�
	WString(const wchar* text, int32 length = -1);

	//���ÿ���
	WString(const WString& text);

	//�����������ͷ���Դ
	virtual ~WString();

	//�����ַ����ĳ��ȣ��ַ�������
	int32 getLength() const { return this->length; }

	//�����ַ����Ĵ�С���ֽ�������
	int32 getSize() const { return this->length * sizeof(wchar); }

	//[]���������أ�֧�ַ����ַ����еĵ����ַ�
	wchar operator [](int32 index) const;

	//char*���أ�֧�ַ���char*���Ը�ֵ������char*
	operator const wchar* () const { return this->buffer; }

	//==���������أ�֧��������char*���ͽ��бȽ�
	tbool operator ==(const wchar* text) const;

	//==���������أ�֧��������TString���ͽ��бȽ�
	tbool operator ==(const WString& text) const;

	//!=���������أ�֧��������char*���ͽ��бȽ�
	inline tbool operator !=(const wchar* text) const { return !this->operator ==(text); }

	//!=���������أ�֧��������TString���ͽ��бȽ�
	inline tbool operator !=(const WString& text) const { return !this->operator ==(text); }

	//=���������أ�֧������char*���ͽ��и�ֵ
	WString& operator =(const wchar* text);

	//=���������أ�֧������TString���ͽ��и�ֵ
	WString& operator =(const WString& text);

	//+=���������أ�֧��������tchar*���ͽ�������
	void operator +=(const wchar* text);

	//+=���������أ�֧��������TString���ͽ�������
	void operator +=(const WString& text);

	//+=���������أ�֧��������tchar���ͽ�������
	void operator +=(wchar ch);

	//+���������أ�֧��������tchar*���ͽ�������
	WString operator +(const wchar* text) const;

	//+���������أ�֧��������TString���ͽ�������
	WString operator +(const WString& text) const;

	//����ַ����Ƿ���ָ����ǰ׺��ʼ
	tbool startsWith(const WString& value) const;

	//����ַ����Ƿ���ָ���ĺ�׺����
	tbool endsWith(const WString& value) const;

	//����ĳ���ַ���λ�á��൱��VB6��InStr������
	//ch Ҫ���ҵ��ַ������磺'a'
	//from ��ָ��λ�ÿ�ʼ����
	int32 indexOf(wchar ch, int32 from = 0) const;

	//����ĳ���ַ�����λ�á��൱��VB6��InStr������
	//value Ҫ���ҵ��ַ��������磺"abc"
	//from ��ָ��λ�ÿ�ʼ����
	int32 indexOf(const WString& value, int32 from = 0) const;

	//�����λ�ÿ�ʼ����ĳ���ַ���λ�á��൱��VB6��InStrRev������
	//ch Ҫ���ҵ��ַ������磺'a'
	//from ��ָ��λ�ÿ�ʼ����
	int32 lastIndexOf(wchar ch, int32 from = -1) const;

	//�����λ�ÿ�ʼ����ĳ���ַ�����λ�á��൱��VB6��InStrRev������
	//value Ҫ���ҵ��ַ��������磺"abc"
	//from ��ָ��λ�ÿ�ʼ����
	int32 lastIndexOf(const WString& value, int32 from = -1) const;

	//��ָ��λ�ÿ�ʼ����ָ������(�ַ�����)���ַ������൱��VB6��Mid������
	//start ��ʼλ��
	//length ����
	WString substr(int32 start, int32 length) const;

	//�����ַ������ָ������(�ַ�����)���ַ������൱��VB6��Left������
	WString left(int32 length);

	//�����ַ����ұ�ָ������(�ַ�����)���ַ������൱��VB6��Right������
	WString right(int32 length);

	//������ǰ���ո���ַ������൱��VB6��LTrim������
	WString ltrim();

	//�����޺�׺�ո���ַ������൱��VB6��RTrim������
	WString rtrim();

	//������ǰ���ո�ͺ�׺�ո���ַ������൱��VB6��Trim������
	WString trim();

	//���ַ��ָ��ַ������൱��VB6��Split������
	TArray<WString> split(wchar ch) const;

	//���ַ����ָ��ַ������൱��VB6��Split������
	TArray<WString> split(const WString& value) const;

	//����ת����Сд���ַ������൱��VB6��LCase������
	WString toLower() const;

	//����ת���ɴ�д���ַ������൱��VB6��Ucase������
	WString toUpper() const;

	//���ص�ǰ�ַ���ָ��(�����޸�)
	wchar* toString() const;

	//��ת�ַ���
	void reserve();

	//���ã�������ַ���
	int32 reset();

	//�����ַ���
	int32 insert(int32 start, const WString& text);

	//ɾ��ָ�����ȵ��ַ���
	int32 remove(int32 start, int32 length);

	//�Ƿ���filterָ����ͨ���ƥ�䣬֧��*��?ͨ�����
	tbool match(const wchar* filter);

	//��ʽ�����
	//wchar* format(const wchar* fmt, ...);

protected:
	wchar* buffer;
	int32 length;

};

extern WString operator +(const wchar* text1, const WString& text2);

#endif  //__WSTRING_H__
