
#include "wstring.h"

//Ĭ�Ϲ��캯��
WString::WString()
{
	this->buffer = NULL;
	this->length = 0;
}

//text ��ʼ���ı�
//length �ı����ȣ���ѡ
WString::WString(const wchar* text, int32 length /*= -1*/)
{
	if (text == NULL)
	{
		this->buffer = NULL;
		this->length = 0;
	}
	else
	{
		if (length == -1) length = qwstrlen(text);
		this->buffer = new wchar[length + 1];
		//ASSERT(this->buffer)
		qwstrncpy(this->buffer, text, length);
		this->buffer[length] = NUL;
		this->length = length;
	}
}

WString::WString(const WString& text)
{
	this->buffer = NULL;
	this->length = 0;
	*this = text;
}


WString::~WString()
{
	if (this->buffer != NULL) delete[] this->buffer;
}

wchar WString::operator [](int32 index) const
{
	//ASSERT(!(index < 0 || this->length <= index))
	return this->buffer[index];
}

tbool WString::operator ==(const wchar* text) const
{
	if (this->buffer == NULL || text == NULL) return this->buffer == text;
	return qwstrcmp(this->buffer, text) == 0;
}

tbool WString::operator ==(const WString& text) const
{
	if (this->buffer == NULL || text.buffer == NULL) return this->buffer == text.buffer;
	if (this->length != text.length) return false;
	return qwstrcmp(this->buffer, text.buffer) == 0;
}

WString& WString::operator =(const wchar* text)
{
	if (this->buffer != NULL)
	{
		delete[] this->buffer;
	}
	if (text == NULL)
	{
		this->buffer = NULL;
		this->length = 0;
	}
	else
	{
		this->length = qwstrlen(text);
		this->buffer = new wchar[this->length + 1];
		//ASSERT(this->buffer)
		qwstrncpy(this->buffer, text, this->length);
		this->buffer[this->length] = NUL;
	}
	return *this;
}

WString& WString::operator =(const WString& text)
{
	if (this->buffer != NULL)
	{
		delete[] this->buffer;
	}
	if (text.buffer == NULL)
	{
		this->buffer = NULL;
		this->length = 0;
	}
	else
	{
		this->length = text.length;
		this->buffer = new wchar[this->length + 1];
		//ASSERT(this->buffer)
		qwstrncpy(this->buffer, text.buffer, this->length);
		this->buffer[this->length] = NUL;
	}
	return *this;
}

void WString::operator +=(const wchar* text)
{
	int32 len1 = this->length;
	int32 len2 = text != NULL ? qwstrlen(text) : 0;
	this->length = len1 + len2;
	wchar* buf;
	if (this->length == 0)
	{
		buf = NULL;
	}
	else
	{
		buf = new wchar[this->length + 1];
		//ASSERT(buf)
		if (this->buffer != NULL) qwstrncpy(buf, this->buffer, len1);
		if (text != NULL) qwstrncpy(&buf[len1], text, len2);
		buf[this->length] = NUL;
	}
	if (this->buffer != NULL) delete[] this->buffer;
	this->buffer = buf;
}

void WString::operator +=(const WString& text)
{
	int32 len1 = this->length;
	int32 len2 = text.length;
	this->length = len1 + len2;
	wchar* buf;
	if (this->length == 0)
	{
		buf = NULL;
	}
	else
	{
		buf = new wchar[this->length + 1];
		//ASSERT(buf)
		if (this->buffer != NULL) qwstrncpy(buf, this->buffer, len1);
		if (text.buffer != NULL) qwstrncpy(&buf[len1], text.buffer, len2);
		buf[this->length] = NUL;
	}
	if (this->buffer != NULL) delete[] this->buffer;
	this->buffer = buf;
}

void WString::operator +=(wchar ch)
{
	wchar* buf = new wchar[this->length + 2];
	//ASSERT(buf)
	qwstrncpy(buf, this->buffer, this->length);
	buf[this->length++] = ch;
	buf[this->length] = NUL;
	if (this->buffer != NULL) delete[] this->buffer;
	this->buffer = buf;
}

