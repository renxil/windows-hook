#pragma once

class CHookMgr
{
public:
	BOOL CHookMgr::StartHook(IN wstring wsExeName);
	BOOL CHookMgr::StopHook();

	void CHookMgr::Init();
public:
	map<wstring,DWORD> m_mapSupportGame;

};