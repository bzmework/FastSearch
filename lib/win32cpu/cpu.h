#ifndef __WIN32CPU_H__
#define __WIN32CPU_H__

#ifndef _WIN32_WINNT
#define _WIN32_WINNT   0x0500
#endif

#include <windows.h>
#include "../typedef.h"
#include "../utility/utility.h"
#include "../string/string.h"
#include "../win32gdi/gdi.h"

#define NTSTATUS long
#define IsWinNT (GetVersion() < 0x80000000)?true:false
#define LI2Double(x) ((double)((x).HighPart) * 4.294967296E9 + (double)((x).LowPart))

//
// CPU使用率
//
class Win32CpuUsage
{
public:
	Win32CpuUsage();
	virtual ~Win32CpuUsage();
	INT Query();

private:
	typedef enum _SYSTEM_INFORMATION
	{
		SystemBasicInfo = 0,
		SystemPerformanceInfo = 2,
		SystemTimeOfDayInfo = 3,
		SystemProcessInfo = 5,
		SystemProcessorPerformanceInfo = 8,
		SystemInterruptInfo = 23,
		SystemExceptionInfo = 33,
		SystemRegistryQuotaInfo = 37,
		SystemLookasideInfo = 45
	} SYSTEM_INFORMATION;

	typedef struct _SYSTEM_BASICINFORMATION
	{
		DWORD   dwUnknown1;
		ULONG   uKeMaximumIncrement;
		ULONG   uPageSize;
		ULONG   uMmNumberOfPhysicalPages;
		ULONG   uMmLowestPhysicalPage;
		ULONG   uMmHighestPhysicalPage;
		ULONG   uAllocationGranularity;
		PVOID   pLowestUserAddress;
		PVOID   pMmHighestUserAddress;
		ULONG   uKeActiveProcessors;
		BYTE    bKeNumberProcessors;
		BYTE    bUnknown2;
		WORD    wUnknown3;
	} SYSTEM_BASICINFORMATION;

	typedef struct _SYSTEM_PERFORMANCEINFORMATION
	{
		LARGE_INTEGER liIdleTime;
		DWORD dwSpare[76];
	} SYSTEM_PERFORMANCEINFORMATION;

	typedef struct _SYSTEM_TIMEINFORMATION
	{
		LARGE_INTEGER liKeBootTime;
		LARGE_INTEGER liKeSystemTime;
		LARGE_INTEGER liExpTimeZoneBias;
		ULONG ulCurrentTimeZoneId;
		DWORD dwReserved;
	} SYSTEM_TIMEINFORMATION;

private:
	HKEY m_hKey;
	DWORD m_dwDataSize;
	DWORD m_dwCpuUsage;
	DWORD m_dwType;
	LARGE_INTEGER m_liOldIdleTime;
	LARGE_INTEGER m_liOldSystemTime;
	BOOL m_bIsWinNT;
	BOOL m_bIsInitialized;
	HINSTANCE m_hNtDllLib;
    NTSTATUS (WINAPI *m_lpfNtQuerySystemInformation)(SYSTEM_INFORMATION,PVOID,ULONG,PULONG);
};

//
// Cpu核心
//
class Win32CpuCore
{
public:
	//关机选项
	typedef enum _ExitWindowOption
	{ 
		Logoff,			//中止进程，然后注销
		Reboot,			//终止所有运行的进程并重启计算机
		Shutdown,		//终止所有进程并安全关闭计算机，注意：用户需要手动关闭电源
		Poweroff,		//关闭计算机并切断电源
		Standby,		//等待
		Hibernate,		//睡眠
		Lock,			//锁定
		MonitorLowPower,//显示器处于低能耗状态
		MonitorShutOff	//关闭显示器
	} ExitWindowOption;

	//关机方式
	typedef enum _ExitWindowMode
	{
		Normally = 0,	//正常执行
		Immediately = 1	//立即执行
	} ExitWindowMode;

public:
	BOOL Action(ExitWindowOption option, ExitWindowMode mode);

private:
	BOOL GetShudownPrivilege();

};

//
// Cpu接口
//
class Win32Cpu: public Win32CpuUsage, Win32CpuCore
{
public:
	HDC hDC;
	RECT Rect;

	Win32Cpu();
	virtual ~Win32Cpu();
	LPCTSTR SysCurrentTime();
	LPCTSTR SysRunningTime();
	virtual VOID Draw();
};

#endif //__WIN32CPU_H__