WString WString::operator +(const wchar* text) const
{
	WString ret = *this;
	ret += text;
	return ret;
}

WString WString::operator +(const WString& text) const
{
	WString ret = *this;
	ret += text;
	return ret;
}

//����ַ����Ƿ���ָ����ǰ׺��ʼ
tbool WString::startsWith(const WString& value) const
{
	int32 len = value.length;
	if (len > this->length) return FALSE;
	for (int32 i = 0; i < len; i++)
	{
		if (this->buffer[i] != value.buffer[i]) return FALSE;
	}
	return TRUE;
}

//����ַ����Ƿ���ָ���ĺ�׺����
tbool WString::endsWith(const WString& value) const
{
	int32 len = value.length;
	int32 pos = this->length - len;
	if (pos < 0) return FALSE;
	for (int32 i = 0; i < len; i++)
	{
		if (this->buffer[pos + i] != value.buffer[i]) return FALSE;
	}
	return TRUE;
}

//����ĳ���ַ���λ�á��൱��VB6��InStr������
int32 WString::indexOf(wchar ch, int32 from /*= 0*/) const
{
	if (this->buffer == NULL || this->length == 0) return -1;

	if (from < 0) from = 0;
	for (int32 i = from; i < this->length; i++)
	{
		if (this->buffer[i] == ch) return i;
	}
	return -1;
}

//����ĳ���ַ�����λ�á��൱��VB6��InStr������
int32 WString::indexOf(const WString& value, int32 from /*= 0*/) const
{
	if (this->buffer == NULL) return value.buffer == NULL;
	if (this->length == 0) return -1;

	if (from < 0) from = 0;
	int32 last = this->length - value.length;
	if (value.buffer == NULL || value.length == 0) return from < this->length ? from : -1;
	for (int32 i = from; i <= last; i++)
	{
		bool ok = true;
		for (int32 j = 0; j < value.length; j++)
		{
			if (this->buffer[i + j] != value.buffer[j])
			{
				ok = false;
				break;
			}
		}
		if (ok) return i;
	}
	return -1;
}

//�����λ�ÿ�ʼ����ĳ���ַ���λ�á��൱��VB6��InStrRev������
int32 WString::lastIndexOf(wchar ch, int32 from /*= -1*/) const
{
	if (this->buffer == NULL || this->length == 0) return -1;

	if (from == -1) from = this->length;
	if (from > this->length) from = this->length;
	for (int32 i = from; i > 0; i--)
	{
		if (this->buffer[i - 1] == ch) return i - 1;
	}
	return -1;
}

//�����λ�ÿ�ʼ����ĳ���ַ�����λ�á��൱��VB6��InStrRev������
int32 WString::lastIndexOf(const WString& value, int32 from /*= -1*/) const
{
	if (this->buffer == NULL) return value.buffer == NULL;
	if (this->length == 0) return -1;

	if (from == -1) from = this->length;
	if (from > this->length) from = this->length;
	if (value.buffer == NULL || value.length == 0) return from - 1;

	for (int32 i = from; i >= value.length; i--)
	{
		bool ok = true;
		for (int32 j = 0; j < value.length; j++)
		{
			if (this->buffer[i - j - 1] != value.buffer[value.length - j - 1])
			{
				ok = false;
				break;
			}
		}
		if (ok) return i - value.length;
	}
	return -1;
}

//��ָ��λ�ÿ�ʼ����ָ������(�ַ�����)���ַ������൱��VB6��Mid������
WString WString::substr(int32 start, int32 length) const
{
	if (start < 0 || this->length <= start || length < 1) return NULL;
	int32 len = this->length - start;
	if (length > len) length = len;

	return WString(&this->buffer[start], length);
}

