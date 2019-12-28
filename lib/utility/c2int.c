/*
模块： c2int.c
功能： 转换字符串为int类型值。
参数： str，要转换的字符串。
返回： 返回int值。
说明： c2int()与标准函数atoi功能一致。
	  c2int()会扫描参数str字符串，跳过前面的空格字符，直到遇上数字或正负符号才开始做转换，
	  而再遇到非数字或字符串结束时('\0')就结束转换，并将结果返回。
版本： 2019-05-20 denglf 模块创建
*/

#include "../typedef.h"
#include "../typecvt.h"

int32 c2int(const tchar* str)
{
	const tchar* p = str;
	int32 number;			//当前合计数
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
	//例如我们要将2019-05转换成201905，只需要将年*100加上月即可，即2019*100+5，既满足了转换又起到了格式化的效果。
	//将数扩大n倍+x(0<x<n),这是转换字符串为数字的常用算法和技巧。
	number = 0;
	c = (int16)*p;
	while (_c_isdigit(c))
	{
		if (_int_overflow(number, c)) return negative ? _INT_MIN : _INT_MAX; //溢出
		number = number * 10 + _c_tonum(c); //数字累加
		c = (int16)*(++p); //获取下一个字符
	}

	//返回结果
	return negative ? -number : number;
}

/*
整型溢出的算法：
1、定义一个无符号整型变量：unsigned int n。
2、去掉无符号数最大值的个位,作为溢出的边界值：max_bound_value = UINT_MAX / 10。
3、保留无符号数最大值的个位,作为每次增加的最大值：max_per_add_value = UINT_MAX % 10。
4、如果当前数值n<max_bound_value说明还未进入溢出边界。
5、如果当前数值n>=max_bound_value说明已经进入溢出边界。那么：
	如果增加的值v<=max_per_add_value，说明还没有超出UINT_MAX；
	如果增加的值v>max_per_add_value,说明当前数值n不能再增加，否则就超出了UINT_MAX。
	溢出检查宏定义如下：
	#define _uint_overflow(n, v) ((n >= (UINT_MAX / 10)) && (v > (UINT_MAX % 10)))
	代码如下：
	const tchar* p = str;
	uint32 n;
	uint32 max_bound_value = UINT_MAX / 10;
	int16  max_per_add_value = UINT_MAX % 10;
	int16  c = (int16)*p; 
	while (_c_isdigit(c))
	{
		if(_uint_overflow(n, c)) break;
		n = n * 10 + _c_tonum(c); //数字累加
		c = (int16)*(++p); //获取下一个字符
	}
6、如果要返回一个有符号整型数，那么直接判断n是否在[INT_MIN, INT_MAX]范围即可, 如果不在则返回INT_MIN或INT_MAX。如下：
	int result = (int)(n);//将结果转换成有符号int
	if (negative)//负号
	{
		result = -result;
		if (result > 0) return INT_MIN;//结果变成了正数，说明溢出。
	}
	else
	{
		if (result < 0) return INT_MAX;//结果变成了负数，说明溢出。
	}

小结：变量溢出与数组溢出不一样，数组溢出会访问只读内存地址，造成内存只读错；而变量溢出CPU只是将最高位舍掉。
也就是说变量溢出发生在CPU寄存器，而数组溢发生在内存访问。所以变量溢出是安全的，将字符串转换成数值时可以不用判断。
*/
 

