/*
	ģ�飺���Զ���
	���ߣ�denglf
	���ڣ�2019-05-20
	˵�������ڵ���
	�ο���
*/

#ifndef __ASSERT_H__
#define __ASSERT_H__

#include <stdio.h>

#ifdef _DEBUG
	#define ASSERT(cond) if (!cond) printf("%s:%d: null pointer exception!\n", __FILE__, __LINE__);
#else
	#define ASSERT(condition) 
#endif

#endif //__ASSERT_H__