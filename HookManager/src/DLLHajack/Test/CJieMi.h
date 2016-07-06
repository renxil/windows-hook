#pragma once

void  InitRecvPak_ASM(IN PBYTE pRecvBuf,IN DWORD dwTotalBits);
DWORD  PopValue_ASM(IN DWORD dwBits);
void  SetPopOffset_ASM(IN DWORD dwOffset);

void  InitSendPak_ASM(IN PBYTE pBuf,IN DWORD dwTotalBits);
void  PushValueBot_ASM(IN DWORD dwLastOffset,IN DWORD dwValue,IN DWORD dwBits);
void  PushValue_ASM(IN DWORD dwValue,IN DWORD dwBits);
void  EndBuf_ASM();
void  SetSendOffset_ASM(IN DWORD dwBitsOffset);
WORD  CalcCrc_ASM(IN DWORD dwBytesCount,IN PBYTE pBuf);


class CJieMi
{
public:
	DWORD CJieMi::PopValue(IN DWORD dwBits);
	void CJieMi::InitRecvPak(IN PBYTE pRecvBuf,IN DWORD dwBits);
	void CJieMi::SetPopOffset(IN DWORD dwOffset);
public:
	PBYTE m_pRecvBuf;  //0
	DWORD m_dwTotalBits;//4
	DWORD m_pEndRecvBuf;//8
	DWORD m_dwCache;//0xc
	PBYTE m_pNextBuf;//0x10
	DWORD m_dwPopedBits;//0x14
	DWORD m_dwReserved;//0x18

};

class CJiaMi
{
public:
	void CJiaMi::InitSendPak(IN PBYTE pSendBuf,IN DWORD dwTotalBytes);
	void CJiaMi::PushValue(IN DWORD dwValue,IN DWORD dwBits);
	void CJiaMi::SetSendOffset(IN DWORD dwOffset);
	void CJiaMi::EndBuf();
	static WORD CJiaMi::CalcCrc(IN DWORD dwSize,IN PBYTE pBuff);
public:
	PBYTE m_pSendBuf;//0
	DWORD m_dwTotalBits;//4
	DWORD m_dwPushedBits;//8
	PBYTE m_pEndSendBuf;//0xc
	DWORD m_dwCache;//0x10
	PBYTE m_pNextBuf;//0x14
	DWORD m_dwReserved;//0x18

};