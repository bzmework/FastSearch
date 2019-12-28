//
// vsprintf.c
//
// Print formatting routines
//
// Copyright (C) 2002 Michael Ringgaard. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright 
//    notice, this list of conditions and the following disclaimer.  
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.  
// 3. Neither the name of the project nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission. 
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
// SUCH DAMAGE.
// 

//
// 本代码根据sanos中的vsprintf.c改写。
// sanos是一个极小内核的操作系统，其源代码主要继承自微软NT操作系统，官网地址:
// http://www.jbox.dk/sanos/
// http://www.jbox.dk/sanos/download.htm
//

//#include <sys/types.h>
#include <stdarg.h>
#include <math.h>
#include "../typedef.h"

#define ZEROPAD 1               // Pad with zero
#define SIGN    2               // Unsigned/signed long
#define PLUS    4               // Show plus
#define SPACE   8               // Space if plus
#define LEFT    16              // Left justified
#define SPECIAL 32              // 0x
#define LARGE   64              // Use 'ABCDEF' instead of 'abcdef'

#define CVTBUFSIZE	(309 + 43)

#define ISDIGIT(c) ((c) >= __T('0') && (c) <= __T('9'))

static tchar *digits = __T("0123456789abcdefghijklmnopqrstuvwxyz");
static tchar *upper_digits = __T("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");

uint32 qstrlen(const tchar* str);
uint32 qstrnlen(const tchar* str, uint32 n);
void* qmemcpy(void* dst, const void* src, tint n);

_INLINE_ tchar* p_cvt(double arg, int32 ndigits, int32* decpt, int32* sign, tchar* buf, int32 eflag)
{
	int32 r2;
	double fi, fj;
	tchar* p, * p1;

	if (ndigits < 0) ndigits = 0;
	if (ndigits >= CVTBUFSIZE - 1) ndigits = CVTBUFSIZE - 2;

	r2 = 0;
	*sign = 0;
	p = &buf[0];
	if (arg < 0)
	{
		*sign = 1;
		arg = -arg;
	}
	arg = modf(arg, &fi);
	p1 = &buf[CVTBUFSIZE];

	if (fi != 0) //有整数部分
	{
		p1 = &buf[CVTBUFSIZE];
		while (fi != 0 && p1 > p)
		{
			fj = modf(fi / 10, &fi);
			*--p1 = (int32)((fj + .03) * 10) + __T('0');
			r2++;
		}
		while (p1 < &buf[CVTBUFSIZE])
		{
			*p++ = *p1++;
		}
	}
	else if (arg > 0) //只有小数部分
	{
		while ((fj = arg * 10) < 1)
		{
			arg = fj;
			r2--;
		}
	}

	p1 = &buf[ndigits];
	if (eflag == 0)
	{
		p1 += r2;
	}
	*decpt = r2;
	if (p1 < &buf[0])
	{
		buf[0] = NUL;
		return buf;
	}

	//处理小数部分
	while (p <= p1 && p < &buf[CVTBUFSIZE])
	{
		arg *= 10;
		arg = modf(arg, &fj);
		*p++ = (int32)fj + __T('0');
	}

	if (p1 >= &buf[CVTBUFSIZE])
	{
		buf[CVTBUFSIZE - 1] = __T('\0');
		return buf;
	}

	//四舍五入
	p = p1;
	*p1 += 5;
	while (*p1 > __T('9'))
	{
		*p1 = __T('0');
		if (p1 > buf)
			++* --p1;
		else
		{
			*p1 = __T('1');
			(*decpt)++;
			if (eflag == 0)
			{
				if (p > buf)* p = __T('0');
				p++;
			}
		}
	}
	*p = NUL;

	return buf;
}

_INLINE_ tchar* p_ecvtbuf(double arg, int32 ndigits, int32 * decpt, int32 * sign, tchar * buf)
{
	return p_cvt(arg, ndigits, decpt, sign, buf, 1);
}

