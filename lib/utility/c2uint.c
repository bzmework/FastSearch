/*
ģ�飺 c2uint.c
���ܣ� ת���ַ���Ϊunsigned int����ֵ��
������ str��Ҫת�����ַ�����
���أ� ����intֵ��
˵���� c2ulng()���׼����strtoul����һ�¡� 
	  c2uint()��ɨ�����str�ַ���������ǰ��Ŀո��ַ���ֱ���������ֻ��������Ųſ�ʼ��ת����
	  �������������ֻ��ַ�������ʱ('\0')�ͽ���ת��������������ء�
�汾�� 2019-05-20 denglf ģ�鴴��
*/

#include "../typedef.h"
#include "../typecvt.h"

uint32 c2uint(const tchar* str)
{
	const tchar* p = str;
	uint32 number;			//��ǰ�ϼ���
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
	//��������Ҫ��2019-05ת����201905��ֻ��Ҫ����*10�����¼��ɣ���2019*10+5����������ת�������˸�ʽ����Ч����
	//��������n��+x(0<x<n),����ת���ַ���Ϊ���ֵĳ����㷨�ͼ��ɡ�
	number = 0;
	c = (int16)*p;
	while (_c_isdigit(c))
	{
		if (_uint_overflow(number, c)) return _UINT_MAX; //���
		number = number * 10 + _c_tonum(c); //�����ۼ�
		c = (int16)*(++p); //��ȡ��һ���ַ�
	}

	//���ؽ��
	return negative ? _UINT_MAX - number + 1 : number;
}
 

