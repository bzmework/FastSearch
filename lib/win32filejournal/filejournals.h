#ifndef __FILEJOURNALS_H__
#define __FILEJOURNALS_H__

#include <windows.h>

#include "../typedef.h"
#include "../utility/utility.h"
//#include "../array/array.h"
//#include "../string/string.h"
//#include "../string/wstring.h"
//#include "../list/list.h"
//#include "../dictionary/usndictionary.h"
//#include "../quicklz/quicklz.h"
//#include "../win32thread/thread.h"
//#include "../win32file/file.h"
//#include "../win32mmap/memmap.h"
//#include "../win32exception/exception.h"

#include "ntfs.h"
#include "filejournal.h"

//�ļ����ݿ�
class Win32FileJournals
{
public:
	//��������
	enum DiskType
	{
		NTFSDISK = 1,

		//not supported
		FAT32DISK = 2,
		FATDISK = 4,
		EXT2 = 8,
		UNKNOWN = 0xff99ff99
	};

	//��������
	enum NameType
	{
		POSIX_NAME = 0,
		WIN32_NAME = 1,
		DOS_NAME = 2,
		WIN32DOS_NAME = 3
	};

	//���̶���
	typedef struct
	{
		HANDLE hDevice; //�豸���
		INT deviceIndex;//�豸����
		DWORD deviceType; //�ļ�ϵͳ����
		TCHAR deviceName; //DOS�豸���ƣ����磺E:
		TCHAR devicePath[8];//�豸·�������磺\\.\E:

		union
		{
			struct
			{
				BOOT_BLOCK bootSector; //ϵͳ��������
				DWORD bytesPerRecord; //ÿ��MFT��¼�ֽ���
				DWORD bytesPerCluster; //ÿ���ֽ���
				BOOL complete;
				DWORD sizeMFT; //MFT��С(�ֽ���)
				DWORD recordCount; //MFT��¼���� = sizeMFT / BytesPerFileRecord
				ULARGE_INTEGER locationMFT; //MFTλ��
				UCHAR* MFT; //MFT����(���ָ��)
				UCHAR* Bitmap;//Bitmap����(���ָ��)
			} NTFS;
			struct
			{
				DWORD FAT;
			} FAT;
		};
	} DISKHANDLE, * PDISKHANDLE;

public:

	// ���ؽ����¼�
	typedef void (CALLBACK* FILE_LOAD_PROGRESS_EVENT_HANDLER)(TString* deviceName, INT deviceIndex, INT deviceCount, DWORD fileCount, DOUBLE elapsedTime);

	// ��������¼�
	typedef void (CALLBACK* FILE_LOAD_COMPLETE_EVENT_HANDLER)(INT deviceCount, DWORD fileCount, DOUBLE elapsedTime);

	// ��ѯ�����¼�
	typedef void (CALLBACK* FILE_QUERY_PROGRESS_EVENT_HANDLER)(UsnList* fileList, TString* deviceName, INT deviceIndex, INT deviceCount, DWORD fileCount, DOUBLE elapsedTime);

	// ��ѯ����¼�
	typedef void (CALLBACK* FILE_QUERY_COMPLETE_EVENT_HANDLER)(UsnList* fileList, INT deviceCount, DWORD fileCount, DOUBLE elapsedTime);

public:

	Win32FileJournals();
	virtual ~Win32FileJournals();
	
	// ˢ�£�������������
	VOID Refresh(FILE_LOAD_PROGRESS_EVENT_HANDLER progressEvent = NULL, FILE_LOAD_COMPLETE_EVENT_HANDLER completeEvent = NULL);

	// ��ѯ�ļ�
	// Filter, �������ˣ�֧��?��*ͨ���, ���֮��������(|)�ָ������磺"*.txt|*.tmp"
	VOID Query(TString filter, FILE_QUERY_PROGRESS_EVENT_HANDLER progressEvent = NULL, FILE_QUERY_COMPLETE_EVENT_HANDLER completeEvent = NULL);

	// ȡ��ˢ�»��ѯ
	VOID Cancel();

	// ����(δʵ��)

	// ȡ��ָ�����������ļ���Ϣ(δʵ��)

private:

	// �����д���
	_INLINE_ BOOL OpenDisks();

	// �ر����д���
	_INLINE_ VOID CloseDisks();

	// �򿪴���
	// deviceName, �豸����,���磺E
	_INLINE_ DISKHANDLE* OpenDisk(TCHAR deviceName);

	// �رմ���
	// disk, �Ѵ򿪵Ĵ��̶���
	_INLINE_ BOOL CloseDisk(DISKHANDLE* disk);

	// ��ʼ��MFT����ȡMFT��¼�ܵĴ�С������
	// disk, �Ѵ򿪵Ĵ��̾��
	_INLINE_ BOOL InitMFT(DISKHANDLE* disk);

	// ����ļ���¼
	_INLINE_ BOOL FixFileRecord(PFILE_RECORD_HEADER file);

	// ��������
	_INLINE_ PATTRIBUTE FindAttribute(PFILE_RECORD_HEADER file, ATTRIBUTE_TYPE type);

private:

	// ���ؽ���
	static VOID CALLBACK RefreshProgress(LPVOID lpvOwner, TString* deviceName, DWORD fileCount, DOUBLE elapsedTime);
	VOID CallRefreshProgress(TString* deviceName, DWORD fileCount, DOUBLE elapsedTime);

	// ��ѯ����
	static VOID CALLBACK QueryProgress(LPVOID lpvOwner, UsnList* fileList, TString* deviceName, DWORD fileCount, DOUBLE elapsedTime);
	VOID CallQueryProgress(UsnList* fileList, TString* deviceName, DWORD fileCount, DOUBLE elapsedTime);

private:

	INT m_diskCount; //��������
	DISKHANDLE* m_disks[32]; //���̶���
	Win32FileJournal* m_files; //�ļ�����
	DWORD m_totalFileCount; //�ܵ��ļ�����
	DWORD m_totalQueryCount; //�ܵĲ�ѯ����
	DOUBLE m_totalTime; //�ܵļ���/��ѯʱ��(������)
	volatile BOOL m_lcokCount; //����/��ѯʱ�����е�����

	FILE_LOAD_PROGRESS_EVENT_HANDLER m_loadProgressEvent; //���ؽ����¼�
	FILE_LOAD_COMPLETE_EVENT_HANDLER m_loadCompleteEvent;//��������¼�
	FILE_QUERY_PROGRESS_EVENT_HANDLER m_queryProgressEvent;//��ѯ�����¼�
	FILE_QUERY_COMPLETE_EVENT_HANDLER m_queryCompleteEvent;//��ѯ����¼�
};

#endif //__FILEJOURNALS_H__