#include "StdAfx.h"
#include "ThreadDataProc.h"
#include "MainProc.h"
#include "hzpacket.h"
#include <PROCESS.H>

CThreadDataProc::CThreadDataProc(void)
{
	m_hThread = NULL;
	m_hEventThrdQuit = NULL;
	m_bExit = FALSE;
	m_pMainProc = NULL;
}

CThreadDataProc::~CThreadDataProc(void)
{

}

void CThreadDataProc::Start()
{
	DWORD dwThreadId = 0;
	m_bExit = FALSE;
	m_hThread = (HANDLE)::_beginthreadex(NULL, 0, thread, this, 0, NULL);
	ASSERT(m_hThread);
	m_hEventThrdQuit = CreateEvent(NULL, TRUE, FALSE, _T("ThreadQuit"));
	ASSERT(m_hEventThrdQuit);
}

BOOL SaveData(CAdoCommand *pAdoCommand,data_record &dr)
{	
	if(!pAdoCommand) return FALSE;
	hz_rcpacket rcpacket;
	for(int iIndex = 0;iIndex<dr.m_data_len;iIndex++)
	{
		rcpacket.u.buffer[iIndex] = dr.m_data_buf[iIndex];
	}
	rcpacket.iLength = dr.m_data_len;
	if(!rcpacket.IsInValid()) return FALSE;

	double dTemp = 0;
	long iTemp = 0;
	
	pAdoCommand->GetParameter(0)->PutValue(_variant_t(long(0)));
	//测站号
	pAdoCommand->GetParameter(1)->PutValue(_variant_t(dr.m_userid));
	//用户号
	iTemp = rcpacket.GetUserNum();
	if(iTemp>10)iTemp = 0;
	pAdoCommand->GetParameter(2)->PutValue(_variant_t(iTemp));
	//用水量
	dTemp = rcpacket.GetUseWt();
	pAdoCommand->GetParameter(3)->PutValue(_variant_t(dTemp));
	dTemp = rcpacket.GetLeftWt();
	pAdoCommand->GetParameter(4)->PutValue(_variant_t(dTemp));
	dTemp = rcpacket.GetTotalUse();
	pAdoCommand->GetParameter(5)->PutValue(_variant_t(dTemp));

	iTemp = rcpacket.IsRuning();
	pAdoCommand->GetParameter(6)->PutValue(_variant_t(iTemp));
	iTemp = rcpacket.IsDoolClosed();
	pAdoCommand->GetParameter(7)->PutValue(_variant_t(iTemp));
	iTemp = 1;
	pAdoCommand->GetParameter(8)->PutValue(_variant_t(iTemp));
	iTemp = rcpacket.GetAI0();
	pAdoCommand->GetParameter(9)->PutValue(_variant_t(iTemp));
	iTemp = rcpacket.GetAI1();
	pAdoCommand->GetParameter(10)->PutValue(_variant_t(iTemp));
	iTemp = rcpacket.GetAI2();
	pAdoCommand->GetParameter(11)->PutValue(_variant_t(iTemp));
	iTemp = rcpacket.GetAI3();
	pAdoCommand->GetParameter(12)->PutValue(_variant_t(iTemp));
	iTemp = rcpacket.GetAI4();
	pAdoCommand->GetParameter(13)->PutValue(_variant_t(iTemp));
	dTemp = rcpacket.GetPower();
	pAdoCommand->GetParameter(14)->PutValue(_variant_t(dTemp));
	dTemp = rcpacket.GetAV();
	pAdoCommand->GetParameter(15)->PutValue(_variant_t(dTemp));
	dTemp = rcpacket.GetBV();
	pAdoCommand->GetParameter(16)->PutValue(_variant_t(dTemp));
	dTemp = rcpacket.GetCV();
	pAdoCommand->GetParameter(17)->PutValue(_variant_t(dTemp));
	dTemp = rcpacket.GetAA();
	pAdoCommand->GetParameter(18)->PutValue(_variant_t(dTemp));
	dTemp = rcpacket.GetBA();
	pAdoCommand->GetParameter(19)->PutValue(_variant_t(dTemp));
	dTemp = rcpacket.GetCA();
	pAdoCommand->GetParameter(20)->PutValue(_variant_t(dTemp));
	dTemp = rcpacket.GetAS();
	pAdoCommand->GetParameter(21)->PutValue(_variant_t(dTemp));
	dTemp = rcpacket.GetBS();
	pAdoCommand->GetParameter(22)->PutValue(_variant_t(dTemp));
	dTemp = rcpacket.GetCS();
	pAdoCommand->GetParameter(23)->PutValue(_variant_t(dTemp));
	PVOID pRet = pAdoCommand->Execute();
	
	if(!pRet)
	{
		string str = dr.m_userid;
		str+=_T(":save error!");
		Output(str);
		Output((LPCTSTR)pAdoCommand->GetLastError());
	}
	
	return pRet!=NULL;
}

