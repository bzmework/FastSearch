
/*
来源： GNU C Library
功能： 计算字符串的长度（字符的数量）。
参数： str，字符串。
返回： 返回字符串的长度（字符的数量）。
注意： 返回的是字符数量不是字节数量。
示例： 如果字符集是多字节字符集：
	    tchar *str = _T("abc你好");
		len = strlen(str);
		返回len = 7个字符。由于多字节字符集，ASCII字符占用1个字节,而非ASCII码字符要用2个字节表达, 所以总计7个字节。
	  如果字符集是Unicode字符集：
		tchar *str = _T("abc你好");
		len = strlen(str);
		返回len = 5个字符，由于Unicode字符集，每个字符占用2个字节, 所以总计10个字节。
	 由此可以知道strlen计算的是字符串的字符数量而不是字节数量。
	 如果要计算字节数量应该是这样：strlen(str) * sizeof(tchar)

	 用strlen这个名称来表示计算字符串的长度是合理的，只是它隐含的意思是计算字符串的字符数量，
	 假设用strcount来代替它，那么strcount("abc你好")就显得不合理了，
	 因为"abc你好"是一个字符串，而strcount的字面意思是计算字符串数量，本来就一个字符串，还需要计算吗？显然二者的含义不一致，
	 如果要真实的表达strlen的实际含义，那么可以用诸如getchrcount或chrcount来替换它，例如：
	 getchrcount("abc你好") 或 chrcount("abc你好") 但都不如 strlen("abc你好") 简洁。

	 要记住：在c/c++中，len表示字符数量(简写n)，size表示字节数量。
*/

#include "../typedef.h"

uint32 qstrlen(const tchar* str)
{
	/*
	WindwosNT:

	register const tchar* eos = str;
	while (*eos++);
	return (int32)(eos - str - 1);
	*/

	const tchar* char_ptr;
	const ulong* longword_ptr;
	ulong longword, magic_bits, himagic, lomagic;

	/*
		Handle the first few characters by reading one character at a time.
		Do this until CHAR_PTR is aligned on a longword boundary.
	*/
	for (char_ptr = str; ((ulong)char_ptr & (sizeof(longword) - 1)) != 0; ++char_ptr)
	{
		if (*char_ptr == NUL)
			return char_ptr - str;
	}

	/*
		All these elucidatory comments refer to 4-byte longwords,
		but the theory applies equally well to 8-byte longwords.
	*/
	longword_ptr = (ulong*)char_ptr;

	/*
		Bits 31, 24, 16, and 8 of this number are zero.  Call these bits
		the "holes."  Note that there is a hole just to the left of
		each byte, with an extra at the end:

		bits:  01111110 11111110 11111110 11111111
		bytes: AAAAAAAA BBBBBBBB CCCCCCCC DDDDDDDD

		The 1-bits make sure that carries propagate to the next 0-bit.
		The 0-bits provide holes for carries to fall into.
	*/
	magic_bits = 0x7efefeffL;
	himagic = 0x80808080L;
	lomagic = 0x01010101L;
	if (sizeof(longword) > 4)
	{
		/* 64-bit version of the magic.  */
		/* Do the shift in two steps to avoid a warning if long has 32 bits.  */
		magic_bits = ((0x7efefefeL << 16) << 16) | 0xfefefeffL;
		himagic = ((himagic << 16) << 16) | himagic;
		lomagic = ((lomagic << 16) << 16) | lomagic;
	}

	//if (sizeof (longword) > 8)
	//	abort();

	/*
		Instead of the traditional loop which tests each character,
		we will test a longword at a time.  The tricky part is testing
		if *any of the four* bytes in the longword in question are zero.
	*/
	for (;;)
	{
		/*
			We tentatively exit the loop if adding MAGIC_BITS to
			LONGWORD fails to change any of the hole bits of LONGWORD.

			1) Is this safe?  Will it catch all the zero bytes?
			Suppose there is a byte with all zeros.  Any carry bits
			propagating from its left will fall into the hole at its
			least significant bit and stop.  Since there will be no
			carry from its most significant bit, the LSB of the
			byte to the left will be unchanged, and the zero will be
			detected.

			2) Is this worthwhile?  Will it ignore everything except
			zero bytes?  Suppose every byte of LONGWORD has a bit set
			somewhere.  There will be a carry into bit 8.  If bit 8
			is set, this will carry into bit 16.  If bit 8 is clear,
			one of bits 9-15 must be set, so there will be a carry
			into bit 16.  Similarly, there will be a carry into bit
			24.  If one of bits 24-30 is set, there will be a carry
			into bit 31, so all of the hole bits will be changed.

			The one misfire occurs when bits 24-30 are clear and bit
			31 is set; in this case, the hole at bit 31 is not
			changed.  If we had access to the processor carry flag,
			we could close this loophole by putting the fourth hole
			at bit 32!

			So it ignores everything except 128's, when they're aligned
			properly.
		*/
		longword = *longword_ptr++;

		if (
#if 0
			/* Add MAGIC_BITS to LONGWORD.  */
			(((longword + magic_bits)

				/* Set those bits that were unchanged by the addition.  */
				^ ~longword)

				/* Look at only the hole bits.  If any of the hole bits
				   are unchanged, most likely one of the bytes was a
				   zero.  */
				& ~magic_bits)
#else
			((longword - lomagic) & himagic)
#endif
			!= 0)
		{
			/*
				Which of the bytes was the zero?  If none of them were, it was
				a misfire; continue the search.
			*/

			const tchar* cp = (const tchar*)(longword_ptr - 1);

			if (cp[0] == 0)
				return cp - str;
			if (cp[1] == 0)
				return cp - str + 1;
			if (cp[2] == 0)
				return cp - str + 2;
			if (cp[3] == 0)
				return cp - str + 3;
			if (sizeof(longword) > 4)
			{
				if (cp[4] == 0)
					return cp - str + 4;
				if (cp[5] == 0)
					return cp - str + 5;
				if (cp[6] == 0)
					return cp - str + 6;
				if (cp[7] == 0)
					return cp - str + 7;
			}
		}
	}
}

