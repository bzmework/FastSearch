
/*
	ģ�飺���õĸ�������ת��
	���ߣ�denglf
	���ڣ�2019-05-20
	˵����
	�ο���
*/

#ifndef __TYPECVT_H__
#define __TYPECVT_H__

#include "typedef.h"

//�ַ��Ƿ��Ǳ�׼ASCII
#define _c_isascii(c)			((uint16)(c) < 0x80) //0x80=128

//�ַ��Ƿ�����չASCII
#define _c_isasciiex(c)			((uint16)(c) > 0x80) //0x80=128

//�ַ��Ƿ�Ϊ0-9֮�������
#define _c_isdigit(c)			((c) >= __T('0') && (c) <= __T('9'))

//�ַ��Ƿ�Ϊ0-9֮�������
#define _c_isnum(c)				((c) >= __T('0') && (c) <= __T('9'))

//ת���ַ�Ϊ��׼ASCII
#define _c_toascii(c)			((c) & 0x7f) //0x7f=127

//ת��'0'-'9'�ַ�Ϊ����
#define _c_tonum(c)				(c - __T('0'))

//ת������Ϊ'0'-'9'�ַ�
#define _c_tochr(n)				(tchar)(n + __T('0'))

//ת������Ϊ'a'-'z'�ַ�
#define _c_tochrz(n)			(tchar)(n - 10 + __T('a'))

//ת���ַ�ΪСд
#define _c_tolower(c)			((c) - __T('A') + __T('a'))

//ת���ַ�Ϊ��д
#define _c_toupper(c)			((c) - __T('a') + __T('A'))

//�ַ��Ƿ�Ϊ�ո�����Ʊ�����س������С������Ʊ���ͻ�ҳ��
// 0x20,�ո��;0x0d(\r),�س�;0x0a(\n),����;0x09(\t),�Ʊ��;0x0b(\v),�����Ʊ��;0x0c(\f),��ҳ��.
#define _c_isspace(c)			(c == 0x20 || c == 0x0d || c == 0x0a || c == 0x09 || c == 0x0b || c == 0x0c)

//����з����������Ƿ����
#define _int_overflow(n, v) ((n >= (_INT_MAX / 10)) && (v > (_INT_MAX % 10)))

//����з��ų��������Ƿ����
#define _long_overflow(n, v) ((n >= (_LONG_MAX / 10)) && (v > (_LONG_MAX % 10)))

//����з���64λ�������Ƿ����
#define _int64_overflow(n, v) ((n >= (_LLONG_MAX / 10)) && (v > (_LLONG_MAX % 10)))

//����޷����������Ƿ����
#define _uint_overflow(n, v) ((n >= (_UINT_MAX / 10)) && (v > (_UINT_MAX % 10)))

//����޷��ų��������Ƿ����
#define _ulong_overflow(n, v) ((n >= (_ULONG_MAX / 10)) && (v > (_ULONG_MAX % 10)))

//����޷��ų��������Ƿ����
#define _uint64_overflow(n, v) ((n >= (_ULLONG_MAX / 10)) && (v > (_ULLONG_MAX % 10)))

#endif //__TYPECVT_H__