BOOL SaveData2(CAdoCommand *pAdoCommand,data_record &dr)
{
	if(!pAdoCommand) return FALSE;
	union{
		short data;
		char chData[2];
	}u;
	

	//
	u.chData[0] = dr.m_data_buf[5];
	u.chData[1] = dr.m_data_buf[6];

	
	//测站号
	pAdoCommand->GetParameter(0)->PutValue(_variant_t(dr.m_userid));
	//报警状态
	pAdoCommand->GetParameter(1)->PutValue(_variant_t(long(u.data)));

	PVOID pRet = pAdoCommand->Execute();
	
	
	if(!pRet)
	{
		string str = dr.m_userid;
		str+=_T(":alarm save error!");
		Output(str);
		Output((LPCTSTR)pAdoCommand->GetLastError());
	}
	
	return pRet!=NULL;
}

BOOL SaveData3(CAdoCommand *pAdoCommand,CString strDtuID,int iFlow)
{
	if(!pAdoCommand) return FALSE;
	
	//测站号
	pAdoCommand->GetParameter(0)->PutValue(_variant_t(strDtuID));
	//报警状态
	pAdoCommand->GetParameter(1)->PutValue(_variant_t(long(iFlow)));
	
	PVOID pRet = pAdoCommand->Execute();
	
	
	if(!pRet)
	{
		string str = strDtuID.GetBuffer(0);
		str+=_T(":alarm save error!");
		Output(str);
		Output((LPCTSTR)pAdoCommand->GetLastError());
	}
	
	return pRet!=NULL;
}

