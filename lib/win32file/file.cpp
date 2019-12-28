
#include "file.h"

Win32File::Win32File()
{
	this->m_hFile = INVALID_HANDLE_VALUE;
}

Win32File::~Win32File()
{
	this->Close(); 
}

BOOL Win32File::Open(LPCTSTR pstrFileName, 
	DWORD dwAccess, 
	DWORD dwShareMode, 
	DWORD dwFlags,
	DWORD dwAttributes)
{
	if (pstrFileName == NULL) 
		return FALSE;
     
	this->Close();
	// Attempt file creation
	HANDLE hFile = ::CreateFile(pstrFileName, 
		dwAccess, 
		dwShareMode, 
		NULL,
		dwFlags, 
		dwAttributes, 
		NULL);
	if( hFile == INVALID_HANDLE_VALUE ) 
		return FALSE;

	this->m_hFile = hFile;
	return TRUE;
}

BOOL Win32File::Create(LPCTSTR pstrFileName,
	DWORD dwAccess, 
	DWORD dwShareMode, 
	DWORD dwFlags,
	DWORD dwAttributes)
{
	return this->Open(pstrFileName, dwAccess, dwShareMode, dwFlags, dwAttributes);
}

VOID Win32File::Close()
{
	if(this->m_hFile == INVALID_HANDLE_VALUE ) return;
	::CloseHandle(this->m_hFile);
	this->m_hFile = INVALID_HANDLE_VALUE;
}

BOOL Win32File::IsOpen() const
{
	return this->m_hFile != INVALID_HANDLE_VALUE;
}

BOOL Win32File::SetEOF()
{
	if (this->m_hFile == INVALID_HANDLE_VALUE) return FALSE;
	return ::SetEndOfFile(this->m_hFile);
}

BOOL Win32File::Flush()
{
	if (this->m_hFile == INVALID_HANDLE_VALUE) return FALSE;
	return ::FlushFileBuffers(this->m_hFile);
}

BOOL Win32File::Read(LPVOID lpBuf, DWORD nCount)
{
	if (this->m_hFile == INVALID_HANDLE_VALUE) return FALSE;
	if (lpBuf == NULL) return FALSE;
	if (nCount == 0) return TRUE;   // avoid Win32 "null-read"
	
	DWORD dwBytesRead = 0;
	if(!::ReadFile(this->m_hFile, lpBuf, nCount, &dwBytesRead, NULL)) return FALSE;
	return TRUE;
}

BOOL Win32File::Read(LPVOID lpBuf, DWORD nCount, LPDWORD pdwRead)
{
	if (this->m_hFile == INVALID_HANDLE_VALUE) return FALSE;
	if (lpBuf == NULL) return FALSE;
	if (pdwRead == NULL) return FALSE;
	if (nCount == 0) return TRUE;   // avoid Win32 "null-read"

	*pdwRead = 0;
	if(!::ReadFile(this->m_hFile, lpBuf, nCount, pdwRead, NULL)) return FALSE;
	return TRUE;
}

BOOL Win32File::Write(LPCVOID lpBuf, DWORD nCount)
{
	if (this->m_hFile == INVALID_HANDLE_VALUE) return FALSE;
	if (lpBuf == NULL) return FALSE;
	if (nCount == 0) return TRUE;   // avoid Win32 "null-read"

	DWORD dwBytesWritten = 0;
	if(!::WriteFile(this->m_hFile, lpBuf, nCount, &dwBytesWritten, NULL)) return FALSE;
	return TRUE;
}

BOOL Win32File::Write(LPCVOID lpBuf, DWORD nCount, LPDWORD pdwWritten)
{
	if (this->m_hFile == INVALID_HANDLE_VALUE) return FALSE;
	if (lpBuf == NULL) return FALSE;
	if (pdwWritten == NULL) return FALSE;
	if (nCount == 0) return TRUE;   // avoid Win32 "null-read"

	*pdwWritten = 0;
	if(!::WriteFile(m_hFile, lpBuf, nCount, pdwWritten, NULL)) return FALSE;
	return TRUE;
}

BOOL Win32File::Lock(DWORD dwOffset, DWORD dwSize)
{
	if (this->m_hFile == INVALID_HANDLE_VALUE) return FALSE;
	return ::LockFile(this->m_hFile, dwOffset, 0, dwSize, 0);
}

BOOL Win32File::Unlock(DWORD dwOffset, DWORD dwSize)
{
	if (this->m_hFile == INVALID_HANDLE_VALUE) return FALSE;
	return ::UnlockFile(this->m_hFile, dwOffset, 0, dwSize, 0);
}

DWORD Win32File::Seek(LONG lOff, UINT nFrom)
{
	if (this->m_hFile == INVALID_HANDLE_VALUE) return 0;
	return ::SetFilePointer(this->m_hFile, lOff, NULL, (DWORD) nFrom);
}

DWORD Win32File::GetPosition() const
{
	if (this->m_hFile == INVALID_HANDLE_VALUE) return 0;
	return ::SetFilePointer(this->m_hFile, 0, NULL, FILE_CURRENT);
}

DWORD Win32File::GetSize() const
{
	if (this->m_hFile == INVALID_HANDLE_VALUE) return 0;
	return ::GetFileSize(m_hFile, NULL);
}

DWORD Win32File::GetType() const
{
	if (this->m_hFile == INVALID_HANDLE_VALUE) return 0;
	return ::GetFileType(this->m_hFile);
}

BOOL Win32File::GetFileTime(FILETIME* ftCreate, FILETIME* ftAccess, FILETIME* ftModified)
{
	if (this->m_hFile == INVALID_HANDLE_VALUE) return FALSE;
	return ::GetFileTime(this->m_hFile, ftCreate, ftAccess, ftModified);
}

BOOL Win32File::SetFileTime(FILETIME* ftCreate, FILETIME* ftAccess, FILETIME* ftModified)
{
	if (this->m_hFile == INVALID_HANDLE_VALUE) return FALSE;
	return ::SetFileTime(this->m_hFile, ftCreate, ftAccess, ftModified);
}

BOOL Win32File::FileExists(LPCTSTR pstrFileName)
{
	if (pstrFileName == NULL) return FALSE;
    DWORD dwErrMode = ::SetErrorMode(SEM_FAILCRITICALERRORS);
    DWORD dwAttribs = ::GetFileAttributes(pstrFileName);
    ::SetErrorMode(dwErrMode);
    return dwAttribs != INVALID_FILE_ATTRIBUTES && ((dwAttribs & FILE_ATTRIBUTE_DIRECTORY) == 0);
}

BOOL Win32File::Delete(LPCTSTR pstrFileName)
{
	if (pstrFileName == NULL) return FALSE;
	return ::DeleteFile(pstrFileName);
}

BOOL Win32File::Rename(LPCTSTR pstrSourceFileName, LPCTSTR pstrTargetFileName)
{
	if (pstrSourceFileName == NULL) return FALSE;
    if (pstrTargetFileName == NULL) return FALSE;
    return ::MoveFile(pstrSourceFileName, pstrTargetFileName);
}
