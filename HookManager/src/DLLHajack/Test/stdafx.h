#pragma once

#define WIN32_LEAN_AND_MEAN
#define TSLOG_GROUP "ms2"
#define _WIN32_WINNT 0x0501

#include <windows.h>
#include <stdio.h>
#include <locale.h>
#include <atlconv.h>
#include <WinSock2.h>
#include <process.h>
#include <vector>
#include <map>
#include "detours.h"
#include <algorithm>
#include <string>
#define __DEBUG__LOG_ENABLE__  // enable log 
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"shlwapi.lib")

//#define GAME_NAME_JPFC L"NFSOL.exe"
#define GAME_NAME_JPFC L"FileReader.exe"

using namespace std;

static VOID DBGFMTLOG(LPCTSTR fmt, ...)
{
#ifdef __DEBUG__LOG_ENABLE__
	USES_CONVERSION;
	/*
	wchar_t wszText[]=L"1.Unicode字符转换为ANSI;";  
	char szText[]="2.ANSI字符转换成Unicode.";  
	printf("%s\n",W2A(wszText));  
	wprintf(L"%s\n",A2W(szText));  
	*/
	
	int			i = 0;
	char		output[0x200] = { 0 };
	va_list arg_ptr;
	va_start(arg_ptr, fmt);
	_vsnprintf_s(output, 0x200, _TRUNCATE, W2A(fmt), arg_ptr);
	va_end(arg_ptr);
	OutputDebugStringA(output); //output debugview tool
	printf(output); // output console
#endif
}


