#include "filejournals.h"

Win32FileJournals::Win32FileJournals()
{
	//m_searchTread = new Win32Thread();
	this->m_diskCount = 0;
	this->m_lcokCount = 0;
	this->m_files = NULL;
	this->m_totalFileCount = 0;
	this->m_totalQueryCount = 0;
	this->m_totalTime = 0;
	this->m_loadProgressEvent = NULL;
	this->m_loadCompleteEvent = NULL;
	this->m_queryProgressEvent = NULL;
	this->m_queryCompleteEvent = NULL;
	qmemset(this->m_disks, NULL, MAX_DISK_COUNT * sizeof(DISKHANDLE*));
}

Win32FileJournals::~Win32FileJournals()
{
	//if (m_searchTread != NULL)
	//{
	//	delete m_searchTread;
	//	m_searchTread = NULL;
	//}
	this->CloseDisks();
}

//
// �����д���
//
BOOL Win32FileJournals::OpenDisks()
{
	DWORD drives;
	DISKHANDLE* disk;

	//��ʼ����������
	m_diskCount = 0;

	//��������߼�������
	drives = GetLogicalDrives();

	//��ʼ��ÿ����������ȡÿ���������ļ���Ŀ¼����
	for (int i = 0; i < MAX_DISK_COUNT; i++)
	{
		if ((drives >> (i)) & 0x1)
		{
			TCHAR str[5];
			UINT type;

			wsprintf(str, TEXT("%C:\\"), 0x41 + i);
			type = GetDriveType(str);
			if (type == DRIVE_FIXED)
			{
				disk = this->OpenDisk(0x41 + i);
				if (disk != NULL)
				{
					if (this->InitMFT(disk))
					{
						m_disks[i] = disk;
						m_diskCount += 1;
					}
					else
					{
						delete disk;
					}
				}
			}
		}
	}

	return m_diskCount > 0;
}

//
// �ر����д���
//
VOID Win32FileJournals::CloseDisks()
{
	DISKHANDLE* disk;
	for (int i = 0; i < MAX_DISK_COUNT; i++)
	{
		disk = this->m_disks[i];
		if (disk != NULL)
		{
			this->CloseDisk(disk);
		}
	}

	if (this->m_files != NULL)
	{
		delete[] this->m_files;
		this->m_files = NULL;
	}
}