_INLINE_ tchar* p_fcvtbuf(double arg, int32 ndigits, int32 * decpt, int32 * sign, tchar * buf)
{
	return p_cvt(arg, ndigits, decpt, sign, buf, 0);
}

_INLINE_ void p_cfltcvt(double value, tchar * buffer, tchar fmt, int32 precision)
{
	int32 decpt, sign, exp, pos;
	tchar* digits = NULL;
	tchar cvtbuf[80] = { '0' };
	int32 capexp = 0;
	int32 magnitude;

	if (fmt == __T('G') || fmt == __T('E'))
	{
		capexp = 1;
		fmt += __T('a') - __T('A');
	}

	if (fmt == __T('g'))
	{
		digits = p_ecvtbuf(value, precision, &decpt, &sign, cvtbuf);
		magnitude = decpt - 1;
		if (magnitude < -4 || magnitude > precision - 1)
		{
			fmt = __T('e');
			precision -= 1;
		}
		else
		{
			fmt = __T('f');
			precision -= decpt;
		}
	}

	if (fmt == __T('e'))
	{
		digits = p_ecvtbuf(value, precision + 1, &decpt, &sign, cvtbuf);

		if (sign)
		{
			*buffer++ = __T('-');
		}
		*buffer++ = *digits;
		if (precision > 0)
		{
			*buffer++ = __T('.');
		}
		qmemcpy(buffer, digits + 1, precision);
		buffer += precision;
		*buffer++ = capexp ? __T('E') : __T('e');

		if (decpt == 0)
		{
			if (value == 0.0)
				exp = 0;
			else
				exp = -1;
		}
		else
		{
			exp = decpt - 1;
		}

		if (exp < 0)
		{
			*buffer++ = __T('-');
			exp = -exp;
		}
		else
		{
			*buffer++ = __T('+');
		}

		buffer[2] = (exp % 10) + __T('0');
		exp = exp / 10;
		buffer[1] = (exp % 10) + __T('0');
		exp = exp / 10;
		buffer[0] = (exp % 10) + __T('0');
		buffer += 3;
	}
	else if (fmt == __T('f'))
	{
		digits = p_fcvtbuf(value, precision, &decpt, &sign, cvtbuf);
		if (sign)
		{
			*buffer++ = __T('-');
		}
		if (*digits)
		{
			if (decpt <= 0)
			{
				*buffer++ = __T('0');
				*buffer++ = __T('.');
				for (pos = 0; pos < -decpt; pos++)
					*buffer++ = __T('0');
				while (*digits)
					*buffer++ = *digits++;
			}
			else
			{
				pos = 0;
				while (*digits)
				{
					if (pos++ == decpt)
						*buffer++ = __T('.');
					*buffer++ = *digits++;
				}
			}
		}
		else
		{
			*buffer++ = __T('0');
			if (precision > 0)
			{
				*buffer++ = __T('.');
				for (pos = 0; pos < precision; pos++)
					*buffer++ = __T('0');
			}
		}
	}

	*buffer = NUL;
}

_INLINE_ void p_forcdecpt(tchar * buffer)
{
	while (*buffer)
	{
		if (*buffer == __T('.')) return;
		if (*buffer == __T('e') || *buffer == __T('E')) break;
		buffer++;
	}

	if (*buffer)
	{
		int n = qstrlen(buffer);
		while (n > 0)
		{
			buffer[n + 1] = buffer[n];
			n--;
		}

		*buffer = __T('.');
	}
	else
	{
		*buffer++ = __T('.');
		*buffer = NUL;
	}
}

_INLINE_ void p_cropzeros(tchar* buffer)
{
	tchar* stop;

	while (*buffer && *buffer != __T('.'))
	{
		buffer++;
	}
	if (*buffer++)
	{
		while (*buffer && *buffer != __T('e') && *buffer != __T('E'))
		{
			buffer++;
		}
		stop = buffer--;
		while (*buffer == __T('0'))
		{
			buffer--;
		}
		if (*buffer == __T('.'))
		{
			buffer--;
		}
		while (*++buffer = *stop++)
			;
	}
}

