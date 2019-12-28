#include "cpu.h"

//警告 C28159 考虑使用“InitiateSystemShutdownEx”而不是“ExitWindowsEx”。原因 : Legacy API.Rearchitect to avoid Reboot
//警告 C4996	 'GetVersion': 被声明为已否决
//警告 C26451 Arithmetic overflow : Using operator '-' on a 4 byte valueand then casting the result to a 8 byte value.Cast the value to the wider type before calling operator '-' to avoid overflow(io.2)
#pragma warning(disable:28159)
#pragma warning(disable:4996)
#pragma warning(disable:26451)

#pragma region CPU使用率

Win32CpuUsage::Win32CpuUsage()
{
	this->m_lpfNtQuerySystemInformation = NULL;
	this->m_hKey = NULL;
	this->m_dwDataSize = 0;
	this->m_dwCpuUsage = 0;
	this->m_dwType = 0;
	this->m_liOldIdleTime = { 0 };
	this->m_liOldSystemTime = { 0 };
	this->m_hNtDllLib = NULL;
	this->m_bIsInitialized = TRUE;
	this->m_bIsWinNT = IsWinNT;
	if(m_bIsWinNT)
	{
		SYSTEM_TIMEINFORMATION sysTimeInfo = { 0 };
		SYSTEM_PERFORMANCEINFORMATION sysPerfInfo = { 0 };
		NTSTATUS status;
		
		m_hNtDllLib = LoadLibrary(_T("ntdll.dll"));
		if (m_hNtDllLib == NULL)
		{
			m_bIsInitialized = FALSE;
		}
		else
		{
			m_lpfNtQuerySystemInformation = 
			(NTSTATUS(WINAPI *)(SYSTEM_INFORMATION,PVOID,ULONG,PULONG))
			GetProcAddress(m_hNtDllLib, "NtQuerySystemInformation");
			if(m_lpfNtQuerySystemInformation == NULL)
			{
				FreeLibrary(m_hNtDllLib);
				m_hNtDllLib = NULL;
			}
			else
			{
				// get new system time
				status = m_lpfNtQuerySystemInformation(SystemTimeOfDayInfo, &sysTimeInfo, sizeof(sysTimeInfo), 0);
				if (status != NO_ERROR) m_bIsInitialized = FALSE;
		 
				// get new CPU's idle time
				status = m_lpfNtQuerySystemInformation(SystemPerformanceInfo,&sysPerfInfo, sizeof(sysPerfInfo), 0);
				if (status != NO_ERROR) m_bIsInitialized = FALSE;
			}
		}
	    // store new CPU's idle and system time
		m_liOldIdleTime = sysPerfInfo.liIdleTime;
		m_liOldSystemTime = sysTimeInfo.liKeSystemTime;
	}
	else
	{
		if (RegOpenKeyEx(HKEY_DYN_DATA, _T("PerfStats\\StartStat"), 0, KEY_ALL_ACCESS, &m_hKey) != ERROR_SUCCESS) 
		{
			m_bIsInitialized = FALSE;
		}

		m_dwDataSize = sizeof(DWORD);
		RegQueryValueEx(m_hKey, _T("KERNEL\\CPUUsage"), NULL, &m_dwType, (LPBYTE)&m_dwCpuUsage, &m_dwDataSize);
		RegCloseKey(m_hKey);

		// geting current counter's value
		if (RegOpenKeyEx( HKEY_DYN_DATA, _T("PerfStats\\StatData"), 0,KEY_READ, &m_hKey) != ERROR_SUCCESS)
		{
			m_bIsInitialized = FALSE;
		}
	}
}

Win32CpuUsage::~Win32CpuUsage()
{
	if(m_bIsInitialized)
	{
		if(m_bIsWinNT)
		{
			if(m_hNtDllLib != NULL)
			{
				FreeLibrary(m_hNtDllLib);
				m_hNtDllLib = NULL;
			}
		}
		else
		{
			// stoping the counter
			RegCloseKey(m_hKey);
			if (RegOpenKeyEx(HKEY_DYN_DATA, _T("PerfStats\\StopStat"), 0,KEY_ALL_ACCESS, &m_hKey ) == ERROR_SUCCESS)
			{
				m_dwDataSize = sizeof(DWORD);
				RegQueryValueEx(m_hKey, _T("KERNEL\\CPUUsage"), NULL,&m_dwType, (LPBYTE)&m_dwCpuUsage, &m_dwDataSize);
				RegCloseKey(m_hKey);
			}
		}
	}
	
}

