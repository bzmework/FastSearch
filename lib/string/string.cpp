
#include "string.h"

//Ĭ�Ϲ��캯��
TString::TString()
{
    this->buffer = NULL;
    this->length = 0;
}

//text ��ʼ���ı�
//length �ı����ȣ���ѡ
TString::TString(const tchar* text, int32 length /*= -1*/)
{
    if (text == NULL)
    {
        this->buffer = NULL;
        this->length = 0;
    }
    else
    {
        if (length == -1) length = qstrlen(text);
        this->buffer = new tchar[length + 1];
		//ASSERT(this->buffer)
        qstrncpy(this->buffer, text, length);
        this->buffer[length] = NUL;
        this->length = length;
    }
}

TString::TString(const TString& text)
{
    this->buffer = NULL;
    this->length = 0;
    *this = text;
}

 
TString::~TString()
{
    if (this->buffer != NULL) delete [] this->buffer;
}

tchar TString::operator [](int32 index) const
{
    //ASSERT(!(index < 0 || this->length <= index))
    return this->buffer[index];
}

tbool TString::operator ==(const tchar* text) const
{
    if (this->buffer == NULL || text == NULL) return this->buffer == text;
    return qstrcmp(this->buffer, text) == 0;
}

tbool TString::operator ==(const TString& text) const
{
    if (this->buffer == NULL || text.buffer == NULL) return this->buffer == text.buffer;
    if (this->length != text.length) return false;
    return qstrcmp(this->buffer, text.buffer) == 0;
}

TString& TString::operator =(const tchar* text)
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
        this->length = qstrlen(text);
        this->buffer = new tchar[this->length + 1];
        //ASSERT(this->buffer)
        qstrncpy(this->buffer, text, this->length);
        this->buffer[this->length] = NUL;
    }
    return *this;
}

TString& TString::operator =(const TString& text)
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
        this->buffer = new tchar[this->length + 1];
        //ASSERT(this->buffer)
        qstrncpy(this->buffer, text.buffer, this->length);
        this->buffer[this->length] = NUL;
    }
    return *this;
}

void TString::operator +=(const tchar* text)
{
    int32 len1 = this->length;
	int32 len2 = text != NULL ? qstrlen(text) : 0;
    this->length = len1 + len2;
    tchar* buf;
    if (this->length == 0)
    {
        buf = NULL;
    }
    else
    {
        buf = new tchar[this->length + 1];
        //ASSERT(buf)
        if (this->buffer != NULL) qstrncpy(buf, this->buffer, len1);
        if (text != NULL) qstrncpy(&buf[len1], text, len2);
        buf[this->length] = NUL;
    }
	if (this->buffer != NULL)
	{
		delete [] this->buffer;
	}
    this->buffer = buf;
}

void TString::operator +=(const TString& text)
{
    int32 len1 = this->length;
	int32 len2 = text.length;
    this->length = len1 + len2;
    tchar* buf;
    if (this->length == 0)
    {
        buf = NULL;
    }
    else
    {
        buf = new tchar[this->length + 1];
        //ASSERT(buf)
        if (this->buffer != NULL) qstrncpy(buf, this->buffer, len1);
        if (text.buffer != NULL) qstrncpy(&buf[len1], text.buffer, len2);
        buf[this->length] = NUL;
    }
    if (this->buffer != NULL) delete [] this->buffer;
    this->buffer = buf;
}

void TString::operator +=(tchar ch)
{
    tchar* buf = new tchar[this->length + 2];
    //ASSERT(buf)
    qstrncpy(buf, this->buffer, this->length);
    buf[this->length++] = ch;
    buf[this->length] = NUL;
    if (this->buffer != NULL) delete [] this->buffer;
    this->buffer = buf;
}

TString TString::operator +(const tchar* text) const
{
    TString ret = *this;
    ret += text;
    return ret;
}

TString TString::operator +(const TString& text) const
{
    TString ret = *this;
    ret += text;
    return ret;
}

//����ַ����Ƿ���ָ����ǰ׺��ʼ
tbool TString::startsWith(const TString& value) const
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
tbool TString::endsWith(const TString& value) const
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
int32 TString::indexOf(tchar ch, int32 from /*= 0*/) const
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
int32 TString::indexOf(const TString& value, int32 from /*= 0*/) const
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
int32 TString::lastIndexOf(tchar ch, int32 from /*= -1*/) const
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
int32 TString::lastIndexOf(const TString& value, int32 from /*= -1*/) const
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
TString TString::substr(int32 start, int32 length) const
{
    if (start < 0 || this->length <= start || length < 1) return NULL;
    int32 len = this->length - start;
    if (length > len) length = len;

    return TString(&this->buffer[start], length);
}

//�����ַ������ָ������(�ַ�����)���ַ���
TString TString::left(int32 length)
{
	if (length >= this->length) return this->buffer;
	if (length <= 0) return NULL;

	return TString(&this->buffer[0], length);
}

