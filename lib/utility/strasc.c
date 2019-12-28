
/*
功能： 返回字符串中第一个字符的ASCII码。
参数： str，字符串；
返回： 返回字符串第一个字符的ASCII码
说明： 
*/

#include "../typedef.h"

int16 qstrasc(const tchar* str)
{
	if (sizeof(tchar) > 1)
	{
		//在Unicode字符集中无论是什么字符，都用2个字节表达, 因此直接转换返回即可。
		return (int16)*str;
	}
	else
	{
		int16 ascii = 0; //一个字符的ASCII码最多占2个字节，所以定义成int16

		ascii = *str;//取得字符的ASCII码

		if (ascii < 0)//判断是否为扩展ASCII
		{
			/*
			注意：由于计算机起源于英语国家的美国，最初制定的标准ASCII码只考虑了英语世界，
			随着计算机传播到世界上非英语国家，占用1个字节的ASCII码便满足不了需求，需要用2个字节来表达各自的字符，
			于是便出现了不同的字符集，但随着标准化的制定，字符集最终被统一成ANSI字符集和Unicode字符集。

			在Unicode字符集中无论是什么字符，都用2个字节表达，其缺点是对于只使用标准ASCII的英语国家，2个字节存储一个字符浪费了空间。
			在ANSI字符集中，标准ASCII码字符占用1个字节，超出标准ASCII码字符占用2个字节，这样既满足了英语国家和非英语国家，存储上也不造成空间浪费。

			在ANSI字符集中，非英语国家在表达字符时，采用标准ASCII(0-127)和扩展ASCII码(128-255)相结合的方式。
			如果字符在标准ASCII范围内就用标准ASCII表达，即用1个字节存储。
			如果字符超过了标准ASCII字符(十进制127，二进制1111111)，则采用扩展ASCII(128-255)表达，即用2个字节存储，每个字节存储的都是扩展ASCII(128-255)。

			所以在采用ANSI字符集的情况下，读取ASCII的方法是，判断该字符是否<0或者>127，如果<0或者>127说明采用的是扩展ASCII码来表达字符，即用2个字节存储，
			因此还需要读取下一个字节，然后合并在一起，即是该字符的ASCII值。

			*/

			char* p = (char*)&ascii + 1; //定义指针变量，便于修改ascii
			*p-- = *str++; //将第一个字符(高位字符)的扩展ASCII码写入高位
			*p = *str; //将第二个字符(低位字符)的扩展ASCII码写入低位
		}
		return ascii;
	}
}

int16 qastrasc(const char* str)
{
	int16 ascii = *str;//取得字符的ASCII码, 一个字符的ASCII码最多占2个字节，所以定义成int16

	if (ascii < 0)//判断是否为扩展ASCII
	{
		char* p = (char*)& ascii + 1; //定义指针变量，便于修改ascii
		*p-- = *str++; //将第一个字符(高位字符)的扩展ASCII码写入高位
		*p = *str; //将第二个字符(低位字符)的扩展ASCII码写入低位
	}
	return ascii;
}

int16 qwstrasc(const wchar* str)
{
	//在Unicode字符集中无论是什么字符，都用2个字节表达, 因此直接转换返回即可。
	return (int16)*str;
}