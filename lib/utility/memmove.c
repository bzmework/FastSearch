
/*
���ܣ� ��src��ָ�ڴ�������n���ֽڵ�dst��ָ�ڴ������У�������ָ��dstĿ�������ָ�롣
������ dst��Ŀ���ڴ�����
	  src��Դ�ڴ�����;
	  n, �������ֽ�����
���أ� ����ָ��Ŀ���ڴ������ָ�롣
˵���� memmove�ܹ���֤Դ�����ڱ�����֮ǰ���ص�������ֽڿ�����Ŀ�������У������ƺ�Դ���ݻᱻ���ġ�
      ���ǵ�Ŀ��������Դ����û���ص�ʱ�����memcpy�����Ĺ�����ͬ��
*/

#include "../typedef.h"

void* qmemmove(void* dst, const void* src, tint n)
{
	void* ret = dst;

	if (n < 4) //���ֽڿ���
	{
		char* pdst = (char*)dst;
		char* psrc = (char*)src;

		int32 count = n;
		for (; count; count--)
		{
			*pdst++ = *psrc++; //copy from lower addresses to higher addresses
		}
	}
	else //n >= 4 ������ٿ���
	{
		if (dst <= src || (char*)dst >= ((char*)src + n))
		{
			//���ص�������
			//Non-Overlapping Buffers
			//copy from lower addresses to higher addresses

			//����0-len֮�������
			int32* pndst = (int32*)dst; //ת��Ϊ4�ֽ�����ָ��
			int32* pnsrc = (int32*)src;
			int32 len = n >> 2; //�������൱�ڣ�(int)(n / 4);
			for (; len; len--)
			{
				*pndst++ = *pnsrc++; //copy from lower addresses to higher addresses
			}

			//��ʣ������ݰ��ֽڿ���
			int32 rem = n & 3; //���࣬�൱�ڣ�n % 4��
			if (rem)
			{
				char* pcdst = (char*)pndst; 
				char* pcsrc = (char*)pnsrc;
				for (; rem; rem--)
				{
					*pcdst++ = *pcsrc++; //copy from lower addresses to higher addresses
				}
			}
		}
		else
		{
			//�ص�������,��ͬһ������
			//Overlapping Buffers
			//copy from higher addresses to lower addresses

			//����0-len֮�������
			int32* pndst = (int32*)((char*)dst + n); //ת��Ϊ4�ֽ�����ָ��
			int32* pnsrc = (int32*)((char*)src + n);
			int32 len = n >> 2; //�������൱�ڣ�(int)(n / 4);
			for (; len; len--)
			{
				*--pndst = *--pnsrc; //copy from lower addresses to higher addresses
			}

			//��ʣ������ݰ��ֽڿ���
			int32 rem = n & 3; //���࣬�൱�ڣ�n % 4��
			if (rem)
			{
				char* pcdst = (char*)pndst;
				char* pcsrc = (char*)pnsrc;
				for (; rem; rem--)
				{
					*--pcdst = *--pcsrc; //copy from lower addresses to higher addresses
				}
			}
		}
	}

	//����
	return ret;
}