/*
	模块：功能函数
	作者：denglf
	日期：2019-04-24
	说明：封装标准c字符串操作和转换等相关函数
	参考：
	GNU C Library: http://www.gnu.org
	FreeBSD Unix: https://github.com/freebsd
	ChromeOS: https://www.chromium.org/chromium-os
	WindowsNT: https://github.com/stephanosio
	SanOS: http://www.jbox.dk/sanos/
	ReactOS: https://reactos.org/
*/

/*
	代码写得再优美，并不代表编译时最优。该如何优化？
	1，如果在函数中要修改参数变量，应该在函数中定义变量来处理，否则编译器会创建临时变量进行优化，例如：
	   语句：while(*dst++ = *src++) 
	   编译器会修改成：
	   char* pdst = *dst;
	   char* psrc = *src;
	   char  v;
	   do
	   {
		  v = *psrc;
		  *pdst++ = *psrc++;
	   }
	   while (v);
	2，尽量用for替换while。
	3, 代码要简单易懂，便于编译器容易识别和优化。
	所以：编程应该做到简单明白，代码易于理解和阅读。
*/

#ifndef __UTILITY_H__
#define __UTILITY_H__


#include <stdarg.h>
#include <math.h>
#include "../typedef.h"
#include "../typecvt.h"
#include "assert.h"