uint32 qastrlen(const char* str)
{
	const char* char_ptr;
	const ulong* longword_ptr;
	ulong longword, magic_bits, himagic, lomagic;

	/* 
		Handle the first few characters by reading one character at a time.
		Do this until CHAR_PTR is aligned on a longword boundary.  
	*/
	for (char_ptr = str; ((ulong)char_ptr & (sizeof(longword) - 1)) != 0; ++char_ptr)
	{
		if (*char_ptr == NUL)
			return char_ptr - str;
	}
	
	/* 
		All these elucidatory comments refer to 4-byte longwords,
		but the theory applies equally well to 8-byte longwords.  
	*/
	longword_ptr = (ulong*)char_ptr;

	/* 
		Bits 31, 24, 16, and 8 of this number are zero.  Call these bits
		the "holes."  Note that there is a hole just to the left of
		each byte, with an extra at the end:

		bits:  01111110 11111110 11111110 11111111
		bytes: AAAAAAAA BBBBBBBB CCCCCCCC DDDDDDDD

		The 1-bits make sure that carries propagate to the next 0-bit.
		The 0-bits provide holes for carries to fall into.  
	*/
	magic_bits = 0x7efefeffL;
	himagic = 0x80808080L;
	lomagic = 0x01010101L;
	if (sizeof(longword) > 4)
	{
		/* 64-bit version of the magic.  */
		/* Do the shift in two steps to avoid a warning if long has 32 bits.  */
		magic_bits = ((0x7efefefeL << 16) << 16) | 0xfefefeffL;
		himagic = ((himagic << 16) << 16) | himagic;
		lomagic = ((lomagic << 16) << 16) | lomagic;
	}

	//if (sizeof (longword) > 8)
	//	abort();

	/* 
		Instead of the traditional loop which tests each character,
		we will test a longword at a time.  The tricky part is testing
		if *any of the four* bytes in the longword in question are zero.  
	*/
	for (;;)
	{
		/* 
			We tentatively exit the loop if adding MAGIC_BITS to
			LONGWORD fails to change any of the hole bits of LONGWORD.

			1) Is this safe?  Will it catch all the zero bytes?
			Suppose there is a byte with all zeros.  Any carry bits
			propagating from its left will fall into the hole at its
			least significant bit and stop.  Since there will be no
			carry from its most significant bit, the LSB of the
			byte to the left will be unchanged, and the zero will be
			detected.

			2) Is this worthwhile?  Will it ignore everything except
			zero bytes?  Suppose every byte of LONGWORD has a bit set
			somewhere.  There will be a carry into bit 8.  If bit 8
			is set, this will carry into bit 16.  If bit 8 is clear,
			one of bits 9-15 must be set, so there will be a carry
			into bit 16.  Similarly, there will be a carry into bit
			24.  If one of bits 24-30 is set, there will be a carry
			into bit 31, so all of the hole bits will be changed.

			The one misfire occurs when bits 24-30 are clear and bit
			31 is set; in this case, the hole at bit 31 is not
			changed.  If we had access to the processor carry flag,
			we could close this loophole by putting the fourth hole
			at bit 32!

			So it ignores everything except 128's, when they're aligned
			properly.  
		*/
		longword = *longword_ptr++;

		if (
#if 0
			/* Add MAGIC_BITS to LONGWORD.  */
			(((longword + magic_bits)

				/* Set those bits that were unchanged by the addition.  */
				^ ~longword)

				/* Look at only the hole bits.  If any of the hole bits
				   are unchanged, most likely one of the bytes was a
				   zero.  */
				& ~magic_bits)
#else
			((longword - lomagic) & himagic)
#endif
			!= 0)
		{
			/* 
				Which of the bytes was the zero?  If none of them were, it was
				a misfire; continue the search.  
			*/

			const char* cp = (const char*)(longword_ptr - 1);

			if (cp[0] == 0)
				return cp - str;
			if (cp[1] == 0)
				return cp - str + 1;
			if (cp[2] == 0)
				return cp - str + 2;
			if (cp[3] == 0)
				return cp - str + 3;
			if (sizeof(longword) > 4)
			{
				if (cp[4] == 0)
					return cp - str + 4;
				if (cp[5] == 0)
					return cp - str + 5;
				if (cp[6] == 0)
					return cp - str + 6;
				if (cp[7] == 0)
					return cp - str + 7;
			}
		}
	}
}

