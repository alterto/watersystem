// MyServer.h: interface for the CMyServer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSERVER_H__2A6A8A8F_58D0_45F1_A921_7FDABE5DB8F8__INCLUDED_)
#define AFX_MYSERVER_H__2A6A8A8F_58D0_45F1_A921_7FDABE5DB8F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "iocp.h"
 #include <afxtempl.h>

struct DI{
	DI(){
		icpc = NULL;
		dt = COleDateTime::GetCurrentTime();
	}

	void UpdateTm()
	{
		dt = COleDateTime::GetCurrentTime();
	}

	bool Test()
	{
		COleDateTimeSpan dtp = COleDateTime::GetCurrentTime() - dt;
		return dtp.GetTotalSeconds()<30;
	}

	CIOCPContext	*	icpc;
	CString				cmd;
	COleDateTime		dt;
};

class CMyServer  : public CIOCPServer
{
public:
	//新连接到来
	void OnConnectionEstablished(CIOCPContext *pContext, CIOCPBuffer *pBuffer);
	//连接关闭
	void OnConnectionClosing(CIOCPContext *pContext, CIOCPBuffer *pBuffer);
	//连接错误发生
	void OnConnectionError(CIOCPContext *pContext, CIOCPBuffer *pBuffer, int nError);
	//读取操作完成
	void OnReadCompleted(CIOCPContext *pContext, CIOCPBuffer *pBuffer);	
	//写入操作完成
	void OnWriteCompleted(CIOCPContext *pContext, CIOCPBuffer *pBuffer);
	//写入数据
	void SendData(CString strDtu,CString strData);

	void RemoveContext(CIOCPContext *pContext);

	void RemoveContext(CString	strDtuID);

public:
	CMap<CString,LPCTSTR,DI,DI>		m_mpDI;
};


#endif // !defined(AFX_MYSERVER_H__2A6A8A8F_58D0_45F1_A921_7FDABE5DB8F8__INCLUDED_)
