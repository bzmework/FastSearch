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

	// ���ؽ����¼�
	typedef void (CALLBACK* FILE_LOAD_COMPLETE_EVENT_HANDLER)(LPVOID lpvOwner, TString* deviceName, DWORD fileCount, DOUBLE elapsedTime);

	// ��ѯ�����¼�
	typedef void (CALLBACK* FILE_QUERY_COMPLETE_EVENT_HANDLER)(LPVOID lpvOwner, UsnList* fileList, TString* deviceName, DWORD fileCount, DOUBLE elapsedTime);

public:
	Win32FileJournal();
	virtual ~Win32FileJournal();

	// ˢ�£�������������
	VOID Refresh(TString path, FILE_LOAD_COMPLETE_EVENT_HANDLER completeEvent = NULL, LPVOID lpvOwner = NULL);

	// ��ѯ�ļ�
	// Filter, �������ˣ�֧��?��*ͨ���, ���֮��������(|)�ָ������磺"*.txt|*.tmp"
	VOID Query(TString filter, FILE_QUERY_COMPLETE_EVENT_HANDLER completeEvent = NULL, LPVOID lpvOwner = NULL);

	// ȡ��ˢ�»��ѯ
	VOID Cancel();

private:

	//CPU����
	DWORD m_cpuCount; 

	// ������Win32FileJournals����
	LPVOID m_lpvOwner;

	// �������ݶ��߳�
	Win32Thread* m_refreshThread;
	static VOID RefreshThreadFunc(LPVOID lpvFileJournal);

	// �������ݶ��߳�
	Win32Thread* m_queryThread;
	static VOID QueryThreadFunc(LPVOID lpvFileJournal);

	// �����¼�
	FILE_LOAD_COMPLETE_EVENT_HANDLER m_loadCompleteEvent;

	// ��ѯ�¼�
	FILE_QUERY_COMPLETE_EVENT_HANDLER m_queryCompleteEvent;

	//�ļ��ֵ�
	UsnDictionary* m_files;

	//�б��ֵ�
	UsnList* m_list;

	//�豸���ƣ����磺E
	TString m_name;

	//�豸����
	INT m_index;

	//�豸·�������磺\\.\E:
	TString m_path;

	//�������ˣ�֧��?��*ͨ���, ���֮��������(|)�ָ������磺"*.txt|*.tmp"
	TString m_filter;
	
private:

	//����ļ�/�б�
	VOID pClear(BOOL bFile = TRUE);

	//ˢ���ļ�
	VOID pRefresh();

	//�첽ˢ���ļ�
	VOID pRefreshIocp();

	//��ѯ�ļ�
	VOID pQuery();

	// GetFullPath(HANDLE hVolume, DWORDLONG FileReferenceNumber);
	//VOID GetFullPath2(HANDLE hVolume, DWORDLONG FileReferenceNumber);

	//���غ�����
	static DOUBLE GetExactTickCount();

};

#endif //__FILEJOURNAL_H__