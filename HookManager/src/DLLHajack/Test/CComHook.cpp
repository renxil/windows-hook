#include "stdafx.h"
#include "CComHook.h"


DWORD CComHook::g_OrigVar_timeoff;
DWORD CComHook::g_OrigFunc_ZwSetInformationThread;
DWORD CComHook::g_OrigFunc_CreateMutexA;
DWORD CComHook::g_OrigFunc_CreateProcessW;

void CComHook::Init()
{
	//HANDLE hMod=LoadLibraryW(L"winmm.dll");

	HMODULE hMod2=LoadLibraryW(L"ntdll.dll");
	g_OrigFunc_ZwSetInformationThread=(DWORD)GetProcAddress(hMod2,"ZwSetInformationThread");
	wprintf(L"g_OrigFunc_ZwSetInformationThread=%0#x\n",g_OrigFunc_ZwSetInformationThread);

	g_OrigFunc_CreateMutexA=(DWORD)CreateMutexA;
	g_OrigFunc_CreateProcessW=(DWORD)CreateProcessW;


	//g_OrigVar_timeoff=(DWORD)hMod+0x28fe0;
	//TSDEBUG(L"time off=%0#x,%0#x\n",g_OrigVar_timeoff,hMod);
}

NTSTATUS WINAPI CComHook::MyZwSetInformationThread(
					   IN HANDLE  ThreadHandle,
					   IN THREADINFOCLASS  ThreadInformationClass,
					   IN PVOID  ThreadInformation,
					   IN ULONG  ThreadInformationLength
					   )
{
	NTSTATUS nStatus=0;
	if(0x11==ThreadInformationClass)
	{
		return nStatus;
	}
	_asm
	{
		pushad
		push ThreadInformationLength
		push ThreadInformation
		push ThreadInformationClass
		push ThreadHandle
		call g_OrigFunc_ZwSetInformationThread
		mov nStatus,eax
		popad

	}
	return nStatus;
}


void CComHook::AddTime(IN DWORD dwTime)
{	
	//int nTime=GetPrivateProfileInt(L"Com",L"TimeOff",0,INI_PATH);

	*(PDWORD)g_OrigVar_timeoff=dwTime;

}

void CComHook::StartHook()
{
	int nRet=0;
	Init();

	nRet=DetourAttach(&(PVOID&)g_OrigFunc_CreateMutexA, (PVOID)MYCreateMutexA);
	wprintf(L"Hook MYCreateMutexA=%d\n",nRet);

	//nRet=DetourAttach(&(PVOID&)g_OrigFunc_CreateProcessW, (PVOID)MyCreateProcessW);
	//wprintf(L"Hook MyCreateProcessW=%d\n",nRet);


	//nRet=DetourAttach(&(PVOID&)g_OrigFunc_ZwSetInformationThread, (PVOID)MyZwSetInformationThread);
	//wprintf(L"Hook MyZwSetInformationThread=%d\n",nRet);
	
}

HANDLE WINAPI CComHook::MYCreateMutexA(
									   LPSECURITY_ATTRIBUTES lpMutexAttributes,
									   BOOL bInitialOwner,
									   LPCSTR lpName
									   )
{
	HANDLE hMutext=NULL;
	string strName;
	if(lpName)
	{
		strName=lpName;
	}

	_asm
	{
		pushad
			push lpName
			push bInitialOwner
			push lpMutexAttributes
			call g_OrigFunc_CreateMutexA
			mov hMutext,eax
			popad
	}
	if(strName.find("Global\0E139E89-A290-4568-9CA5-5868FF94446E")!=-1)
	{
		SetLastError(0);
	}
	else if(strName.find("Global\\{5C009BF0-B353-4fc3-A37D-CC14511238AC}_Instance_Mutex")!=-1)
	{
		SetLastError(0);
	}

//CLEANUP:
	return hMutext;
}

BOOL WINAPI CComHook::MyCreateProcessW(
			    LPCWSTR lpApplicationName,
			    LPWSTR lpCommandLine,
			    LPSECURITY_ATTRIBUTES lpProcessAttributes,
			    LPSECURITY_ATTRIBUTES lpThreadAttributes,
			    BOOL bInheritHandles,
			    DWORD dwCreationFlags,
			    LPVOID lpEnvironment,
			    LPCWSTR lpCurrentDirectory,
			    LPSTARTUPINFOW lpStartupInfo,
			    LPPROCESS_INFORMATION lpProcessInformation
			   )
{
	BOOL bRet=TRUE;
	if(lpApplicationName)
		wprintf(L"exe=%s\n",lpApplicationName);
	if(lpCommandLine)
		wprintf(L"cmd=%s\n",lpCommandLine);


	_asm
	{
	
		push lpProcessInformation
		push lpStartupInfo
		push lpCurrentDirectory
		push dwCreationFlags
		push bInheritHandles
		push lpThreadAttributes
		push lpProcessAttributes
		push lpCommandLine
		push lpApplicationName
		call g_OrigFunc_CreateProcessW
		mov bRet,eax
		
	}

	return bRet;
}