_INLINE_ tchar* p_flt(tchar * str, double num, int32 size, int32 precision, tchar fmt, int32 flags)
{
	tchar tmp[80] = { '0' };
	tchar c, sign;
	int32 n, i;

	// Left align means no zero padding
	if (flags & LEFT)
	{
		flags &= ~ZEROPAD;
	}
	
	// Determine padding and sign char
	c = (flags & ZEROPAD) ? __T('0') : __T(' ');
	sign = 0;
	if (flags & SIGN)
	{
		if (num < 0.0)
		{
			sign = __T('-');
			num = -num;
			size--;
		}
		else if (flags & PLUS)
		{
			sign = __T('+');
			size--;
		}
		else if (flags & SPACE)
		{
			sign = __T(' ');
			size--;
		}
	}

	// Compute the precision value
	if (precision < 0)
	{
		precision = 6; // Default precision: 6
	}
	else if (precision == 0 && fmt == __T('g'))
	{
		precision = 1; // ANSI specified
	}

	// Convert floating point number to text
	p_cfltcvt(num, tmp, fmt, precision);

	// '#' and precision == 0 means force a decimal point
	if ((flags & SPECIAL) && precision == 0)
	{
		p_forcdecpt(tmp);
	}
		
	// 'g' format means crop zero unless '#' given
	if (fmt == __T('g') && !(flags & SPECIAL))
	{
		p_cropzeros(tmp);
	}
	
	n = qstrlen(tmp);

	// Output number with alignment and padding
	size -= n;
	if (!(flags & (ZEROPAD | LEFT)))
	{
		while (size-- > 0) 
			*str++ = __T(' ');
	}
	if (sign)
	{
		*str++ = sign;
	}
	if (!(flags & LEFT))
	{
		while (size-- > 0) 
			*str++ = c;
	}
	for (i = 0; i < n; i++)
	{
		*str++ = tmp[i];
	}
		
	while (size-- > 0)
	{
		*str++ = __T(' ');
	}

	return str;
}

_INLINE_ int32 p_skip_atoi(const tchar * *s)
{
	int32 i = 0;
	while (ISDIGIT(**s))
	{
		i = i * 10 + *((*s)++) - __T('0');
	}
	return i;
}

_INLINE_ tchar* p_number(tchar * str, int32 num, int32 base, int32 size, int32 precision, int32 type)
{
	tchar c, sign, tmp[66] = { '0' };
	tchar* dig = digits;
	int32 i;

	if (type & LARGE) 
		dig = upper_digits;
	if (type & LEFT) 
		type &= ~ZEROPAD;
	if (base < 2 || base > 36) 
		return 0;

	c = (type & ZEROPAD) ? __T('0') : __T(' ');
	sign = 0;
	if (type & SIGN)
	{
		if (num < 0)
		{
			sign = __T('-');
			num = -num;
			size--;
		}
		else if (type & PLUS)
		{
			sign = __T('+');
			size--;
		}
		else if (type & SPACE)
		{
			sign = __T(' ');
			size--;
		}
	}

	if (type & SPECIAL)
	{
		if (base == 16)
			size -= 2;
		else if (base == 8)
			size--;
	}

	i = 0;

	if (num == 0)
	{
		tmp[i++] = __T('0');
	}
	else
	{
		while (num != 0)
		{
			tmp[i++] = dig[((uint32)num) % (unsigned)base];
			num = ((uint32)num) / (unsigned)base;
		}
	}

	if (i > precision)
	{
		precision = i;
	}
	size -= precision;
	if (!(type & (ZEROPAD | LEFT)))
	{
		while (size-- > 0)
			* str++ = __T(' ');
	}
	if (sign)
	{
		*str++ = sign;
	}
	if (type & SPECIAL)
	{
		if (base == 8)
		{
			*str++ = __T('0');
		}
		else if (base == 16)
		{
			*str++ = __T('0');
			*str++ = digits[33];
		}
	}

	if (!(type & LEFT))
	{
		while (size-- > 0)
			*str++ = c;
	}
		
	while (i < precision--)
		*str++ = __T('0');

	while (i-- > 0)
		*str++ = tmp[i];

	while (size-- > 0)
		*str++ = __T(' ');

	return str;
}

