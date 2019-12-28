/* The MIT License

   Copyright (C) 2012 Zilong Tan (eric.zltan@gmail.com)

   Permission is hereby granted, free of charge, to any person
   obtaining a copy of this software and associated documentation
   files (the "Software"), to deal in the Software without
   restriction, including without limitation the rights to use, copy,
   modify, merge, publish, distribute, sublicense, and/or sell copies
   of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

#ifndef _FASTHASH_H
#define _FASTHASH_H

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * fasthash64 - 64-bit implementation of fasthash
	 * @buf:  data buffer
	 * @len:  data size
	 * @seed: the seed
	 */
	uint64_t hash_fast64(const void* buf, size_t len, uint64_t seed)
	{
		const uint64_t	  m = 0x880355f21e6d1965ULL;
		const uint64_t* pos = (const uint64_t*)buf;
		const uint64_t* end = pos + (len >> 3);
		const unsigned char* pc;
		uint64_t h = len * m ^ seed;
		uint64_t v;

		while (pos != end) {
			v = *pos++;
			v ^= v >> 23;
			v *= 0x2127599bf4325c37ULL;
			h ^= v ^ (v >> 47);
			h *= m;
		}

		pc = (const unsigned char*)pos;
		v = 0;

		switch (len & 7) {
		case 7: v ^= (uint64_t)pc[6] << 48;
		case 6: v ^= (uint64_t)pc[5] << 40;
		case 5: v ^= (uint64_t)pc[4] << 32;
		case 4: v ^= (uint64_t)pc[3] << 24;
		case 3: v ^= (uint64_t)pc[2] << 16;
		case 2: v ^= (uint64_t)pc[1] << 8;
		case 1: v ^= (uint64_t)pc[0];
			v ^= v >> 23;
			v *= 0x2127599bf4325c37ULL;
			h ^= v ^ (v >> 47);
			h *= m;
		}

		h ^= h >> 23;
		h *= 0x2127599bf4325c37ULL;
		h ^= h >> 47;

		return h;
	}

	/**
	 * fasthash32 - 32-bit implementation of fasthash
	 * @buf:  data buffer
	 * @len:  data size
	 * @seed: the seed
	 */
	uint32_t hash_fast32(const void* buf, size_t len, uint32_t seed)
	{
		uint64_t h = hash_fast64(buf, len, seed);
		/* The following trick converts the 64-bit hashcode to a
		 * residue over a Fermat Number, in which information from
		 * both the higher and lower parts of hashcode shall be
		 * retained. */
		return (uint32_t)(h - (h >> 32));
	}



#ifdef __cplusplus
}
#endif

#endif
