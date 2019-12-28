/*
ģ�飺 cstrui64.c
���ܣ� ת��uint64(unsigned long long)����ֵΪ�ַ�����
������ val��Ҫת����uint64(unsigned long long)����ֵ��
	  out������ת������ַ�������uint64(unsigned long long)����ֵ�Ļ��棬���С������22*sizeof(tchar)���ֽڡ�
���أ� ָ��out��ָ�롣
˵���� �����ͬ�Ľ���Ӧ����printf/sprintf�����
�汾�� 2019-05-20 denglf ģ�鴴��
*/

#include "../typedef.h"
#include "../typecvt.h"

tchar* cstrui64(uint64 val, tchar* out)
{
	tchar* left = out;	//���λ��
	tchar* p = out;		//��ǰλ��
	uint64 n;			//��ǰ����
	tchar c;			//��ǰ�ַ�

	//������ת�����ַ�
	do
	{
		n = val % 10; //ȡ�ø�λ
		val /= 10; //ȥ����λ
		*p++ = _c_tochr(n);//ת����'0'-'9'֮����ַ�
	} while (val > 0);
	*p-- = NUL;//������ӽ�����������ָ��

	//��ת'-'��'\0'֮�������Ϊ��ȷ��˳��
	do
	{
		c = *left;
		*left++ = *p;
		*p-- = c;
	} while (left < p);

	//����
	return out;
}