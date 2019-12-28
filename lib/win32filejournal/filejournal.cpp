#include "filejournal.h"

Win32FileJournal::Win32FileJournal()
{
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	m_cpuCount = sysinfo.dwNumberOfProcessors;
	m_lpvOwner = NULL;
	m_loadCompleteEvent = NULL;
	m_queryCompleteEvent = NULL;
	m_refreshThread = new Win32Thread();
	m_queryThread = new Win32Thread();
	m_files = new UsnDictionary();
	m_list = NULL;
	m_index = 0;
}

Win32FileJournal::~Win32FileJournal()
{
	delete m_refreshThread;
	delete m_queryThread;
	delete m_files;
}

// ˢ�£�������������
VOID Win32FileJournal::Refresh(TString path, FILE_LOAD_COMPLETE_EVENT_HANDLER completeEvent, LPVOID lpvOwner)
{
	//����·��
	if (path.getLength() > 0)
	{
		m_path = path;
		m_name = m_path.substr(4, 1); //�豸���ƣ����磺E
		m_index = (INT)m_path[4]; //�豸���������磺5
	}
	else
	{
		return;
	}

	//���ò���
	m_loadCompleteEvent = completeEvent;
	m_lpvOwner = lpvOwner;

	//�����̣߳���������
	m_refreshThread->Create(Win32FileJournal::RefreshThreadFunc, this);
}

// ��ѯ�ļ�
// filter, ɸѡ��֧��?��*ͨ���, ���֮��������(|)�ָ������磺"*.txt|*.tmp"
VOID Win32FileJournal::Query(TString filter, FILE_QUERY_COMPLETE_EVENT_HANDLER completeEvent, LPVOID lpvOwner)
{
	//����ɸѡ
	if (filter.getLength() > 0)
	{
		m_filter = filter;
	}
	else
	{
		m_filter = __T("*");
	}

	//���ò���
	m_queryCompleteEvent = completeEvent;
	m_lpvOwner = lpvOwner;

	//�����̣߳���ѯ����
	m_queryThread->Create(Win32FileJournal::QueryThreadFunc, this);
}

// ȡ��ˢ�»��ѯ
VOID Win32FileJournal::Cancel()
{
	if (m_refreshThread->IsRunning())
	{
		m_refreshThread->Stop();
	}
	if (m_queryThread->IsRunning())
	{
		m_queryThread->Stop();
	}
}

//ˢ���߳�
void Win32FileJournal::RefreshThreadFunc(LPVOID lpvFileJournal)
{
	DOUBLE elapsedTime = GetExactTickCount();
	Win32FileJournal* fileObj = (Win32FileJournal*)lpvFileJournal;

	//����ļ�
	fileObj->pClear(TRUE);

	//�����ļ�
	//fileObj->pRefresh();
	fileObj->pRefreshIocp();

	//���
	if (fileObj->m_loadCompleteEvent != NULL)
	{
		fileObj->m_loadCompleteEvent(fileObj->m_lpvOwner, &fileObj->m_name, fileObj->m_files->Count(), GetExactTickCount() - elapsedTime);
	}
}

//��ѯ�߳�
void Win32FileJournal::QueryThreadFunc(LPVOID lpvFileJournal)
{
	DOUBLE elapsedTime = GetExactTickCount();
	Win32FileJournal* fileObj = (Win32FileJournal*)lpvFileJournal;

	//����б�
	fileObj->pClear(FALSE);

	//��ѯ�ļ�
	fileObj->pQuery();

	//���
	if (fileObj->m_queryCompleteEvent != NULL)
	{
		fileObj->m_queryCompleteEvent(fileObj->m_lpvOwner, fileObj->m_list, &fileObj->m_name, fileObj->m_list->Count(), GetExactTickCount() - elapsedTime);
	}
}

//�������
VOID Win32FileJournal::pClear(BOOL bFile)
{
	if (bFile)
	{
		if (this->m_files != NULL)
		{
			this->m_files->RemoveAll();
		}
	}
	else
	{
		if (this->m_list != NULL)
		{
			this->m_list->RemoveAll();
		}
	}
}
 
