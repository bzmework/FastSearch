
/*
	模块：内存分配
	作者：denglf
	日期：2019-04-24
	说明：用于字典的虚拟内存管理
	注意：
	实验证明虚拟内存适合一次分配MB以上较大的内存块且速度快，
	malloc比较适合分配较小的内存块且速度快（比HeapAlloc和VirtualAlloc都快）
*/

#ifndef __VMEMALLOC_H_
#define __VMEMALLOC_H_

#include <windows.h>

class VMemAlloc
{
public:

	VMemAlloc()
	{
		//
	}

	virtual ~VMemAlloc()
	{
		//
	}

	//分配内存
	LPVOID Alloc(SIZE_T size)
	{

		/*
		函数：
		LPVOID VirtualAlloc(
		  LPVOID lpAddress,
		  SIZE_T dwSize,
		  DWORD  flAllocationType,
		  DWORD  flProtect
		);

		功能：
		在调用进程的虚拟地址空间中保留、提交或更改页面区域的状态。
		这个函数分配的内存被自动初始化为零。
			
		lpAddress：
		内存地址，用来告诉系统我们想要预订地址空间中的哪一块，由于系统会记录所有闲置的区域，
		因此只需要将该参数设置成NULL即可，这等于告诉系统自动找一块。

		dwSize：
		预订的区域大小，以字节为单位。
			
		flAllocationType：
		用来告诉系统只是预订区域还是要调拨物理储存器，虚拟内存只有当调拨物理储存器以后才能使用。
		如果我们打算预订一块区域很长一段时间，并希望系统从尽可能高的内存地址来预订区域，
		以防止在进程地址空间的中间预订区域，从而避免引起内存碎片。为了达到此目的应该以如下的
		方式调用VirtualAlloc：
				
			VirtualAlloc(NULL, dwSize, MEM_RESERVE | MEM_TOP_DOWN, PAGE_READWRITE);
			
		flProtect：
		对区域所做的保护属性。区域的保护属性仅用来保护地址区域，它对调拨给该区域的物理储存器不起任何作用。

		在预订了区域后，我们还需要给区域调拨物理储存器，这样才能访问其中的内存地址。
		系统会从页面交换文件中调拨储存器给区域，为了调拨物理储存器，我们必须再次调用VirtualAlloc，
		但这次要传MEM_COMMIT给flAllocationType参数，如下：

			VirtualAlloc(NULL, dwSize, MEM_COMMIT | MEM_TOP_DOWN, PAGE_READWRITE);

		当然，如果想在预订区域的同时调拨物理储存器，这样即可：

			VirtualAlloc(NULL, dwSize, MEM_RESERVE | MEM_COMMIT | MEM_TOP_DOWN, PAGE_READWRITE);

		*/

		LPVOID pAddress = NULL;
		
		//如果函数成功，返回值是分配的页面区域的基本地址。
		//如果函数失败，返回值为NULL。要获取扩展的错误信息，请调用GetLastError。
		pAddress = VirtualAlloc(NULL, size, MEM_RESERVE | MEM_COMMIT | MEM_TOP_DOWN, PAGE_READWRITE);
		
		//返回
		return pAddress;

	}

	//释放内存
	BOOL Free(LPVOID pAddress)
	{
		/*
		函数：
		BOOL VirtualFree(
		  LPVOID lpAddress,
		  SIZE_T dwSize,
		  DWORD  dwFreeType
		);

		功能：
		释放调拨给区域的物理储存器。

		lpAddress：
		VirtualAlloc返回的地址。

		dwSize：
		要释放的物理存储器大小。给定一个地址，系统知道位于该地址处的区域的大小，因此传0给dwSize即可。

		dwFreeType：
		撤销类型，如果想要撤销调拨给区域的一部分物理储存器，但又不想释放整个区域，可以这样：

			VirtualFree(lpAddress, dwSize, MEM_DECOMMIT);

		如果dwSize为0，且lpAddress是区域的基地址，那么VirtualFree会撤销调拨给该区域的所有页面，如下：
			
			VirtualFree(lpAddress, 0, MEM_RELEASE);

		一旦系统撤销了调拨给页面的物理储存器，所释放的物理储存器就可以用于系统中的其它进程，
		试图访问已撤销调拨的内存地址将引发访问违规。

		*/
		
		BOOL bOk = FALSE;
		if (pAddress != NULL)
		{
			//如果函数成功，返回值为非零。
			//如果函数失败，返回值为0(0)。要获取扩展的错误信息，请调用GetLastError。
			bOk = VirtualFree(pAddress, 0, MEM_RELEASE);
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

#endif //__VMEMALLOC_H_

