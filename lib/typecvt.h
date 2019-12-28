
/*
	模块：常用的各种类型转换
	作者：denglf
	日期：2019-05-20
	说明：
	参考：
*/

#ifndef __TYPECVT_H__
#define __TYPECVT_H__

#include "typedef.h"

//字符是否是标准ASCII
#define _c_isascii(c)			((uint16)(c) < 0x80) //0x80=128

//字符是否是扩展ASCII
#define _c_isasciiex(c)			((uint16)(c) > 0x80) //0x80=128

//字符是否为0-9之间的数字
#define _c_isdigit(c)			((c) >= __T('0') && (c) <= __T('9'))

//字符是否为0-9之间的数字
#define _c_isnum(c)				((c) >= __T('0') && (c) <= __T('9'))

//转换字符为标准ASCII
#define _c_toascii(c)			((c) & 0x7f) //0x7f=127

//转换'0'-'9'字符为数字
#define _c_tonum(c)				(c - __T('0'))

//转换数字为'0'-'9'字符
#define _c_tochr(n)				(tchar)(n + __T('0'))

//转换数字为'a'-'z'字符
#define _c_tochrz(n)			(tchar)(n - 10 + __T('a'))

//转换字符为小写
#define _c_tolower(c)			((c) - __T('A') + __T('a'))

//转换字符为大写
#define _c_toupper(c)			((c) - __T('a') + __T('A'))

//字符是否为空格符、制表符、回车、换行、纵向制表符和换页符
// 0x20,空格符;0x0d(\r),回车;0x0a(\n),换行;0x09(\t),制表符;0x0b(\v),纵向制表符;0x0c(\f),换页符.
#define _c_isspace(c)			(c == 0x20 || c == 0x0d || c == 0x0a || c == 0x09 || c == 0x0b || c == 0x0c)

//检查有符号整型数是否溢出
#define _int_overflow(n, v) ((n >= (_INT_MAX / 10)) && (v > (_INT_MAX % 10)))

//检查有符号长整型数是否溢出
#define _long_overflow(n, v) ((n >= (_LONG_MAX / 10)) && (v > (_LONG_MAX % 10)))

//检查有符号64位整型数是否溢出
#define _int64_overflow(n, v) ((n >= (_LLONG_MAX / 10)) && (v > (_LLONG_MAX % 10)))

//检查无符号整型数是否溢出
#define _uint_overflow(n, v) ((n >= (_UINT_MAX / 10)) && (v > (_UINT_MAX % 10)))

//检查无符号长整型数是否溢出
#define _ulong_overflow(n, v) ((n >= (_ULONG_MAX / 10)) && (v > (_ULONG_MAX % 10)))

//检查无符号长整型数是否溢出
#define _uint64_overflow(n, v) ((n >= (_ULLONG_MAX / 10)) && (v > (_ULLONG_MAX % 10)))

#endif //__TYPECVT_H__