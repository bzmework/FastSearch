/*
模块： cstrui.c
功能： 转换unsigned int类型值为字符串。
参数： val，要转换的unsigned int类型值。
	  out，缓存转换后的字符。容纳unsigned int类型值的缓存，其大小至少是12*sizeof(tchar)个字节。
返回： 指向out的指针。
说明： 输出不同的进制应该用printf/sprintf替代。
版本： 2019-05-20 denglf 模块创建
*/

#include "../typedef.h"
#include "../typecvt.h"

tchar* cstrui(uint32 val, tchar* out)
{
	tchar* left = out;	//左边位置
	tchar* p = out;		//当前位置
	uint32 n;			//当前数字
	tchar c;			//当前字符

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