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

// 刷新，加载所有数据
VOID Win32FileJournal::Refresh(TString path, FILE_LOAD_COMPLETE_EVENT_HANDLER completeEvent, LPVOID lpvOwner)
{
	//设置路径
	if (path.getLength() > 0)
	{
		m_path = path;
		m_name = m_path.substr(4, 1); //设备名称，例如：E
		m_index = (INT)m_path[4]; //设备索引，例如：5
	}
	else
	{
		return;
	}

	//设置参数
	m_loadCompleteEvent = completeEvent;
	m_lpvOwner = lpvOwner;

	//创建线程，加载数据
	m_refreshThread->Create(Win32FileJournal::RefreshThreadFunc, this);
}

// 查询文件
// filter, 筛选，支持?和*通配符, 多个之间用竖线(|)分隔，例如："*.txt|*.tmp"
VOID Win32FileJournal::Query(TString filter, FILE_QUERY_COMPLETE_EVENT_HANDLER completeEvent, LPVOID lpvOwner)
{
	//设置筛选
	if (filter.getLength() > 0)
	{
		m_filter = filter;
	}
	else
	{
		m_filter = __T("*");
	}

	//设置参数
	m_queryCompleteEvent = completeEvent;
	m_lpvOwner = lpvOwner;

	//创建线程，查询数据
	m_queryThread->Create(Win32FileJournal::QueryThreadFunc, this);
}

// 取消刷新或查询
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

//刷新线程
void Win32FileJournal::RefreshThreadFunc(LPVOID lpvFileJournal)
{
	DOUBLE elapsedTime = GetExactTickCount();
	Win32FileJournal* fileObj = (Win32FileJournal*)lpvFileJournal;

	//清除文件
	fileObj->pClear(TRUE);

	//加载文件
	//fileObj->pRefresh();
	fileObj->pRefreshIocp();

	//完成
	if (fileObj->m_loadCompleteEvent != NULL)
	{
		fileObj->m_loadCompleteEvent(fileObj->m_lpvOwner, &fileObj->m_name, fileObj->m_files->Count(), GetExactTickCount() - elapsedTime);
	}
}

//查询线程
void Win32FileJournal::QueryThreadFunc(LPVOID lpvFileJournal)
{
	DOUBLE elapsedTime = GetExactTickCount();
	Win32FileJournal* fileObj = (Win32FileJournal*)lpvFileJournal;

	//清除列表
	fileObj->pClear(FALSE);

	//查询文件
	fileObj->pQuery();

	//完成
	if (fileObj->m_queryCompleteEvent != NULL)
	{
		fileObj->m_queryCompleteEvent(fileObj->m_lpvOwner, fileObj->m_list, &fileObj->m_name, fileObj->m_list->Count(), GetExactTickCount() - elapsedTime);
	}
}

//清除数据
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
 