// �����ļ�
// �ο���https://docs.microsoft.com/zh-cn/windows/desktop/FileIO/walking-a-buffer-of-change-journal-records
//		https://www.installsetupconfig.com/win32programming/windowsvolumeapis1_11.html
VOID Win32FileJournal::pRefresh()
{
	BOOL bOk = FALSE;

	/* 1.ȡ�þ��ľ�� */
	
	//��ȡ�����̾�� ����CreateFile������Ҫ����ԱȨ��
	HANDLE hDevice = CreateFile(this->m_path,//����ľ��������Ϊ\\.\C:����ʽ
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE, //���������FILE_SHARE_WRITE
		NULL,
		OPEN_EXISTING,  //�������OPEN_EXISTING, ��CREATE_ALWAYS���ܻᵼ�´���
		FILE_ATTRIBUTE_READONLY, //FILE_ATTRIBUTE_NORMAL���ܻᵼ�´���
		NULL);
	if (hDevice != INVALID_HANDLE_VALUE)
	{
		/* 2.�Ӿ���ȡ����־ */

		DWORD lpBytesReturned = 0;
		CREATE_USN_JOURNAL_DATA cujd;
		cujd.MaximumSize = 0; //ʹ��Ĭ��ֵ
		cujd.AllocationDelta = 0; //ʹ��Ĭ��ֵ
		bOk = DeviceIoControl(hDevice,
			FSCTL_CREATE_USN_JOURNAL, //������������־�Ѿ���������Թ���ȡ���޸ģ�û����־�ʹ���һ���µĹ���ȡ���޸ġ�
			&cujd,
			sizeof(CREATE_USN_JOURNAL_DATA),
			NULL,
			0,
			&lpBytesReturned,
			NULL);
		if (bOk)
		{
			/* 3.��ȡ��־����Ϣ */
			USN_JOURNAL_DATA ujd; 
			bOk = DeviceIoControl(hDevice,
				FSCTL_QUERY_USN_JOURNAL,
				NULL,
				0,
				&ujd,
				sizeof(USN_JOURNAL_DATA),
				&lpBytesReturned,
				NULL);
			if (bOk) 
			{
				/* 4.ö�����е����� */

				TCHAR buffer[RECORD_BUF_SIZE] = { 0 }; //���ڴ����¼�Ļ���,�����㹻�ش�
				TCHAR fileName[MAX_PATH] = { 0 };
				DWORD dwRetBytes = 0;
				PUSN_RECORD_V2 pUR;
				MFT_ENUM_DATA med = { 0 };

				med.StartFileReferenceNumber = 0;
				med.LowUsn = 0;//UJD.FirstUsn; ���Է��֣������FirstUsn��ʱ����ȷ�����»�ȡ��������������.
				med.HighUsn = ujd.NextUsn;
				med.MinMajorVersion = 0;
				med.MaxMajorVersion = 2; //����ָ���汾��
				while (bOk = DeviceIoControl(hDevice,
					FSCTL_ENUM_USN_DATA,
					&med,
					sizeof(MFT_ENUM_DATA),
					buffer,
					RECORD_BUF_SIZE,
					&lpBytesReturned,
					NULL))
				{
					//��ȡUSN��¼
					pUR = (PUSN_RECORD_V2)((PCHAR)buffer + sizeof(USN)); //��λ����һ���ļ�
					dwRetBytes = lpBytesReturned - sizeof(USN);
					while (dwRetBytes > sizeof(USN))
					{
						//��ȡ�ļ��������ֵ�
						ZeroMemory(fileName, MAX_PATH);
						WideCharToMultiByte(CP_OEMCP, NULL, pUR->FileName, pUR->FileNameLength / 2, fileName, pUR->FileNameLength, NULL, FALSE);//Unicode->ANSI
						m_files->Add(pUR->FileReferenceNumber, pUR->ParentFileReferenceNumber, fileName);
								
						//��λ����һ���ļ�
						dwRetBytes -= pUR->RecordLength;
						pUR = (PUSN_RECORD_V2)((PCHAR)pUR + pUR->RecordLength);
					}

					//��ȡ��һ��USN��¼
					med.StartFileReferenceNumber = *(USN*)&buffer;
				}
			}
		}
		CloseHandle(hDevice);
	}
}

