
/*
功能： 字符串匹配。
参数： text，字符串；
	  filter，通配符，支持*和?
返回： 匹配返回true，不匹配返回false。
*/

#include "../typedef.h"

tbool qstrmatch(const tchar* text, const tchar* filter)
{
	if (text == NULL || filter == NULL)
	{
		return FALSE;
	}

	const tchar* cp = NULL;
	const tchar* mp = NULL;
	while ((*text) && (*filter != __T('*')))
	{
		if ((*filter != *text) && (*filter != __T('?'))) return FALSE;
		filter++;
		text++;
	}

	while (*text)
	{
		if (*filter == __T('*'))
		{
			if (!*++filter) return TRUE;
			mp = filter;
			cp = text + 1;
		}
		else if ((*filter == *text) || (*filter == __T('?')))
		{
			filter++;
			text++;
		}
		else
		{
			filter = mp;
			text = cp++;
		}
	}

	while (*filter == __T('*'))
	{
		filter++;
	}

	return !*filter;
}

tbool qastrmatch(const char* text, const char* filter)
{
	if (text == NULL || filter == NULL)
	{
		return FALSE;
	}

	const char* cp = NULL;
	const char* mp = NULL;
	while ((*text) && (*filter != '*'))
	{
		if ((*filter != *text) && (*filter != '?')) return FALSE;
		filter++;
		text++;
	}

	while (*text)
	{
		if (*filter == '*')
		{
			if (!*++filter) return TRUE;
			mp = filter;
			cp = text + 1;
		}
		else if ((*filter == *text) || (*filter == '?'))
		{
			filter++;
			text++;
		}
		else
		{
			filter = mp;
			text = cp++;
		}
	}

	while (*filter == '*')
	{
		filter++;
	}

	return !*filter;
}

tbool qwstrmatch(const wchar* text, const wchar* filter)
{
	if (text == NULL || filter == NULL)
	{
		return FALSE;
	}

	const wchar* cp = NULL;
	const wchar* mp = NULL;
	while ((*text) && (*filter != __W('*')))
	{
		if ((*filter != *text) && (*filter != __W('?'))) return FALSE;
		filter++;
		text++;
	}

	while (*text)
	{
		if (*filter == __W('*'))
		{
			if (!*++filter) return TRUE;
			mp = filter;
			cp = text + 1;
		}
		else if ((*filter == *text) || (*filter == __W('?')))
		{
			filter++;
			text++;
		}
		else
		{
			filter = mp;
			text = cp++;
		}
	}

	while (*filter == __W('*'))
	{
		filter++;
	}

	return !*filter;
}