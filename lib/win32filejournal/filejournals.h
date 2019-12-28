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

//文件数据库
class Win32FileJournals
{
public:
	//磁盘类型
	enum DiskType
	{
		NTFSDISK = 1,

		//not supported
		FAT32DISK = 2,
		FATDISK = 4,
		EXT2 = 8,
		UNKNOWN = 0xff99ff99
	};

	//名称类型
	enum NameType
	{
		POSIX_NAME = 0,
		WIN32_NAME = 1,
		DOS_NAME = 2,
		WIN32DOS_NAME = 3
	};

	//磁盘对象
	typedef struct
	{
		HANDLE hDevice; //设备句柄
		INT deviceIndex;//设备索引
		DWORD deviceType; //文件系统类型
		TCHAR deviceName; //DOS设备名称，例如：E:
		TCHAR devicePath[8];//设备路径，例如：\\.\E:

		union
		{
			struct
			{
				BOOT_BLOCK bootSector; //系统引导扇区
				DWORD bytesPerRecord; //每条MFT记录字节数
				DWORD bytesPerCluster; //每簇字节数
				BOOL complete;
				DWORD sizeMFT; //MFT大小(字节数)
				DWORD recordCount; //MFT记录数量 = sizeMFT / BytesPerFileRecord
				ULARGE_INTEGER locationMFT; //MFT位置
				UCHAR* MFT; //MFT数据(入口指针)
				UCHAR* Bitmap;//Bitmap数据(入口指针)
			} NTFS;
			struct
			{
				DWORD FAT;
			} FAT;
		};
	} DISKHANDLE, * PDISKHANDLE;

public:

	// 加载进度事件
	typedef void (CALLBACK* FILE_LOAD_PROGRESS_EVENT_HANDLER)(TString* deviceName, INT deviceIndex, INT deviceCount, DWORD fileCount, DOUBLE elapsedTime);

	// 加载完成事件
	typedef void (CALLBACK* FILE_LOAD_COMPLETE_EVENT_HANDLER)(INT deviceCount, DWORD fileCount, DOUBLE elapsedTime);

	// 查询进度事件
	typedef void (CALLBACK* FILE_QUERY_PROGRESS_EVENT_HANDLER)(UsnList* fileList, TString* deviceName, INT deviceIndex, INT deviceCount, DWORD fileCount, DOUBLE elapsedTime);

	// 查询完成事件
	typedef void (CALLBACK* FILE_QUERY_COMPLETE_EVENT_HANDLER)(UsnList* fileList, INT deviceCount, DWORD fileCount, DOUBLE elapsedTime);

public:

	Win32FileJournals();
	virtual ~Win32FileJournals();
	
	// 刷新，加载所有数据
	VOID Refresh(FILE_LOAD_PROGRESS_EVENT_HANDLER progressEvent = NULL, FILE_LOAD_COMPLETE_EVENT_HANDLER completeEvent = NULL);

	// 查询文件
	// Filter, 搜索过滤，支持?和*通配符, 多个之间用竖线(|)分隔，例如："*.txt|*.tmp"
	VOID Query(TString filter, FILE_QUERY_PROGRESS_EVENT_HANDLER progressEvent = NULL, FILE_QUERY_COMPLETE_EVENT_HANDLER completeEvent = NULL);

	// 取消刷新或查询
	VOID Cancel();

	// 排序(未实现)

	// 取得指定索引处的文件信息(未实现)

private:

	// 打开所有磁盘
	_INLINE_ BOOL OpenDisks();

	// 关闭所有磁盘
	_INLINE_ VOID CloseDisks();

	// 打开磁盘
	// deviceName, 设备名称,例如：E
	_INLINE_ DISKHANDLE* OpenDisk(TCHAR deviceName);

	// 关闭磁盘
	// disk, 已打开的磁盘对象
	_INLINE_ BOOL CloseDisk(DISKHANDLE* disk);

	// 初始化MFT，获取MFT记录总的大小和数量
	// disk, 已打开的磁盘句柄
	_INLINE_ BOOL InitMFT(DISKHANDLE* disk);

	// 填充文件记录
	_INLINE_ BOOL FixFileRecord(PFILE_RECORD_HEADER file);

	// 查找属性
	_INLINE_ PATTRIBUTE FindAttribute(PFILE_RECORD_HEADER file, ATTRIBUTE_TYPE type);

private:

	// 加载进度
	static VOID CALLBACK RefreshProgress(LPVOID lpvOwner, TString* deviceName, DWORD fileCount, DOUBLE elapsedTime);
	VOID CallRefreshProgress(TString* deviceName, DWORD fileCount, DOUBLE elapsedTime);

	// 查询进度
	static VOID CALLBACK QueryProgress(LPVOID lpvOwner, UsnList* fileList, TString* deviceName, DWORD fileCount, DOUBLE elapsedTime);
	VOID CallQueryProgress(UsnList* fileList, TString* deviceName, DWORD fileCount, DOUBLE elapsedTime);

private:

	INT m_diskCount; //磁盘数量
	DISKHANDLE* m_disks[32]; //磁盘对象
	Win32FileJournal* m_files; //文件对象
	DWORD m_totalFileCount; //总的文件数量
	DWORD m_totalQueryCount; //总的查询数量
	DOUBLE m_totalTime; //总的加载/查询时间(毫秒数)
	volatile BOOL m_lcokCount; //加载/查询时锁定中的数量

	FILE_LOAD_PROGRESS_EVENT_HANDLER m_loadProgressEvent; //加载进度事件
	FILE_LOAD_COMPLETE_EVENT_HANDLER m_loadCompleteEvent;//加载完成事件
	FILE_QUERY_PROGRESS_EVENT_HANDLER m_queryProgressEvent;//查询进度事件
	FILE_QUERY_COMPLETE_EVENT_HANDLER m_queryCompleteEvent;//查询完成事件
};

#endif //__FILEJOURNALS_H__