// �첽�����ļ�(����ڶ��̣߳�ͨ��IO��ɶ˿ڶ�ȡ���ݴ���������������������)
VOID Win32FileJournal::pRefreshIocp()
{
	BOOL bOk = FALSE;

	/* 1.ȡ�þ��ľ�� */
	//��ȡ�����̾�� ����CreateFile������Ҫ����ԱȨ��
	HANDLE hDevice = CreateFile(this->m_path,//����ľ��������Ϊ\\.\C:����ʽ
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE, //���������FILE_SHARE_WRITE
		NULL,
		OPEN_EXISTING,  //�������OPEN_EXISTING, ��CREATE_ALWAYS���ܻᵼ�´���
		FILE_ATTRIBUTE_READONLY, //FILE_ATTRIBUTE_NORMAL���ܻᵼ�´���
		NULL);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		return;
	}

	/* 2.�Ӿ���ȡ����־ */
	DWORD dwRetBytes;
	DWORD dwNumBytes;
	CREATE_USN_JOURNAL_DATA cujd;
	cujd.MaximumSize = 0; //ʹ��Ĭ��ֵ
	cujd.AllocationDelta = 0; //ʹ��Ĭ��ֵ
	bOk = DeviceIoControl(hDevice,
		FSCTL_CREATE_USN_JOURNAL, //������������־�Ѿ���������Թ���ȡ���޸ģ�û����־�ʹ���һ���µĹ���ȡ���޸ġ�
		&cujd,
		sizeof(CREATE_USN_JOURNAL_DATA),
		NULL,
		0,
		&dwRetBytes,
		NULL);
	if (!bOk)
	{
		return;
	}

	/* 3.��ȡ��־����Ϣ */
	USN_JOURNAL_DATA ujd;
	bOk = DeviceIoControl(hDevice,
		FSCTL_QUERY_USN_JOURNAL,
		NULL,
		0,
		&ujd,
		sizeof(USN_JOURNAL_DATA),
		&dwRetBytes,
		NULL);
	if (!bOk)
	{
		return;
	}
		
	/* 4.��ʼ��IO��ɶ˿� */
	//���첽��ʽ���´��豸
	CloseHandle(hDevice);
	hDevice = CreateFile(m_path,
		GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED, /*����FILE_FLAG_NO_BUFFERING������ReadFile/WriteFile��lpBuffer��С������1024��������*/
		NULL);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		return;
	}
	//����IO��ɶ˿�
	IoCompletionPort* iocp = new IoCompletionPort();
	if (!iocp->Create())
	{
		return;
	}
	//���ļ���IO��ɶ˿ڹ���
	if (!iocp->AddDevice(hDevice, this->m_index))
	{
		return;
	}

	//��ʼ��IO��ɶ˿ڽ������ݽ���ʱ��������ݻ�����
	IoCompletionPortFileReq* ior = new IoCompletionPortFileReq();
	bOk = ior->Alloc(RECORD_IOCP_BUF_SIZE);//ÿ��I/O������Ҫһ�����ݻ��������������ݴ���
	if (!bOk)
	{
		return;
	}

	/* 5.ö�����е����� */
	LARGE_INTEGER offset = { 0 };
	ULONG_PTR uniqueKey;
	IoCompletionPortFileReq* pior;

	PUSN_RECORD_V2 pUR;
	MFT_ENUM_DATA med = { 0 };
	TCHAR fileName[MAX_PATH] = { 0 };

	med.StartFileReferenceNumber = 0;
	med.LowUsn = 0;//UJD.FirstUsn; ���Է��֣������FirstUsn��ʱ����ȷ�����»�ȡ��������������.
	med.HighUsn = ujd.NextUsn;
	med.MinMajorVersion = 0;
	med.MaxMajorVersion = 2; //����ָ���汾��

	iocp->PostStatus(m_index, 0, ior); //����IO��ɶ˿ڶ�ȡ����

	while (iocp->GetStatus(&uniqueKey, &dwRetBytes, (OVERLAPPED **)& pior, INFINITE)) //��IO��ɶ˿ڶ�ȡ����
	{
		//��������
		if (dwRetBytes == 0)
		{
			//��ȡ��һ��USN��¼
			med.StartFileReferenceNumber = 0; 
			offset.QuadPart = med.StartFileReferenceNumber;
			bOk = pior->ReadUsn(hDevice, &med, &offset);
		}
		else
		{
			//��ȡUSN��¼
			pUR = (PUSN_RECORD_V2)((PCHAR)pior->m_pvData + sizeof(USN)); //��λ����һ���ļ�
			dwNumBytes = dwRetBytes - sizeof(USN);
			while (dwNumBytes > sizeof(USN))
			{
				//��ȡ�ļ��������ֵ�
				ZeroMemory(fileName, MAX_PATH);
				WideCharToMultiByte(CP_OEMCP, NULL, pUR->FileName, pUR->FileNameLength / 2, fileName, pUR->FileNameLength, NULL, FALSE);//Unicode->ANSI
				m_files->Add(pUR->FileReferenceNumber, pUR->ParentFileReferenceNumber, fileName);

				//��λ����һ���ļ�
				dwNumBytes -= pUR->RecordLength;
				pUR = (PUSN_RECORD_V2)((PCHAR)pUR + pUR->RecordLength);
			}

			//��ȡ��һ��USN��¼
			med.StartFileReferenceNumber = *(USN*)pior->m_pvData; //�´ζ�ȡ
			offset.QuadPart = med.StartFileReferenceNumber;
			bOk = pior->ReadUsn(hDevice, &med, &offset);
			if (!bOk) //��ȡ���
			{
				break;
			}
		}
	}

	//�ر��豸
	CloseHandle(hDevice);
	delete iocp;
	delete ior;
}

