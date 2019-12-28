
/*

	模块：内存分配
	作者：denglf
	日期：2019-04-24
	说明：用于字典的堆内存管理。测试发现大数据时堆上分配、读写速度都比较慢。
*/

#ifndef __HMEMALLOC_H_
#define __HMEMALLOC_H_

#include <windows.h>

class HMemAlloc
{
private:
	HANDLE hHeap; //堆句柄

public:

	HMemAlloc()
	{
		//
		hHeap = NULL;
	}

	virtual ~HMemAlloc()
	{
		//销毁创建的堆
		if (hHeap != NULL)
		{
			HeapDestroy(hHeap);
		}
	}

	//初始化 
	BOOL Initialize(SIZE_T size)
	{
		/*
		函数：
		HANDLE HeapCreate(
		  DWORD  flOptions,
		  SIZE_T dwInitialSize,
		  SIZE_T dwMaximumSize
		);

		功能：
		创建可由调用进程使用的私有堆对象。

		dwInitialSize：
		一开始要调拨给堆的字节数。

		dwMaximumSize：
		堆上限，为0表示从堆中分配内存会使堆自动增长，直到用尽所有物理内存位置。
		*/

		//如果函数成功，返回值是新创建堆的句柄。
		//如果函数失败，返回值为NULL。要获取扩展的错误信息，请调用GetLastError。
		hHeap = HeapCreate(0, size, 0);
	
		/*
		if (hHeap != NULL)
		{
			//将堆调整成低碎片堆
			//如果函数成功，返回值为非零。
			//如果函数失败，返回值为0。要获取扩展的错误信息，请调用GetLastError。
			//HeapCompatibilityInformation=0
			ULONG ic = 2;
			BOOL bOk = FALSE;
			bOk = HeapSetInformation(hHeap, HeapCompatibilityInformation, &ic, sizeof(ic));
			if (bOk)
			{
				//调整成低碎片堆成功
			}
		}
		*/

		return (hHeap != NULL);

	}

	//分配内存
	LPVOID Alloc(SIZE_T size)
	{

		/*
		函数：
		DECLSPEC_ALLOCATOR LPVOID HeapAlloc(
		  HANDLE hHeap,
		  DWORD  dwFlags,
		  SIZE_T dwBytes
		);

		功能：
		从堆中分配内存块。分配的内存不可移动。

		*/

		LPVOID pAddress = NULL;

		//如果函数成功，返回值是指向已分配内存块的指针。
		//如果函数失败，并且没有指定dwFlags为HEAP_GENERATE_EXCEPTION(0x00000004)，则返回值为NULL。
		//要获取扩展的错误信息，请调用GetExceptionCode/GetLastError。
		if (hHeap != NULL)
		{
			pAddress = HeapAlloc(hHeap, 0, size);
		}

		//返回
		return pAddress;

	}

	//释放内存
	BOOL Free(LPVOID pAddress)
	{
		/*
		函数：
		BOOL HeapFree(
		  HANDLE                 hHeap,
		  DWORD                  dwFlags,
		  _Frees_ptr_opt_ LPVOID lpMem
		);

		功能：
		释放由HeapAlloc或HeapReAlloc函数从堆中分配的内存块。

		hHeap：
		要释放内存块的堆的句柄。这个句柄由HeapCreate或GetProcessHeap函数返回。

		lpMem：
		指向要释放的内存块的指针。这个指针由HeapAlloc或HeapReAlloc函数返回。如果该指针为空，则行为未知。

		*/

		BOOL bOk = FALSE;
		if (hHeap != NULL && pAddress != NULL)
		{
			//如果函数成功，返回值为非零。
			//如果函数失败，返回值为零。应用程序可以调用GetLastError获取扩展的错误信息。
			bOk = HeapFree(hHeap, 0, pAddress);
		}
		return bOk;

	}

private:

	void OutputErr()
	{
		LPTSTR lpMsgBuf;
		DWORD nErrno = GetLastError();
		if (nErrno != 0)
		{
			FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			nErrno,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0,
			NULL);

			MessageBox(NULL, (LPCTSTR)lpMsgBuf, TEXT("Error"), MB_OK | MB_ICONINFORMATION);

			LocalFree(lpMsgBuf);
		}
	}

};

#endif //__HMEMALLOC_H_

