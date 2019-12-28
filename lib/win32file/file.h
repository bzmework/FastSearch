#ifndef __WIN32_FILE_H__
#define __WIN32_FILE_H__

#include <windows.h>
#include <winnt.h>

class Win32File
{
public:
	enum FileDesiredAccessConstants
	{
		fdaRead				= GENERIC_READ,
		fdaWrite			= GENERIC_WRITE,
		fdaExecute			= GENERIC_EXECUTE,
		fdaAll				= GENERIC_ALL
	};

	enum FileShareModeConstants
	{
		fsmRead				= FILE_SHARE_READ,
		fsmWrite			= FILE_SHARE_WRITE,
		fsmDelete			= FILE_SHARE_DELETE
	};

	enum FileCreationConstants
	{
		fcCreateNew			= CREATE_NEW,
		fcCreateAlways		= CREATE_ALWAYS,
		fcOpenExisting		= OPEN_EXISTING,
		fcOpenAlways		= OPEN_ALWAYS,
		fcTruncateExisting	= TRUNCATE_EXISTING
	};

	enum FileAttributeConstants
	{
		faReadOnly			= FILE_ATTRIBUTE_READONLY,
		faHidden			= FILE_ATTRIBUTE_HIDDEN,
		faSystem			= FILE_ATTRIBUTE_SYSTEM,
		faDirectory			= FILE_ATTRIBUTE_DIRECTORY,
		faArchive			= FILE_ATTRIBUTE_ARCHIVE,
		faDevice			= FILE_ATTRIBUTE_DEVICE,
		faNormal			= FILE_ATTRIBUTE_NORMAL,
		faTemporary			= FILE_ATTRIBUTE_TEMPORARY,
		faSparseFile		= FILE_ATTRIBUTE_SPARSE_FILE,
		faReparsePoint		= FILE_ATTRIBUTE_REPARSE_POINT,
		faCompressed		= FILE_ATTRIBUTE_COMPRESSED,
		faOffline			= FILE_ATTRIBUTE_OFFLINE,
		faNoContentIndexed	= FILE_ATTRIBUTE_NOT_CONTENT_INDEXED,
		faEncrypted			= FILE_ATTRIBUTE_ENCRYPTED,
		faVirtual			= FILE_ATTRIBUTE_VIRTUAL
	};

public:
	Win32File();
	virtual ~Win32File();

	BOOL Open(LPCTSTR pstrFileName, 
		DWORD dwAccess = fdaRead, 
		DWORD dwShareMode = fsmRead, 
		DWORD dwFlags = fcOpenExisting,
		DWORD dwAttributes = faNormal);

	BOOL Create(LPCTSTR pstrFileName,
		DWORD dwAccess = fdaWrite, 
		DWORD dwShareMode = 0 /*DENY ALL*/, 
		DWORD dwFlags = fcCreateAlways,
		DWORD dwAttributes = faNormal);

	VOID Close();
	BOOL IsOpen() const;
	BOOL SetEOF();
	BOOL Flush();

	BOOL Read(LPVOID lpBuf, DWORD nCount);
	BOOL Read(LPVOID lpBuf, DWORD nCount, LPDWORD pdwRead);

	BOOL Write(LPCVOID lpBuf, DWORD nCount);
	BOOL Write(LPCVOID lpBuf, DWORD nCount, LPDWORD pdwWritten);

	BOOL Lock(DWORD dwOffset, DWORD dwSize);
	BOOL Unlock(DWORD dwOffset, DWORD dwSize);

	DWORD Seek(LONG lOff, UINT nFrom = FILE_BEGIN);
	DWORD GetPosition() const;
	DWORD GetSize() const;
	DWORD GetType() const;

	BOOL GetFileTime(FILETIME* ftCreate, FILETIME* ftAccess, FILETIME* ftModified);
	BOOL SetFileTime(FILETIME* ftCreate, FILETIME* ftAccess, FILETIME* ftModified);

	static BOOL FileExists(LPCTSTR pstrFileName);
	static BOOL Delete(LPCTSTR pstrFileName);
	static BOOL Rename(LPCTSTR pstrSourceFileName, LPCTSTR pstrTargetFileName);

private:
	HANDLE m_hFile;

};

#endif /* __WIN32_FILE_H__ */