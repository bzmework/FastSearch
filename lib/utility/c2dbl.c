/*
模块： c2dbl.c
功能： 转换字符串为double类型值。
参数： str，要转换的字符串。
返回： 返回double值。
说明： c2dbl()与标准函数strtod功能一致。
	  c2dbl()会扫描参数str字符串，跳过前面的空格字符，直到遇上数字或正负符号才开始做转换，
	  到出现非数字或字符串结束时('\0')才结束转换，并将结果返回；
	  若endptr不为NULL，则会将遇到不合条件而终止的str中的字符指针由endptr传回；
	  参数str字符串可包含正负号、小数点或E(e)来表示指数部分。如123.456或123e-2或123.456e2。
版本： 2019-05-20 denglf 模块创建
*/

#include <errno.h>
#include <math.h>
#include <float.h>

#include "../typedef.h"
#include "../typecvt.h"

double c2dbl(const tchar* str/*, tchar** endptr*/)
{
	double number = 0.0;
	int32 num_digits = 0;
	int32 num_decimals = 0;	//小数位数
	int32 exponent = 0;		//指数
	int32 negative;			//负号 
	tchar* p = (tchar*)str;
	double p10;
	int32 n;

	//跳过前导空格等
	while (_c_isspace(*p))
	{
		p++;
	}

	//处理正负符号
	negative = 0;
	switch (*p)
	{
	case '-': negative = 1; //下降到增量位置
	case '+': p++;//跳过正负符号
	}

	//转换字符为数字
	//number*10: 将number扩大10倍，目的是加上个位数，为什么不扩大100倍，因为只需要加两位数的0-9。
	//例如我们要将2019-05转换成201905，只需要将年*100加上月即可，即2019*100+5，既满足了转换又起到了格式化的效果。
	//将数扩大n倍+x(0<x<n),这是转换字符串为数字的常用算法和技巧。
	while (_c_isdigit(*p))
	{
		number = number * 10.0 + _c_tonum((double)*p); 
		p++;
		num_digits++; 
	}

	//处理小数部分
	//例如：123.456
	if (*p == '.')
	{
		p++;
		while (_c_isdigit(*p))
		{
			number = number * 10.0 + _c_tonum((double)*p);
			p++;
			num_digits++;
			num_decimals++; //记录小数位数
		}

		exponent -= num_decimals;
	}

	//无任何数字
	if (num_digits == 0)
	{
		errno = ERANGE;
		return 0.0;
	}

	//加上负号
	if (negative)
	{
		number = -number;
	}

	//处理指数
	//例如：123e-2
	if (*p == 'e' || *p == 'E')
	{
		//处理正负符号
		negative = 0;
		switch (*++p)
		{
		case '-': negative = 1;  //下降到增量位置
		case '+': p++; //跳过正负符号
		}

		//转换字符为数字
		n = 0;
		while (_c_isdigit(*p))
		{
			n = n * 10 + _c_tonum(*p);
			p++;
		}

		//处理指数
		if (negative)
		{
			exponent -= n;
		}
		else {
			exponent += n;
		}
	}

	//指数溢出
	if (exponent < DBL_MIN_EXP || exponent > DBL_MAX_EXP)
	{
		errno = ERANGE;
		return HUGE_VAL;
	}

	//将指数部分附加到number上
	p10 = 10.0;
	n = exponent;
	if (n < 0) 
		n = -n;
	while (n) 
	{
		if (n & 1) //求余，相当于：n % 2；
		{
			if (exponent < 0) 
			{
				number /= p10;
			}
			else 
			{
				number *= p10;
			}
		}
		n >>= 1;//整除，相当于：(int)(n / 2);
		p10 *= p10;
	}

	//溢出
	if (number == HUGE_VAL)
		errno = ERANGE;
	
	//if (endptr) 
	//	*endptr = p;

	return number;
}