// MainProc.h: interface for the CMainProc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINPROC_H__AFCE9E98_E492_4F33_82D5_CCE3C93136B7__INCLUDED_)
#define AFX_MAINPROC_H__AFCE9E98_E492_4F33_82D5_CCE3C93136B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SystemConfig.h"
#include "ThreadDataProc.h"

class CMyServer;
class CMainProc  
{
public:
	CMainProc();
	virtual ~CMainProc();
	BOOL Init();
	void CoUnInit();
	BOOL StartDtu();
	void StopDtu();
	void Run();
	void StartThread1();//�������ݽ����߳�
	void StopThread1();//ֹͣ���ݽ����߳�
	void PrintOnline();//��ӡ������Ϣ
	void PrintOnline(LPCTSTR lpszdtuid);//��ӡ������
	void PrintCommand();//��ӡ������
	

public:
	CSystemConfig		m_sysConfig;
	CMyServer			*m_pServer;
	INT					m_iTimeID;
	CAdoRecordSet		*m_pAdoRecordSet;
	CAdoCommand			*m_pAdoCommand;
	CAdoConnection		*m_pAdoConnection;
	CThreadDataProc		m_thrdDataProc;
};

#endif // !defined(AFX_MAINPROC_H__AFCE9E98_E492_4F33_82D5_CCE3C93136B7__INCLUDED_)
