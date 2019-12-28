/*
	模块：字符串
	作者：denglf
	日期：2019-04-24
	说明：
	参考：MonaOS：https://github.com/higepon/mona https://sourceforge.net/projects/monaos/
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
    
	//默认构造函数
	TString();

	//自定构造函数
	//text 初始化文本
	//length 文本长度，可选。-1表示拷贝text中的所有字符，>=0表只拷贝text中length长度的字符
    TString(const tchar* text, int32 length = -1);

	//引用拷贝
    TString(const TString& text);

	//析构函数，释放资源
    virtual ~TString();

	//返回字符串的长度（字符数量）
    int32 getLength() const { return this->length; }

	//返回字符串的大小（字节数量）
    int32 getSize() const { return this->length * sizeof(tchar); }

	//[]操作符重载，支持访问字符串中的单个字符
	tchar operator [](int32 index) const;

	//char*重载，支持返回char*，以赋值给其它char*
	operator const tchar*() const { return this->buffer; }

	//==操作符重载，支持与其它char*类型进行比较
	tbool operator ==(const tchar* text) const;

	//==操作符重载，支持与其它TString类型进行比较
	tbool operator ==(const TString& text) const;

	//!=操作符重载，支持与其它char*类型进行比较
	inline tbool operator !=(const tchar* text) const { return !this->operator ==(text); }
	
	//!=操作符重载，支持与其它TString类型进行比较
	inline tbool operator !=(const TString& text) const { return !this->operator ==(text); }

	//=操作符重载，支持其它char*类型进行赋值
	TString& operator =(const tchar* text);

	//=操作符重载，支持其它TString类型进行赋值
	TString& operator =(const TString& text);

	//+=操作符重载，支持与其它tchar*类型进行连接
	void operator +=(const tchar* text);

	//+=操作符重载，支持与其它TString类型进行连接
	void operator +=(const TString& text);

	//+=操作符重载，支持与其它tchar类型进行连接
	void operator +=(tchar ch);

	//+操作符重载，支持与其它tchar*类型进行连接
	TString operator +(const tchar* text) const;

	//+操作符重载，支持与其它TString类型进行连接
	TString operator +(const TString& text) const;

	//检测字符串是否以指定的前缀开始
	tbool startsWith(const TString& value) const;

	//检测字符串是否以指定的后缀结束
	tbool endsWith(const TString& value) const;

	//查找某个字符的位置。相当于VB6的InStr函数。
	//ch 要查找的字符，例如：'a'
	//from 从指定位置开始查找
	int32 indexOf(tchar ch, int32 from = 0) const;

	//查找某个字符串的位置。相当于VB6的InStr函数。
	//value 要查找的字符串，例如："abc"
	//from 从指定位置开始查找
	int32 indexOf(const TString& value, int32 from = 0) const;

	//从最后位置开始查找某个字符的位置。相当于VB6的InStrRev函数。
	//ch 要查找的字符，例如：'a'
	//from 从指定位置开始查找
	int32 lastIndexOf(tchar ch, int32 from = -1) const;

	//从最后位置开始查找某个字符串的位置。相当于VB6的InStrRev函数。
	//value 要查找的字符串，例如："abc"
	//from 从指定位置开始查找
	int32 lastIndexOf(const TString& value, int32 from = -1) const;

	//从指定位置开始返回指定长度(字符数量)的字符串。相当于VB6的Mid函数。
	//start 开始位置
	//length 长度(数量)
	TString substr(int32 start, int32 length) const;

	//返回字符串左边指定长度(字符数量)的字符串。相当于VB6的Left函数。
	TString left(int32 length);

	//返回字符串右边指定长度(字符数量)的字符串。相当于VB6的Right函数。
	TString right(int32 length);

	//返回无前导空格的字符串。相当于VB6的LTrim函数。
	TString ltrim();

	//返回无后缀空格的字符串。相当于VB6的RTrim函数。
	TString rtrim();

	//返回无前导空格和后缀空格的字符串。相当于VB6的Trim函数。
	TString trim();

	//按字符分割字符串。相当于VB6的Split函数。
	TArray<TString> split(tchar ch) const;

	//按字符串分割字符串。相当于VB6的Split函数。
	TArray<TString> split(const TString& value) const;

	//返回转换成小写的字符串。相当于VB6的LCase函数。
	TString toLower() const;

	//返回转换成大写的字符串。相当于VB6的Ucase函数。
	TString toUpper() const;
	
	//返回当前字符串指针(不能修改)
	tchar* toString() const;

	//反转字符串
	void reserve();

	//重置（清除）字符串
	int32 reset();

	//插入字符串
	int32 insert(int32 start, const TString& text);

	//删除指定长度的字符串
	int32 remove(int32 start, int32 length);
	
	//是否与filter指定的通配符匹配，支持*和?通配符。
	tbool match(const tchar* filter);

	//格式化输出
	tchar* format(const tchar* fmt, ...);

protected:
    tchar* buffer;
    int32 length;

};

extern TString operator +(const tchar* text1, const TString& text2);

#endif  //__STRING_H__
