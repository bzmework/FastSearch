#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

// ������ʶ
#define _INLINE_ __forceinline

// ���������ֵ
#ifndef _MAX_DRIVE
#define _MAX_DRIVE 3
#endif

// ·�����ֵ
#ifndef _MAX_PATH
	#define _MAX_PATH 260
#endif

// Ŀ¼���ֵ
#ifndef _MAX_DIR
#define _MAX_DIR 256
#endif

// �ļ������ֵ
#ifndef _MAX_FNAME
#define _MAX_FNAME 256
#endif

// �ļ���չ�����ֵ
#ifndef _MAX_EXT
#define _MAX_EXT 256
#endif

// Ŀ¼�ָ���.Windows:"\";UNIX:"/".
#if defined(WIN32) || defined(OS2) || defined(__DOS__)
	#define DIR_MARKER '\\'
	#define DIR_MARKER_STR "\\"
#else
	#define DIR_MARKER '/'
	#define DIR_MARKER_STR "/"
#endif

// ���з�.Windows:"\r\n";UNIX:"\r".
#if defined(WIN32) || defined(OS2) || defined(__DOS__)
	#define LINE_END_MARKER_STR "\r\n"
#else
	#define LINE_END_MARKER_STR "\n"
#endif

// �����ַ���ת��
#if _UNICODE
	#define __T(x)      L ## x
#else
	#define __T(x)      x
#endif
#define __W(x)      L ## x

// NUL����
#ifndef NUL
	#ifdef WIN32
		#define NUL 0
	#else
		#define NUL '\0'
	#endif
#endif

// NULL����
#ifndef NULL
	#ifdef  __cplusplus
		#define NULL    0
	#else
		#define NULL    ((void *)0)
	#endif
#endif

// ����ֵ����
#ifndef FALSE
	#define FALSE		0
#endif
#ifndef TRUE
	#define TRUE		1
#endif

// ���������������
#define _in_
#define _out_
#define _inout_

//������������ȡֵ��Χ
#define _CHAR_BIT      8								// number of bits in a char
#define _SCHAR_MIN   (-128)								// minimum signed char value
#define _SCHAR_MAX     127								// maximum signed char value
#define _UCHAR_MAX     0xff								// maximum unsigned char value

#define _SHRT_MIN    (-32768)							// minimum (signed) short value
#define _SHRT_MAX      32767							// maximum (signed) short value
#define _USHRT_MAX     0xffff							// maximum unsigned short value

#define _INT_MIN     (-2147483647 - 1)					// minimum (signed) int value
#define _INT_MAX      2147483647						// maximum (signed) int value
#define _UINT_MAX     0xffffffff						// maximum unsigned int value

#define _LONG_MIN    (-2147483647L - 1)					// minimum (signed) long value
#define _LONG_MAX      2147483647L						// maximum (signed) long value
#define _ULONG_MAX     0xffffffffUL						// maximum unsigned long value

#define _LLONG_MIN   (-9223372036854775807i64 - 1)		// minimum signed long long int value
#define _LLONG_MAX     9223372036854775807i64			// maximum signed long long int value
#define _ULLONG_MAX    0xffffffffffffffffui64			// maximum unsigned long long int value

/* ��׼���Ͷ���							�ֽڳ���			�ȼ۵�C��׼��<stdint.h>�еĶ���	*/		
typedef signed char				int8;   //1�ֽ�			typedef signed char        int8_t;
typedef signed short			int16;	//2�ֽ�			typedef short              int16_t;	
typedef signed int				int32;	//4�ֽ�			typedef int                int32_t;
typedef signed __int64			int64;	//8�ֽ�			typedef long long          int64_t;

typedef unsigned char			uint8;	//1�ֽ�			typedef unsigned char      uint8_t;
typedef unsigned short			uint16;	//2�ֽ�			typedef unsigned short     uint16_t;
typedef unsigned int			uint32;	//4�ֽ�			typedef unsigned int       uint32_t;
typedef unsigned __int64		uint64;	//8�ֽ�			typedef unsigned long long uint64_t;

//typedef long					long;	//4�ֽ�
typedef unsigned long			ulong;	//4�ֽ�

typedef long long				intmax;	//�������		typedef long long          intmax_t;
typedef unsigned long long		uintmax;//�޷����������	typedef unsigned long long uintmax_t;

typedef long double				real;	//ʵ������

#ifdef _WIN64 
	typedef unsigned char		tbool;
	typedef unsigned __int64	tint;
	typedef __int64				tptrdiff;
	typedef __int64				tintptr;
#else
	typedef unsigned char		tbool;
	typedef unsigned int		tint;
	typedef int					tptrdiff;
	typedef int					tintptr;
#endif
	 
/* �ֽ����Ͷ���							�ֽڳ���			�ȼ۵�Windows#include<windef.h>�еĶ��� */
typedef unsigned char			uchar;	//1�ֽ�			typedef unsigned char		BYTE;
typedef unsigned char			byte;	//1�ֽ�			typedef unsigned char		BYTE;
typedef unsigned short			word;	//2�ֽ�			typedef unsigned short		WORD;
typedef unsigned long			dword;	//4�ֽ�			typedef unsigned long		DWORD;
typedef unsigned short			wchar;	//2�ֽ�(���ֽ�)	typedef wchar_t				WCHAR
typedef unsigned short			wbyte;	//2�ֽ�(���ֽ�)	typedef wchar_t				WCHAR

#if _UNICODE
	typedef unsigned short		tchar;
	typedef unsigned short		tbyte;
	typedef unsigned short		utchar; //wchar_t = unsigned short
	typedef unsigned short		utbyte; //wchar_t = unsigned short
#else
	typedef char				tchar;
	typedef char				tbyte;
	typedef unsigned char		utchar;
	typedef unsigned char		utbyte;
#endif

/* 

��׼���ͺ�Windows����ӳ��
��׼����					Windows����
-----------------------------------------
byte						BYTE
word						WORD
dword						DWORD
char						CHAR
wchar						WCHAR
tchar						TCHAR
wbyte						TBYTE
-----------------------------------------
int16(short)				SHORT
int32(int)					INT
int64						LONGLONG
uint16						USHORT
uint32						UINT
uint64						ULONGLONG
long						LONG
ulong						ULONG

*/


#endif //__TYPEDEF_H__