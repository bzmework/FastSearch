
/*
���ܣ� ��src��ָԴ�ڴ�������n���ֽڵ�dst��ָ��Ŀ���ڴ������У�������ָ��dstĿ���ڴ������ָ�롣
������ dst��Ŀ���ڴ�����
	  src��Դ�ڴ�����;
	  n, �������ֽ�����
���أ� ����ָ��Ŀ���ڴ������ָ�롣
˵���� ����ʱע����C/C++�����м��ϣ�/arch:SSE2 �μ���https://docs.microsoft.com/en-us/cpp/build/reference/arch-x86?view=vs-2019
*/

#include <emmintrin.h>
#include <string.h>
#include "../typedef.h"

//����С����(<= 128 �ֽ�), ��4�ֽڶ��뿽��
_INLINE_ void* qmemcpys(void* dst, const void* src, tint n)
{
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
			len = n >> 2;
			char* pcdst = (char*)pndst;
			char* pcsrc = (char*)pnsrc;
			for (; rem; rem--)
			{
				*pcdst++ = *pcsrc++; //copy from lower addresses to higher addresses
			}
		}
	}

	//����
	return dst;
}

//�ο���https://github.com/skywind3000/FastMemcpy
//����������(>128�ֽ�), ��16�ֽڶ��뿽��
_INLINE_ void* qmemcpyl(void* dst, const void* src, tint n)
{
	static tint cachesize = 0x200000; // L2-cache size

	void* ret = dst;
	char* pdst = (char*)dst;
	const char* psrc = (const char*)src;
	tint padding = 0;
	tint count = n;

	//���뽫Ŀ����뵽16�ֽڱ߽�
	padding = (16 - (((tint)pdst) & 15)) & 15;
	if (padding > 0)
	{
		__m128i head = _mm_loadu_si128((const __m128i*)psrc);
		_mm_storeu_si128((__m128i*)pdst, head);
		pdst += padding;
		psrc += padding;
		count -= padding;
	}

	// �еȴ�С�����ݿ���
	if (count <= cachesize)
	{
		__m128i c0, c1, c2, c3, c4, c5, c6, c7;

		for (; count >= 128; count -= 128)
		{
			//��128�ֽ�
			c0 = _mm_loadu_si128(((const __m128i*)psrc) + 0);
			c1 = _mm_loadu_si128(((const __m128i*)psrc) + 1);
			c2 = _mm_loadu_si128(((const __m128i*)psrc) + 2);
			c3 = _mm_loadu_si128(((const __m128i*)psrc) + 3);
			c4 = _mm_loadu_si128(((const __m128i*)psrc) + 4);
			c5 = _mm_loadu_si128(((const __m128i*)psrc) + 5);
			c6 = _mm_loadu_si128(((const __m128i*)psrc) + 6);
			c7 = _mm_loadu_si128(((const __m128i*)psrc) + 7);

			//Ԥȡ
			_mm_prefetch((const char*)(psrc + 256), _MM_HINT_NTA);

			//д128�ֽ�
			_mm_store_si128((((__m128i*)pdst) + 0), c0);
			_mm_store_si128((((__m128i*)pdst) + 1), c1);
			_mm_store_si128((((__m128i*)pdst) + 2), c2);
			_mm_store_si128((((__m128i*)pdst) + 3), c3);
			_mm_store_si128((((__m128i*)pdst) + 4), c4);
			_mm_store_si128((((__m128i*)pdst) + 5), c5);
			_mm_store_si128((((__m128i*)pdst) + 6), c6);
			_mm_store_si128((((__m128i*)pdst) + 7), c7);

			//��һ��
			psrc += 128;
			pdst += 128;
		}

		//����ʣ������
		memcpy(pdst, psrc, count);
	}
	else //�����ݿ���
	{
		__m128i c0, c1, c2, c3, c4, c5, c6, c7;

		//��Ԥȡ
		_mm_prefetch((const char*)(psrc), _MM_HINT_NTA);

		//����
		if ((((size_t)psrc) & 15) == 0) //Դ16�ֽڶ���
		{	
			for (; count >= 128; count -= 128)
			{
				//��128�ֽ�
				c0 = _mm_load_si128(((const __m128i*)psrc) + 0);
				c1 = _mm_load_si128(((const __m128i*)psrc) + 1);
				c2 = _mm_load_si128(((const __m128i*)psrc) + 2);
				c3 = _mm_load_si128(((const __m128i*)psrc) + 3);
				c4 = _mm_load_si128(((const __m128i*)psrc) + 4);
				c5 = _mm_load_si128(((const __m128i*)psrc) + 5);
				c6 = _mm_load_si128(((const __m128i*)psrc) + 6);
				c7 = _mm_load_si128(((const __m128i*)psrc) + 7);

				//Ԥȡ
				_mm_prefetch((const char*)(psrc + 256), _MM_HINT_NTA);

				//д128�ֽ�
				_mm_stream_si128((((__m128i*)pdst) + 0), c0);
				_mm_stream_si128((((__m128i*)pdst) + 1), c1);
				_mm_stream_si128((((__m128i*)pdst) + 2), c2);
				_mm_stream_si128((((__m128i*)pdst) + 3), c3);
				_mm_stream_si128((((__m128i*)pdst) + 4), c4);
				_mm_stream_si128((((__m128i*)pdst) + 5), c5);
				_mm_stream_si128((((__m128i*)pdst) + 6), c6);
				_mm_stream_si128((((__m128i*)pdst) + 7), c7);

				//��һ��
				psrc += 128;
				pdst += 128;
			}
		}
		else //Դδ16�ֽڶ���
		{	
			for (; count >= 128; count -= 128)
			{
				//��128�ֽ�
				c0 = _mm_loadu_si128(((const __m128i*)psrc) + 0);
				c1 = _mm_loadu_si128(((const __m128i*)psrc) + 1);
				c2 = _mm_loadu_si128(((const __m128i*)psrc) + 2);
				c3 = _mm_loadu_si128(((const __m128i*)psrc) + 3);
				c4 = _mm_loadu_si128(((const __m128i*)psrc) + 4);
				c5 = _mm_loadu_si128(((const __m128i*)psrc) + 5);
				c6 = _mm_loadu_si128(((const __m128i*)psrc) + 6);
				c7 = _mm_loadu_si128(((const __m128i*)psrc) + 7);

				//Ԥȡ
				_mm_prefetch((const char*)(psrc + 256), _MM_HINT_NTA);
				
				//д��
				_mm_stream_si128((((__m128i*)pdst) + 0), c0);
				_mm_stream_si128((((__m128i*)pdst) + 1), c1);
				_mm_stream_si128((((__m128i*)pdst) + 2), c2);
				_mm_stream_si128((((__m128i*)pdst) + 3), c3);
				_mm_stream_si128((((__m128i*)pdst) + 4), c4);
				_mm_stream_si128((((__m128i*)pdst) + 5), c5);
				_mm_stream_si128((((__m128i*)pdst) + 6), c6);
				_mm_stream_si128((((__m128i*)pdst) + 7), c7);
				
				//��һ��
				psrc += 128;
				pdst += 128;
			}
		}
		_mm_sfence();

		//����ʣ������
		memcpy(pdst, psrc, count);
	}

	return ret;

}

//���ٿ���
//ע�������ѭ���ڽ��д�������������Ӧ��ֱ�ӵ���qmemcpys/qmemcpyl��
//��Ϊ������ջ�л������÷ǳ����ʱ�䡣
void* qmemcpy(void* dst, const void* src, tint n)
{
	if (n > 128) //������
	{
		return qmemcpyl(dst, src, n);
	}
	else //С����
	{
		//���Է���ϵͳ���õ�memcpy��������С��128�ֽڵ�����£��������Ѿ���������Ż�
		//return memcpy(dst, src, n);
		return qmemcpys(dst, src, n);
	}
}
