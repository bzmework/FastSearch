/*
ģ�飺 cstri64.c
���ܣ� ת��int64(long long)����ֵΪ�ַ�����
������ val��Ҫת����int64(long long)����ֵ��
	  out������ת������ַ�������int64(long long)����ֵ�Ļ��棬���С������22*sizeof(tchar)���ֽڡ�
���أ� ָ��out��ָ�롣
˵���� �����ͬ�Ľ���Ӧ����printf/sprintf�����
�汾�� 2019-05-20 denglf ģ�鴴��
*/

#include "../typedef.h"
#include "../typecvt.h"

tchar* cstri64(int64 val, tchar* out)
{
	tchar* left = out;	//���λ��
	tchar* p = out;		//��ǰλ��
	int64 n;			//��ǰ����
	tchar c;			//��ǰ�ַ�

	//������ת��������
	if (val < 0)
	{
		val = -val; //ת��������
		*p = __T('-'); //���������д�븺��
		left = ++p; //��ǰ�ƶ�һ��λ��
	}

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