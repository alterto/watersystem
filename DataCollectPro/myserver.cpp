// MyServer.cpp: implementation of the CMyServer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyServer.h"
#include "HzFormat.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void CMyServer::OnConnectionEstablished(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
{
	//Output(pBuffer->buff);
}

void CMyServer::OnConnectionClosing(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
{
	RemoveContext(pContext);
}

void CMyServer::OnConnectionError(CIOCPContext *pContext, CIOCPBuffer *pBuffer, int nError)
{
	RemoveContext(pContext);
}

void CMyServer::OnReadCompleted(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
{
	Output(pBuffer->buff);
	cJSON *root = cJSON_Parse(pBuffer->buff);
	CString strDtu;
	CString strCmd;
	CString strData;
	HZFORMAT hzformat;

	BOOL bRet = TRUE;

	do 
	{
		if(NULL == root)
		{
			SendText(pContext,_T("ERROR"),6);
			bRet = FALSE;
			break;
		}
		cJSON * pSub = cJSON_GetObjectItem(root, "DTU");
		if(NULL== pSub)
		{
			SendText(pContext,_T("ERROR"),6);
			bRet = FALSE;
			break;
		}
		strDtu = pSub->valuestring;
		pSub = cJSON_GetObjectItem(root, "CMD");
		if(NULL== pSub)
		{
			SendText(pContext,_T("ERROR"),6);
			bRet = FALSE;
			break;
		}
		strCmd = pSub->valuestring;
		pSub = cJSON_GetObjectItem(root, "DATA");
		if(NULL== pSub)
		{
			SendText(pContext,_T("ERROR"),6);
			bRet = FALSE;
			break;
		}
		strData = pSub->valuestring;
		
		break;
	} while (TRUE);
	

	cJSON_Delete(root);
	
	if(!bRet) return;
	int ret = -1;

	if(strCmd == "LOCK")
	{
		hzformat.CreateOpenCloseCode();
	}else if(strCmd == "UNLOCK"){
		hzformat.CreateOpenCloseCode(false);
	}else if(strCmd == "SETFLOW"){
		hzformat.CreateFlowSetCode(atoi(strData));
	}else if(strCmd == "GETFLOW"){
		hzformat.CreateFlowGetCode();
	}

	ret = do_send_user_data((unsigned char*)(LPCTSTR)strDtu,(unsigned char*)hzformat.GetData(),hzformat.GetLength(),NULL);

	if(ret==0)
	{
		DI di;
		di.cmd = strCmd;
		di.icpc = pContext;
		
		m_mpDI[strDtu].cmd = strCmd;
		m_mpDI[strDtu].icpc = pContext;
		m_mpDI[strDtu].UpdateTm();
	}
	
	CString strRet;
	if(ret == 0)
	{
		strRet.Format(_T("{\"DTU\":\"%s\",\"VEC\":\"IN\",\"CMD\":\"%s\",\"DATA\":\"W\"}"),strDtu,strCmd);
	}else{
		strRet.Format(_T("{\"DTU\":\"%s\",\"VEC\":\"IN\",\"CMD\":\"%s\",\"DATA\":\"F\"}"),strDtu,strCmd);
	}
	
	SendText(pContext,(CHAR *)(LPCTSTR)strRet,strRet.GetLength()+1);
	

	/*
	HZFORMAT hzformat(pBuffer->buff,pBuffer->nLen);

	CString strDtu = hzformat.GetDtuID();
	unsigned char *udtu = (unsigned char*)(LPCTSTR)strDtu;
 	unsigned char *data = (unsigned char*)hzformat.GetData();
 	int iLen = hzformat.GetDataLength();
	string strRet;
	
	switch(hzformat.GetType())
	{
	case 1:
		//锁定/解锁指令设定
		break;
	case 2:
		//总累计设定
		break;
	case 3:
		//流量设定
		break;
	default:
		//SendText(pContext,_T("未定义指令异常."),strlen(_T("未定义指令异常.")));
		return;
	}
	
	int ret=do_send_user_data(udtu,data,iLen,NULL);
	if(ret==0)
	{
		m_mpSocket[strDtu] = pContext;
		m_mpSocket.SetAt(strDtu,pContext);

	}else
	{
		

	}

	SendText(pContext,(char*)strRet.c_str(),strRet.length());
	*/
}

void CMyServer::OnWriteCompleted(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
{

}

void CMyServer::SendData(CString strDtu,CString strData)
{
	CIOCPContext *pContext = m_mpDI[strDtu].icpc;
	if(pContext)
	{
		CString strRet;
		strRet.Format(_T("{\"DTU\":\"%s\",\"VEC\":\"IN\",\"CMD\":\"%s\",\"DATA\":\"%s\"}"),strDtu,m_mpDI[strDtu].cmd,strData);
		SendText(pContext,(char*)strRet.GetBuffer(0),strRet.GetLength()+1);
	}	
}

void CMyServer::RemoveContext(CIOCPContext *pContext)
{
	POSITION pos = m_mpDI.GetStartPosition();
	CString strKey;
	DI di;
	CStringArray arrKey;
    while(pos)
    {
        m_mpDI.GetNextAssoc(pos, strKey, di);
		if(pContext == di.icpc)
		{
			arrKey.Add(strKey);
		}
    }

	int iIndex = 0;
	while(iIndex<arrKey.GetSize())
	{
		m_mpDI.RemoveKey(arrKey[iIndex]);
		iIndex++;
	}
}

void CMyServer::RemoveContext(CString strDtuID)
{
	m_mpDI.RemoveKey(strDtuID);
}