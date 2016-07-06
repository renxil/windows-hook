#include "stdafx.h"
#include "CMyFunc.h"
#include "CJieMi.h"
#include "FileContainer.h"

DWORD CMyFunc::g_OrigFunc_SendTo;
DWORD CMyFunc::g_OrigFunc_CreateFile;
DWORD CMyFunc::g_OrigFunc_ReadFile;





BOOL CMyFunc::Init()
{
	BOOL bRet=FALSE;
	WCHAR wValue[MAX_PATH]={0};
	HMODULE hMod=GetModuleHandle(NULL);

	//DBGFMTLOG("nik -- instance before.\n");
	FileContainer::Instance();
	//DBGFMTLOG("nik -- instance afteer.\n");
	g_OrigFunc_SendTo=(DWORD)sendto;
	g_OrigFunc_CreateFile = (DWORD)CreateFile;
	g_OrigFunc_ReadFile = (DWORD)ReadFile;



	return bRet;

}

BOOL CMyFunc::StartHook()
{
	BOOL bRet=FALSE;
	int nRet=0;
	Init();

	if(g_OrigFunc_SendTo)
	{
		nRet=DetourAttach(&(PVOID&)g_OrigFunc_SendTo, (PVOID)Mysendto);
		wprintf(L"Hook g_OrigFunc_SendTo=%0#x,ret=%d\n",g_OrigFunc_SendTo,nRet);
		DBGFMTLOG(L"Hook g_OrigFunc_SendTo=%0#x,ret=%d\n", g_OrigFunc_SendTo, nRet);
		// @nik added 
		nRet = DetourAttach(&(PVOID&)g_OrigFunc_CreateFile, (PVOID)MyCreateFile);
		wprintf(L"Hook g_OrigFunc_CreateFile=%0#x,ret=%d\n", g_OrigFunc_CreateFile, nRet);
		
		DBGFMTLOG(L"nik Hook g_OrigFunc_CreateFile=%0#x,ret=%d\n", g_OrigFunc_CreateFile, nRet);

		nRet = DetourAttach(&(PVOID&)g_OrigFunc_ReadFile, (PVOID)MyReadFile);
		wprintf(L"Hook g_OrigFunc_ReadFile=%0#x,ret=%d\n", g_OrigFunc_ReadFile, nRet);
		DBGFMTLOG(L"Hook g_OrigFunc_ReadFile=%0#x,ret=%d\n", g_OrigFunc_ReadFile, nRet);
	}

	return bRet;
}


HANDLE WINAPI CMyFunc::MyCreateFile(
	_In_     LPCTSTR               lpFileName,
	_In_     DWORD                 dwDesiredAccess,
	_In_     DWORD                 dwShareMode,
	_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	_In_     DWORD                 dwCreationDisposition,
	_In_     DWORD                 dwFlagsAndAttributes,
	_In_opt_ HANDLE                hTemplateFile
	)
{

	HANDLE hd;
	_asm
	{
		pushad
		push hTemplateFile
			push dwFlagsAndAttributes
			push dwCreationDisposition
			push lpSecurityAttributes
			push dwShareMode
			push dwDesiredAccess
			push lpFileName
			call g_OrigFunc_CreateFile
			mov hd, eax
			popad
	}
	USES_CONVERSION;
	DBGFMTLOG(L"MyCreateFile::nik -- filename: %s ;handle:%ld \n", W2A(lpFileName), hd);
	FileContainer::Instance()->AddHandle(hd, lpFileName);

	return hd;

}



BOOL WINAPI CMyFunc::MyReadFile(
	_In_        HANDLE       hFile,
	_Out_       LPVOID       lpBuffer,
	_In_        DWORD        nNumberOfBytesToRead,
	_Out_opt_   LPDWORD      lpNumberOfBytesRead,
	_Inout_opt_ LPOVERLAPPED lpOverlapped
	)
{
	int nRet = 0;
	DWORD nLen = 0;
	_asm
	{
		pushad
		push lpOverlapped
		push lpNumberOfBytesRead
		push nNumberOfBytesToRead
		push lpBuffer
		push hFile
		call g_OrigFunc_ReadFile
		mov nRet, eax
		popad
	}

	nLen = *lpNumberOfBytesRead;
	DBGFMTLOG(L"MyReadFile::nik -- nRet : %ld ; hd: %ld ; read len:%ld \n",nRet,hFile, nLen);
	FileH fh (hFile, 1, nLen);
	FileContainer::Instance()->AddFile(fh);

	return (BOOL)nRet;
}