//Cpu频率查询
INT Win32CpuUsage::Query()
{
	if (!m_bIsInitialized)
	{
		return -1;
	}
	if (m_bIsWinNT)
	{
		SYSTEM_BASICINFORMATION sysBaseInfo;
		SYSTEM_PERFORMANCEINFORMATION sysPerfInfo;
		SYSTEM_TIMEINFORMATION sysTimeInfo;
		DOUBLE dbIdleTime;
		DOUBLE dbSystemTime;
		NTSTATUS status;

		// get number of processors in the system
		status = m_lpfNtQuerySystemInformation(SystemBasicInfo, &sysBaseInfo, sizeof(sysBaseInfo), NULL);
		if (status != NO_ERROR) return -1;
			
		// get new system time
		status = m_lpfNtQuerySystemInformation(SystemTimeOfDayInfo, &sysTimeInfo, sizeof(sysTimeInfo), NULL);
		if (status != NO_ERROR) return -1;
		
		// get new CPU's idle time
		status = m_lpfNtQuerySystemInformation(SystemPerformanceInfo, &sysPerfInfo, sizeof(sysPerfInfo), NULL);
		if (status != NO_ERROR) return -1;

		// CurrentValue = NewValue - OldValue
		dbIdleTime = LI2Double(sysPerfInfo.liIdleTime) - LI2Double(m_liOldIdleTime);
		dbSystemTime = LI2Double(sysTimeInfo.liKeSystemTime) - LI2Double(m_liOldSystemTime);

		// CurrentCpuIdle = IdleTime / SystemTime
		if (dbSystemTime != 0) dbIdleTime = dbIdleTime / dbSystemTime;

		// CurrentCpuUsage% = 100 - (CurrentCpuIdle * 100) / NumberOfProcessors
		dbIdleTime = 100 - (dbIdleTime * 100) / sysBaseInfo.bKeNumberProcessors + 0.5;
			
		// store new CPU's idle and system time
		m_liOldIdleTime = sysPerfInfo.liIdleTime;
		m_liOldSystemTime = sysTimeInfo.liKeSystemTime;

		return (INT)dbIdleTime;
	}
	else
	{
		m_dwDataSize = sizeof(DWORD);
		RegQueryValueEx(m_hKey, _T("KERNEL\\CPUUsage"), NULL, &m_dwType, (LPBYTE)&m_dwCpuUsage, &m_dwDataSize);
		return (INT)m_dwCpuUsage;
	}
}

#pragma endregion

#pragma region Cpu核心

//
// NT操作系统下获得关机特权
//
BOOL Win32CpuCore::GetShudownPrivilege()
{
	HANDLE hProcessHandle;
	HANDLE hTokenHandle;
	LUID tmpLuid;
	TOKEN_PRIVILEGES tkp;

	hProcessHandle = GetCurrentProcess();
	if (!OpenProcessToken(hProcessHandle, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hTokenHandle))
	{
		return FALSE;
	}

	// Get the LUID for shutdown privilege.
	if (!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tmpLuid))
	{
		return FALSE;
	}

	tkp.PrivilegeCount = 1; // One privilege to set
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	tkp.Privileges[0].Luid = tmpLuid;
	// Enable the shutdown privilege in the access token of this process.
	if (!AdjustTokenPrivileges(hTokenHandle, FALSE, &tkp, 0, NULL, NULL))
	{
		return FALSE;
	}

	return TRUE;
}

//
// 关机动作
//
BOOL Win32CpuCore::Action(ExitWindowOption option, ExitWindowMode mode)
{
	UINT flag;
	BOOL IsNT = IsWinNT;

	switch(option)
	{
	case Logoff:			//中止进程，然后注销
		flag = EWX_LOGOFF;
		break;
	case Reboot:			//终止所有运行的进程并重启计算机
		flag = EWX_REBOOT;
		break;
	case Shutdown:			//终止所有进程并安全关闭计算机，注意：用户需要手动关闭电源
		flag = EWX_SHUTDOWN;
		break;
	case Poweroff:			//关闭计算机并切断电源
		flag =(IsNT)?EWX_POWEROFF:EWX_SHUTDOWN;
		break;
	case Standby:			//等待
		return SetSystemPowerState(TRUE,((mode == Immediately)?TRUE:FALSE));
	case Hibernate:			//睡眠
		return SetSystemPowerState(FALSE,((mode == Immediately)?TRUE:FALSE));
	case Lock:				//锁定
		return LockWorkStation();
	case MonitorLowPower:	//显示器处于低能耗状态
		return (SendMessage(GetDesktopWindow(), WM_SYSCOMMAND, SC_MONITORPOWER, 1) == 0);
	case MonitorShutOff:	//关闭显示器
		return (SendMessage(GetDesktopWindow(), WM_SYSCOMMAND, SC_MONITORPOWER, 2) == 0);
	default:
		return FALSE;
	}
	if(mode == Immediately)
	{
		flag |= EWX_FORCE;
	}
	if(!IsNT)
	{

		//InitiateSystemShutdownEx(NULL, NULL, 0, TRUE, FALSE, 0);
		return ExitWindowsEx(flag, 0);
	}
	else
	{
		if (GetShudownPrivilege())
		{
			return ExitWindowsEx(flag, 0);
		}
		else
		{
			return FALSE;
		}
	}
}

