#pragma once
#


class CMyFunc
{
public:
	static BOOL CMyFunc::Init();
	static BOOL CMyFunc::StartHook();
	
	static int WINAPI CMyFunc::Mysendto(
		SOCKET s,
		char* buf,
		int len,
		int flags,
		const struct sockaddr* to,
		int tolen
		);
	static BOOL WINAPI CMyFunc::MyReadFile(
		_In_        HANDLE       hFile,
		_Out_       LPVOID       lpBuffer,
		_In_        DWORD        nNumberOfBytesToRead,
		_Out_opt_   LPDWORD      lpNumberOfBytesRead,
		_Inout_opt_ LPOVERLAPPED lpOverlapped
		);
	static HANDLE WINAPI CMyFunc::MyCreateFile(
			_In_     LPCTSTR               lpFileName,
			_In_     DWORD                 dwDesiredAccess,
			_In_     DWORD                 dwShareMode,
			_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
			_In_     DWORD                 dwCreationDisposition,
			_In_     DWORD                 dwFlagsAndAttributes,
			_In_opt_ HANDLE                hTemplateFile
			);


public:

	static DWORD CMyFunc::g_OrigFunc_SendTo;
	static DWORD CMyFunc::g_OrigFunc_CreateFile;
	static DWORD CMyFunc::g_OrigFunc_ReadFile;
};