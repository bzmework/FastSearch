/*
	模块：各种字符移位等操作
	作者：denglf
	日期：2019-04-24
	说明：
*/


#include "../typedef.h"

//高字
static int32 hiword(int32 n) 
{ 
	return ((n >> 0x10) & 0xffff); 
}

//低字
static int32 loword(int32 n)
{ 
	return (n & 0xffff); 
}

//带符号位高字
static int32 hiwords(int32 n)
{ 
	return (int16)((n >> 0x10) & 0xffff); 
}

//带符号位低字
static int32 lowords(int32 n)
{ 
	return (int16)(n & 0xffff); 
}

//制作long型
static int32 makelong(int32 low, int32 high)
{ 
	return ((high << 0x10) | (low & 0xffff)); 
}

//是否数字
tbool isadigit(tchar c)
{
	return ((c) >= __T('0') && (c) <= __T('9'));
}