//�����ַ������ָ������(�ַ�����)���ַ���
WString WString::left(int32 length)
{
	if (length >= this->length) return this->buffer;
	if (length <= 0) return NULL;

	return WString(&this->buffer[0], length);
}

//�����ַ����ұ�ָ������(�ַ�����)���ַ���
WString WString::right(int32 length)
{
	if (length >= this->length) return this->buffer;
	if (length <= 0) return NULL;
	return WString(&this->buffer[this->length - length], length);
}

//������ǰ���ո���ַ���
WString WString::ltrim()
{
	int32 n = 0;
	wchar* p = this->buffer;
	wchar ch = *p;

	while (ch == 0x20) //0x20=32������ո�
	{
		ch = *(++p);
		n++;
	}

	if (n > 0 && n < this->length)
	{
		return WString(&this->buffer[n], length - n);
	}

	return NULL;
}

//�����޺�׺�ո���ַ���
WString WString::rtrim()
{
	int32 n = 0;
	wchar* p = this->buffer + this->length - 1;
	wchar ch = *p;

	while (ch == 0x20) //0x20=32������ո�
	{
		ch = *(--p);
		n++;
	}

	if (n > 0 && n < this->length) //Ҫ���ȫ��Ϊ�ո�����
	{
		return WString(&this->buffer[length - n], n);
	}

	return NULL;
}

//������ǰ���ո�ͺ�׺�ո���ַ���
WString WString::trim()
{
	int32 n1 = 0;
	int32 n2 = 0;
	wchar* p1 = this->buffer;
	wchar* p2 = this->buffer + this->length - 1;
	wchar ch;

	//����ǰ���ո�
	ch = *p1;
	while (ch == 0x20) //0x20=32������ո�
	{
		ch = *(++p1);
		n1++;
	}

	//�Ƿ�ȫ��Ϊ�ո�
	if (n1 == this->length)
	{
		return NULL;
	}

	//���Һ�׺�ո�
	ch = *p2;
	while (ch == 0x20) //0x20=32������ո�
	{
		ch = *(--p2);
		n2++;
	}

	//��ǰ�����׺�ո�
	if ((n1 > 0 && n1 == this->length) || (n2 > 0 && n2 < this->length))
	{
		return WString(&this->buffer[n1], length - n1 - n2);
	}

	return NULL;
}

//���ַ��ָ��ַ������൱��VB6��Split������
TArray<WString> WString::split(wchar ch) const
{
	int32 len = 0, p = 0, pp = 0;
	for (;;)
	{
		pp = p;
		p = this->indexOf(ch, p);
		if (p < 0)
		{
			len++;
			break;
		}
		len++;
		p++;
	}

	TArray<WString> ret(len);
	p = 0;
	for (int32 i = 0;; i++)
	{
		pp = p;
		p = this->indexOf(ch, p);
		if (p < 0)
		{
			ret[i] = this->substr(pp, this->length - pp);
			break;
		}
		ret[i] = this->substr(pp, p - pp);
		p++;
	}
	return ret;
}

//���ַ����ָ��ַ������൱��VB6��Split������
TArray<WString> WString::split(const WString& value) const
{
	int32 len = 0, p = 0, pp = 0;
	for (;;)
	{
		pp = p;
		p = this->indexOf(value, p);
		if (p < 0)
		{
			len++;
			break;
		}
		len++;
		p += value.length;
	}

	TArray<WString> ret(len);
	p = 0;
	for (int32 i = 0;; i++)
	{
		pp = p;
		p = this->indexOf(value, p);
		if (p < 0)
		{
			ret[i] = this->substr(pp, this->length - pp);
			break;
		}
		ret[i] = this->substr(pp, p - pp);
		p += value.length;
	}
	return ret;
}

//����ת����Сд���ַ����൱��VB6��LCase������
WString WString::toLower() const
{
	WString ret = *this;
	for (int32 i = 0; i < ret.length; i++)
	{
		wchar ch = ret.buffer[i];
		if (__W('A') <= ch && ch <= __W('Z')) ret.buffer[i] = ch + (__W('a') - __W('A'));
	}
	return ret;
}

