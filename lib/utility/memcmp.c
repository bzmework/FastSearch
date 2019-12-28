
/*
���ܣ� ��srcԴ�ڴ�������dstĿ���ڴ������n���ֽڽ��бȽ�
������ dst��Ŀ���ڴ�����
	  src��Դ�ڴ�����;
	  n, �Ƚϵ��ֽ�����
���أ� ��� dst > src ����ֵ > 0
	  ��� dst < src ����ֵ < 0
	  ��� dst = src ����ֵ = 0
˵����
*/

#include <emmintrin.h>
#include "../typedef.h"

int32 qmemcmp(const void* dst, const void* src, tint n)
{
	//�����ж�
	//if (!n) //Ϊ��
	//	return 0;
	//if (dst == src) //��ͬ���ڴ�����
	//	return 0;

	//�ο�: https://macosxfilerecovery.com/faster-memory-comparison-in-c/
	//     ����������˵���Ż���Ҫ��һЩ��׼��Ĵ�ͳ�㷨��35%��
	//     ������Ϊ���ڵ�CPU�Ƚ�����4�ֽ��������ȱȽ��������ֽ��ַ�Ҫ�죬������Ҫ���ٵ�CPU���ڡ�
	//�ο���http://www.picklingtools.com/study.pdf
	//     ����ƪ���������߸�����ͬ���Ľ��ۣ���4�ֽڶ����Ż��㷨����������Ϊ������㷨��
	//�ο���GNU C Library: http://www.gnu.org
	//     glibc��׼��ͬ����4�ֽڶ����Ż��㷨������memcmp.c��������һ����Ϊ���Ƶ��Ż��㷨��
	//�������ϲο��㷨����д���Ż��㷨���£�
	if (n < 4)//���ֽڱȽ�
	{
		//�ο�FreeBSD�ȸ����㷨���Ż��Ĵ�ͳ�㷨���£�
		const uchar* pdst = (uchar*)dst;
		const uchar* psrc = (uchar*)src;
		tint count = n;
		for (; count; count--)
		{
			if (*pdst != *psrc)
				return (uchar)*pdst - (uchar)*psrc;
			pdst++;
			psrc++;
		}
	}
	else //n >= 4 ������ٱȽ�
	{
		//�Ƚ�0-len֮�������
		const int32* pndst = (int32*)dst; //ת��Ϊ4�ֽ�����ָ��
		const int32* pnsrc = (int32*)src;
		int32 len = n >> 2; //�������൱�ڣ�(int)(n / 4);
		for (; len; len--)
		{
			if (*pndst != *pnsrc) 
				return (uchar)*pndst - (uchar)*pnsrc;
			pndst++;
			pnsrc++;
		}

		//��ʣ������4���ֽڰ��մ�ͳ�㷨�Ƚ�
		const uint8* pcdst = (uint8*)pndst; 
		const uint8* pcsrc = (uint8*)pnsrc;
		int32 rem = n & 3; //���࣬�൱�ڣ�n % 4��
		for (; rem; rem--)
		{
			if (*pcdst != *pcsrc) 
				return (uchar)*pcdst - (uchar)*pcsrc;
			pcdst++;
			pcsrc++;
		}
	}

	//���
	return 0;
}
 