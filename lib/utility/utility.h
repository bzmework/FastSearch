/*
	ģ�飺���ܺ���
	���ߣ�denglf
	���ڣ�2019-04-24
	˵������װ��׼c�ַ���������ת������غ���
	�ο���
	GNU C Library: http://www.gnu.org
	FreeBSD Unix: https://github.com/freebsd
	ChromeOS: https://www.chromium.org/chromium-os
	WindowsNT: https://github.com/stephanosio
	SanOS: http://www.jbox.dk/sanos/
	ReactOS: https://reactos.org/
*/

/*
	����д���������������������ʱ���š�������Ż���
	1������ں�����Ҫ�޸Ĳ���������Ӧ���ں����ж����������������������ᴴ����ʱ���������Ż������磺
	   ��䣺while(*dst++ = *src++) 
	   ���������޸ĳɣ�
	   char* pdst = *dst;
	   char* psrc = *src;
	   char  v;
	   do
	   {
		  v = *psrc;
		  *pdst++ = *psrc++;
	   }
	   while (v);
	2��������for�滻while��
	3, ����Ҫ���׶������ڱ���������ʶ����Ż���
	���ԣ����Ӧ�����������ף��������������Ķ���
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

	//����
	int32 hiword(int32 n);

	//����
	int32 loword(int32 n);

	//������λ����
	int32 hiwords(int32 n);

	//������λ����
	int32 lowords(int32 n);

	//����long��
	int32 makelong(int32 low, int32 high);

	//�Ƿ�����
	tbool isadigit(tchar c);
	
	//�����ַ����е�һ���ַ���ASCII��
	int16 qstrasc(const tchar* str);

	//�ַ���ƥ��
	//text���ַ�����
	//filter��ͨ�����֧�� * �� ?
	//ƥ�䷵��true����ƥ�䷵��false��
	tbool qstrmatch(const tchar* text, const tchar* filter);
	tbool qastrmatch(const char* text, const char* filter);
	tbool qwstrmatch(const wchar* text, const wchar* filter);

	//�����ַ����ĳ���(�ַ�������)��
	//ע�⣺���ص����ַ����������ֽ����������磺
	//����ַ����Ƕ��ֽ��ַ�����
	//	tchar *str = _T("abc���");
	//	len = strlen(str); 
	//	����len = 7���ַ������ڶ��ֽ��ַ�����ASCII�ַ�ռ��1���ֽ�,����ASCII���ַ�Ҫ��2���ֽڱ��, �����ܼ�7���ֽڡ�
	//����ַ�����Unicode�ַ�����
	//	tchar *str = _T("abc���");
	//	len = strlen(str); 
	//	����len = 5���ַ�������Unicode�ַ�����ÿ���ַ�ռ��2���ֽ�, �����ܼ�10���ֽڡ�
	//�ɴ˿���֪��strlen��������ַ������ַ������������ֽ�������
	//���Ҫ�����ֽ�����Ӧ����������strlen(str) * sizeof(tchar)
	//��strlen�����������ʾ�����ַ����ĳ����Ǻ���ģ�ֻ������������˼�Ǽ����ַ������ַ�������
	//������strcount������������ôstrcount("abc���")���Եò������ˣ�
	//��Ϊ"abc���"��һ���ַ�������strcount��������˼�Ǽ����ַ���������������һ���ַ���������Ҫ������������Ȼ���ߵĺ��岻һ�£�
	//���Ҫ��ʵ�ı��strlen��ʵ�ʺ��壬��ô����������getchrcount��chrcount���滻�������磺
	//getchrcount("abc���") �� chrcount("abc���") �������� strlen("abc���") ��ࡣ
	//Ҫ��ס����c/c++�У�len(����)��ʾ�ַ�����(��дn)��size(��С)��ʾ�ֽ�������
	uint32 qstrlen(const tchar* str);
	uint32 qastrlen(const char* str);
	uint32 qwstrlen(const wchar* str);

	//�����ַ����Ĵ�С(�ֽڵ�����)��
	uint32 qstrlenb(const tchar* str);
	uint32 qastrlenb(const char* str);
	uint32 qwstrlenb(const wchar* str);

	//����ָ�����ȵ��ַ����ĳ��ȣ��ַ�����������
	uint32 qstrnlen(const tchar* str, uint32 n);
	uint32 qastrnlen(const char* str, uint32 n);
	uint32 qwstrnlen(const wchar* str, uint32 n);

	//�����ַ����Ŀ�����
	//strdup()���ڲ�������malloc()Ϊ���������ڴ棬����Ҫʹ�÷��ص��ַ���ʱ��������free()�ͷ���Ӧ���ڴ�ռ䣬���������ڴ�й©��
	//�ɹ�����ָ���ַ���������ָ�룬ʧ�ܷ���NULL��
	tchar* qstrdup(const tchar* str);
	char*  qastrdup(const char* str);
	tchar* qwstrdup(const wchar* str);

	//����ָ�����ȵ��ַ���������
	//strndup()���ڲ�������malloc()Ϊ���������ڴ棬����Ҫʹ�÷��ص��ַ���ʱ��������free()�ͷ���Ӧ���ڴ�ռ䣬���������ڴ�й©��
	//�ɹ�����ָ���ַ���������ָ�룬ʧ�ܷ���NULL��
	tchar* qstrndup(const tchar* str, uint32 n);
	char*  qastrndup(const char* str, uint32 n);
	wchar* qwstrndup(const wchar* str, uint32 n);

	//����Դ�ַ�����Ŀ���ַ����У����ۿ����Ƿ�ɹ���������Ŀ���ַ����ĵ�ַ��
	tchar* qstrcpy(tchar* dst, const tchar* src);
	char*  qastrcpy(char* dst, const char* src);
	wchar* qwstrcpy(wchar* dst, const wchar* src);

	//����ָ�����ȵ�Դ�ַ�����Ŀ���ַ����У����ۿ����Ƿ�ɹ���������Ŀ���ַ����ĵ�ַ��
	tchar* qstrncpy(tchar* dst, const tchar* src, uint32 n);
	char*  qastrncpy(char* dst, const char* src, uint32 n);
	wchar* qwstrncpy(wchar* dst, const wchar* src, uint32 n);

	//����ָ�����ȵ�Դ�ַ�����Ŀ���ַ����У����سɹ��������ַ�������
	//strlcpy��strcpy�����İ�ȫ�汾��Ŀ���Ƿ�ֹdstĿ�껺���������
	//sizeָdstĿ���ַ����ĳ��ȣ��ַ�����������strlen����õ���
	uint32 qstrlcpy(tchar* dst, const tchar* src, uint32 n);
	uint32 qastrlcpy(char* dst, const char* src, uint32 n);
	uint32 qwstrlcpy(wchar* dst, const wchar* src, uint32 n);

	//����Դ�ַ�����Ŀ���ַ�����ĩβ(�����������ַ���)��
	//���뱣֤Ŀ���ַ������㹻�Ŀռ���������Դ�ַ�����
	//���ۿ����Ƿ�ɹ���������Ŀ���ַ����ĵ�ַ��
	tchar* qstrcat(tchar* dst, const tchar* src);
	char*  qastrcat(char* dst, const char* src);
	tchar* qwstrcat(wchar* dst, const wchar* src);

	//����ָ�����ȵ�Դ�ַ�����Ŀ���ַ�����ĩβ(���������������ַ���)��
	//���뱣֤Ŀ���ַ������㹻�Ŀռ���������Դ�ַ�����
	//���ۿ����Ƿ�ɹ���������Ŀ���ַ����ĵ�ַ��
	tchar* qstrncat(tchar* dst, const tchar* src, uint32 n);
	char*  qastrncat(char* dst, const char* src, uint32 n);
	tchar* qwstrncat(wchar* dst, const wchar* src, uint32 n);

	//����ָ�����ȵ�Դ�ַ�����Ŀ���ַ�����ĩβ(���������������ַ���)�������سɹ��������ַ�������
	//strlcat��strcat�����İ�ȫ�汾��Ŀ���Ƿ�ֹdstĿ�껺���������
	//nָdstĿ���ַ����ĳ��ȣ��ַ�����������strlen����õ���
	uint32 qstrlcat(tchar* dst, const tchar* src, uint32 n);
	uint32 qastrlcat(char* dst, const char* src, uint32 n);
	uint32 qwstrlcat(wchar* dst, const wchar* src, uint32 n);

	//�ַ����Ƚϣ����ִ�Сд���������ƱȽϡ�
	//��� str1 > str2 ����ֵ > 0
	//��� str1 < str2 ����ֵ < 0
	//��� str1 = str2 ����ֵ = 0
	int32 qstrcmp(const tchar* str1, const tchar* str2);
	int32 qastrcmp(const char* str1, const char* str2);
	int32 qwstrcmp(const wchar* str1, const wchar* str2);

	//�Ƚ�ָ�����ȵ��ַ��������ִ�Сд���������ƱȽϡ�
	//��� str1 > str2 ����ֵ > 0
	//��� str1 < str2 ����ֵ < 0
	//��� str1 = str2 ����ֵ = 0
	int32 qstrncmp(const tchar* str1, const tchar* str2, uint32 n);
	int32 qastrncmp(const char* str1, const char* str2, uint32 n);
	int32 qwstrncmp(const wchar* str1, const wchar* str2, uint32 n);

	//�ַ����Ƚϣ������ִ�Сд�����ı��Ƚϡ�
	//��� str1 > str2 ����ֵ > 0
	//��� str1 < str2 ����ֵ < 0
	//��� str1 = str2 ����ֵ = 0
	int32 qstricmp(const tchar* str1, const tchar* str2);
	int32 qastricmp(const char* str1, const char* str2);
	int32 qwstricmp(const wchar* str1, const wchar* str2);

	//�Ƚ�ָ�����ȵ��ַ����������ִ�Сд�����ı��Ƚϡ�
	//��� str1 > str2 ����ֵ > 0
	//��� str1 < str2 ����ֵ < 0
	//��� str1 = str2 ����ֵ = 0
	int32 qstrnicmp(const tchar* str1, const tchar* str2, uint32 n);
	int32 qastrnicmp(const char* str1, const char* str2, uint32 n);
	int32 qwstrnicmp(const wchar* str1, const wchar* str2, uint32 n);

	//�����ַ���str���״γ����ַ�c��λ�á�
	//����ָ���ַ����е�һ�γ����ַ�c��ָ�룬
	//����ַ�����û�г����ַ�c���򷵻�NULL��
	tchar* qstrchr(const tchar* str, int32 c);
	char*  qastrchr(const char* str, int32 c);
	wchar* qwstrchr(const wchar* str, int32 c);

	//�����ַ���str�����һ�γ����ַ�c��λ�á�
	//����ָ���ַ��������γ����ַ�c��ָ�룬
	//����ַ�����û�г����ַ�c���򷵻�NULL��
	tchar* qstrrchr(const tchar* str, int32 c);
	char*  qastrrchr(const char* str, int32 c);
	wchar* qwstrrchr(const wchar* str, int32 c);

	//�����ַ���str2�Ƿ�������ַ���str1�С�
	//����ָ���һ�γ����ַ���str2��ָ�룬
	//���str2��������str1�У��򷵻�NULL��
	tchar* qstrstr(const tchar* str1, const tchar* str2);
	char*  qastrstr(const char* str1, const char* str2);
	wchar* qwstrstr(const wchar* str1, const wchar* str2);

	//�ַ�����ת��
	//����ָ��str��ָ�롣
	tchar* qstrrev(tchar* str);
	char*  qastrrev(char* str);
	wchar* qwstrrev(wchar* str);

	//���ַ���str�е������ַ������ó��ַ�c��
	//����ָ��str��ָ�롣
	tchar* qstrset(tchar* str, int32 c);
	char*  qastrset(char* str, int32 c);
	wchar* qwstrset(wchar* str, int32 c);

	//���ַ���str�е�n���ַ����ó��ַ�c��
	//����ָ��str��ָ�롣
	tchar* qstrnset(tchar* str, int32 c, uint32 n);
	char*  qastrnset(char* str, int32 c, uint32 n);
	wchar* qwstrnset(wchar* str, int32 c, uint32 n);

	//��ѯ�ַ���str��ͷ���������ַ���accept�����ַ������ַ���������
	//��ѯ�ַ���str���״γ����ַ���reject�е������ַ���λ�á�
	//parity=0�ȼ���qstrspn; parity=1�ȼ���qstrcspn��
	uint32 qstrxspn(const tchar* str, const tchar* map, int32 parity);
	uint32 qastrxspn(const char* str, const char* map, int32 parity);
	uint32 qwstrxspn(const wchar* str, const wchar* map, int32 parity);

	//��ѯ�ַ���str��ͷ���������ַ���accept�����ַ������ַ����������ַ�����������ƥ�䡣
	//�����ַ���str��ͷ���������ַ���accept�ڵ��ַ����������磺
	//char* str =       "The Linux was first developed for 386/486-based pcs.";
	//len = strspn(str, "The Linux was"); ����14��˵����10���ַ�����ƥ�䡣
	//len = strspn(str, "The Linux"); ����10��˵����10���ַ�����ƥ�䡣
	//len = strspn(str, "The"); ����3��˵����10���ַ�����ƥ�䡣
	//len = strspn(str, "Linux was"); ����0��δ����ƥ�䣬��Ȼ��������������δ���롣
	//len = strspn(str, "was"); ����0��δ����ƥ�䣬������ͬ��δ���롣
	//uint32 qstrspn(const tchar* str, const tchar* accept);
	#define qstrspn(str, accept) qstrxspn(str, accept, 0)
	#define qastrspn(str, accept) qastrxspn(str, accept, 0)
	#define qwstrspn(str, accept) qwstrxspn(str, accept, 0)

	//��ѯ�ַ���str���״γ����ַ���reject�е������ַ���λ�á�
	//�����ַ���str��ͷ�����������ַ���reject�ڵ��ַ����������磺
	//char* str = "The Linux was first developed for 386/486-based pcs.";
	//len = strcspn(str1, " "); ����3�������״γ���" "(�ո�)ʱ�ĳ��ȣ����Է���3��
	//len = strcspn(str1, "/-"); ����37�������״γ���"/"��"-"ʱ�ĳ��ȣ����Է���37��
	//len = strcspn(str1, "1234567890"); ����34�������״γ���0-9֮���������ֵʱ�ĳ��ȣ����Է���34��
	//len = strcspn(str1, "jk"); ����52��û���ҵ��״γ��ֵ��ַ����򷵻������ַ����ĳ��ȣ����Է���52��
	//uint32 qstrcspn(const tchar* str, const tchar* reject);
	#define qstrcspn(str, reject) qstrxspn(str, reject, 1)
	#define qastrcspn(str, reject) qastrxspn(str, reject, 1)
	#define qwstrcspn(str, reject) qwstrxspn(str, reject, 1)

	//�����ַ���str�г���accept�������ַ�ʱ��λ�á�
	//����ҵ��������ҵ��ַ�����λ�õ�ָ��;���򷵻�NULL�����磺
	//char* p = NULL;
	//char* str = "The Linux was first developed for 386/486-based pcs.";
	//p = strpbrk(str1, "ix"); ����"inux was first developed for 386/486-based pcs."
	//p = strpbrk(str1, "lop"); ����"loped for 386/486-based pcs."
	//p = strpbrk(str1, "jk"); ����NULL
	tchar* qstrpbrk(const tchar* str, const tchar* accept);
	char*  qastrpbrk(const char* str, const char* accept);
	wchar* qwstrpbrk(const wchar* str, const wchar* accept);

	//�ַ����ָ
	//���ַ���strָ���λ�������ɨ�裬����delim�ַ����е������ַ��󣬽����ַ��滻ΪNUL���������ָ
	//Ȼ��strָ�붨λ�ڷָ���Ա��str�������зָ
	//��󷵻�str��ʼλ�õ�ָ�룬�Ա�����ָ�õ��ַ��������磺
	//char str[] = "hello&world&���&����"; 
	//char* q = str;
	//char* p;
	//while (p = strsep(&q, "&"))
	//{
	//	printf("%s\n", p);
	//}
	//�����
	//hello
	//world
	//���
	//����
	//ע�⴫����ַ��������ǿ��޸ĵģ����������malloc����Ķ�̬�ڴ棬���ܴ��볣���ַ�����
	tchar* qstrsep(tchar** pstr, const tchar* delim);
	char*  qastrsep(char** pstr, const char* delim);
	wchar* qwstrsep(wchar** pstr, const wchar* delim);

	//��src��ָ��Դ�ڴ�������n���ֽڵ�dst��ָ��Ŀ���ڴ������У�������ָ��dstĿ���ڴ������ָ�롣
	//memmove�ܹ���֤Դ�����ڱ�����֮ǰ���ص�������ֽڿ�����Ŀ�������У������ƺ�Դ���ݻᱻ���ġ�
	//���ǵ�Ŀ��������Դ����û���ص�ʱ�����memcpy�����Ĺ�����ͬ��
	//���Է���ϵͳ���õ�memmove�������������Ѿ���������Ż�(�μ�: memcpy.asm), �ҵ��õ���memcpy, ��˲���ϵͳmemmove
	//void* qmemmove(void* dst, const void* src, tint n);
	#define qmemmove(dst, src, n) memmove(dst, src, n)

	//��src��ָ��Դ�ڴ�������n���ֽڵ�dst��ָ��Ŀ���ڴ������У�������ָ��dstĿ���ڴ������ָ�롣
	//��ѭ���п�������������ʱ������ջ�л���ռ��ʱ�䣬��˲��ú����qmemcpyl��qmemcpys��
	//���Է���ϵͳ���õ�memcpy��������С��128�ֽڵ�����£��������Ѿ���������Ż�(�μ�: memcpy.asm),
	//��ˣ�Ϊ�ﵽ���Ч��С��128�ֽ�ʱ����ϵͳmemcpy��
	//void* qmemcpy(void* dst, const void* src, tint n);
	#define qmemcpy(dst, src, n) n > 128 ? qmemcpyl(dst, src, n) : memcpy(dst, src, n)
	//����С����(<= 128 �ֽ�), ��4�ֽڶ��뿽��
	void* qmemcpys(void* dst, const void* src, tint n);
	//����������(>128�ֽ�), ��16�ֽڶ��뿽��
	void* qmemcpyl(void* dst, const void* src, tint n);

	//��dstĿ���ڴ�������n���ֽڵ�c�滻��������ָ��dstĿ���ڴ������ָ�롣
	//���Է���ϵͳ���õ�memset��������С��128�ֽڵ�����£��������Ѿ���������Ż�(�μ�: memset.asm), ��˲���ϵͳmemset��
	//void* qmemset(void* dst, int32 c, tint n);
	#define qmemset(dst, c, n) memset(dst, c, n)

	//��srcԴ�ڴ�������dstĿ���ڴ������n���ֽڽ��бȽ�
	//��� dst > src ����ֵ > 0
	//��� dst < src ����ֵ < 0
	//��� dst = src ����ֵ = 0
	int32 qmemcmp(const void* dst, const void* src, tint n);

	//��buf�ڴ������е�n���ֽ��в����ַ�ch��
	void* qmemchr(const void* buf, int chr, tint n);

	//�ַ�����ʽ��
	int32 tvsprintf(tchar* buf, const tchar* fmt, va_list args);
	int32 tsprintf(tchar* dst, const tchar* fmt, ...);
	int32 format(tchar* dst, const tchar* fmt, ...);

	//ת���ַ���Ϊint����ֵ��
	//str: Ҫת�����ַ���������123456��
	int32 c2int(const tchar* str);

	//ת���ַ���Ϊunsigned int����ֵ��
	//str: Ҫת�����ַ���������123456��
	uint32 c2uint(const tchar* str);

	//ת���ַ���Ϊint64����ֵ��
	//str: Ҫת�����ַ���������123456��
	int64 c2int64(const tchar* str);

	//ת���ַ���Ϊlong����ֵ��
	//str: Ҫת�����ַ���������123456��
	long c2lng(const tchar* str);

	//ת���ַ���Ϊunsigned long����ֵ��
	//str: Ҫת�����ַ���������123456��
	long c2ulng(const tchar* str);

	//ת���ַ���Ϊuint64(unsigned long long)����ֵ��
	//str: Ҫת�����ַ���������123456��
	uint64 c2uint64(const tchar* str);

	//ת���ַ���Ϊfloat����ֵ��
	//str: Ҫת�����ַ���������123.456��123e-2��123.456e2��
	float c2flt(const tchar* str);

	//ת���ַ���Ϊdouble����ֵ��
	//str: Ҫת�����ַ���������123.456��123e-2��123.456e2��
	double c2dbl(const tchar* str);

	//ת��int����ֵΪ�ַ�����
	//val��Ҫת����int����ֵ��
	//out: ����ת������ַ�������int����ֵ�Ļ��棬���С������12*sizeof(tchar)���ֽڡ�
	//����: ָ��out��ָ�롣
	tchar* cstri(int32 val, tchar* out);
	
	//ת��unsigned int����ֵΪ�ַ�����
	//val��Ҫת����unsigned int����ֵ��
	//out: ����ת������ַ�������unsigned int����ֵ�Ļ��棬���С������12*sizeof(tchar)���ֽڡ�
	//����: ָ��out��ָ�롣
	tchar* cstrui(uint32 val, tchar* out);

	//ת��long����ֵΪ�ַ�����
	//val��Ҫת����long����ֵ��
	//out: ����ת������ַ�������long����ֵ�Ļ��棬���С������12*sizeof(tchar)���ֽڡ�
	//����: ָ��out��ָ�롣
	tchar* cstrl(long val, tchar* out);

	//ת��unsigned long����ֵΪ�ַ�����
	//val��Ҫת����unsigned long����ֵ��
	//out: ����ת������ַ�������unsigned long����ֵ�Ļ��棬���С������12*sizeof(tchar)���ֽڡ�
	//����: ָ��out��ָ�롣
	tchar* cstrul(ulong val, tchar* out);

	//ת��int64(long long)����ֵΪ�ַ�����
	//val��Ҫת����int64(long long)����ֵ��
	//out: ����ת������ַ�������int64(long long)����ֵ�Ļ��棬���С������22*sizeof(tchar)���ֽڡ�
	//����: ָ��out��ָ�롣
	tchar* cstri64(int64 val, tchar* out);

	//ת��uint64(unsigned long long)����ֵΪ�ַ�����
	//val��Ҫת����uint64(unsigned long long)����ֵ��
	//out: ����ת������ַ�������uint64(unsigned long long)����ֵ�Ļ��棬���С������22*sizeof(tchar)���ֽڡ�
	//����: ָ��out��ָ�롣
	tchar* cstrui64(uint64 val, tchar* out);

	//ת��double����ֵΪ�ַ�����
	//val: Ҫת����double����ֵ��
	//ndigits: Ҫ������С��λ��
	//out: ����ת������ַ�������double����ֵ�Ļ��棬���С������352 * sizeof(tchar)���ֽڡ�
	//����: ָ��out��ָ�롣
	tchar* cstrd(double val, int32 ndigits, tchar* out);

	//ת��float����ֵΪ�ַ�����
	//val: Ҫת����float����ֵ��
	//ndigits: Ҫ������С��λ��
	//out: ����ת������ַ�������float����ֵ�Ļ��棬���С������56 * sizeof(tchar)���ֽڡ�
	//����: ָ��out��ָ�롣
	tchar* cstrf(float val, int32 ndigits, tchar* out);

#ifdef __cplusplus
}
#endif

#endif  //__UTILITY_H__