_INLINE_ tchar* p_eaddr(tchar * str, utchar * addr, int32 size, int32 precision, int32 type)
{
	tchar tmp[24] = { '0' };
	tchar* dig = digits;
	int32 i, len;

	if (type & LARGE)
	{
		dig = upper_digits;
	}
	
	len = 0;
	for (i = 0; i < 6; i++)
	{
		if (i != 0) tmp[len++] = __T(':');
		tmp[len++] = dig[addr[i] >> 4];
		tmp[len++] = dig[addr[i] & 0x0F];
	}

	if (!(type & LEFT))
	{
		while (len < size--)
			*str++ = __T(' ');
	}
		
	for (i = 0; i < len; ++i)
	{
		*str++ = tmp[i];
	}
		
	while (len < size--)
	{
		*str++ = __T(' ');
	}

	return str;
}

_INLINE_ tchar* p_iaddr(tchar * str, utchar * addr, int32 size, int32 precision, int32 type)
{
	tchar tmp[24] = { '0' };
	int32 i, n, len;

	len = 0;
	for (i = 0; i < 4; i++)
	{
		if (i != 0)
		{
			tmp[len++] = __T('.');
		}
	
		n = addr[i];
		if (n == 0)
		{
			tmp[len++] = digits[0];
		}
		else
		{
			if (n >= 100)
			{
				tmp[len++] = digits[n / 100];
				n = n % 100;
				tmp[len++] = digits[n / 10];
				n = n % 10;
			}
			else if (n >= 10)
			{
				tmp[len++] = digits[n / 10];
				n = n % 10;
			}

			tmp[len++] = digits[n];
		}
	}

	if (!(type & LEFT))
	{
		while (len < size--)
			*str++ = __T(' ');
	}
		
	for (i = 0; i < len; ++i)
	{
		*str++ = tmp[i];
	}
		
	while (len < size--)
	{
		*str++ = __T(' ');
	}

	return str;
}

