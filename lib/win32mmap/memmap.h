/*
Module : Memmap.h
Purpose: Interface for an C++ class to wrap memory mapped files
History: PJN / 30-07-1997 1. Initial Public release.
         PJN / 31-03-1998 1. Class now avoids trying to lock the mutex if only read access is required
                          2. User now has the option of specifying whether a file should be mapped with 
                             A Null terminator at the end. Can prove helpful when you want to use some
                             of the "C" runtime functions on the pointer returned.
         PJN / 20-04-1998 1. Now uses GetFileSize SDK call instead of GetFileInformationByHandle as a
                             more "reliable" way to determine file length.
                          2. Included TRACE statements to call GetLastError in all places where 
                             SDK functions fail
         PJN / 29-05-1998 1. Mapping a file now has the option of making it named or not.
         PJN / 22-10-1998 1. Fixed a bug in a number of calls to CreateMappingName when the classes were 
                             being used to share memory.
                          2. Tidy up of the demo app including:
                             a) Made the amount of text being shared a constant of MAX_EDIT_TEXT instead
                             of hardcoding it to 20 everywhere in the sample.
                             b) Changed where the timer is being created to OnInitDialog
                             c) Tidied up the initialisation sequence in OnInitDialog
                             d) Now using _tcscpy instead of _tcsncpy to ensure array is null terminated
                             e) Fixed resource.h which was causing the resources to fail to compile
                             f) Removed unnecessary symbols from resource.h
                             g) Optimized the way the OnTimer code works to only update the text when it
                             has changed in the MMF. This means that you can type continuously into the
                             edit control.
                          3. New documentation in the form of a HTML file.
                          4. Sample now ships as standard with VC 5 workspace files
         PJN / 30-03-1999 1. Code is now unicode compliant
                          2. Code now supports growable MMF's
                          3. Addition of accessor functions for file handle and file mapping handle
         PJN / 21-04-1999 1. Works around a Window bug where you try to memory map a zero length file on
                             Windows 95 or 98.
         PJN / 24-04-2000 1. Fixed a simple typo problem in a TRACE statement when compiled for UNICODE
         PJN / 07-03-2001 1. Updated copyright information   
                          2. Fixed problem where mutex was not being locked when read only access to 
                          the memory mapped file was desired. Access to the MMF should be synchronised
                          irrespective of the access mode.
         PJN / 02-04-2001 1. Now supports passing in a security descriptor to MapFile and MapMemory
                          2. Now supports opening a specified portion of a file, rather than always mapping
                          all of the file
         PJN / 05-10-2003 1. Updated copyright details.
                          2. Fixed a small typo in the description of the FSCTL_SET_SPARSE IOCTL. Thanks to 
                          amores perros for reporting this.
                          3. Fixed a minor tab indentation problem at the start of the MapFile method. Again
                          thanks to amores perros for reporting this.
                          4. Removed the unnecessary AssertValid function. Again thanks to amores perros for 
                          reporting this.
         PJN / 17-11-2003 1. Fixed a memory leak in UnMap as reported by Bart Duijndam using 
                          "Memory Validator". The memory leak may in fact not be real, but the code change 
                          avoids the reported problem.
         PJN / 06-06-2004 1. Fixed an issue in MapHandle where the wrong value was sent to CreateFileMapping. 
                          This issue only occurs when you are not mapping the whole of a file, but instead
                          decide to perform the mapping a chunk at a time. Thanks to Nicolas Stohler for
                          reporting this problem.
                          2. Removed the AppendNull option as it is incompatible with general use of memory 
                          mapped files.
                          3. Reviewed all the TRACE statements throughout the class
                          4. Added ASSERT validation at the top of functions which modify member variables
                          5. Failing to create mutex in MapHandle and MapExistingMemory not fails the function
         PJN / 23-12-2004 1. Removed unnecessary include of winioctl.h header file.
                          2. Fixed a problem with the declaration of the FSCTL_SET_SPARSE macro which is used
                          in the support of growable MMF. With the VC6 WinIOCTL.h, the macro generates the 
                          value 0x000980C4, but the correct value for the DeviceIoControl function to enable 
                          the sparse file option is 0x000900C4. Thanks to a posting on CodeProject for pointing
                          out this problem.
                          3. Optimized CMemMapFile::CreateMappingName by now using CString::Replace.
                          4. Addition of a bInheritHandle parameter to MapExistingMemory.
                          5. Fixed a bug in the handling of calls to GetFileSize()
                          6. Removed unnecessary check to verify that mapping size is greater than 4GB since
                          CreateFileMapping will do this for us.
                          7. Updated sample app to use a sample "input.txt" file for demonstration purposes
                          instead of using "c:\config.sys"
                          8. Code now uses unsigned __int64 for specifying indexes and lengths for memory 
                          mapping. This should allow the code to be easily used to map > 4GB on 64 bit 
                          versions of Windows.
                          9. Tidied up some of the ASSERT code at the top of each key function in the class.
         PJN / 30-04-2005 1. Removed derivation from CObject MFC class. In fact the class can now operate entirely
                          independently of MFC.
                          2. Destructor is now virtual.
                          3. Fixed a bug where the mutex name which is used to synchronize access to the MMF's data
                          could by default have the same name for multiple MMF's.
                          4. CreateMappingName and CreateMutexName methods are now virtual
         PJN / 02-05-2006 1. Updated the copyright details in the modules.
                          2. Updated the documentation to use the same style as the web site.
                          3. Addition of a CMEMMAPFILE_EXT_CLASS macro to allow the class to be easily incorporated
                          into an extension dll.
                          4. The sample app now uses filenames without "(" or ")" characters in their filenames. 
                          Thanks to Andrew MacGinitie for reporting this issue.
                          5. Fixed a bug in the sample app where it reports the wrong filename when doing the lowercase
                          conversion. Again thanks to Andrew MacGinitie for reporting this issue.
                          6. Fixed an issue in the download where the sample file called "input.txt" is now provided
                          out of the box. Again thanks to Andrew MacGinitie for reporting this issue.
                          7. Fixed an issue in the sample app when the code is compiled with /Wp64
         PJN / 07-07-2006 1. Code now uses newer C++ style casts instead of C style casts.
                          2. The code now requires the Platform SDK if compiled using VC 6.
                          3. Updated code to compile cleanly using VC 2005.
         PJN / 16-08-2008 1. Updated copyright details
                          2. Code now compiles cleanly using Code Analysis (/analyze)
                          3. Updated code to compile correctly using _ATL_CSTRING_EXPLICIT_CONSTRUCTORS define
                          4. The code now only supports VC 2005 or later. 
                          5. Removed VC 6 style AppWizard comments from the code.
                          6. Updated the code to correctly handle "Global\" and "Local\" prefixes which can be included
                          with any Win32 Named object. Thanks to Gert Rijs for reporting this bug. To achieve this the
                          parameters to the MapFile method has been reworked. This function now takes a pszMappingName
                          which allows client code to explicitly specify the mapping name
                          7. The mutex name use to serialize access to the contents of the memory mapped file is now
                          explicitly provided as a external parameter to the class.
                          8. MapExistingMemory now has a LPSECURITY_ATTRIBUTES parameter
         PJN / 06-07-2009 1. Updated copyright details
                          2. Updated the sample apps project settings to more modern default values. 
                          3. All the failure paths in the methods now preserve the Win32 last error value
                          4. Removed unnecessary code to get the length of the file in MapFile. Also simplified the logic
                          in this method.
                          5. All parameters which specify a mapping size now use a SIZE_T parameter which is consistent
                          with their equivalent API values.
                          6. All calls to MapViewOfFile are now checked for failure
         PJN / 20-11-2011 1. Updated copyright details
                          2. Updated code to clean compile on VC 2010
                          3. Replaced ASSERT calls with ATLASSERT
                          4. Updated MapMemory and MapFile methods to include a new BOOL bNoCache parameter. Thanks to 
                          HaeRim Lee for providing this nice update
                          5. Removed m_bOpen member variable and all code which uses it. This can avoid thread safety 
                          issues when a class instance of CMemMapFile is shared across threads. Thanks to HaeRim Lee 
                          for providing this nice update
                          6. Reworked the code in CMemMapFile::UnMap to call ReleaseMutex directly instead of calling
                          Close. This method now shares the same consistent ordering of object creation / destruction 
                          as the main Map... methods. This also fixes an issue where the mutex previously got released
                          twice. Thanks to HaeRim Lee for providing this nice update
                          7. The whole class is now implemented in Memmap.h and the Memmap.cpp module is now defunct 
                          and redundant. You now just need to #include the Memmap.h in client code to use the class.
         PJN / 25-11-2011 1. Updated class to be completely MFC independent. You can now use CMemMapFile in an ATL only
                          project. Thanks to HaeRim Lee for prompting this update.
         PJN / 15-03-2012 1. Updated copyright details
                          2. Made the class completely thread-safe meaning that you can share instances of CMemMapFile 
                          across threads without worrying about corruption of its member variables. Thanks to HaeRim 
                          Lee for prompting this update. To achieve this I used a nested class called CMemMapCriticalSection
                          which is derived from the ATL::CCriticalSection class. This CMemMapCriticalSection class provides 
                          for critical sections with spin locks and normal critical sections as well as stack based release
                          semantics for critical sections through the use of the ATL::CComCritSecLock class. It would be 
                          nice if the built in ATL CriticalSection class supported this but this is still not present as of
                          VC 2010 SP1.
         PJN / 16-03-2012 1. Thread protected the GetFileHandle and GetFileMappingHandle methods.
         PJN / 26-01-2014 1. Updated copyright details.
                          2. Updated the code to clean compile on VC 2013
                          3. Updated the code to clean compile using /analyze
                          4. Reverted the code changes which made the class completely thread-safe. The issue was that you 
                          ended up with a class which had two different synchronisation primitives running in parallel 
                          throughout the class codebase. This design pattern is always bad and in this case could lead to 
                          thread deadlock situations in some situations. Similar to the MFC thread synchronisation 
                          primitives design patterns, an instance of the CMemMapFile class should only be used from the 
                          one thread. If you want to use the same logical memory mapped file from multiple threads in  
                          your application, then create a second instance and simple call the MapFile, MapMemory or 
                          MapExistingMemory methods to get access to the mapping. See the "MoniterSM" worker thread in 
                          testmemmap.cpp in the sample app on how to do this correctly. Thanks to "jianhongsong" for 
                          reporting this issue.
         PJN / 18-12-2015 1. Updated copyright details
                          2. Added SAL annotations to all the code.
                          3. Updated the code to clean compile on VC 2015
         PJN / 03-06-2018 1. Updated copyright details
                          2. Fixed a number of C++ core guidelines compiler warnings. These changes mean that the code
                          will now only compile on VC 2017 or later.
                          3. Replaced NULL throughout the code with nullptr
                          4. Added support for MapViewOfFileEx API
                          5. Added support for UnmapViewOfFileEx API
                          6. Added support for OpenFileMappingFromApp API
                          7. Added support for MapViewOfFileFromApp API
         PJN / 08-06-2018 1. Removed dependency on std::invoke from the code
         PJN / 01-09-2018 1. Fixed a number of compiler warnings when using VS 2017 15.8.2
         PJN / 21-04-2019 1. Updated copyright details
                          2. Updated the code to clean compile on VC 2019

Copyright (c) 1997 - 2019 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/


//////////////// Macros / Defines /////////////////////////////////////////////

#pragma once

#ifndef __MEMMAP_H__
#define __MEMMAP_H__

#ifndef CMEMMAPFILE_EXT_CLASS
#define CMEMMAPFILE_EXT_CLASS
#endif //#ifndef CMEMMAPFILE_EXT_CLASS


//////////////// Includes /////////////////////////////////////////////////////
#include <windows.h>
#include "../win32exception/exception.h"

//#ifndef __ATLBASE_H__
//#pragma message("To avoid this message, please put atlbase.h in your pre compiled header (normally stdafx.h)")
//#include <atlbase.h>
//#endif //#ifndef __ATLBASE_H__


//////////////// Classes //////////////////////////////////////////////////////

class CMEMMAPFILE_EXT_CLASS CMemMapFile
{
public:
	//Constructors / Destructors
	CMemMapFile() noexcept
	{
		m_hFile = INVALID_HANDLE_VALUE;
		m_hMapping = nullptr;
		m_lpData = nullptr;
		m_hMutex = nullptr;
	}

	CMemMapFile(_In_ const CMemMapFile&) = delete;
	CMemMapFile(_In_ CMemMapFile&&) = delete;

	virtual ~CMemMapFile()
	{
		UnMap();
	}

	//Methods
	CMemMapFile& operator=(_In_ const CMemMapFile&) = delete;
	CMemMapFile& operator=(_In_ CMemMapFile&&) = delete;

	BOOL MapFile(
		_In_ LPCTSTR pszFilename, 
		_In_ BOOL bReadOnly = FALSE, 
		_In_ DWORD dwShareMode = 0, 
		_In_opt_ LPCTSTR pszMappingName = nullptr, 
		_In_opt_ LPCTSTR pszMutexName = nullptr, 
		_In_ BOOL bGrowable = FALSE,
        _In_ const unsigned __int64& dwStartOffset = 0, 
		_In_ const SIZE_T& nNumberOfBytesToMap = 0, 
		_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes = nullptr, 
		_In_ BOOL bNoCache = FALSE, 
		_In_opt_ LPVOID pBaseAddress = nullptr) noexcept
	{
		return MapFileHelper(pszFilename, bReadOnly, dwShareMode, pszMappingName, pszMutexName, bGrowable, dwStartOffset, nNumberOfBytesToMap, lpSecurityAttributes, bNoCache, pBaseAddress, &CMemMapFile::MapViewAndCreateMutex, nullptr);
	}

	__if_exists(MapViewOfFileFromApp)
	{
		BOOL MapFileFromApp(
			_In_ LPCTSTR pszFilename, 
			_In_ BOOL bReadOnly = FALSE, 
			_In_ DWORD dwShareMode = 0, 
			_In_opt_ LPCTSTR pszMappingName = nullptr, 
			_In_opt_ LPCTSTR pszMutexName = nullptr, 
			_In_ BOOL bGrowable = FALSE,
			_In_ const unsigned __int64& dwStartOffset = 0, 
			_In_ const SIZE_T& nNumberOfBytesToMap = 0, 
			_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes = nullptr, 
			_In_ BOOL bNoCache = FALSE) noexcept
		{
			return MapFileHelper(pszFilename, bReadOnly, dwShareMode, pszMappingName, pszMutexName, bGrowable, dwStartOffset, nNumberOfBytesToMap, lpSecurityAttributes, bNoCache, nullptr, nullptr, &CMemMapFile::MapViewAndCreateMutexFromApp);
		}
	}

	BOOL MapMemory(
		_In_ LPCTSTR pszMappingName, 
		_In_opt_ LPCTSTR pszMutexName, 
		_In_ const SIZE_T& nBytes, 
		_In_ BOOL bReadOnly = FALSE,
		_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes = nullptr, 
		_In_ BOOL bNoCache = FALSE, 
		_In_ const unsigned __int64& dwStartOffset = 0, 
		_In_opt_ LPVOID pBaseAddress = nullptr) noexcept
	{
		return MapMemoryHelper(pszMappingName, pszMutexName, nBytes, bReadOnly, lpSecurityAttributes, bNoCache, dwStartOffset, pBaseAddress, &CMemMapFile::MapViewAndCreateMutex, nullptr);
	}

	__if_exists(MapViewOfFileFromApp)
	{
		BOOL MapMemoryFromApp(
			_In_ LPCTSTR pszMappingName, 
			_In_opt_ LPCTSTR pszMutexName, 
			_In_ const SIZE_T& nBytes, 
			_In_ BOOL bReadOnly = FALSE,
			_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes = nullptr, 
			_In_ BOOL bNoCache = FALSE, 
			_In_ const unsigned __int64& dwStartOffset = 0) noexcept
		{
			return MapMemoryHelper(pszMappingName, pszMutexName, nBytes, bReadOnly, lpSecurityAttributes, bNoCache, dwStartOffset, nullptr, nullptr, &CMemMapFile::MapViewAndCreateMutexFromApp);
		}
	}

	BOOL MapExistingMemory(
		_In_ LPCTSTR pszMappingName, 
		_In_opt_ LPCTSTR pszMutexName, 
		_In_ const SIZE_T& nBytes, 
		_In_ BOOL bReadOnly = FALSE,
		_In_ BOOL bInheritHandle = FALSE, 
		_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes = nullptr, 
		_In_ const unsigned __int64& dwStartOffset = 0, 
		_In_opt_ LPVOID pBaseAddress = nullptr) noexcept
	{
		return MapExistingMemoryHelper(pszMappingName, pszMutexName, nBytes, bReadOnly, bInheritHandle, lpSecurityAttributes, dwStartOffset, pBaseAddress, &CMemMapFile::MapViewAndCreateMutex, nullptr);
	}

	__if_exists(MapViewOfFileFromApp)
	{
		BOOL MapExistingMemoryFromApp(
			_In_ LPCTSTR pszMappingName, 
			_In_opt_ LPCTSTR pszMutexName, 
			_In_ const SIZE_T& nBytes, 
			_In_ BOOL bReadOnly = FALSE,
			_In_ BOOL bInheritHandle = FALSE, 
			_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes = nullptr, 
			_In_ const unsigned __int64& dwStartOffset = 0) noexcept
		{
			return MapExistingMemoryHelper(pszMappingName, pszMutexName, nBytes, bReadOnly, bInheritHandle, lpSecurityAttributes, dwStartOffset, nullptr, nullptr, &CMemMapFile::MapViewAndCreateMutexFromApp);
		}
	}

	void UnMap() noexcept
	{
		if (m_hMutex != nullptr)
		{
			CloseHandle(m_hMutex);
			m_hMutex = nullptr;
		}

		//unmap the view
		if (m_lpData != nullptr)
		{
			FlushViewOfFile(m_lpData, 0);
			UnmapViewOfFile(m_lpData);
			m_lpData = nullptr;
		}

		//remove the file mapping
		if (m_hMapping != nullptr)
		{
			CloseHandle(m_hMapping);
			m_hMapping = nullptr;
		}

		//close the file system file if its open
		if (m_hFile != INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hFile);
			m_hFile = INVALID_HANDLE_VALUE;
		}
	}

	__if_exists(UnmapViewOfFileEx)
	{
		void UnMapEx(_In_ ULONG UnMapFlags) noexcept
		{
			if (m_hMutex != nullptr)
			{
				CloseHandle(m_hMutex);
				m_hMutex = nullptr;
			}

			//unmap the view
			if (m_lpData != nullptr)
			{
				FlushViewOfFile(m_lpData, 0);
				UnmapViewOfFileEx(m_lpData, UnMapFlags);
				m_lpData = nullptr;
			}

			//remove the file mapping
			if (m_hMapping != nullptr)
			{
				CloseHandle(m_hMapping);
				m_hMapping = nullptr;
			}

			//close the file system file if its open
			if (m_hFile != INVALID_HANDLE_VALUE)
			{
				CloseHandle(m_hFile);
				m_hFile = INVALID_HANDLE_VALUE;
			}
		}
	}

	_When_(return != 0, _Acquires_lock_(this->m_hMutex))
	LPVOID Open(_In_ DWORD dwTimeout = INFINITE) noexcept
	{
		//Validate our parameters
		if (m_lpData == nullptr)
		{
			SetLastError(ERROR_INVALID_DATA);
			return nullptr;
		}

		//Validate our parameters
		#pragma warning(suppress: 26477)
		OASSUME(m_hMutex != nullptr); //Mutex should be valid

		//Synchronise access to the MMF using the named mutex
		#pragma warning(suppress: 26135)
		return (WaitForSingleObject(m_hMutex, dwTimeout) == WAIT_OBJECT_0) ? m_lpData : nullptr;
	}

	_Releases_lock_(this->m_hMutex)
	BOOL Close() noexcept
	{
		//Validate our parameters
		#pragma warning(suppress: 26477)
		OASSUME(m_hMutex != nullptr); //Mutex should be valid

		return ReleaseMutex(m_hMutex);
	}

	BOOL Flush() noexcept
	{
		//No mapping open, so nothing to do
		if (m_lpData == nullptr)
		{
			SetLastError(ERROR_INVALID_DATA);
			return FALSE;
		}

		return FlushViewOfFile(m_lpData, 0);
	}

	//Accessors
	HANDLE GetFileHandle() noexcept
	{
		return m_hFile;
	}

	HANDLE GetFileMappingHandle() noexcept
	{
		return m_hMapping;
	}

protected:
	
	//Typdefs
	typedef BOOL(CMemMapFile::*MAPVIEWANDCREATEMUTEXAPP)(DWORD, const unsigned __int64&, const SIZE_T&, LPSECURITY_ATTRIBUTES, LPCTSTR);
	typedef BOOL(CMemMapFile::*MAPVIEWANDCREATEMUTEX)(DWORD, const unsigned __int64&, const SIZE_T&, LPSECURITY_ATTRIBUTES, LPCTSTR, LPVOID);

	//Member variables
	HANDLE m_hFile;
	HANDLE m_hMapping;
	LPVOID m_lpData;
	HANDLE m_hMutex;

	//Methods
	BOOL MapFileHelper(
		_In_ LPCTSTR pszFilename, 
		_In_ BOOL bReadOnly, 
		_In_ DWORD dwShareMode, 
		_In_opt_ LPCTSTR pszMappingName, 
		_In_opt_ LPCTSTR pszMutexName, 
		_In_ BOOL bGrowable,
        _In_ const unsigned __int64& dwStartOffset, 
		_In_ const SIZE_T& nNumberOfBytesToMap, 
		_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes, 
		_In_ BOOL bNoCache, 
		_In_opt_ LPVOID pBaseAddress,
        _In_opt_ MAPVIEWANDCREATEMUTEX f1, 
		_In_opt_ MAPVIEWANDCREATEMUTEXAPP f2) noexcept
	{
		//Validate our parameters
		#pragma warning(suppress: 26477)
		//OASSERT(m_hFile == INVALID_HANDLE_VALUE); //Should not be already open
		#pragma warning(suppress: 26477)
		OASSERT(m_hMapping == nullptr);
		#pragma warning(suppress: 26477)
		OASSERT(m_lpData == nullptr);
		#pragma warning(suppress: 26477)
		OASSERT(m_hMutex == nullptr);
    
		//Open the real file on the file system
		m_hFile = CreateFile(pszFilename, bReadOnly ? GENERIC_READ : GENERIC_READ | GENERIC_WRITE, dwShareMode, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (m_hFile == INVALID_HANDLE_VALUE)
		{
			const DWORD dwError = GetLastError();
			UnMap();
			SetLastError(dwError);
			return FALSE;
		}

		//Make the file sparse, if requested to make the memory mapped file growable
		if (bGrowable)
		{
			DWORD dwBytesReturned = 0;
			if (!DeviceIoControl(m_hFile, FSCTL_SET_SPARSE, nullptr, 0, nullptr, 0, &dwBytesReturned, nullptr))
			{
				const DWORD dwError = GetLastError();
				UnMap();
				SetLastError(dwError);
				return FALSE;
			}
		}

		//Create the file mapping object
		DWORD dwFileMappingFlags = bReadOnly ? PAGE_READONLY : PAGE_READWRITE;
		if (bNoCache)
		{
			dwFileMappingFlags |= SEC_NOCACHE;
		}
		m_hMapping = CreateFileMapping(m_hFile, lpSecurityAttributes, dwFileMappingFlags, 0, 0, pszMappingName);
		if (m_hMapping == nullptr)
		{
			const DWORD dwError = GetLastError();
			UnMap();
			SetLastError(dwError);
			return FALSE;
		}

		const DWORD dwDesiredAccess = bReadOnly ? FILE_MAP_READ : FILE_MAP_WRITE;
		if (f1 != nullptr)
		{
			return (this->*f1)(dwDesiredAccess, dwStartOffset, nNumberOfBytesToMap, lpSecurityAttributes, pszMutexName, pBaseAddress);
		}
		else
		{
			#pragma warning(suppress: 26477)
			OASSUME(f2 != nullptr);
			return (this->*f2)(dwDesiredAccess, dwStartOffset, nNumberOfBytesToMap, lpSecurityAttributes, pszMutexName);
		}
	}

	BOOL MapMemoryHelper(
		_In_ LPCTSTR pszMappingName, 
		_In_opt_ LPCTSTR pszMutexName, 
		_In_ const SIZE_T& nBytes, 
		_In_ BOOL bReadOnly,
        _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes, 
		_In_ BOOL bNoCache, 
		_In_ const unsigned __int64& dwStartOffset, 
		_In_opt_ LPVOID pBaseAddress,
        _In_opt_ MAPVIEWANDCREATEMUTEX f1, 
		_In_opt_ MAPVIEWANDCREATEMUTEXAPP f2) noexcept
	{
		//Validate our parameters
		#pragma warning(suppress: 26477)
		OASSERT(m_hMapping == nullptr);
		#pragma warning(suppress: 26477)
		OASSERT(m_lpData == nullptr);
		#pragma warning(suppress: 26477)
		OASSERT(m_hMutex == nullptr);

		//Create the file mapping object
		DWORD dwFileMappingFlags = bReadOnly ? PAGE_READONLY : PAGE_READWRITE;
		if (bNoCache)
		{
			dwFileMappingFlags |= SEC_NOCACHE;
		}
		#pragma warning(suppress: 26472)
		m_hMapping = CreateFileMapping(INVALID_HANDLE_VALUE, lpSecurityAttributes, dwFileMappingFlags, static_cast<DWORD>((nBytes & 0xFFFFFFFF00000000) >> 32), static_cast<DWORD>(nBytes & 0xFFFFFFFF), pszMappingName);
		if (m_hMapping == nullptr)
		{
			const DWORD dwError = GetLastError();
			UnMap();
			SetLastError(dwError);
			return FALSE;
		}

		const DWORD dwDesiredAccess = bReadOnly ? FILE_MAP_READ : FILE_MAP_WRITE;
		if (f1 != nullptr)
		{
			return (this->*f1)(dwDesiredAccess, dwStartOffset, nBytes, lpSecurityAttributes, pszMutexName, pBaseAddress);
		}
		else
		{
			#pragma warning(suppress: 26477)
			OASSUME(f2 != nullptr);
			return (this->*f2)(dwDesiredAccess, dwStartOffset, nBytes, lpSecurityAttributes, pszMutexName);
		}
	}

	BOOL MapExistingMemoryHelper(
		_In_ LPCTSTR pszMappingName, 
		_In_opt_ LPCTSTR pszMutexName, 
		_In_ const SIZE_T& nBytes, 
		_In_ BOOL bReadOnly,
		_In_ BOOL bInheritHandle, 
		_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes, 
		_In_ const unsigned __int64& dwStartOffset, 
		_In_opt_ LPVOID pBaseAddress,
        _In_opt_ MAPVIEWANDCREATEMUTEX f1, 
		_In_opt_ MAPVIEWANDCREATEMUTEXAPP f2) noexcept
	{
		//Validate our parameters
		#pragma warning(suppress: 26477)
		OASSERT(m_hMapping == nullptr);
		#pragma warning(suppress: 26477)
		OASSERT(m_lpData == nullptr);
		#pragma warning(suppress: 26477)
		OASSERT(m_hMutex == nullptr);

		//Open the file mapping object
		const DWORD dwDesiredAccess = bReadOnly ? FILE_MAP_READ : FILE_MAP_WRITE;
		m_hMapping = OpenFileMapping(dwDesiredAccess, bInheritHandle, pszMappingName);
		if (m_hMapping == nullptr)
		{
			const DWORD dwError = GetLastError();
			UnMap();
			SetLastError(dwError);
			return FALSE;
		}

		if (f1 != nullptr)
		{
			return (this->*f1)(dwDesiredAccess, dwStartOffset, nBytes, lpSecurityAttributes, pszMutexName, pBaseAddress);
		}
		else
		{
			#pragma warning(suppress: 26477)
			OASSUME(f2 != nullptr);
			return (this->*f2)(dwDesiredAccess, dwStartOffset, nBytes, lpSecurityAttributes, pszMutexName);
		}
	}

	BOOL MapViewAndCreateMutex(
		_In_ DWORD dwDesiredAccess, 
		_In_ const unsigned __int64& dwStartOffset, 
		_In_ const SIZE_T& nBytes, 
		_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes, 
		_In_opt_ LPCTSTR pszMutexName, 
		_In_opt_ LPVOID lpBaseAddress) noexcept
	{
		#pragma warning(suppress: 26472)
		m_lpData = MapViewOfFileEx(m_hMapping, dwDesiredAccess, static_cast<DWORD>((dwStartOffset & 0xFFFFFFFF00000000) >> 32), static_cast<DWORD>(dwStartOffset & 0xFFFFFFFF), nBytes, lpBaseAddress);
		if (m_lpData == nullptr)
		{
			const DWORD dwError = GetLastError();
			UnMap();
			SetLastError(dwError);
			return FALSE;
		}

		//Create the mutex to sync access
		m_hMutex = CreateMutex(lpSecurityAttributes, FALSE, pszMutexName);
		if (m_hMutex == nullptr)
		{
			const DWORD dwError = GetLastError();
			UnMap();
			SetLastError(dwError);
			return FALSE;
		}

		return (m_lpData != nullptr);
	}

	__if_exists(MapViewOfFileFromApp)
	{
		BOOL MapViewAndCreateMutexFromApp(
			_In_ DWORD dwDesiredAccess, 
			_In_ const ULONG64& dwStartOffset, 
			_In_ const SIZE_T& nBytes, 
			_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes, 
			_In_opt_ LPCTSTR pszMutexName) noexcept
		{
			m_lpData = MapViewOfFileFromApp(m_hMapping, dwDesiredAccess, dwStartOffset, nBytes);
			if (m_lpData == nullptr)
			{
				const DWORD dwError = GetLastError();
				UnMap();
				SetLastError(dwError);
				return FALSE;
			}

			//Create the mutex to sync access
			m_hMutex = CreateMutex(lpSecurityAttributes, FALSE, pszMutexName);
			if (m_hMutex == nullptr)
			{
				const DWORD dwError = GetLastError();
				UnMap();
				SetLastError(dwError);
				return FALSE;
			}

			return (m_lpData != nullptr);
		}
	}

};

#endif //#ifndef __MEMMAP_H__