//��ѯ�ļ������ز�ѯ�����ļ��б�
VOID Win32FileJournal::pQuery()
{
	this->m_list = this->m_files->Query(this->m_filter.toString());
}

//����FileReferenceNumber����ļ�·�����ٶȷǳ���
/*
VOID Win32FileJournal::GetFullPath(HANDLE hVolume, DWORDLONG FileReferenceNumber)
{
	UNICODE_STRING uStr;
	OBJECT_ATTRIBUTES oba;
	IO_STATUS_BLOCK isb = { 0 };
	HANDLE hFile;

	//RtlInitUnicodeString(&uStr, filePath);
	uStr.Length = sizeof(DWORDLONG);
	uStr.MaximumLength = uStr.Length;
	uStr.Buffer = (PWSTR)&FileReferenceNumber;

	//InitializeObjectAttributes(&oba, &uStr, OBJ_CASE_INSENSITIVE, NULL, NULL);
	oba.Length = sizeof(OBJECT_ATTRIBUTES);
	oba.ObjectName = &uStr;
	oba.RootDirectory = hVolume;
	oba.Attributes = OBJ_CASE_INSENSITIVE;
	oba.SecurityDescriptor = NULL;
	oba.SecurityQualityOfService = NULL;

	NTSTATUS status = NtCreateFile(&hFile,
		FILE_READ_ATTRIBUTES,
		&oba,
		&isb,
		0,
		0,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		FILE_OPEN,
		FILE_OPEN_BY_FILE_ID,
		NULL,
		0);

	if (status == NO_ERROR)
	{
		SIZE_T size = sizeof(OBJECT_NAME_INFORMATION) + MAX_PATH * sizeof(WCHAR);
		POBJECT_NAME_INFORMATION pfni = (POBJECT_NAME_INFORMATION)operator new(size);
		status = NtQueryInformationFile(hFile, &isb, pfni, size, FileNameInformation);
		if (status == NO_ERROR)
		{
			CHAR filePath[MAX_PATH] = { 0 };
			sprintf(filePath, "%.*S", pfni->Name.Length / 2, &pfni->Name.Buffer);
		}
		operator delete(pfni);
		CloseHandle(hFile);
	}
}
*/

