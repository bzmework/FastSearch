/*
ģ�飺 c2dbl.c
���ܣ� ת���ַ���Ϊdouble����ֵ��
������ str��Ҫת�����ַ�����
���أ� ����doubleֵ��
˵���� c2dbl()���׼����strtod����һ�¡�
	  c2dbl()��ɨ�����str�ַ���������ǰ��Ŀո��ַ���ֱ���������ֻ��������Ųſ�ʼ��ת����
	  �����ַ����ֻ��ַ�������ʱ('\0')�Ž���ת��������������أ�
	  ��endptr��ΪNULL����Ὣ����������������ֹ��str�е��ַ�ָ����endptr���أ�
	  ����str�ַ����ɰ��������š�С�����E(e)����ʾָ�����֡���123.456��123e-2��123.456e2��
�汾�� 2019-05-20 denglf ģ�鴴��
*/

#include <errno.h>
#include <math.h>
#include <float.h>

#include "../typedef.h"
#include "../typecvt.h"

double c2dbl(const tchar* str/*, tchar** endptr*/)
{
	double number = 0.0;
	int32 num_digits = 0;
	int32 num_decimals = 0;	//С��λ��
	int32 exponent = 0;		//ָ��
	int32 negative;			//���� 
	tchar* p = (tchar*)str;
	double p10;
	int32 n;

	//����ǰ���ո��
	while (_c_isspace(*p))
	{
		p++;
	}

	//������������
	negative = 0;
	switch (*p)
	{
	case '-': negative = 1; //�½�������λ��
	case '+': p++;//������������
	}

	//ת���ַ�Ϊ����
	//number*10: ��number����10����Ŀ���Ǽ��ϸ�λ����Ϊʲô������100������Ϊֻ��Ҫ����λ����0-9��
	//��������Ҫ��2019-05ת����201905��ֻ��Ҫ����*100�����¼��ɣ���2019*100+5����������ת�������˸�ʽ����Ч����
	//��������n��+x(0<x<n),����ת���ַ���Ϊ���ֵĳ����㷨�ͼ��ɡ�
	while (_c_isdigit(*p))
	{
		number = number * 10.0 + _c_tonum((double)*p); 
		p++;
		num_digits++; 
	}

	//����С������
	//���磺123.456
	if (*p == '.')
	{
		p++;
		while (_c_isdigit(*p))
		{
			number = number * 10.0 + _c_tonum((double)*p);
			p++;
			num_digits++;
			num_decimals++; //��¼С��λ��
		}

		exponent -= num_decimals;
	}

	//���κ�����
	if (num_digits == 0)
	{
		errno = ERANGE;
		return 0.0;
	}

	//���ϸ���
	if (negative)
	{
		number = -number;
	}

	//����ָ��
	//���磺123e-2
	if (*p == 'e' || *p == 'E')
	{
		//������������
		negative = 0;
		switch (*++p)
		{
		case '-': negative = 1;  //�½�������λ��
		case '+': p++; //������������
		}

		//ת���ַ�Ϊ����
		n = 0;
		while (_c_isdigit(*p))
		{
			n = n * 10 + _c_tonum(*p);
			p++;
		}

		//����ָ��
		if (negative)
		{
			exponent -= n;
		}
		else {
			exponent += n;
		}
	}

	//ָ�����
	if (exponent < DBL_MIN_EXP || exponent > DBL_MAX_EXP)
	{
		errno = ERANGE;
		return HUGE_VAL;
	}

	//��ָ�����ָ��ӵ�number��
	p10 = 10.0;
	n = exponent;
	if (n < 0) 
		n = -n;
	while (n) 
	{
		if (n & 1) //���࣬�൱�ڣ�n % 2��
		{
			if (exponent < 0) 
			{
				number /= p10;
			}
			else 
			{
				number *= p10;
			}
		}
		n >>= 1;//�������൱�ڣ�(int)(n / 2);
		p10 *= p10;
	}

	//���
	if (number == HUGE_VAL)
		errno = ERANGE;
	
	//if (endptr) 
	//	*endptr = p;

	return number;
}