/*
	模块：断言定义
	作者：denglf
	日期：2019-05-20
	说明：用于调试
	参考：
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