/*
//����FileReferenceNumber����ļ���Ϣ
VOID Win32FileJournal::GetFileInfo(HANDLE hVolume, DWORDLONG FileReferenceNumber)
{
	DWORD lpBytesReturned = 0;
	NTFS_VOLUME_DATA_BUFFER ntfsVolData;
	DWORD dwFileRecordOutputBufferSize = 4096;
	PNTFS_FILE_RECORD_OUTPUT_BUFFER ntfsFileRecordOutput = NULL;
	NTFS_FILE_RECORD_INPUT_BUFFER mftRecordInput;
	BOOL bOk = DeviceIoControl(hVolume, FSCTL_GET_NTFS_VOLUME_DATA, NULL, 0, &ntfsVolData, sizeof(ntfsVolData), &lpBytesReturned, NULL);
	if (bOk)
	{
		dwFileRecordOutputBufferSize = sizeof(NTFS_FILE_RECORD_OUTPUT_BUFFER) + ntfsVolData.BytesPerFileRecordSegment - 1;
		ntfsFileRecordOutput = (PNTFS_FILE_RECORD_OUTPUT_BUFFER)malloc(dwFileRecordOutputBufferSize);
	}
	
	//��ȡ�ļ���ϸ��Ϣ(�ٶȷǳ���)
	mftRecordInput.FileReferenceNumber.QuadPart = FileReferenceNumber;
	if (DeviceIoControl(hVolume, FSCTL_GET_NTFS_FILE_RECORD, &mftRecordInput, sizeof(mftRecordInput), ntfsFileRecordOutput, dwFileRecordOutputBufferSize, &lpBytesReturned, NULL))
	{
		PFILE_RECORD_HEADER pfileRecordheader = (PFILE_RECORD_HEADER)ntfsFileRecordOutput->FileRecordBuffer;

		//if (pfileRecordheader->Ntfs.Type != NRH_FILE_TYPE)
		//	continue;
		//if (pfileRecordheader->Flags == FRH_IN_USE)
		//{
			for (PATTRIBUTE pAttribute = (PATTRIBUTE)((PBYTE)pfileRecordheader + pfileRecordheader->AttributesOffset);
				pAttribute->AttributeType != -1;
				pAttribute = (PATTRIBUTE)((PBYTE)pAttribute + pAttribute->Length))
			{
				if (pAttribute->AttributeType == FileName)
				{
					PFILENAME_ATTRIBUTE pFileNameAttr = PFILENAME_ATTRIBUTE((PBYTE)pAttribute + PRESIDENT_ATTRIBUTE(pAttribute)->ValueOffset);

					// ��ǰ��¼���ļ�
					if (0x0001 & pfileRecordheader->Flags)
					{
						WCHAR sFileName[MAX_PATH] = { 0 };
						swprintf_s(sFileName, L"%.*ws", pFileNameAttr->NameLength, pFileNameAttr->Name);
						//wprintf(sFileName);>	FastSearch.exe!Win32FileJournal::Search(TString strFilePath) �� 293	C++
					}
					break;
				}
			}
		//}
	}
		
	if (ntfsFileRecordOutput)
	{
		free(ntfsFileRecordOutput);
	}
						
}
*/

//���غ�����
DOUBLE Win32FileJournal::GetExactTickCount()
{
	static __int64 nFrequency = -1;
	if (nFrequency == -1)
	{
		if (!::QueryPerformanceFrequency((LARGE_INTEGER*)& nFrequency))
		{
			nFrequency = 0;
		}
	}
	if (nFrequency == 0)
	{
		return ::GetTickCount();
	}
	else
	{
		__int64 nCounter;
		::QueryPerformanceCounter((LARGE_INTEGER*)& nCounter);

		//1000.0f ����
		//1000000.0f ΢��
		return (1000 * (double)nCounter) / nFrequency;
	}
}