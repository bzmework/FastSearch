#include "filesearch.h"

Win32FileSearch::Win32FileSearch()
{
	m_searchInfo.lpszFindingFile = __T("");
	m_searchInfo.lpszFindedFile = __T("");
	m_searchInfo.dwFindingCount = 0;
	m_searchInfo.dwFindedCount = 0;
	m_searchInfo.dwFindedSize = 0;
	m_searchInfo.bIsFinded = FALSE;
	m_searchInfo.bIsFinish = FALSE;
	m_searchEvent = NULL;
	m_searchTread = new Win32Thread();
}

Win32FileSearch::~Win32FileSearch()
{
	delete m_searchTread;
	m_searchTread = NULL;
}

//
//��������·�������֮��������(|)�ָ������磺"C:\|D:\|E:\"
//
void Win32FileSearch::SetSearchPath(LPCTSTR lpszSearchPath)
{
	if (lpszSearchPath != NULL)
	{
		TString s = lpszSearchPath;
		m_searchPath.Clear(); //�������������Ŀ
		m_searchPath = s.split(__T('|'));
	}
}

//
//�����������ˣ����֮��������(|)�ָ������磺"*.txt|*.tmp"
//
void Win32FileSearch::SetSearchFilter(LPCTSTR lpszSearchFilter)
{
	if (lpszSearchFilter != NULL)
	{
		TString s = lpszSearchFilter;
		m_searchFilter.Clear(); //�������������Ŀ
		m_searchFilter = s.split(__T('|'));
	}
}

//
// ���������¼�
//
void Win32FileSearch::SetSearchEvent(FILE_SEARCH_EVENT_HANDLER searchEvent)
{
	m_searchEvent = searchEvent;
}

//
//��ʼ����
//
void Win32FileSearch::Start()
{
	if((m_searchPath.Count() > 0) && (m_searchFilter.Count() > 0) && (m_searchEvent != NULL))
	{
		m_searchInfo.lpszFindingFile = __T("");
		m_searchInfo.lpszFindedFile = __T("");
		m_searchInfo.dwFindingCount = 0;
		m_searchInfo.dwFindedCount = 0;
		m_searchInfo.dwFindedSize = 0;
		m_searchInfo.bIsFinded = FALSE;
		m_searchInfo.bIsFinish = FALSE;
		m_searchTread->Create(Win32FileSearch::SearchThreadFunc, this);
	}
}

//
//ֹͣ����
//
void Win32FileSearch::Stop()
{
	if((m_searchPath.Count() > 0) && (m_searchFilter.Count() > 0) && (m_searchEvent != NULL))
	{
		m_searchTread->Stop();
	}
}

//
//�����̺߳���
//
void Win32FileSearch::SearchThreadFunc(LPVOID lpvFileSearchObject)
{
	Win32FileSearch* search = (Win32FileSearch*)lpvFileSearchObject;
	
	//����
	for(int i = 0; i < search->m_searchPath.Count(); i++)
	{
		search->Search(search->m_searchPath[i].toString());
	}
	//���
	if(search->m_searchEvent != NULL)
	{
		search->m_searchInfo.bIsFinded = FALSE;
		search->m_searchInfo.bIsFinish = TRUE;
		search->m_searchEvent(search->m_searchInfo);
	}
}

//
// �ļ�����
//
void Win32FileSearch::Search(LPCTSTR lpszFilePath)
{
	HANDLE hFile;
	WIN32_FIND_DATA WFD;
	TCHAR szCurPath[MAX_PATH];
	TCHAR szNextPath[MAX_PATH];
	int i = 0;

	qstrcpy(szCurPath, lpszFilePath);
	qstrcpy(szNextPath, lpszFilePath);
	qstrcat(szNextPath, __T("*.*"));

	hFile = FindFirstFile(szNextPath, &WFD);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		do 
		{
			if(m_searchTread->IsStopping())
			{
				break;
			}
			if (WFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (qstrcmp(WFD.cFileName, __T(".")) && 
					qstrcmp(WFD.cFileName, __T("..")))
				{
					qstrcpy(szNextPath, szCurPath);
					qstrcat(szNextPath, WFD.cFileName);
					qstrcat(szNextPath, __T("\\"));
					Search(szNextPath);
				}
			}
			else
			{
				for(i = 0; i < m_searchFilter.Count(); i++)
				{
					if (qstrmatch(WFD.cFileName, m_searchFilter[i].toString()))
					{			 
						m_searchInfo.lpszFindingFile = WFD.cFileName;
						m_searchInfo.lpszFindedFile = WFD.cFileName;
						m_searchInfo.dwFindedSize += WFD.nFileSizeHigh;
						m_searchInfo.dwFindingCount += 1;
						m_searchInfo.dwFindedCount += 1;
						m_searchInfo.bIsFinded = TRUE;
						m_searchEvent(m_searchInfo); 
					}
					else
					{
						m_searchInfo.lpszFindingFile = WFD.cFileName;
						m_searchInfo.dwFindingCount += 1;
						m_searchInfo.bIsFinded = FALSE;
						m_searchEvent(m_searchInfo);
					}
				}
			}
		} while (FindNextFile(hFile, &WFD));
		FindClose(hFile);
	}
}
