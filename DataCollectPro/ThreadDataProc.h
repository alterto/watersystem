#pragma once
#include <afxmt.h>
#include "MyServer.h"


class CMainProc;
class CThreadDataProc
{
public:
	CThreadDataProc(void);
	~CThreadDataProc(void);


public:
	void Start();
	void Stop();
	BOOL IsExist();//线程对象是否存在


public:
	static unsigned int __stdcall thread(void *);
	
	
public:
	HANDLE				m_hThread;
	HANDLE				m_hEventThrdQuit;
	DWORD				m_dwParam;
	BOOL				m_bExit;
	CMainProc			*m_pMainProc;
};
