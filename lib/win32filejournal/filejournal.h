#ifndef __FILEJOURNAL_H__
#define __FILEJOURNAL_H__

#include <windows.h>
#include "../typedef.h"
#include "../utility/utility.h"
#include "../string/string.h"
#include "../win32thread/thread.h"
#include "usn.h"
#include "iocp.h"
#include "iocpfilereq.h"

#define RECORD_BUF_SIZE 1024*64 //0x1000
#define RECORD_IOCP_BUF_SIZE 1024*4096

class Win32FileJournal
{
public:

	// 加载进度事件
	typedef void (CALLBACK* FILE_LOAD_COMPLETE_EVENT_HANDLER)(LPVOID lpvOwner, TString* deviceName, DWORD fileCount, DOUBLE elapsedTime);

	// 查询进度事件
	typedef void (CALLBACK* FILE_QUERY_COMPLETE_EVENT_HANDLER)(LPVOID lpvOwner, UsnList* fileList, TString* deviceName, DWORD fileCount, DOUBLE elapsedTime);

public:
	Win32FileJournal();
	virtual ~Win32FileJournal();

	// 刷新，加载所有数据
	VOID Refresh(TString path, FILE_LOAD_COMPLETE_EVENT_HANDLER completeEvent = NULL, LPVOID lpvOwner = NULL);

	// 查询文件
	// Filter, 搜索过滤，支持?和*通配符, 多个之间用竖线(|)分隔，例如："*.txt|*.tmp"
	VOID Query(TString filter, FILE_QUERY_COMPLETE_EVENT_HANDLER completeEvent = NULL, LPVOID lpvOwner = NULL);

	// 取消刷新或查询
	VOID Cancel();

private:

	//CPU数量
	DWORD m_cpuCount; 

	// 所有者Win32FileJournals对象
	LPVOID m_lpvOwner;

	// 加载数据多线程
	Win32Thread* m_refreshThread;
	static VOID RefreshThreadFunc(LPVOID lpvFileJournal);

	// 查找数据多线程
	Win32Thread* m_queryThread;
	static VOID QueryThreadFunc(LPVOID lpvFileJournal);

	// 加载事件
	FILE_LOAD_COMPLETE_EVENT_HANDLER m_loadCompleteEvent;

	// 查询事件
	FILE_QUERY_COMPLETE_EVENT_HANDLER m_queryCompleteEvent;

	//文件字典
	UsnDictionary* m_files;

	//列表字典
	UsnList* m_list;

	//设备名称，例如：E
	TString m_name;

	//设备索引
	INT m_index;

	//设备路径，例如：\\.\E:
	TString m_path;

	//搜索过滤，支持?和*通配符, 多个之间用竖线(|)分隔，例如："*.txt|*.tmp"
	TString m_filter;
	
private:

	//清除文件/列表
	VOID pClear(BOOL bFile = TRUE);

	//刷新文件
	VOID pRefresh();

	//异步刷新文件
	VOID pRefreshIocp();

	//查询文件
	VOID pQuery();

	// GetFullPath(HANDLE hVolume, DWORDLONG FileReferenceNumber);
	//VOID GetFullPath2(HANDLE hVolume, DWORDLONG FileReferenceNumber);

	//返回毫秒数
	static DOUBLE GetExactTickCount();

};

#endif //__FILEJOURNAL_H__