unsigned int __stdcall CThreadDataProc::thread(void *pParam)
{
	CThreadDataProc *pThread = (CThreadDataProc*)pParam;
	ASSERT(pThread);
	CMainProc *pMainProc = pThread->m_pMainProc;
	ASSERT(pMainProc);
	CMyServer	*pServer = pThread->m_pMainProc->m_pServer;
	//连接数据库
	CoInitialize(NULL);
	CAdoConnection * pAdoConnection= ::GetConnect(pMainProc->m_sysConfig.ip,
		pMainProc->m_sysConfig.dbname,
		pMainProc->m_sysConfig.username,
		pMainProc->m_sysConfig.passwd);

	if(!pAdoConnection)
	{
		cout<<"Thread database connect failed!!Data cannot be save.Please restart thread!"<<endl;
		CoUninitialize();
		pThread->m_bExit = TRUE;
		ExitThread(100);    //结束线程
		return 0;
	}
	
	CAdoCommand *pAdoCommand = new CAdoCommand;
	pAdoCommand->SetConnection(pAdoConnection);
	pAdoCommand->SetCommandText(_T("prSaveRtData"));
	_ParameterPtr paramPtr = NULL;
	
	_variant_t var;
	var.vt = VT_EMPTY;
	paramPtr = pAdoCommand->CreateParameter(_T("UpdateStated"),adInteger,adParamInput,4,var);
	pAdoCommand->Append(paramPtr);
	paramPtr = pAdoCommand->CreateParameter(_T("CezhanID"),adVarChar,adParamInput,30,var);
	pAdoCommand->Append(paramPtr);
	paramPtr = pAdoCommand->CreateParameter(_T("UserNo"),adInteger,adParamInput,4,var);
	pAdoCommand->Append(paramPtr);
	paramPtr = pAdoCommand->CreateParameter(_T("UseWt"),adDouble,adParamInput,8,var);
	pAdoCommand->Append(paramPtr);
	paramPtr = pAdoCommand->CreateParameter(_T("LeftWt"),adDouble,adParamInput,8,var);
	pAdoCommand->Append(paramPtr);
	paramPtr = pAdoCommand->CreateParameter(_T("CurWtBase"),adDouble,adParamInput,8,var);
	pAdoCommand->Append(paramPtr);
	paramPtr = pAdoCommand->CreateParameter(_T("OpenState"),adInteger,adParamInput,4,var);
	pAdoCommand->Append(paramPtr);
	paramPtr = pAdoCommand->CreateParameter(_T("DoolState"),adInteger,adParamInput,4,var);
	pAdoCommand->Append(paramPtr);
	paramPtr = pAdoCommand->CreateParameter(_T("NetState"),adInteger,adParamInput,4,var);
	pAdoCommand->Append(paramPtr);
	paramPtr = pAdoCommand->CreateParameter(_T("Ai0"),adInteger,adParamInput,4,var);
	pAdoCommand->Append(paramPtr);
	paramPtr = pAdoCommand->CreateParameter(_T("Ai1"),adInteger,adParamInput,4,var);
	pAdoCommand->Append(paramPtr);
	paramPtr = pAdoCommand->CreateParameter(_T("Ai2"),adInteger,adParamInput,4,var);
	pAdoCommand->Append(paramPtr);
	paramPtr = pAdoCommand->CreateParameter(_T("Ai3"),adInteger,adParamInput,4,var);
	pAdoCommand->Append(paramPtr);
	paramPtr = pAdoCommand->CreateParameter(_T("Ai4"),adInteger,adParamInput,4,var);
	pAdoCommand->Append(paramPtr);
	paramPtr = pAdoCommand->CreateParameter(_T("UsePower"),adDouble,adParamInput,8,var);
	pAdoCommand->Append(paramPtr);
	paramPtr = pAdoCommand->CreateParameter(_T("Av"),adDouble,adParamInput,8,var);
	pAdoCommand->Append(paramPtr);
	paramPtr = pAdoCommand->CreateParameter(_T("Bv"),adDouble,adParamInput,8,var);
	pAdoCommand->Append(paramPtr);
	paramPtr = pAdoCommand->CreateParameter(_T("Cv"),adDouble,adParamInput,8,var);
	pAdoCommand->Append(paramPtr);
	paramPtr = pAdoCommand->CreateParameter(_T("Aa"),adDouble,adParamInput,8,var);
	pAdoCommand->Append(paramPtr);
	paramPtr = pAdoCommand->CreateParameter(_T("Ba"),adDouble,adParamInput,8,var);
	pAdoCommand->Append(paramPtr);
	paramPtr = pAdoCommand->CreateParameter(_T("Ca"),adDouble,adParamInput,8,var);
	pAdoCommand->Append(paramPtr);
	paramPtr = pAdoCommand->CreateParameter(_T("AsState"),adInteger,adParamInput,4,var);
	pAdoCommand->Append(paramPtr);
	paramPtr = pAdoCommand->CreateParameter(_T("BsState"),adInteger,adParamInput,4,var);
	pAdoCommand->Append(paramPtr);
	paramPtr = pAdoCommand->CreateParameter(_T("CsState"),adInteger,adParamInput,4,var);
	pAdoCommand->Append(paramPtr);


	//////////////////////////////////////////////////////////////////////////
	CAdoCommand *pAdoCommand2 = new CAdoCommand;
	pAdoCommand2->SetConnection(pAdoConnection);
	pAdoCommand2->SetCommandText(_T("prSaveRtAlarm"));
	paramPtr = pAdoCommand2->CreateParameter(_T("CezhanID"),adVarChar,adParamInput,30,var);
	pAdoCommand2->Append(paramPtr);
	paramPtr = pAdoCommand2->CreateParameter(_T("AlarmState"),adInteger,adParamInput,4,var);
	pAdoCommand2->Append(paramPtr);
	//////////////////////////////////////////////////////////////////////////
	/*
	CAdoCommand *pAdoCommand3 = new CAdoCommand;
	pAdoCommand3->SetConnection(pAdoConnection);
	pAdoCommand3->SetCommandText(_T("prUpdateFlow"));
	paramPtr = pAdoCommand3->CreateParameter(_T("CezhanID"),adVarChar,adParamInput,30,var);
	pAdoCommand3->Append(paramPtr);
	paramPtr = pAdoCommand3->CreateParameter(_T("PerWtOut"),adInteger,adParamInput,4,var);
	pAdoCommand3->Append(paramPtr);
	*/
	
	data_record dr;
	PVOID pRet = NULL;

	while (WaitForSingleObject(pThread->m_hEventThrdQuit, 0) != WAIT_OBJECT_0)
	{
		//开发包函数,读取数据
		if (do_read_proc(&dr,NULL,FALSE)>=0)
		{
			char buf[1280] = {0}; //1024+256
			if (0x09==dr.m_data_type)
			{
				//处理数据
				//sprintf(buf,"用户ID:%s,   接收时间:%s,   数据长度:%d",
				//dr.m_userid,dr.m_recv_date,dr.m_data_len);
				
				if(dr.m_data_buf[2] == 0 && dr.m_data_buf[3] ==0)
				{
					switch (dr.m_data_buf[4])
					{
					case 1:
						//数据保存
						SaveData(pAdoCommand,dr);
						break;
					case 6:
						{
							//读取命令返回
							if(!pServer) break;
							DI di;
							if (pServer->m_mpDI.Lookup(dr.m_userid,di))
							{
								if(di.cmd == "GETFLOW"){
									if(dr.m_data_buf[5]!=2) break;
									union{
										short data;
										char chdata[2];
									}u;
									u.chdata[0] = dr.m_data_buf[6];
									u.chdata[1] = dr.m_data_buf[7];
									CString strData;
									strData.Format(_T("%d"),u.data);
									pServer->SendData(dr.m_userid,strData);
								}

								pServer->m_mpDI.RemoveKey(dr.m_userid);
							}
						}
						break;
					case 7:
						{
							//写入命令返回
							if(!pServer) break;
							pServer->SendData(dr.m_userid,"1");
						}
						break;
					case 8:
						//报警
						pAdoConnection->BeginTrans();
						SaveData2(pAdoCommand2,dr);
						if(!pAdoConnection->CommitTrans())
						{
							pAdoConnection->RollbackTrans();
						}
						break;
					}
				}		
			}else 
			{
				switch (dr.m_data_type)
				{
				case 0x01:
					//sprintf(buf,"%s  登录",dr.m_userid);
					//Output(buf);
					break;
				case 0x02:
					//sprintf(buf,"%s  注销",dr.m_userid);
					//Output(buf);
					pAdoConnection->BeginTrans();
					pAdoCommand->GetParameter(0)->PutValue(_variant_t(long(1)));
					pAdoCommand->GetParameter(1)->PutValue(_variant_t(dr.m_userid));
					pAdoCommand->GetParameter(7)->PutValue(_variant_t(long(0)));
					pRet = pAdoCommand->Execute();
					if(!pRet)
					{
						string str = dr.m_userid;
						str+=_T(":state save error!");
						Output((LPCTSTR)pAdoCommand->GetLastError());
					}
					if(!pAdoConnection->CommitTrans())
					{
						pAdoConnection->RollbackTrans();
					}
					break;
				case 0x0d:
					//sprintf(buf,"%s  参数设置成功",dr.m_userid);		
					break;
				case 0x0b:
					//sprintf(buf,"%s  参数查询成功",dr.m_userid);		   				    	
					break;
				case 0x06:	
					//sprintf(buf,"%s  断开PPP连接成功",dr.m_userid);
					break;
				case 0x07:
					//sprintf(buf,"%s  停止向DSC发送数据",dr.m_userid);
					break;
				case 0x08:
					//sprintf(buf,"%s  允许向DSC发送数据",dr.m_userid);
					break;
				case 0x0A:
					//sprintf(buf,"%s   丢弃用户数据",dr.m_userid);
					break;
				default:
					//sprintf(buf,"%s",dr.m_userid);
					break;
				}	
			}
		}
	}
	if(pAdoCommand)
	{
		delete pAdoCommand;
		pAdoCommand =NULL;
	}
	if(pAdoCommand2)
	{
		delete pAdoCommand2;
		pAdoCommand2 =NULL;
	}
	/*
	if(pAdoCommand3)
	{
		delete pAdoCommand3;
		pAdoCommand3 =NULL;
	}
	*/
	if(pAdoConnection)
	{
		pAdoConnection->Close();
		delete pAdoConnection;
		pAdoConnection = NULL;
	}
	CoUninitialize();
	return 0;
}


void CThreadDataProc::Stop()
{
	if(m_hThread)
	{
		while (ResumeThread(m_hThread) > 0);
		m_bExit = TRUE;
		
		::SetEvent(m_hEventThrdQuit);
		Sleep(10);
		WaitForSingleObject(m_hThread, INFINITE);
		
		ReleaseMutex(m_hEventThrdQuit);
		CloseHandle(m_hEventThrdQuit);
		m_hEventThrdQuit = NULL;
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	
	TRACE("exit");
}

