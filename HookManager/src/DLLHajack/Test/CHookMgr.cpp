#include "stdafx.h"
#include "CHookMgr.h"
#include "detours.h"


#include "CMyFunc.h"
#include <shlwapi.h>
#pragma comment(lib,"detours.lib")
#include "CComHook.h"
#include "FileContainer.h"



void CHookMgr::Init()
{
	m_mapSupportGame.clear();
	m_mapSupportGame[GAME_NAME_JPFC]=(DWORD)CMyFunc::StartHook;
	
}



BOOL CHookMgr::StartHook(IN wstring wsExeName)
{
	BOOL bRet=FALSE;
	LONG nRet=0;
	std::wstring wsTarget;
	FILE* fWrite=NULL;
	BOOL bHook=FALSE;
	DWORD dwFunc=0;
	WCHAR wShortPath[MAX_PATH]={0};
	std::wstring wsExe;
	PWCHAR pTmp=NULL;

	Init();

	
	transform(wsExeName.begin(),wsExeName.end(),wsExeName.begin(),tolower);

	
	DetourRestoreAfterWith();
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	CComHook::StartHook();
	
	pTmp=PathFindFileNameW(wsExeName.c_str());
	wprintf(L"nik -- name=%s\n",pTmp);
	if(NULL!=pTmp)
		wsExe=pTmp;
		

	for(map<wstring,DWORD>::iterator it=m_mapSupportGame.begin();it!=m_mapSupportGame.end();it++)
	{
		wsTarget=it->first;
		transform(wsTarget.begin(),wsTarget.end(),wsTarget.begin(),tolower);
		if(wsExe!=wsTarget)
		{
			continue;
		}
		dwFunc=it->second;
	
		if(IDOK==MessageBox(NULL,L"Hook Enable?",wsTarget.c_str(),MB_OKCANCEL))
		{
			AllocConsole();
			freopen("CONOUT$", "w", stdout);
			setlocale(LC_ALL,".936");
			ShowWindow( GetConsoleWindow(), SW_SHOW );
			_asm
			{
				call dwFunc;
			}
		}
		break;
	}
 
	DetourTransactionCommit();
	return bRet;
}

BOOL CHookMgr::StopHook()
{
	BOOL bRet=FALSE;

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	DetourTransactionCommit();
	
	//handles output
	map<HANDLE, wstring> handles = FileContainer::Instance()->GetHandles();
	DBGFMTLOG(L"nik -- Output handles maps :\n");
	DBGFMTLOG(L"nik -- map len : %d\n",handles.size());
	USES_CONVERSION;
	for (auto& h : handles)
		DBGFMTLOG(L"nik -- map key[%ld] and value[%s]\n",h.first, W2A(h.second.c_str()));
	//file read output
	vector<FileH> vec = FileContainer::Instance()->GetFileS();
	DBGFMTLOG(L"nik -- Output file reads :\n");
	DBGFMTLOG(L"nik -- file read len : %d\n", vec.size());
	
	for (int i = 0; i < vec.size(); i++)
	{
		DBGFMTLOG(L"nik -- file hd[%ld] and oper_type[%ld] and oper_len[%ld] \n",
			vec[i].GetFileHandle(),
			vec[i].GetFileOperType(),
			vec[i].GetFileOperLen());
	}

	return bRet;
}


