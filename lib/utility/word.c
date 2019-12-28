/*
	ģ�飺�����ַ���λ�Ȳ���
	���ߣ�denglf
	���ڣ�2019-04-24
	˵����
*/


#include "../typedef.h"

//����
static int32 hiword(int32 n) 
{ 
	return ((n >> 0x10) & 0xffff); 
}

//����
static int32 loword(int32 n)
{ 
	return (n & 0xffff); 
}

//������λ����
static int32 hiwords(int32 n)
{ 
	return (int16)((n >> 0x10) & 0xffff); 
}

//������λ����
static int32 lowords(int32 n)
{ 
	return (int16)(n & 0xffff); 
}

//����long��
static int32 makelong(int32 low, int32 high)
{ 
	return ((high << 0x10) | (low & 0xffff)); 
}

//�Ƿ�����
tbool isadigit(tchar c)
{
	return ((c) >= __T('0') && (c) <= __T('9'));
}
