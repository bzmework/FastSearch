/* The MIT License

   Copyright (C) 2011, 2012 Zilong Tan (eric.zltan@gmail.com)

   Permission is hereby granted, free of charge, to any person obtaining
   a copy of this software and associated documentation files (the
   "Software"), to deal in the Software without restriction, including
   without limitation the rights to use, copy, modify, merge, publish,
   distribute, sublicense, and/or sell copies of the Software, and to
   permit persons to whom the Software is furnished to do so, subject to
   the following conditions:

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

#ifndef _ULIB_HASH_FUNC_H
#define _ULIB_HASH_FUNC_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint64_t hash_fast64(const void *buf, size_t len, uint64_t seed);

/* The implementation is based on hash_fast64.
 * It should only be used when 1) 32-bit hash value is required, and
 * 2) hash value distribution needs serious attention. Thus, use
 * hash_fast64() instead unless under the above conditions. */
uint32_t hash_fast32(const void *buf, size_t len, uint32_t seed);

/* This hash function combines the values of compression function with
 * Fermat Transformation, T(x) = (M * x mod 2^n) mod (2^n + 1)  */
uint64_t hash_ferm64(const void *buf, size_t len, uint64_t seed);
/* Refer to the notes on hash_fast32 */
uint32_t hash_ferm32(const void *buf, size_t len, uint32_t seed);

/* Bob Jenkins's 32-bit hash function, which is slower than
 * hash_fast32; yet it might still be preferable if multiplication is
 * undesired. This hash function does not use multiplication. */
uint32_t hash_jenkins(const void *buf, size_t len, uint32_t seed);

/* Bob Jenkins's 64-bit hash function, which is slower than
 * hash_fast64. Use this one instead of hash_fast64 if multiplication
 * isn't desired. */
/* @pc and @pb must be non-NULL and initialized with seeds.
 * Hash value will be stored in @pc and @pb also. */
void hash_jenkins2(const void *buf, size_t len, uint32_t * pc, uint32_t * pb);

#ifdef __cplusplus
}
#endif

#endif	/* _ULIB_HASH_FUNC_H */
