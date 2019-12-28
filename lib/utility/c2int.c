/*
ģ�飺 c2int.c
���ܣ� ת���ַ���Ϊint����ֵ��
������ str��Ҫת�����ַ�����
���أ� ����intֵ��
˵���� c2int()���׼����atoi����һ�¡�
	  c2int()��ɨ�����str�ַ���������ǰ��Ŀո��ַ���ֱ���������ֻ��������Ųſ�ʼ��ת����
	  �������������ֻ��ַ�������ʱ('\0')�ͽ���ת��������������ء�
�汾�� 2019-05-20 denglf ģ�鴴��
*/

#include "../typedef.h"
#include "../typecvt.h"

int32 c2int(const tchar* str)
{
	const tchar* p = str;
	int32 number;			//��ǰ�ϼ���
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
		if (_int_overflow(number, c)) return negative ? _INT_MIN : _INT_MAX; //���
		number = number * 10 + _c_tonum(c); //�����ۼ�
		c = (int16)*(++p); //��ȡ��һ���ַ�
	}

	//���ؽ��
	return negative ? -number : number;
}

/*
����������㷨��
1������һ���޷������ͱ�����unsigned int n��
2��ȥ���޷��������ֵ�ĸ�λ,��Ϊ����ı߽�ֵ��max_bound_value = UINT_MAX / 10��
3�������޷��������ֵ�ĸ�λ,��Ϊÿ�����ӵ����ֵ��max_per_add_value = UINT_MAX % 10��
4�������ǰ��ֵn<max_bound_value˵����δ��������߽硣
5�������ǰ��ֵn>=max_bound_value˵���Ѿ���������߽硣��ô��
	������ӵ�ֵv<=max_per_add_value��˵����û�г���UINT_MAX��
	������ӵ�ֵv>max_per_add_value,˵����ǰ��ֵn���������ӣ�����ͳ�����UINT_MAX��
	������궨�����£�
	#define _uint_overflow(n, v) ((n >= (UINT_MAX / 10)) && (v > (UINT_MAX % 10)))
	�������£�
	const tchar* p = str;
	uint32 n;
	uint32 max_bound_value = UINT_MAX / 10;
	int16  max_per_add_value = UINT_MAX % 10;
	int16  c = (int16)*p; 
	while (_c_isdigit(c))
	{
		if(_uint_overflow(n, c)) break;
		n = n * 10 + _c_tonum(c); //�����ۼ�
		c = (int16)*(++p); //��ȡ��һ���ַ�
	}
6�����Ҫ����һ���з�������������ôֱ���ж�n�Ƿ���[INT_MIN, INT_MAX]��Χ����, ��������򷵻�INT_MIN��INT_MAX�����£�
	int result = (int)(n);//�����ת�����з���int
	if (negative)//����
	{
		result = -result;
		if (result > 0) return INT_MIN;//��������������˵�������
	}
	else
	{
		if (result < 0) return INT_MAX;//�������˸�����˵�������
	}

С�᣺������������������һ����������������ֻ���ڴ��ַ������ڴ�ֻ�������������CPUֻ�ǽ����λ�����
Ҳ����˵�������������CPU�Ĵ������������緢�����ڴ���ʡ����Ա�������ǰ�ȫ�ģ����ַ���ת������ֵʱ���Բ����жϡ�
*/
 

