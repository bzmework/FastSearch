#ifndef __WIN32FILESEARCH_H__
#define __WIN32FILESEARCH_H__

#include <windows.h>
#include "../typedef.h"
#include "../utility/utility.h"
#include "../array/array.h"
#include "../string/string.h"
#include "../win32thread/thread.h"

class Win32FileSearch
{
public:
	
	//������Ϣ
	typedef struct FileSearchInfo
	{
		LPCTSTR lpszFindingFile;//�����е��ļ�
		LPCTSTR lpszFindedFile;//�ҵ����ļ�
		DWORD dwFindingCount;//�����е��ļ��ۼ�
		DWORD dwFindedCount;//�ҵ����ļ��ۼ�
		DWORD dwFindedSize;//�ҵ����ļ���С�ۼ�
		BOOL bIsFinded;//�ҵ���־
		BOOL bIsFinish;//�������
	} FileSearchInfo;

	//�����¼�
	typedef void (CALLBACK *FILE_SEARCH_EVENT_HANDLER)(FileSearchInfo searchInfo);

public:
	Win32FileSearch();
	virtual ~Win32FileSearch();

	VOID SetSearchPath(LPCTSTR lpszSearchPath);
	VOID SetSearchFilter(LPCTSTR lpszSearchFilter);
	VOID SetSearchEvent(FILE_SEARCH_EVENT_HANDLER searchEvent);

	VOID Start();
	VOID Stop();

private:
	TArray<TString> m_searchPath;
	TArray<TString> m_searchFilter;

	FileSearchInfo m_searchInfo;
	FILE_SEARCH_EVENT_HANDLER m_searchEvent;
	
	Win32Thread* m_searchTread;
	static VOID SearchThreadFunc(LPVOID lpvFileSearch);

	VOID Search(LPCTSTR lpszFilePath);
};

#endif //__WIN32FILESEARCH_H__