uint32 qwstrlen(const wchar* str)
{
	const wchar* char_ptr;
	const ulong* longword_ptr;
	ulong longword, magic_bits, himagic, lomagic;

	/*
		Handle the first few characters by reading one character at a time.
		Do this until CHAR_PTR is aligned on a longword boundary.
	*/
	for (char_ptr = str; ((ulong)char_ptr & (sizeof(longword) - 1)) != 0; ++char_ptr)
	{
		if (*char_ptr == NUL)
			return char_ptr - str;
	}

	/*
		All these elucidatory comments refer to 4-byte longwords,
		but the theory applies equally well to 8-byte longwords.
	*/
	longword_ptr = (ulong*)char_ptr;

	/*
		Bits 31, 24, 16, and 8 of this number are zero.  Call these bits
		the "holes."  Note that there is a hole just to the left of
		each byte, with an extra at the end:

		bits:  01111110 11111110 11111110 11111111
		bytes: AAAAAAAA BBBBBBBB CCCCCCCC DDDDDDDD

		The 1-bits make sure that carries propagate to the next 0-bit.
		The 0-bits provide holes for carries to fall into.
	*/
	magic_bits = 0x7efefeffL;
	himagic = 0x80808080L;
	lomagic = 0x01010101L;
	if (sizeof(longword) > 4)
	{
		/* 64-bit version of the magic.  */
		/* Do the shift in two steps to avoid a warning if long has 32 bits.  */
		magic_bits = ((0x7efefefeL << 16) << 16) | 0xfefefeffL;
		himagic = ((himagic << 16) << 16) | himagic;
		lomagic = ((lomagic << 16) << 16) | lomagic;
	}

	//if (sizeof (longword) > 8)
	//	abort();

	/*
		Instead of the traditional loop which tests each character,
		we will test a longword at a time.  The tricky part is testing
		if *any of the four* bytes in the longword in question are zero.
	*/
	for (;;)
	{
		/*
			We tentatively exit the loop if adding MAGIC_BITS to
			LONGWORD fails to change any of the hole bits of LONGWORD.

			1) Is this safe?  Will it catch all the zero bytes?
			Suppose there is a byte with all zeros.  Any carry bits
			propagating from its left will fall into the hole at its
			least significant bit and stop.  Since there will be no
			carry from its most significant bit, the LSB of the
			byte to the left will be unchanged, and the zero will be
			detected.

			2) Is this worthwhile?  Will it ignore everything except
			zero bytes?  Suppose every byte of LONGWORD has a bit set
			somewhere.  There will be a carry into bit 8.  If bit 8
			is set, this will carry into bit 16.  If bit 8 is clear,
			one of bits 9-15 must be set, so there will be a carry
			into bit 16.  Similarly, there will be a carry into bit
			24.  If one of bits 24-30 is set, there will be a carry
			into bit 31, so all of the hole bits will be changed.

			The one misfire occurs when bits 24-30 are clear and bit
			31 is set; in this case, the hole at bit 31 is not
			changed.  If we had access to the processor carry flag,
			we could close this loophole by putting the fourth hole
			at bit 32!

			So it ignores everything except 128's, when they're aligned
			properly.
		*/
		longword = *longword_ptr++;

		if (
#if 0
			/* Add MAGIC_BITS to LONGWORD.  */
			(((longword + magic_bits)

				/* Set those bits that were unchanged by the addition.  */
				^ ~longword)

				/* Look at only the hole bits.  If any of the hole bits
				   are unchanged, most likely one of the bytes was a
				   zero.  */
				& ~magic_bits)
#else
			((longword - lomagic) & himagic)
#endif
			!= 0)
		{
			/*
				Which of the bytes was the zero?  If none of them were, it was
				a misfire; continue the search.
			*/

			const wchar* cp = (const wchar*)(longword_ptr - 1);

			if (cp[0] == 0)
				return cp - str;
			if (cp[1] == 0)
				return cp - str + 1;
			if (cp[2] == 0)
				return cp - str + 2;
			if (cp[3] == 0)
				return cp - str + 3;
			if (sizeof(longword) > 4)
			{
				if (cp[4] == 0)
					return cp - str + 4;
				if (cp[5] == 0)
					return cp - str + 5;
				if (cp[6] == 0)
					return cp - str + 6;
				if (cp[7] == 0)
					return cp - str + 7;
			}
		}
	}
}

uint32 qstrlenb(const tchar* str)
{
	return qstrlen(str) * sizeof(tchar);
}

uint32 qastrlenb(const char* str)
{
	return qastrlen(str) * sizeof(char);
}

uint32 qwstrlenb(const wchar * str)
{
	return qwstrlen(str) * sizeof(wchar);
}