#ifdef __cplusplus
extern "C" {
#endif

	//高字
	int32 hiword(int32 n);

	//低字
	int32 loword(int32 n);

	//带符号位高字
	int32 hiwords(int32 n);

	//带符号位低字
	int32 lowords(int32 n);

	//制作long型
	int32 makelong(int32 low, int32 high);

	//是否数字
	tbool isadigit(tchar c);
	
	//返回字符串中第一个字符的ASCII码
	int16 qstrasc(const tchar* str);

	//字符串匹配
	//text，字符串；
	//filter，通配符，支持 * 和 ?
	//匹配返回true，不匹配返回false。
	tbool qstrmatch(const tchar* text, const tchar* filter);
	tbool qastrmatch(const char* text, const char* filter);
	tbool qwstrmatch(const wchar* text, const wchar* filter);

	//返回字符串的长度(字符的数量)。
	//注意：返回的是字符数量不是字节数量。例如：
	//如果字符集是多字节字符集：
	//	tchar *str = _T("abc你好");
	//	len = strlen(str); 
	//	返回len = 7个字符。由于多字节字符集，ASCII字符占用1个字节,而非ASCII码字符要用2个字节表达, 所以总计7个字节。
	//如果字符集是Unicode字符集：
	//	tchar *str = _T("abc你好");
	//	len = strlen(str); 
	//	返回len = 5个字符，由于Unicode字符集，每个字符占用2个字节, 所以总计10个字节。
	//由此可以知道strlen计算的是字符串的字符数量而不是字节数量。
	//如果要计算字节数量应该是这样：strlen(str) * sizeof(tchar)
	//用strlen这个名称来表示计算字符串的长度是合理的，只是它隐含的意思是计算字符串的字符数量，
	//假设用strcount来代替它，那么strcount("abc你好")就显得不合理了，
	//因为"abc你好"是一个字符串，而strcount的字面意思是计算字符串数量，本来就一个字符串，还需要计算数量吗？显然二者的含义不一致，
	//如果要真实的表达strlen的实际含义，那么可以用诸如getchrcount或chrcount来替换它，例如：
	//getchrcount("abc你好") 或 chrcount("abc你好") 但都不如 strlen("abc你好") 简洁。
	//要记住：在c/c++中，len(长度)表示字符数量(简写n)，size(大小)表示字节数量。
	uint32 qstrlen(const tchar* str);
	uint32 qastrlen(const char* str);
	uint32 qwstrlen(const wchar* str);

	//返回字符串的大小(字节的数量)。
	uint32 qstrlenb(const tchar* str);
	uint32 qastrlenb(const char* str);
	uint32 qwstrlenb(const wchar* str);

	//返回指定长度的字符串的长度（字符的数量）。
	uint32 qstrnlen(const tchar* str, uint32 n);
	uint32 qastrnlen(const char* str, uint32 n);
	uint32 qwstrnlen(const wchar* str, uint32 n);

	//返回字符串的拷贝。
	//strdup()在内部调用了malloc()为变量分配内存，不需要使用返回的字符串时，必须用free()释放相应的内存空间，否则会造成内存泄漏。
	//成功返回指向字符串拷贝的指针，失败返回NULL。
	tchar* qstrdup(const tchar* str);
	char*  qastrdup(const char* str);
	tchar* qwstrdup(const wchar* str);

	//返回指定长度的字符串拷贝。
	//strndup()在内部调用了malloc()为变量分配内存，不需要使用返回的字符串时，必须用free()释放相应的内存空间，否则会造成内存泄漏。
	//成功返回指向字符串拷贝的指针，失败返回NULL。
	tchar* qstrndup(const tchar* str, uint32 n);
	char*  qastrndup(const char* str, uint32 n);
	wchar* qwstrndup(const wchar* str, uint32 n);

	//拷贝源字符串到目标字符串中，无论拷贝是否成功，都返回目标字符串的地址。
	tchar* qstrcpy(tchar* dst, const tchar* src);
	char*  qastrcpy(char* dst, const char* src);
	wchar* qwstrcpy(wchar* dst, const wchar* src);

	//拷贝指定长度的源字符串到目标字符串中，无论拷贝是否成功，都返回目标字符串的地址。
	tchar* qstrncpy(tchar* dst, const tchar* src, uint32 n);
	char*  qastrncpy(char* dst, const char* src, uint32 n);
	wchar* qwstrncpy(wchar* dst, const wchar* src, uint32 n);

	//拷贝指定长度的源字符串到目标字符串中，返回成功拷贝的字符数量。
	//strlcpy是strcpy函数的安全版本，目的是防止dst目标缓冲区溢出。
	//size指dst目标字符串的长度（字符数量），用strlen计算得到。
	uint32 qstrlcpy(tchar* dst, const tchar* src, uint32 n);
	uint32 qastrlcpy(char* dst, const char* src, uint32 n);
	uint32 qwstrlcpy(wchar* dst, const wchar* src, uint32 n);

	//拷贝源字符串到目标字符串的末尾(即连接两个字符串)。
	//必须保证目标字符串有足够的空间用来容纳源字符串。
	//无论拷贝是否成功，都返回目标字符串的地址。
	tchar* qstrcat(tchar* dst, const tchar* src);
	char*  qastrcat(char* dst, const char* src);
	tchar* qwstrcat(wchar* dst, const wchar* src);

	//拷贝指定长度的源字符串到目标字符串的末尾(即部分连接两个字符串)。
	//必须保证目标字符串有足够的空间用来容纳源字符串。
	//无论拷贝是否成功，都返回目标字符串的地址。
	tchar* qstrncat(tchar* dst, const tchar* src, uint32 n);
	char*  qastrncat(char* dst, const char* src, uint32 n);
	tchar* qwstrncat(wchar* dst, const wchar* src, uint32 n);

	//拷贝指定长度的源字符串到目标字符串的末尾(即部分连接两个字符串)，并返回成功拷贝的字符数量。
	//strlcat是strcat函数的安全版本，目的是防止dst目标缓冲区溢出。
	//n指dst目标字符串的长度（字符数量），用strlen计算得到。
	uint32 qstrlcat(tchar* dst, const tchar* src, uint32 n);
	uint32 qastrlcat(char* dst, const char* src, uint32 n);
	uint32 qwstrlcat(wchar* dst, const wchar* src, uint32 n);

	//字符串比较，区分大小写，即二进制比较。
	//如果 str1 > str2 返回值 > 0
	//如果 str1 < str2 返回值 < 0
	//如果 str1 = str2 返回值 = 0
	int32 qstrcmp(const tchar* str1, const tchar* str2);
	int32 qastrcmp(const char* str1, const char* str2);
	int32 qwstrcmp(const wchar* str1, const wchar* str2);

	//比较指定长度的字符串，区分大小写，即二进制比较。
	//如果 str1 > str2 返回值 > 0
	//如果 str1 < str2 返回值 < 0
	//如果 str1 = str2 返回值 = 0
	int32 qstrncmp(const tchar* str1, const tchar* str2, uint32 n);
	int32 qastrncmp(const char* str1, const char* str2, uint32 n);
	int32 qwstrncmp(const wchar* str1, const wchar* str2, uint32 n);

	//字符串比较，不区分大小写，即文本比较。
	//如果 str1 > str2 返回值 > 0
	//如果 str1 < str2 返回值 < 0
	//如果 str1 = str2 返回值 = 0
	int32 qstricmp(const tchar* str1, const tchar* str2);
	int32 qastricmp(const char* str1, const char* str2);
	int32 qwstricmp(const wchar* str1, const wchar* str2);

	//比较指定长度的字符串，不区分大小写，即文本比较。
	//如果 str1 > str2 返回值 > 0
	//如果 str1 < str2 返回值 < 0
	//如果 str1 = str2 返回值 = 0
	int32 qstrnicmp(const tchar* str1, const tchar* str2, uint32 n);
	int32 qastrnicmp(const char* str1, const char* str2, uint32 n);
	int32 qwstrnicmp(const wchar* str1, const wchar* str2, uint32 n);

	//查找字符串str中首次出现字符c的位置。
	//返回指向字符串中第一次出现字符c的指针，
	//如果字符串中没有出现字符c，则返回NULL。
	tchar* qstrchr(const tchar* str, int32 c);
	char*  qastrchr(const char* str, int32 c);
	wchar* qwstrchr(const wchar* str, int32 c);

	//查找字符串str中最后一次出现字符c的位置。
	//返回指向字符串中最后次出现字符c的指针，
	//如果字符串中没有出现字符c，则返回NULL。
	tchar* qstrrchr(const tchar* str, int32 c);
	char*  qastrrchr(const char* str, int32 c);
	wchar* qwstrrchr(const wchar* str, int32 c);

	//查找字符串str2是否存在于字符串str1中。
	//返回指向第一次出现字符串str2的指针，
	//如果str2不存在于str1中，则返回NULL。
	tchar* qstrstr(const tchar* str1, const tchar* str2);
	char*  qastrstr(const char* str1, const char* str2);
	wchar* qwstrstr(const wchar* str1, const wchar* str2);

	//字符串反转。
	//返回指向str的指针。
	tchar* qstrrev(tchar* str);
	char*  qastrrev(char* str);
	wchar* qwstrrev(wchar* str);

	//将字符串str中的所有字符都设置成字符c。
	//返回指向str的指针。
	tchar* qstrset(tchar* str, int32 c);
	char*  qastrset(char* str, int32 c);
	wchar* qwstrset(wchar* str, int32 c);

	//将字符串str中的n个字符设置成字符c。
	//返回指向str的指针。
	tchar* qstrnset(tchar* str, int32 c, uint32 n);
	char*  qastrnset(char* str, int32 c, uint32 n);
	wchar* qwstrnset(wchar* str, int32 c, uint32 n);

	//查询字符串str开头连续包含字符串accept的子字符串的字符数量。或
	//查询字符串str中首次出现字符串reject中的任意字符的位置。
	//parity=0等价于qstrspn; parity=1等价于qstrcspn。
	uint32 qstrxspn(const tchar* str, const tchar* map, int32 parity);
	uint32 qastrxspn(const char* str, const char* map, int32 parity);
	uint32 qwstrxspn(const wchar* str, const wchar* map, int32 parity);

	//查询字符串str开头连续包含字符串accept的子字符串的字符数量。即字符串对齐连续匹配。
	//返回字符串str开头连续包含字符串accept内的字符数量。例如：
	//char* str =       "The Linux was first developed for 386/486-based pcs.";
	//len = strspn(str, "The Linux was"); 返回14，说明有10个字符连续匹配。
	//len = strspn(str, "The Linux"); 返回10，说明有10个字符连续匹配。
	//len = strspn(str, "The"); 返回3，说明有10个字符连续匹配。
	//len = strspn(str, "Linux was"); 返回0，未连续匹配，虽然看起来连续，但未对齐。
	//len = strspn(str, "was"); 返回0，未连续匹配，与上相同，未对齐。
	//uint32 qstrspn(const tchar* str, const tchar* accept);
	#define qstrspn(str, accept) qstrxspn(str, accept, 0)
	#define qastrspn(str, accept) qastrxspn(str, accept, 0)
	#define qwstrspn(str, accept) qwstrxspn(str, accept, 0)

	//查询字符串str中首次出现字符串reject中的任意字符的位置。
	//返回字符串str开头连续不包含字符串reject内的字符数量。例如：
	//char* str = "The Linux was first developed for 386/486-based pcs.";
	//len = strcspn(str1, " "); 返回3，返回首次出现" "(空格)时的长度，所以返回3。
	//len = strcspn(str1, "/-"); 返回37，返回首次出现"/"或"-"时的长度，所以返回37。
	//len = strcspn(str1, "1234567890"); 返回34，返回首次出现0-9之间的任意数值时的长度，所以返回34。
	//len = strcspn(str1, "jk"); 返回52，没有找到首次出现的字符，则返回整个字符串的长度，所以返回52。
	//uint32 qstrcspn(const tchar* str, const tchar* reject);
	#define qstrcspn(str, reject) qstrxspn(str, reject, 1)
	#define qastrcspn(str, reject) qastrxspn(str, reject, 1)
	#define qwstrcspn(str, reject) qwstrxspn(str, reject, 1)

	//查找字符串str中出现accept中任意字符时的位置。
	//如果找到，返回找到字符所在位置的指针;否则返回NULL。例如：
	//char* p = NULL;
	//char* str = "The Linux was first developed for 386/486-based pcs.";
	//p = strpbrk(str1, "ix"); 返回"inux was first developed for 386/486-based pcs."
	//p = strpbrk(str1, "lop"); 返回"loped for 386/486-based pcs."
	//p = strpbrk(str1, "jk"); 返回NULL
	tchar* qstrpbrk(const tchar* str, const tchar* accept);
	char*  qastrpbrk(const char* str, const char* accept);
	wchar* qwstrpbrk(const wchar* str, const wchar* accept);

	//字符串分割。
	//从字符串str指向的位置起向后扫描，遇到delim字符串中的任意字符后，将此字符替换为NUL，并结束分割，
	//然后将str指针定位在分割处，以便对str继续进行分割，
	//最后返回str起始位置的指针，以便输出分割好的字符串。例如：
	//char str[] = "hello&world&你好&世界"; 
	//char* q = str;
	//char* p;
	//while (p = strsep(&q, "&"))
	//{
	//	printf("%s\n", p);
	//}
	//输出：
	//hello
	//world
	//你好
	//世界
	//注意传入的字符串必须是可修改的，例如数组或malloc分配的动态内存，不能传入常量字符串。
	tchar* qstrsep(tchar** pstr, const tchar* delim);
	char*  qastrsep(char** pstr, const char* delim);
	wchar* qwstrsep(wchar** pstr, const wchar* delim);

	//将src所指的源内存区域复制n个字节到dst所指的目标内存区域中，并返回指向dst目标内存区域的指针。
	//memmove能够保证源区域在被覆盖之前将重叠区域的字节拷贝到目标区域中，但复制后源内容会被更改。
	//但是当目标区域与源区域没有重叠时，则和memcpy函数的功能相同。
	//测试发现系统内置的memmove函数，编译器已经做了最大优化(参见: memcpy.asm), 且调用的是memcpy, 因此采用系统memmove
	//void* qmemmove(void* dst, const void* src, tint n);
	#define qmemmove(dst, src, n) memmove(dst, src, n)

	//将src所指的源内存区域复制n个字节到dst所指的目标内存区域中，并返回指向dst目标内存区域的指针。
	//在循环中拷贝数据量数据时函数堆栈切换将占用时间，因此采用宏调用qmemcpyl和qmemcpys。
	//测试发现系统内置的memcpy函数，在小于128字节的情况下，编译器已经做了最大优化(参见: memcpy.asm),
	//因此，为达到最佳效果小于128字节时调用系统memcpy。
	//void* qmemcpy(void* dst, const void* src, tint n);
	#define qmemcpy(dst, src, n) n > 128 ? qmemcpyl(dst, src, n) : memcpy(dst, src, n)
	//拷贝小数据(<= 128 字节), 按4字节对齐拷贝
	void* qmemcpys(void* dst, const void* src, tint n);
	//拷贝大数据(>128字节), 按16字节对齐拷贝
	void* qmemcpyl(void* dst, const void* src, tint n);

	//将dst目标内存区域用n个字节的c替换，并返回指向dst目标内存区域的指针。
	//测试发现系统内置的memset函数，在小于128字节的情况下，编译器已经做了最大优化(参见: memset.asm), 因此采用系统memset。
	//void* qmemset(void* dst, int32 c, tint n);
	#define qmemset(dst, c, n) memset(dst, c, n)

	//将src源内存区域与dst目标内存区域的n个字节进行比较
	//如果 dst > src 返回值 > 0
	//如果 dst < src 返回值 < 0
	//如果 dst = src 返回值 = 0
	int32 qmemcmp(const void* dst, const void* src, tint n);

	//从buf内存区域中的n个字节中查找字符ch。
	void* qmemchr(const void* buf, int chr, tint n);

	//字符串格式化
	int32 tvsprintf(tchar* buf, const tchar* fmt, va_list args);
	int32 tsprintf(tchar* dst, const tchar* fmt, ...);
	int32 format(tchar* dst, const tchar* fmt, ...);

	//转换字符串为int类型值。
	//str: 要转换的字符串，例如123456。
	int32 c2int(const tchar* str);

	//转换字符串为unsigned int类型值。
	//str: 要转换的字符串，例如123456。
	uint32 c2uint(const tchar* str);

	//转换字符串为int64类型值。
	//str: 要转换的字符串，例如123456。
	int64 c2int64(const tchar* str);

	//转换字符串为long类型值。
	//str: 要转换的字符串，例如123456。
	long c2lng(const tchar* str);

	//转换字符串为unsigned long类型值。
	//str: 要转换的字符串，例如123456。
	long c2ulng(const tchar* str);

	//转换字符串为uint64(unsigned long long)类型值。
	//str: 要转换的字符串，例如123456。
	uint64 c2uint64(const tchar* str);

	//转换字符串为float类型值。
	//str: 要转换的字符串，例如123.456或123e-2或123.456e2。
	float c2flt(const tchar* str);

	//转换字符串为double类型值。
	//str: 要转换的字符串，例如123.456或123e-2或123.456e2。
	double c2dbl(const tchar* str);

	//转换int类型值为字符串。
	//val，要转换的int类型值。
	//out: 缓存转换后的字符，容纳int类型值的缓存，其大小至少是12*sizeof(tchar)个字节。
	//返回: 指向out的指针。
	tchar* cstri(int32 val, tchar* out);
	
	//转换unsigned int类型值为字符串。
	//val，要转换的unsigned int类型值。
	//out: 缓存转换后的字符，容纳unsigned int类型值的缓存，其大小至少是12*sizeof(tchar)个字节。
	//返回: 指向out的指针。
	tchar* cstrui(uint32 val, tchar* out);

	//转换long类型值为字符串。
	//val，要转换的long类型值。
	//out: 缓存转换后的字符，容纳long类型值的缓存，其大小至少是12*sizeof(tchar)个字节。
	//返回: 指向out的指针。
	tchar* cstrl(long val, tchar* out);

	//转换unsigned long类型值为字符串。
	//val，要转换的unsigned long类型值。
	//out: 缓存转换后的字符，容纳unsigned long类型值的缓存，其大小至少是12*sizeof(tchar)个字节。
	//返回: 指向out的指针。
	tchar* cstrul(ulong val, tchar* out);

	//转换int64(long long)类型值为字符串。
	//val，要转换的int64(long long)类型值。
	//out: 缓存转换后的字符，容纳int64(long long)类型值的缓存，其大小至少是22*sizeof(tchar)个字节。
	//返回: 指向out的指针。
	tchar* cstri64(int64 val, tchar* out);

	//转换uint64(unsigned long long)类型值为字符串。
	//val，要转换的uint64(unsigned long long)类型值。
	//out: 缓存转换后的字符，容纳uint64(unsigned long long)类型值的缓存，其大小至少是22*sizeof(tchar)个字节。
	//返回: 指向out的指针。
	tchar* cstrui64(uint64 val, tchar* out);

	//转换double类型值为字符串。
	//val: 要转换的double类型值。
	//ndigits: 要保留的小数位。
	//out: 缓存转换后的字符。容纳double类型值的缓存，其大小至少是352 * sizeof(tchar)个字节。
	//返回: 指向out的指针。
	tchar* cstrd(double val, int32 ndigits, tchar* out);

	//转换float类型值为字符串。
	//val: 要转换的float类型值。
	//ndigits: 要保留的小数位。
	//out: 缓存转换后的字符。容纳float类型值的缓存，其大小至少是56 * sizeof(tchar)个字节。
	//返回: 指向out的指针。
	tchar* cstrf(float val, int32 ndigits, tchar* out);

#ifdef __cplusplus
}
#endif

#endif  //__UTILITY_H__
