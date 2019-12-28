/*
模块： c2uint.c
功能： 转换字符串为unsigned int类型值。
参数： str，要转换的字符串。
返回： 返回int值。
说明： c2ulng()与标准函数strtoul功能一致。 
	  c2uint()会扫描参数str字符串，跳过前面的空格字符，直到遇上数字或正负符号才开始做转换，
	  而再遇到非数字或字符串结束时('\0')就结束转换，并将结果返回。
版本： 2019-05-20 denglf 模块创建
*/

#include "../typedef.h"
#include "../typecvt.h"

uint32 c2uint(const tchar* str)
{
	const tchar* p = str;
	uint32 number;			//当前合计数
	int32 negative;			//负号 
	int16 c;				//当前字符，字符最多占用两个字节，所以定义成int16

	//跳过前导空格等字符
	while (_c_isspace(*p))
	{
		p++;
	}

	//处理正负符号
	negative = 0;
	switch (*p)
	{
	case '-': negative = 1; //下降到+位置
	case '+': p++;//跳过正负符号
	}

	//转换字符为数字
	//number * 10: 将number扩大10倍，目的是加上个位数，为什么不扩大100倍，因为只需要加两位数的0-9。
	//例如我们要将2019-05转换成201905，只需要将年*10加上月即可，即2019*10+5，既满足了转换又起到了格式化的效果。
	//将数扩大n倍+x(0<x<n),这是转换字符串为数字的常用算法和技巧。
	number = 0;
	c = (int16)*p;
	while (_c_isdigit(c))
	{
		if (_uint_overflow(number, c)) return _UINT_MAX; //溢出
		number = number * 10 + _c_tonum(c); //数字累加
		c = (int16)*(++p); //获取下一个字符
	}

	//返回结果
	return negative ? _UINT_MAX - number + 1 : number;
}
 

