
/*
参考： SanOS cvt.c，GNU libc-4.6.27
模块： cstrd.c
功能： 转换double类型值为字符串。
参数： val，要转换的double类型值。
	  ndigits，要保留的小数位数。
	  out，缓存转换后的字符。容纳double类型值的缓存，其大小至少是352*sizeof(tchar)个字节。
返回： 指向out的指针。
说明： cstrd()与标准函数fcvt功能相似，去掉了输出参数：小数点位置(decpt)，正负符号(sign)。
版本： 2019-05-20 denglf 模块创建
*/

#include <math.h>
#include "../typecvt.h"
#include "../typedef.h"

//double：1bit（符号位）+ 11bits（指数位）+ 52bits（尾数位）
//double取值范围为: -1.79E+308 ~ +1.79E+308
//#define CVTBUFSIZE (308 + 1 + (52 - 11) + 2) //存储浮点数的缓存大小
#define CVTBUFSIZE (309 + 43) 

tchar* cstrd(double val, int32 ndigits, tchar* out)
{
	int32 n;
	double fint, fdec;
	double fi, fd;
	tchar* p = &out[0];
	tchar* p0, *p1, *pt;
	tchar c;

	//调整小数位数
	if (ndigits < 0)
	{
		ndigits = 0;
	}
	if (ndigits >= CVTBUFSIZE - 1)
	{
		ndigits = CVTBUFSIZE - 2;
	}

	//负数须转换成正数
	if (val < 0) 
	{
		val = -val; //转换成正数
		*p++ = __T('-'); //在输出缓存写入负号, 并向前移动一个位置
	}

	//分解出整数和小数部分
	fdec = modf(val, &fint);

	//转换
	if (fint != 0) //有整数和小数部分，例如：123.456
	{
		p0 = p; //记录起始位置
		p1 = &out[CVTBUFSIZE];

		//将整数部分解析出来放入缓存
		fi = fint;
		while (fi != 0 && p1 > p0)
		{
			fd = modf(fi / 10, &fi); //分解出每一位
			c = _c_tochr((int16)((fd + .03) * 10)); //转换成字符
			*(--p1)= c; //放入缓存
		}
		while (p1 < &out[CVTBUFSIZE]) //反转成正确的顺序并放入目标缓存
		{
			*p++ = *p1++;
		}

		//将小数部分解析出来放入缓存
		if (fdec > 0 && ndigits > 0)
		{
			 pt = p; //记录小数点位置
			*p++ = __T('.'); //添加小数符号
			
			//转换成字符
			fd = fdec;
			n = ndigits + 1; //多取1个数便于四舍五入
			while (fd != 0 && p < &out[CVTBUFSIZE] && n > 0)
			{
				fd = modf(fd * 10, &fi); //分解出每一位
				c = _c_tochr((int16)(fi)); //转换成字符
				*p++ = c; //放入缓存
				n--;
			}
			
			//四舍五入
			p1 = --p; //取得最后位置
			*p1 += 5; //字符+5>9,则需要进行四舍五入
			while (*p1 > __T('9') || *p1 < __T('0'))
			{
				*p1 = __T('0'); //设置当前字符为0
				if (p1 > p0) //还没有到第一个字符
				{
					++(*--p1); //跳转到下一个字符并+1
				}
				else //已经是第一个字符了,例如±9999.99999999，就会进入
				{
					*p1 = __T('1'); //将第一个字符设置成1
					*(++pt) = __T('.');//将小数点后移
					*(p++) = __T('0');//将最后位置后移
					break;
				}
			}
		}

		*p = NUL;//添加结束符号
	} 
	else if (fdec > 0 && ndigits > 0) //只有小数部分, 例如：0.618
	{
		p0 = p; //记录起始位置
		*p++ = __T('0'); //添加0
		pt = p; //记录小数点位置
		*p++ = __T('.'); //添加小数符号

		//转换成字符
		fd = fdec;
		n = ndigits + 1; //多取1个数用于四舍五入
		while (fd != 0 && p < &out[CVTBUFSIZE] && n > 0)
		{
			fd = modf(fd * 10, &fi); //分解出每一位
			c = _c_tochr((int32)(fi)); //转换成字符
			*p++ = c; //放入缓存
			n--;
		}

		//四舍五入
		p1 = --p; //取得最后位置
		*p1 += 5; //字符+5>9,则需要进行四舍五入
		while (*p1 > __T('9') || *p1 < __T('0'))
		{
			*p1 = __T('0'); //设置当前字符为0
			if (p1 > p0) //还没有到第一个字符
			{
				++(*--p1); //跳转到下一个字符并+1
				if (p1 == p0) //已经是第一个字符了,例如±0.99999999，就会进入
				{
					*p1 = __T('1'); //将第一个字符设置成1
					*pt = __T('.');//重新设置小数点
					break;
				}
			}
		}

		*p = NUL; //添加结束符号
	}
	else //零，例如：0.00000000
	{
		//输出n个0
		if (ndigits == 0)
		{
			*p++ = __T('0'); //添加0
		}
		else
		{
			*p++ = __T('0'); //添加0
			*p++ = __T('.'); //添加小数符号
			n = ndigits;
			while (n > 0)
			{
				*p++ = __T('0');
				n--;
			}
		}
		
		*p = NUL; //添加结束符号
	}
	
	//返回
	return out;
}