#include "stdafx.h"
#include "CHookMgr.h"


CHookMgr g_HookMgr;
BOOL WINAPI DllMain(IN HANDLE hInst,IN DWORD dwReason,IN LPVOID lpReserved)
{
	WCHAR wBuf[MAX_PATH]={0};
	wstring wsTmp;
	
	if(DLL_PROCESS_ATTACH==dwReason)
	{
	
		GetModuleFileName(NULL,wBuf,MAX_PATH);
		wsTmp=wBuf;
		g_HookMgr.StartHook(wsTmp);

		
	}
	else if(DLL_PROCESS_DETACH==dwReason)
	{
		g_HookMgr.StopHook();
	}
	return TRUE;
}

 extern "C" _declspec(dllexport) void Test()
{
	return;
}
