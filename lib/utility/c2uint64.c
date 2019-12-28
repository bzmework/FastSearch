/*
ģ�飺 c2uint64.c
���ܣ� ת���ַ���Ϊuint64(unsigned long long)����ֵ��
������ str��Ҫת�����ַ�����
���أ� ����uint64(unsigned long long)ֵ��
˵���� c2uint64()���׼����strtoull����һ�¡�
	  c2uint64()��ɨ�����str�ַ���������ǰ��Ŀո��ַ���ֱ���������ֻ��������Ųſ�ʼ��ת����
	  �������������ֻ��ַ�������ʱ('\0')�ͽ���ת��������������ء�
�汾�� 2019-05-20 denglf ģ�鴴��
*/

#include "../typedef.h"
#include "../typecvt.h"

uint64 c2uint64(const tchar* str)
{
	const tchar* p = str;
	uint64 number;			//��ǰ�ϼ���
	int32 negative;			//���� 
	int16 c;				//��ǰ�ַ����ַ����ռ�������ֽڣ����Զ����int16

	//����ǰ���ո���ַ�
	while (_c_isspace(*p))
	{
		p++;
	}

	//������������
	negative = 0;
	switch (*p)
	{
	case '-': negative = 1; //�½���+λ��
	case '+': p++;//������������
	}

	//ת���ַ�Ϊ����
	//number * 10: ��number����10����Ŀ���Ǽ��ϸ�λ����Ϊʲô������100������Ϊֻ��Ҫ����λ����0-9��
	//��������Ҫ��2019-05ת����201905��ֻ��Ҫ����*100�����¼��ɣ���2019*100+5����������ת�������˸�ʽ����Ч����
	//��������n��+x(0<x<n),����ת���ַ���Ϊ���ֵĳ����㷨�ͼ��ɡ�
	number = 0;
	c = (int16)*p;
	while (_c_isdigit(c))
	{
		if (_uint64_overflow(number, c)) return _ULLONG_MAX; //���
		number = number * 10 + _c_tonum((uint64)c); //�����ۼ�
		c = (int16) * (++p); //��ȡ��һ���ַ�
	}

	//���ؽ��
	return negative ? _ULLONG_MAX - number + 1 : number;
}


