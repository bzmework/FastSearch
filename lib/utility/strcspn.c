
/*
��Դ�� ChromeOS
���ܣ� ��ѯ�ַ���str���״γ����ַ���reject�е������ַ���λ�á�
������ str���ַ���;
	  reject��Ҫƥ����ַ�����
���أ� �����ַ���str��ͷ�����������ַ���reject�ڵ��ַ�������
˵���� strcspn����string complementary span����д��ɣ���ʾ��ȡ�ַ���str��ʼλ�������ַ���reject�е��ַ���������
ʾ���� char* str = "The Linux was first developed for 386/486-based pcs.";
	  len = strcspn(str1, " "); ����3�������״γ���" "(�ո�)ʱ�ĳ��ȣ����Է���3��
	  len = strcspn(str1, "/-"); ����37�������״γ���"/"��"-"ʱ�ĳ��ȣ����Է���37��
	  len = strcspn(str1, "1234567890"); ����34�������״γ���0-9֮���������ֵʱ�ĳ��ȣ����Է���34��
	  len = strcspn(str1, "jk"); ����52��û���ҵ��״γ��ֵ��ַ����򷵻������ַ����ĳ��ȣ����Է���52��
*/

#include "../typedef.h"

uint32 qstrxspn(const tchar* str, const tchar* map, int32 parity);
uint32 qastrxspn(const char* str, const char* map, int32 parity);
uint32 qwstrxspn(const wchar* str, const wchar* map, int32 parity);

uint32 qstrcspn(const tchar* str, const tchar* reject)
{
	return qstrxspn(str, reject, 1);
}

uint32 qastrcspn(const char* str, const char* reject)
{
	return qastrxspn(str, reject, 1);
}

uint32 qwstrcspn(const wchar* str, const wchar* reject)
{
	return qwstrxspn(str, reject, 1);
}