#pragma endregion

#pragma region Cpu接口

Win32Cpu::Win32Cpu():Win32CpuUsage()
{
	hDC = 0;
	Rect.left = 0;
	Rect.top = 0;
	Rect.right = 0;
	Rect.bottom = 0;
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
}

Win32Cpu::~Win32Cpu()
{
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
	SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
}

//获取系统当前时间
LPCTSTR Win32Cpu::SysCurrentTime()
{
	static TCHAR times[9] = {0};
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
	format(times, _T("%.2d:%.2d:%.2d"),systemTime.wHour,systemTime.wMinute,systemTime.wSecond);
	return times;
}

//获取系统运行时间
LPCTSTR Win32Cpu::SysRunningTime()
{
		static TCHAR times[9] = {0};
		INT minutes;
		INT seconds;
		INT hour;
		INT minute;
		INT second;

		seconds = GetTickCount() / 1000;
		minutes = seconds / 60;

		hour = minutes / 60;
		minute = minutes % 60;
		second = seconds % 60;

		format(times, _T("%.2d:%.2d:%.2d"),hour,minute,second);
		return times;
}

VOID Win32Cpu::Draw()
{
	TCHAR systemTime[9] = {0};
	TCHAR runningTime[9] = {0};
	TCHAR percentUsage[16] = {0};
	TCHAR *psystemTime = systemTime;
	TCHAR *prunningTime = runningTime;
	TCHAR *ppercentUsage = percentUsage;
	INT usage;
	INT xOffset;
	INT yOffset;
	COLORREF clrColor;

	usage = this->Query();
	format(systemTime, _T("%s"), this->SysCurrentTime());
	format(runningTime, _T("%s"), this->SysRunningTime());
	format(percentUsage, _T("%.2d%s"), usage, _T("%"));

	Win32GDI::DrawArea(hDC, Rect, 0x00000000);
	
	//系统当前时间
	clrColor = 0x0000FF00;
	xOffset = Rect.left;
	yOffset = Rect.top;
	while(*psystemTime != '\0')
	{
		if(*psystemTime >= 48 && *psystemTime <= 57)
		{
			Win32GDI::DrawDigit(hDC,xOffset + 1, yOffset + 1, *psystemTime - 48, 3, clrColor);
		}
		else
		{
			if(*prunningTime != 32)
			{
				Win32GDI::DrawCharacter(hDC, xOffset + 2, yOffset + 3, psystemTime, clrColor);
			}
		}
		xOffset += 5;
		psystemTime++;
	}

	//系统运行时间
	clrColor = 0x0000FFFF;
	xOffset += 8;
	while(*prunningTime != '\0')
	{
		if(*prunningTime >= 48 && *prunningTime <= 57)
		{
			Win32GDI::DrawDigit(hDC,xOffset + 1, yOffset + 1, *prunningTime - 48, 3, clrColor);
		}
		else
		{
			if(*prunningTime != 32)
			{
				Win32GDI::DrawCharacter(hDC, xOffset + 2, yOffset + 2, prunningTime, clrColor);
			}
		}
		xOffset += 5;
		prunningTime++;
	}
	
	//Cpu使用率
	xOffset += 8;
	while(*ppercentUsage != '\0')
	{
		if (usage >=0 && usage <= 69)
		{
			clrColor = 0x0000FF00;
		}
		else if (usage >= 70 && usage <= 89)
		{
			clrColor = 0x0000FFFF;
		}
		else
		{
			clrColor = 0x000000FF;
		}
		if(*ppercentUsage >= 48 && *ppercentUsage <= 57)
		{
			Win32GDI::DrawDigit(hDC, xOffset + 1, yOffset + 1, *ppercentUsage - 48, 3, clrColor);
		}
		else
		{
			if(*ppercentUsage != 32)
			{
				Win32GDI::DrawCharacter(hDC, xOffset + 2, yOffset + 2, ppercentUsage, clrColor);
			}
		}
		xOffset += 5;
		ppercentUsage++;
	}

	//Cpu图表
	clrColor = 0x00004000;
	xOffset = Rect.left + 1;
	yOffset += 11;
	while(xOffset <= Rect.right - 1)
	{
		Win32GDI::DrawArea(hDC, xOffset, yOffset, 2, Rect.bottom - yOffset - 2, clrColor);
		xOffset += 3;
	}
	clrColor = 0x0000FF00;
	xOffset = Rect.left + 1;
	INT nLen = (INT)(Rect.left + (((DOUBLE)usage / 100) * (Rect.right - Rect.left - 1L)));
	while(xOffset <= nLen)
	{
		Win32GDI::DrawArea(hDC, xOffset, yOffset, 2, Rect.bottom - yOffset - 2, clrColor);
		xOffset += 3;
	}
}

#pragma endregion