//
// �򿪴���
// deviceName, �豸����,���磺E
//
Win32FileJournals::DISKHANDLE* Win32FileJournals::OpenDisk(TCHAR deviceName)
{
	DWORD readBytes;

	//�����豸����
	PDISKHANDLE tmpDisk = new DISKHANDLE;
	memset(tmpDisk, 0, sizeof(DISKHANDLE));

	//�����豸�������ƣ����磺\\.\E:
	tmpDisk->devicePath[0] = '\\';
	tmpDisk->devicePath[1] = '\\';
	tmpDisk->devicePath[2] = '.';
	tmpDisk->devicePath[3] = '\\';
	tmpDisk->devicePath[4] = deviceName;
	tmpDisk->devicePath[5] = ':';
	tmpDisk->devicePath[6] = '\0';

	//�򿪴���
	tmpDisk->hDevice = CreateFile(tmpDisk->devicePath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (tmpDisk->hDevice != INVALID_HANDLE_VALUE)
	{
		//��ȡ��������
		ReadFile(tmpDisk->hDevice, &tmpDisk->NTFS.bootSector, sizeof(BOOT_BLOCK), &readBytes, NULL);
		if (readBytes == sizeof(BOOT_BLOCK))
		{
			//ֻ֧��NTFS�ļ�ϵͳ
			if (strncmp("NTFS", (const char*)& tmpDisk->NTFS.bootSector.Format, 4) == 0)
			{
				tmpDisk->deviceIndex = (INT)deviceName;
				tmpDisk->deviceType = NTFSDISK;
				tmpDisk->deviceName = deviceName;

				tmpDisk->NTFS.bytesPerCluster = tmpDisk->NTFS.bootSector.BytesPerSector * tmpDisk->NTFS.bootSector.SectorsPerCluster;
				tmpDisk->NTFS.bytesPerRecord = tmpDisk->NTFS.bootSector.ClustersPerFileRecord < 0x80 ? tmpDisk->NTFS.bootSector.ClustersPerFileRecord * tmpDisk->NTFS.bytesPerCluster : 1 << (0x100 - tmpDisk->NTFS.bootSector.ClustersPerFileRecord);

				tmpDisk->NTFS.complete = FALSE;
				tmpDisk->NTFS.locationMFT.QuadPart = tmpDisk->NTFS.bootSector.MftStartLcn * tmpDisk->NTFS.bytesPerCluster;
				tmpDisk->NTFS.MFT = NULL;
				tmpDisk->NTFS.sizeMFT = 0;

				return tmpDisk;
			}
		}
	}

	delete tmpDisk;
	return NULL;
}

//
// �رմ���
// disk, �Ѵ򿪵Ĵ��̶���
//
BOOL Win32FileJournals::CloseDisk(DISKHANDLE* disk)
{
	if (disk != NULL)
	{
		//�ر�����
		if (disk->hDevice != INVALID_HANDLE_VALUE)
		{
			CloseHandle(disk->hDevice);
		}

		//�ͷ�MFTռ�õ��ڴ�
		if (disk->deviceType == NTFSDISK)
		{
			if (disk->NTFS.MFT != NULL)
				delete disk->NTFS.MFT;
			disk->NTFS.MFT = NULL;

			if (disk->NTFS.Bitmap != NULL)
				delete disk->NTFS.Bitmap;
			disk->NTFS.Bitmap = NULL;
		}

		//ɾ�����̶���
		delete disk;
		disk = NULL;

		return TRUE;
	}
	return FALSE;
}

//
// ��ʼ��MFT����ȡMFT��¼�ܵĴ�С������
// disk, �Ѵ򿪵Ĵ��̾��
//
BOOL Win32FileJournals::InitMFT(DISKHANDLE* disk)
{
	if (disk != NULL)
	{
		DWORD readBytes;
		ULARGE_INTEGER offset;
		UCHAR* buf;
		PFILE_RECORD_HEADER fh;
		PNONRESIDENT_ATTRIBUTE nattr;

		if (disk->deviceType == NTFSDISK) //ֻ֧��NTFS�ļ�ϵͳ
		{
			//�ƶ�ָ�뵽MFT��ʼλ��
			offset = disk->NTFS.locationMFT;
			SetFilePointer(disk->hDevice, offset.LowPart, (PLONG)& offset.HighPart, FILE_BEGIN);

			/*ע�⣺MFT��¼��FILE_RECORD_HEADER��һϵ��������ɡ�*/

			//��ȡMFT��¼ͷ
			buf = new UCHAR[disk->NTFS.bytesPerCluster];
			ReadFile(disk->hDevice, buf, disk->NTFS.bytesPerCluster, &readBytes, NULL);
			fh = (PFILE_RECORD_HEADER)(buf);
			FixFileRecord(fh);

			//��ȡ$DATA�м�¼������
			nattr = (PNONRESIDENT_ATTRIBUTE)FindAttribute(fh, Data);
			if (nattr != NULL)
			{
				disk->NTFS.sizeMFT = (DWORD)nattr->DataSize; //$DATA��С(�ֽ���)
				disk->NTFS.recordCount = disk->NTFS.sizeMFT / disk->NTFS.bytesPerRecord; //$DATA��¼����
				disk->NTFS.MFT = buf;//MFT����(���ָ��)

				return disk->NTFS.recordCount > 0;
			}
		}
	}
	return FALSE;
}

// ����ļ���¼
BOOL Win32FileJournals::FixFileRecord(PFILE_RECORD_HEADER file)
{
	//int sec = 2048;
	PUSHORT usa = PUSHORT(PUCHAR(file) + file->Ntfs.UsaOffset);
	PUSHORT sector = PUSHORT(file);

	if (file->Ntfs.UsaCount > 4)
	{
		return FALSE;
	}

	for (ULONG i = 1; i < file->Ntfs.UsaCount; i++)
	{
		sector[255] = usa[i];
		sector += 256;
	}

	return TRUE;
}

// ��������
PATTRIBUTE Win32FileJournals::FindAttribute(PFILE_RECORD_HEADER file, ATTRIBUTE_TYPE type)
{
	PATTRIBUTE attr = (PATTRIBUTE)((PUCHAR)(file)+file->AttributesOffset); //��λ����һ������

	for (int i = 1; i < file->NextAttributeNumber; i++)
	{
		if (attr->AttributeType == type)
		{
			return attr;
		}
		if (attr->AttributeType < 1 || attr->AttributeType > 0x100)
		{
			break;
		}
		if (attr->Length > 0 && attr->Length < file->BytesInUse)
		{
			attr = PATTRIBUTE(PUCHAR(attr) + attr->Length);
		}
		else
		{
			if (attr->Nonresident == TRUE)
			{
				attr = PATTRIBUTE(PUCHAR(attr) + sizeof(NONRESIDENT_ATTRIBUTE));
			}
		}
	}
	return NULL;
}

// ˢ�£�������������
VOID Win32FileJournals::Refresh(FILE_LOAD_PROGRESS_EVENT_HANDLER progressEvent, FILE_LOAD_COMPLETE_EVENT_HANDLER completeEvent)
{
	//������
	if (m_lcokCount > 0)
	{
		return;
	}

	//���ò���
	m_lcokCount = 0;
	m_totalFileCount = 0;
	m_totalQueryCount = 0;
	m_totalTime = 0;
	m_loadProgressEvent = progressEvent;
	m_loadCompleteEvent = completeEvent;

	//�򿪴���
	if (m_diskCount == 0)
	{
		qmemset(this->m_disks, NULL, MAX_DISK_COUNT * sizeof(DISKHANDLE*));
	}
	else
	{
		this->CloseDisks();
	}
	if (this->OpenDisks())
	{
		//��������
		m_files = new Win32FileJournal[m_diskCount];
	}
	
	//��������
	if (m_diskCount > 0)
	{
		int n = 0;
		for (int i = 0; i < 32; i++)
		{
			if (m_disks[i] != NULL)
			{
				m_files[n].Refresh(m_disks[i]->devicePath, RefreshProgress, this);
				n += 1;
			}
		}
	}
	
}

// ��ѯ�ļ�
// Filter, �������ˣ�֧��?��*ͨ���, ���֮��������(|)�ָ������磺"*.txt|*.tmp"
VOID Win32FileJournals::Query(TString filter, FILE_QUERY_PROGRESS_EVENT_HANDLER progressEvent, FILE_QUERY_COMPLETE_EVENT_HANDLER completeEvent)
{
	//������
	if (m_lcokCount > 0)
	{
		return;
	}

	//���ò���
	m_lcokCount = 0;
	m_totalFileCount = 0;
	m_totalQueryCount = 0;
	m_totalTime = 0;
	m_queryProgressEvent = progressEvent;
	m_queryCompleteEvent = completeEvent;

	//��ѯ
	if (m_diskCount > 0)
	{
		for (int i = 0; i < m_diskCount; i++)
		{
			m_files[i].Query(filter, QueryProgress, this);
		}
	}
}

// ȡ��ˢ�»��ѯ
VOID Win32FileJournals::Cancel()
{
	if (m_diskCount > 0)
	{
		for (int i = 0; i < m_diskCount; i++)
		{
			m_files[i].Cancel();
		}
	}
}

// ���ؽ���
VOID Win32FileJournals::RefreshProgress(LPVOID lpvOwner, TString* deviceName, DWORD fileCount, DOUBLE elapsedTime)
{
	Win32FileJournals* p = (Win32FileJournals*)lpvOwner;
	p->CallRefreshProgress(deviceName, fileCount, elapsedTime);
}

VOID Win32FileJournals::CallRefreshProgress(TString* deviceName, DWORD fileCount, DOUBLE elapsedTime)
{
	//������
	m_lcokCount += 1;//��������
	m_totalFileCount += fileCount;
	m_totalTime += elapsedTime;
	if (m_loadProgressEvent != NULL)
	{
		m_loadProgressEvent(deviceName, m_lcokCount, m_diskCount, fileCount, elapsedTime);
	}

	//�������
	if (m_lcokCount >= m_diskCount)
	{
		m_lcokCount = 0;//��������
		if (m_loadCompleteEvent != NULL)
		{
			m_loadCompleteEvent(m_diskCount, m_totalFileCount, m_totalTime);
		}
	}
}

// ��ѯ����
VOID Win32FileJournals::QueryProgress(LPVOID lpvOwner, UsnList* fileList, TString* deviceName, DWORD fileCount, DOUBLE elapsedTime)
{
	Win32FileJournals* p = (Win32FileJournals*)lpvOwner;
	p->CallQueryProgress(fileList, deviceName, fileCount, elapsedTime);
}
VOID Win32FileJournals::CallQueryProgress(UsnList* fileList, TString* deviceName, DWORD fileCount, DOUBLE elapsedTime)
{
	//��ѯ��
	m_lcokCount += 1;//��������
	m_totalQueryCount += fileCount;
	m_totalTime += elapsedTime;
	if (m_queryProgressEvent != NULL)
	{
		m_queryProgressEvent(fileList, deviceName, m_lcokCount, m_diskCount, fileCount, elapsedTime);
	}

	//��ѯ���
	if (m_lcokCount >= m_diskCount)
	{
		m_lcokCount = 0;//��������
		if (m_queryCompleteEvent != NULL)
		{
			m_queryCompleteEvent(fileList, m_diskCount, m_totalQueryCount, m_totalTime);
		}
	}
}
