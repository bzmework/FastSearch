
/*
��Դ�� ChromeOS
���ܣ� �ַ����ָ
������ pstr���ַ�����ַ;
	  delim���ָ��ַ�����
���أ� ����str��ʼλ�õ�ָ�롣
˵���� ���ַ���strָ���λ�������ɨ�裬����delim�ַ����е������ַ��󣬽����ַ��滻ΪNUL���������ָ
      Ȼ��strָ�붨λ�ڷָ���Ա��str�������зָ
      ��󷵻�str��ʼλ�õ�ָ�룬�Ա�����ָ�õ��ַ�����
ʾ���� char str[] = "hello&world&���&����"; 
	  char* q = str;
	  char* p;
	  while (p = strsep(&q, "&"))
      {
         printf("%s\n", p);
      }
      �����
      hello
      world
      ���
      ����
ע�⣺ ������ַ��������ǿ��޸ĵģ����������malloc����Ķ�̬�ڴ棬���ܴ��볣���ַ�����
*/

#include "../typedef.h"

tchar* qstrpbrk(const tchar* str, const tchar* accept);
char* qastrpbrk(const char* str, const char* accept);
wchar* qwstrpbrk(const wchar* str, const wchar* accept);

tchar* qstrsep(tchar** pstr, const tchar* delim)
{
	tchar* s = *pstr;
	tchar* e;

	if (!s) //�ַ�������Ϊ��
		return NULL;

	e = qstrpbrk(s, delim); //����delim
	if (e)
		*e++ = NUL;

	*pstr = e;//��ָ�붨λ���ָ�λ�ã��Ա��ڶ�str�������зָ�
	return s;//����str��ʼλ�õ�ָ�룬�Ա�����ָ�õ��ַ���
}

char* qastrsep(char** pstr, const char* delim)
{
	char* s = *pstr;
	char* e;

	if (!s) //�ַ�������Ϊ��
		return NULL;

	e = qastrpbrk(s, delim); //����delim
	if (e)
		* e++ = NUL;

	*pstr = e;//��ָ�붨λ���ָ�λ�ã��Ա��ڶ�str�������зָ�
	return s;//����str��ʼλ�õ�ָ�룬�Ա�����ָ�õ��ַ���
}

wchar* qwstrsep(wchar** pstr, const wchar* delim)
{
	wchar* s = *pstr;
	wchar* e;

	if (!s) //�ַ�������Ϊ��
		return NULL;

	e = qwstrpbrk(s, delim); //����delim
	if (e)
		* e++ = NUL;

	*pstr = e;//��ָ�붨λ���ָ�λ�ã��Ա��ڶ�str�������зָ�
	return s;//����str��ʼλ�õ�ָ�룬�Ա�����ָ�õ��ַ���
}
