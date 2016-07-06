#pragma once
#include <winternl.h>

class CComHook
{
public:
	static void CComHook::Init();
	static void CComHook::AddTime(IN DWORD dwTime);
	static void CComHook::StartHook();
	static NTSTATUS WINAPI CComHook::MyZwSetInformationThread(
		IN HANDLE  ThreadHandle,
		IN THREADINFOCLASS  ThreadInformationClass,
		IN PVOID  ThreadInformation,
		IN ULONG  ThreadInformationLength
		);
	static HANDLE WINAPI CComHook::MYCreateMutexA(
		LPSECURITY_ATTRIBUTES lpMutexAttributes,
		BOOL bInitialOwner,
		LPCSTR lpName
		);
	static BOOL WINAPI CComHook::MyCreateProcessW(
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
		);
public:
	static DWORD CComHook::g_OrigFunc_CreateProcessW;
	static DWORD CComHook::g_OrigFunc_CreateMutexA;
	static DWORD CComHook::g_OrigVar_timeoff;
	static DWORD CComHook::g_OrigFunc_ZwSetInformationThread;

};