//�����ַ����ұ�ָ������(�ַ�����)���ַ���
TString TString::right(int32 length)
{
	if (length >= this->length) return this->buffer;
	if (length <= 0) return NULL;
	return TString(&this->buffer[this->length - length], length);
}

//������ǰ���ո���ַ���
TString TString::ltrim()
{
	int32 n = 0;
	tchar* p = this->buffer;
	tchar ch = *p;

	while (ch == 0x20) //0x20=32������ո�
	{ 
		ch = *(++p);
		n++;
	}

	if (n > 0 && n < this->length)
	{
		return TString(&this->buffer[n], length - n);
	}

	return NULL;
}

//�����޺�׺�ո���ַ���
TString TString::rtrim()
{
	int32 n = 0;
	tchar* p = this->buffer + this->length - 1;
	tchar ch = *p;

	while (ch == 0x20) //0x20=32������ո�
	{
		ch = *(--p);
		n++;
	}

	if (n > 0 && n < this->length) //Ҫ���ȫ��Ϊ�ո�����
	{
		return TString(&this->buffer[length - n], n);
	}

	return NULL;
}

//������ǰ���ո�ͺ�׺�ո���ַ���
TString TString::trim()
{
	int32 n1 = 0;
	int32 n2 = 0;
	tchar* p1 = this->buffer;
	tchar* p2 = this->buffer + this->length - 1;
	tchar ch;

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
		return TString(&this->buffer[n1], length - n1 - n2);
	}

	return NULL;
}

//���ַ��ָ��ַ������൱��VB6��Split������
TArray<TString> TString::split(tchar ch) const
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

    TArray<TString> ret(len);
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
TArray<TString> TString::split(const TString& value) const
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

	TArray<TString> ret(len);
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
TString TString::toLower() const
{
    TString ret = *this;
    for (int32 i = 0; i < ret.length; i++)
    {
        tchar ch = ret.buffer[i];
        if (__T('A') <= ch && ch <= __T('Z')) ret.buffer[i] = ch + (__T('a') - __T('A'));
    }
    return ret;
}

//����ת���ɴ�д���ַ������൱��VB6��Ucase������
TString TString::toUpper() const
{
    TString ret = *this;
    for (int32 i = 0; i < ret.length; i++)
    {
        tchar ch = ret.buffer[i];
        if (__T('a') <= ch && ch <= __T('z')) ret.buffer[i] = ch - (__T('a') - __T('A'));
    }
    return ret;
}

//���ص�ǰ�ַ���ָ��(�����޸�)
tchar* TString::toString() const
{
	return this->buffer;
}

//��ת�ַ���
void TString::reserve()
{
	qstrrev(this->buffer);
}

/*!
  \breif reset string in TString

  \return >=0 : length of string resetted

*/
int32 TString::reset()
{
   int32 result = this->length;
   if (this->buffer != NULL)
   {
       delete [] this->buffer;
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
int32 TString::insert(int32 start, const TString& text)
{
   int32 result;

   if ( (start < 0) || (start > this->length) ){
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
       tchar *buf1 = new tchar[start + 1];
       //ASSERT(buf1)
       qstrncpy(buf1, this->buffer, start);
       buf1[start] = NUL;
       tchar *buf2 = new tchar[this->length - start + 1];
       //ASSERT(buf2)
       qstrncpy(buf2, &(this->buffer[start]), this->length - start);
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
int32 TString::remove(int32 start, int32 length)
{
    int32 result = 0;
    tchar *buf;

    if ( (start < 0) || (start > this->length) )
    {
        result = -1; /* NEED? : modify message macro */
    }
    else if (length <= 0)
    {
        result = -2; /* NEED? : modify message macro */
    }
    else if ( (start == 0) && (length >= this->length) )
    {
        result = this->reset();
    }
    else if ( (start + length) >= this->length)
    {
        buf = new tchar[start + 1];
        //ASSERT(buf)
        qstrncpy(buf, this->buffer, start);
        delete [] this->buffer;
        this->buffer = buf;
        this->buffer[start] = NUL;
        result = this->length - start;
        this->length = start;
    }
    else
    {
        buf = new tchar[this->length - length + 1];
        //ASSERT(buf)
        qstrncpy(buf, this->buffer, start);
        qstrncpy(&buf[start], &(this->buffer[start + length]), this->length - start - length);
        delete [] this->buffer;
        this->buffer = buf;
        this->buffer[this->length - length] = NUL;
        this->length = this->length - length;
        result = length;
    }
    return result;
}

TString operator +(const tchar* text1, const TString& text2)
{
    TString ret = text1;
    ret += text2;
    return ret;
}

tbool TString::match(const tchar* filter)
{
	return qstrmatch(this->buffer, filter);
}

tchar* TString::format(const tchar* fmt, ...)
{
	va_list args;
	int32 n;

	tchar* buf = new tchar[this->length + 1];
    qstrncpy(buf, this->buffer, this->length);
	buf[this->length] = NUL;

	va_start(args, fmt);
	n = tvsprintf(buf, fmt, args);
	va_end(args);

	return buf;
}