int WINAPI CMyFunc::Mysendto(
		    SOCKET s,
		     char* buf,
		     int len,
		     int flags,
		     const struct sockaddr* to,
		     int tolen
		   )
{
	int nRet=0;
	CJiaMi jiami;
	CJieMi xx;
	DWORD dwValue;
	DWORD dwX=0;
	DWORD dwY=0;
	DWORD dwZ=0;

	ZeroMemory(&xx,sizeof(xx));
	ZeroMemory(&jiami,sizeof(jiami));


	DWORD dwDropSize=GetPrivateProfileIntA("NFSOL.exe","Size",0,"D:\\config\\cap.ini");

	if(dwDropSize!=0 && len>=(int)dwDropSize)
	{
		return len;
	}

	xx.InitRecvPak((PBYTE)buf,len*8);
	xx.SetPopOffset(0);
	dwValue=xx.PopValue(3);
	dwValue=xx.PopValue(3);
	dwValue=xx.PopValue(0xe);
	dwValue=xx.PopValue(0x8);
	if(0x14==dwValue)
	{
		dwValue=xx.PopValue(0xa);
		dwValue=xx.PopValue(0xa);
		dwValue=xx.PopValue(0x20); //hdr 0x50

		dwValue=xx.PopValue(1);
		if(0x0==dwValue)
		{
			dwValue=xx.PopValue(1);
			dwValue=xx.PopValue(0x10);
			dwValue=xx.PopValue(0x6);
			dwValue=xx.PopValue(0xf);
			dwValue=xx.PopValue(0x1);//0x28

			dwValue=xx.PopValue(0x5);

			dwValue=xx.PopValue(0x20); //time
			dwValue=xx.PopValue(0x20);
			dwValue=xx.PopValue(0x20); //attr
			dwValue=xx.PopValue(0x6);
			dwValue=xx.PopValue(0x10);
			dwValue=xx.PopValue(0x4);
			dwValue=xx.PopValue(0x10);

			dwValue=xx.PopValue(0x3); //pos
			if(0x7==dwValue)
			{
				dwX=xx.PopValue(0x15);
				dwY=xx.PopValue(0x15);
				dwZ=xx.PopValue(0x15);


				CHAR aValue[MAX_PATH]={0};
				DWORD x=0;
				DWORD y=0;
				DWORD z=0;

				ZeroMemory(aValue,sizeof(aValue));
				GetPrivateProfileStringA("NFSOL.exe","X","0",aValue,MAX_PATH,"D:\\config\\cap.ini");
				sscanf_s(aValue,"%d",&x);

				ZeroMemory(aValue,sizeof(aValue));
				GetPrivateProfileStringA("NFSOL.exe","Y","0",aValue,MAX_PATH,"D:\\config\\cap.ini");
				sscanf_s(aValue,"%d",&y);

				ZeroMemory(aValue,sizeof(aValue));
				GetPrivateProfileStringA("NFSOL.exe","Z","0",aValue,MAX_PATH,"D:\\config\\cap.ini");
				sscanf_s(aValue,"%d",&z);

				//DBGFMTLOG(L"prid x=%0#x,y=%0#x,z=%0#x,size=%0#x,xx=%d,yy=%d,zz=%d\n",dwX,dwY,dwZ,len,x,y,z);

				if(x!=0)
				{
					WORD wCrc=jiami.CalcCrc(len-2,(PBYTE)buf);
					//TSDEBUG(L"OrigCrc=%0#x,mycrc=%0#x",*(PWORD)(pBuffer+dwSize-2),wCrc);


					jiami.InitSendPak((PBYTE)buf,0x640);
					jiami.SetSendOffset(0x50+0x28+5+0x20*3+6+0x10+4+0x10+3);
					jiami.PushValue(x,0x15);
					jiami.PushValue(y,0x15);
					jiami.PushValue(z,0x15);
					jiami.EndBuf();
					wCrc=jiami.CalcCrc(len-2,(PBYTE)buf);
					DBGFMTLOG(L"OrigCrc=%0#x,mycrc=%0#x",*(PWORD)(buf+len-2),wCrc);
					memcpy(buf+len-2,&wCrc,2);

					xx.InitRecvPak((PBYTE)buf,len*8);
					/* 0485F260       -168527.5       23705.90      -304012.2*/
					xx.SetPopOffset(0x50+0x28+5+0x20*3+6+0x10+4+0x10);
					dwValue=xx.PopValue(3);
					dwX=xx.PopValue(0x15); 
					dwY=xx.PopValue(0x15);
					dwZ=xx.PopValue(0x15);
					DBGFMTLOG(L"after prid x=%0#x,y=%0#x,z=%0#x,size=%0#x,xx=%d,yy=%d,zz=%d\n",dwX,dwY,dwZ,len,x,y,z);

				}
			}
		}
	}


	_asm
	{
		pushad
		push tolen
		push to
		push flags
		push len
		push buf
		push s
		call g_OrigFunc_SendTo
		mov nRet,eax
		popad
	}
	DBGFMTLOG(L"Sndto socket=%0#x,pBuffer=%0#x,dwSize=%0#x\n",s,buf,len);

	return nRet;
}