//����ת���ɴ�д���ַ������൱��VB6��Ucase������
WString WString::toUpper() const
{
	WString ret = *this;
	for (int32 i = 0; i < ret.length; i++)
	{
		wchar ch = ret.buffer[i];
		if (__W('a') <= ch && ch <= __W('z')) ret.buffer[i] = ch - (__W('a') - __W('A'));
	}
	return ret;
}

//���ص�ǰ�ַ���ָ��(�����޸�)
wchar* WString::toString() const
{
	return this->buffer;
}

//��ת�ַ���
void WString::reserve()
{
	qwstrrev(this->buffer);
}

/*!
  \breif reset string in TString

  \return >=0 : length of string resetted

*/
int32 WString::reset()
{
	int32 result = this->length;
	if (this->buffer != NULL)
	{
		delete[] this->buffer;
		this->buffer = NULL;
		this->length = 0;
	}
	return result;
}

/*!
  \breif insert string in TString

  \param start  start point of string inserted
  \param text   string inserted
  \return >=0 : length of string inserted
		   -1 : start is bad value

*/
int32 WString::insert(int32 start, const WString& text)
{
	int32 result;

	if ((start < 0) || (start > this->length)) {
		result = -1;
	}
	else if (this->length == 0)
	{
		*this = text;
		result = text.length;
	}
	else if (text.length == 0)
	{
		result = 0;
	}
	else
	{
		wchar* buf1 = new wchar[start + 1];
		//ASSERT(buf1)
		qwstrncpy(buf1, this->buffer, start);
		buf1[start] = NUL;
		wchar* buf2 = new wchar[this->length - start + 1];
		//ASSERT(buf2)
		qwstrncpy(buf2, &(this->buffer[start]), this->length - start);
		buf2[this->length - start] = NUL;
		*this = buf1 + text + buf2;
		result = text.length;
	}
	return result;
}

/*!
  \breif remove string in TString

  \param start  start point of string removed
  \param length length of string removed
  \return >=0 : length of string removed
		   -1 : start is bad value
		   -2 : length is bad value
*/
int32 WString::remove(int32 start, int32 length)
{
	int32 result = 0;
	wchar* buf;

	if ((start < 0) || (start > this->length))
	{
		result = -1; /* NEED? : modify message macro */
	}
	else if (length <= 0)
	{
		result = -2; /* NEED? : modify message macro */
	}
	else if ((start == 0) && (length >= this->length))
	{
		result = this->reset();
	}
	else if ((start + length) >= this->length)
	{
		buf = new wchar[start + 1];
		//ASSERT(buf)
		qwstrncpy(buf, this->buffer, start);
		delete[] this->buffer;
		this->buffer = buf;
		this->buffer[start] = NUL;
		result = this->length - start;
		this->length = start;
	}
	else
	{
		buf = new wchar[this->length - length + 1];
		//ASSERT(buf)
		qwstrncpy(buf, this->buffer, start);
		qwstrncpy(&buf[start], &(this->buffer[start + length]), this->length - start - length);
		delete[] this->buffer;
		this->buffer = buf;
		this->buffer[this->length - length] = NUL;
		this->length = this->length - length;
		result = length;
	}
	return result;
}

WString operator +(const wchar* text1, const WString& text2)
{
	WString ret = text1;
	ret += text2;
	return ret;
}

tbool WString::match(const wchar* filter)
{
	return qwstrmatch(this->buffer, filter);
}

//tchar* TWString::format(const tchar * fmt, ...)
//{
//	va_list args;
//	int32 n;
//
//	tchar* buf = new tchar[this->length + 1];
//	qstrncpy(buf, this->buffer, this->length);
//	buf[this->length] = NUL;
//
//	va_start(args, fmt);
//	n = tvsprintf(buf, fmt, args);
//	va_end(args);
//
//	return buf;
//}