// 加载文件
// 参考：https://docs.microsoft.com/zh-cn/windows/desktop/FileIO/walking-a-buffer-of-change-journal-records
//		https://www.installsetupconfig.com/win32programming/windowsvolumeapis1_11.html
VOID Win32FileJournal::pRefresh()
{
	BOOL bOk = FALSE;

	/* 1.取得卷标的句柄 */
	
	//获取驱动盘句柄 调用CreateFile函数需要管理员权限
	HANDLE hDevice = CreateFile(this->m_path,//传入的卷标名必须为\\.\C:的形式
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE, //必须包含有FILE_SHARE_WRITE
		NULL,
		OPEN_EXISTING,  //必须包含OPEN_EXISTING, 而CREATE_ALWAYS可能会导致错误
		FILE_ATTRIBUTE_READONLY, //FILE_ATTRIBUTE_NORMAL可能会导致错误
		NULL);
	if (hDevice != INVALID_HANDLE_VALUE)
	{
		/* 2.从卷中取得日志 */

		DWORD lpBytesReturned = 0;
		CREATE_USN_JOURNAL_DATA cujd;
		cujd.MaximumSize = 0; //使用默认值
		cujd.AllocationDelta = 0; //使用默认值
		bOk = DeviceIoControl(hDevice,
			FSCTL_CREATE_USN_JOURNAL, //其作用是有日志已经存在则打开以供读取或修改，没有日志就创建一个新的供读取或修改。
			&cujd,
			sizeof(CREATE_USN_JOURNAL_DATA),
			NULL,
			0,
			&lpBytesReturned,
			NULL);
		if (bOk)
		{
			/* 3.读取日志的信息 */
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
				/* 4.枚举所有的数据 */

				TCHAR buffer[RECORD_BUF_SIZE] = { 0 }; //用于储存记录的缓冲,尽量足够地大
				TCHAR fileName[MAX_PATH] = { 0 };
				DWORD dwRetBytes = 0;
				PUSN_RECORD_V2 pUR;
				MFT_ENUM_DATA med = { 0 };

				med.StartFileReferenceNumber = 0;
				med.LowUsn = 0;//UJD.FirstUsn; 测试发现，如果用FirstUsn有时候不正确，导致获取到不完整的数据.
				med.HighUsn = ujd.NextUsn;
				med.MinMajorVersion = 0;
				med.MaxMajorVersion = 2; //必须指定版本号
				while (bOk = DeviceIoControl(hDevice,
					FSCTL_ENUM_USN_DATA,
					&med,
					sizeof(MFT_ENUM_DATA),
					buffer,
					RECORD_BUF_SIZE,
					&lpBytesReturned,
					NULL))
				{
					//读取USN记录
					pUR = (PUSN_RECORD_V2)((PCHAR)buffer + sizeof(USN)); //定位到第一个文件
					dwRetBytes = lpBytesReturned - sizeof(USN);
					while (dwRetBytes > sizeof(USN))
					{
						//读取文件并放入字典
						ZeroMemory(fileName, MAX_PATH);
						WideCharToMultiByte(CP_OEMCP, NULL, pUR->FileName, pUR->FileNameLength / 2, fileName, pUR->FileNameLength, NULL, FALSE);//Unicode->ANSI
						m_files->Add(pUR->FileReferenceNumber, pUR->ParentFileReferenceNumber, fileName);
								
						//定位到下一个文件
						dwRetBytes -= pUR->RecordLength;
						pUR = (PUSN_RECORD_V2)((PCHAR)pUR + pUR->RecordLength);
					}

					//读取下一个USN记录
					med.StartFileReferenceNumber = *(USN*)&buffer;
				}
			}
		}
		CloseHandle(hDevice);
	}
}

