/*
模块： cstrl.c
功能： 转换long类型值为字符串。
参数： val，要转换的long类型值。
	  out，缓存转换后的字符。容纳int类型值的缓存，其大小至少是12*sizeof(tchar)个字节。
返回： 指向out的指针。
说明： cstrl()与标准函数ltoa功能相同，但去掉了输出不同进制(2/8/10/16)的功能，输出不同的进制应该用printf/sprintf替代。
版本： 2019-05-20 denglf 模块创建
*/

#include "../typedef.h"
#include "../typecvt.h"

tchar* cstrl(long val, tchar* out)
{
	tchar* left = out;	//左边位置
	tchar* p = out;		//当前位置
	long n;				//当前数字
	tchar c;			//当前字符

	//负数须转换成正数
	if (val < 0)
	{
		val = -val; //转换成正数
		*p = __T('-'); //在输出缓存写入负号
		left = ++p; //向前移动一个位置
	}

	//将数字转换成字符
	do
	{
		n = val % 10; //取得个位
		val /= 10; //去掉个位
		*p++ = _c_tochr(n);//转换成'0'-'9'之间的字符
	} while (val > 0);
	*p-- = NUL;//必须添加结束符并后移指针

	//反转'-'到'\0'之间的数字为正确的顺序
	do
	{
		c = *left;
		*left++ = *p;
		*p-- = c;
	} while (left < p);

	//返回
	return out;
}