int32 tvsprintf(tchar* buf, const tchar* fmt, va_list args)
{
	int32 len;
	int32 num;
	int32 i, base;
	tchar* str;
	tchar* s;

	int32 flags;        // Flags to number()

	int32 field_width;	// Width of output field
	int32 precision;	// Min. # of digits for integers; max number of chars for from string
	int32 qualifier;	// 'h', 'l', or 'L' for integer fields

	for (str = buf; *fmt; fmt++)
	{
		if (*fmt != __T('%'))
		{
			*str++ = *fmt;
			continue;
		}

		// Process flags
		flags = 0;
	repeat:
		fmt++; // This also skips first '%'
		switch (*fmt)
		{
		case __T('-'): flags |= LEFT; goto repeat;
		case __T('+'): flags |= PLUS; goto repeat;
		case __T(' '): flags |= SPACE; goto repeat;
		case __T('#'): flags |= SPECIAL; goto repeat;
		case __T('0'): flags |= ZEROPAD; goto repeat;
		}

		// Get field width
		field_width = -1;
		if (ISDIGIT(*fmt))
		{
			field_width = p_skip_atoi(&fmt);
		}
		else if (*fmt == __T('*'))
		{
			fmt++;
			field_width = va_arg(args, int32);
			if (field_width < 0)
			{
				field_width = -field_width;
				flags |= LEFT;
			}
		}

		// Get the precision
		precision = -1;
		if (*fmt == __T('.'))
		{
			++fmt;
			if (ISDIGIT(*fmt))
			{
				precision = p_skip_atoi(&fmt);
			}
			else if (*fmt == __T('*'))
			{
				++fmt;
				precision = va_arg(args, int32);
			}
			if (precision < 0)
			{
				precision = 0;
			}
		}

		// Get the conversion qualifier
		qualifier = -1;
		if (*fmt == __T('h') || *fmt == __T('l') || *fmt == __T('L'))
		{
			qualifier = *fmt;
			fmt++;
		}

		// Default base
		base = 10;

		switch (*fmt)
		{
		case __T('c'):
			if (!(flags & LEFT))
			{
				while (--field_width > 0) 
					*str++ = __T(' ');
			}
				
			*str++ = (utchar)va_arg(args, int32);
			while (--field_width > 0)
			{
				*str++ = __T(' ');
			}
			continue;

		case __T('s'):
			s = va_arg(args, tchar*);
			if (!s)
			{
				s = __T("<NULL>");
			}
				
			if (precision > 0)
			{
				len = qstrnlen(s, precision);
			}
			else
			{
				len = qstrlen(s);
			}

			if (!(flags & LEFT))
			{
				while (len < field_width--)
					*str++ = __T(' ');
			}
	
			for (i = 0; i < len; ++i)
			{
				*str++ = *s++;
			}
				
			while (len < field_width--)
			{
				*str++ = __T(' ');
			}
			
			continue;

		case __T('p'):
			if (field_width == -1)
			{
				field_width = 2 * sizeof(void*);
				flags |= ZEROPAD;
			}
			str = p_number(str, (int32)va_arg(args, void*), 16, field_width, precision, flags);
			continue;

		case __T('n'):
			if (qualifier == __T('l'))
			{
				int32* ip = va_arg(args, int32*);
				*ip = (str - buf);
			}
			else
			{
				int32* ip = va_arg(args, int32*);
				*ip = (str - buf);
			}
			continue;

		case __T('A'):
			flags |= LARGE;

		case __T('a'):
			if (qualifier == __T('l'))
				str = p_eaddr(str, va_arg(args, utchar*), field_width, precision, flags);
			else
				str = p_iaddr(str, va_arg(args, utchar*), field_width, precision, flags);
			continue;

			// Integer number formats - set up the flags and "break"
		case __T('o'):
			base = 8;
			break;

		case __T('X'):
			flags |= LARGE;

		case __T('x'):
			base = 16;
			break;

		case __T('d'):
		case __T('i'):
			flags |= SIGN;

		case __T('u'):
			break;

		case __T('E'):
		case __T('G'):
		case __T('e'):
		case __T('f'):
		case __T('g'):
			str = p_flt(str, va_arg(args, double), field_width, precision, *fmt, flags | SIGN);
			continue;

		default:
			if (*fmt != __T('%')) 
				*str++ = __T('%');
			if (*fmt)
				*str++ = *fmt;
			else
				--fmt;
			continue;
		}

		if (qualifier == __T('l'))
		{
			num = va_arg(args, uint32);
		}
		else if (qualifier == __T('h'))
		{
			if (flags & SIGN)
				num = va_arg(args, int16);
			else
				num = va_arg(args, uint16);
		}
		else if (flags & SIGN)
		{
			num = va_arg(args, int32);
		}
		else
		{
			num = va_arg(args, uint32);
		}

		str = p_number(str, num, base, field_width, precision, flags);
	}

	*str = NUL;

	return str - buf;
}

int32 tsprintf(tchar* buf, const tchar* fmt, ...) 
{
	va_list args;
	int32 n;

	va_start(args, fmt);
	n = tvsprintf(buf, fmt, args);
	va_end(args);

	return n;
}

int32 format(tchar* dst, const tchar* fmt, ...)
{
	va_list args;
	int32 n;

	va_start(args, fmt);
	n = tvsprintf(dst, fmt, args);
	va_end(args);

	return n;
}
