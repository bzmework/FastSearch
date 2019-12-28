
#ifndef __BLIZZARDHASH_H__
#define __BLIZZARDHASH_H__

//暴雪Hash

#include <stdlib.h>
#include <stdint.h> 

uint32_t cryptTable[0x1000];  //密码表

//生成密码表
static void PrepareCryptTable()
{
	uint32_t seed = 0x00100001, index1 = 0, index2 = 0, i;

	for (index1 = 0; index1 < 0x100; index1++)
	{
		for (index2 = index1, i = 0; i < 5; i++, index2 += 0x100)
		{
			uint32_t temp1, temp2;
			seed = (seed * 125 + 3) % 0x2AAAAB;
			temp1 = (seed & 0xFFFF) << 0x10;
			seed = (seed * 125 + 3) % 0x2AAAAB;
			temp2 = (seed & 0xFFFF);
			cryptTable[index2] = (temp1 | temp2);
		}
	}
}

/*按照dwHashType定义的类型取得字符串lpszFileName的各项hash值*/
static uint32_t BlizzardHash(const char* data, uint32_t dwHashType)
{
	unsigned char* key = (unsigned char*)data;
	uint32_t seed1 = 0x7FED7FED;
	uint32_t seed2 = 0xEEEEEEEE;
	int ch;

	while (*key)
	{
		ch = *key++;
		seed1 = cryptTable[(dwHashType << 8) + ch] ^ (seed1 + seed2);
		seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;
	}

	return seed1;
}


#endif /* __BLIZZARDHASH_H__ */