// 异步加载文件(相对于多线程，通过IO完成端口读取数据带来的性能提升并不明显)
VOID Win32FileJournal::pRefreshIocp()
{
	BOOL bOk = FALSE;

	/* 1.取得卷标的句柄 */
	//获取驱动盘句柄 调用CreateFile函数需要管理员权限
	HANDLE hDevice = CreateFile(this->m_path,//传入的卷标名必须为\\.\C:的形式
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE, //必须包含有FILE_SHARE_WRITE
		NULL,
		OPEN_EXISTING,  //必须包含OPEN_EXISTING, 而CREATE_ALWAYS可能会导致错误
		FILE_ATTRIBUTE_READONLY, //FILE_ATTRIBUTE_NORMAL可能会导致错误
		NULL);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		return;
	}

	/* 2.从卷中取得日志 */
	DWORD dwRetBytes;
	DWORD dwNumBytes;
	CREATE_USN_JOURNAL_DATA cujd;
	cujd.MaximumSize = 0; //使用默认值
	cujd.AllocationDelta = 0; //使用默认值
	bOk = DeviceIoControl(hDevice,
		FSCTL_CREATE_USN_JOURNAL, //其作用是有日志已经存在则打开以供读取或修改，没有日志就创建一个新的供读取或修改。
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

	/* 3.读取日志的信息 */
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
		
	/* 4.初始化IO完成端口 */
	//以异步方式重新打开设备
	CloseHandle(hDevice);
	hDevice = CreateFile(m_path,
		GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED, /*设置FILE_FLAG_NO_BUFFERING参数后，ReadFile/WriteFile的lpBuffer大小必须是1024的整数倍*/
		NULL);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		return;
	}
	//创建IO完成端口
	IoCompletionPort* iocp = new IoCompletionPort();
	if (!iocp->Create())
	{
		return;
	}
	//将文件与IO完成端口关联
	if (!iocp->AddDevice(hDevice, this->m_index))
	{
		return;
	}

	//初始化IO完成端口进行数据交互时所需的数据缓冲区
	IoCompletionPortFileReq* ior = new IoCompletionPortFileReq();
	bOk = ior->Alloc(RECORD_IOCP_BUF_SIZE);//每个I/O请求都需要一个数据缓冲区来进行数据传输
	if (!bOk)
	{
		return;
	}

	/* 5.枚举所有的数据 */
	LARGE_INTEGER offset = { 0 };
	ULONG_PTR uniqueKey;
	IoCompletionPortFileReq* pior;

	PUSN_RECORD_V2 pUR;
	MFT_ENUM_DATA med = { 0 };
	TCHAR fileName[MAX_PATH] = { 0 };

	med.StartFileReferenceNumber = 0;
	med.LowUsn = 0;//UJD.FirstUsn; 测试发现，如果用FirstUsn有时候不正确，导致获取到不完整的数据.
	med.HighUsn = ujd.NextUsn;
	med.MinMajorVersion = 0;
	med.MaxMajorVersion = 2; //必须指定版本号

	iocp->PostStatus(m_index, 0, ior); //触发IO完成端口读取数据

	while (iocp->GetStatus(&uniqueKey, &dwRetBytes, (OVERLAPPED **)& pior, INFINITE)) //从IO完成端口读取数据
	{
		//解析数据
		if (dwRetBytes == 0)
		{
			//读取第一个USN记录
			med.StartFileReferenceNumber = 0; 
			offset.QuadPart = med.StartFileReferenceNumber;
			bOk = pior->ReadUsn(hDevice, &med, &offset);
		}
		else
		{
			//读取USN记录
			pUR = (PUSN_RECORD_V2)((PCHAR)pior->m_pvData + sizeof(USN)); //定位到第一个文件
			dwNumBytes = dwRetBytes - sizeof(USN);
			while (dwNumBytes > sizeof(USN))
			{
				//读取文件并放入字典
				ZeroMemory(fileName, MAX_PATH);
				WideCharToMultiByte(CP_OEMCP, NULL, pUR->FileName, pUR->FileNameLength / 2, fileName, pUR->FileNameLength, NULL, FALSE);//Unicode->ANSI
				m_files->Add(pUR->FileReferenceNumber, pUR->ParentFileReferenceNumber, fileName);

				//定位到下一个文件
				dwNumBytes -= pUR->RecordLength;
				pUR = (PUSN_RECORD_V2)((PCHAR)pUR + pUR->RecordLength);
			}

			//读取下一个USN记录
			med.StartFileReferenceNumber = *(USN*)pior->m_pvData; //下次读取
			offset.QuadPart = med.StartFileReferenceNumber;
			bOk = pior->ReadUsn(hDevice, &med, &offset);
			if (!bOk) //读取完毕
			{
				break;
			}
		}
	}

	//关闭设备
	CloseHandle(hDevice);
	delete iocp;
	delete ior;
}

//查询文件，返回查询到的文件列表
VOID Win32FileJournal::pQuery()
{
	this->m_list = this->m_files->Query(this->m_filter.toString());
}

//根据FileReferenceNumber获得文件路径，速度非常慢
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
//根据FileReferenceNumber获得文件信息
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
	
	//获取文件详细信息(速度非常快)
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

					// 当前记录是文件
					if (0x0001 & pfileRecordheader->Flags)
					{
						WCHAR sFileName[MAX_PATH] = { 0 };
						swprintf_s(sFileName, L"%.*ws", pFileNameAttr->NameLength, pFileNameAttr->Name);
						//wprintf(sFileName);>	FastSearch.exe!Win32FileJournal::Search(TString strFilePath) 行 293	C++
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

//返回毫秒数
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

		//1000.0f 毫秒
		//1000000.0f 微秒
		return (1000 * (double)nCounter) / nFrequency;
	}
}