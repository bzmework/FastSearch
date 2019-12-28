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
	
	//搜索信息
	typedef struct FileSearchInfo
	{
		LPCTSTR lpszFindingFile;//搜索中的文件
		LPCTSTR lpszFindedFile;//找到的文件
		DWORD dwFindingCount;//搜索中的文件累计
		DWORD dwFindedCount;//找到的文件累计
		DWORD dwFindedSize;//找到的文件大小累计
		BOOL bIsFinded;//找到标志
		BOOL bIsFinish;//查找完成
	} FileSearchInfo;

	//搜索事件
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