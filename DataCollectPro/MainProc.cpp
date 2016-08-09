// MainProc.cpp: implementation of the CMainProc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataCollectPro.h"
#include "MainProc.h"
#include "myserver.h"
#include "HzFormat.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
stack<WellInfo> g_stWellInfo;
void CALLBACK CallBackFunc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	CMainProc *pDataPtr = (CMainProc*)dwUser;
	ASSERT(pDataPtr);
	if(g_stWellInfo.empty())
	{
		pDataPtr->m_pAdoRecordSet->Requery();
		int iCounts = pDataPtr->m_pAdoRecordSet->GetRecordCount();
		if(iCounts<=0) return;
		static bool flag = 0;
		if(!flag)
		{
			timeKillEvent(pDataPtr->m_iTimeID);
			pDataPtr->m_iTimeID = ::timeSetEvent(pDataPtr->m_sysConfig.requiresecond/iCounts, 1, CallBackFunc, (DWORD)dwUser, TIME_PERIODIC); //开启计时器
			flag=!flag;
		}
	
		pDataPtr->m_pAdoRecordSet->MoveFirst();
		
		CString strCezhanID;
		INT iDevID;
		while(!pDataPtr->m_pAdoRecordSet->IsEOF())
		{
			pDataPtr->m_pAdoRecordSet->GetCollect(_T("CezhanID"),strCezhanID);
			pDataPtr->m_pAdoRecordSet->GetCollect(_T("DevID"),iDevID);
			if(!strCezhanID.IsEmpty())
				g_stWellInfo.push(WellInfo(strCezhanID,iDevID));
			pDataPtr->m_pAdoRecordSet->MoveNext();
		}
	}
	
	do{
		if(!g_stWellInfo.empty())
		{
			WellInfo wellInfo = g_stWellInfo.top();
			g_stWellInfo.pop();
			user_info ui;
			time_t t_now,t_update;
			uint8 *userid = (unsigned char *)LPCTSTR(wellInfo.strCezhanID);
			if(0!=get_user_info(userid,&ui))	break;
			pDataPtr->m_pAdoCommand->GetParameter(0)->PutValue(_variant_t((char*)userid));
			if (1==ui.m_status)  //DTU在线
			{
				//判断超时
				t_now=time(NULL);
				t_update=*((time_t *)ui.m_update_time);
				time_t iTimeSec = t_now-t_update;
				BOOL bRet = iTimeSec>=(60*pDataPtr->m_sysConfig.outlinetime);
				
				if(bRet)
				{
					//开发包函数,使DTU下线
					//string str = ui.m_userid;
					//str+="已下线!";
					//Output(str);
					do_close_one_user2((unsigned char *)ui.m_userid,NULL);
					
					pDataPtr->m_pAdoCommand->GetParameter(1)->PutValue(_variant_t(long(0)));
				}else
				{
					//请求
					int iH = wellInfo.iDevID/256;
					int iL = wellInfo.iDevID%256;
					
					int crc = 0;
					crc^=iH;
					crc^=iL;
					crc^=01;
					
					char strBuf[1024];
					CString strSend;
					strSend.Format(_T("00 00 %02x %02x 01 %02x"),iH,iL,crc);
					int len = HexToStr((char*)LPCTSTR(strSend),strBuf);		
					if(0==do_send_user_data(userid,	(unsigned char *)strBuf,
						len, NULL))
					{
						pDataPtr->m_pAdoCommand->GetParameter(1)->PutValue(_variant_t(long(1)));
					}else
						pDataPtr->m_pAdoCommand->GetParameter(1)->PutValue(_variant_t(long(0)));
					
					Sleep(100);
					/*
					HZFORMAT hzformat;
					hzformat.CreateFlowGetCode();	
					if(0==do_send_user_data(userid,	(unsigned char *)hzformat.buf,
						hzformat.iLen, NULL))
					{
						pDataPtr->m_pAdoCommand->GetParameter(1)->PutValue(_variant_t(long(1)));
					}else
						pDataPtr->m_pAdoCommand->GetParameter(1)->PutValue(_variant_t(long(0)));
					*/
				}
			}
			void *pRet = pDataPtr->m_pAdoCommand->Execute();
			if(!pRet)
			{
				cout<<pDataPtr->m_pAdoRecordSet->GetConnection()->GetLastErrorText().GetBuffer(100)<<endl;
			}
		}
	}while(FALSE);
}	
//////////////////////////////////////////////////////////////////////////

CMainProc::CMainProc()
{
	m_pServer = NULL;
	m_iTimeID = NULL;
	m_pAdoRecordSet = NULL;
	m_pAdoCommand = NULL;
	m_pAdoConnection = NULL;
}

CMainProc::~CMainProc()
{
}

BOOL CMainProc::Init()
{
	do 
	{
		//更新配置文件
		if(!m_sysConfig.UpdateData()) break;
		//连接数据库
		CoInitialize(NULL);
		m_pAdoConnection=::GetConnect(m_sysConfig.ip,m_sysConfig.dbname,m_sysConfig.username,m_sysConfig.passwd);
		if(!m_pAdoConnection) 
		{
			::Output(_T("Database conneting failed!"));
			break;
		}
		
		//数据库相关
		m_pAdoRecordSet = new CAdoRecordSet;
		m_pAdoRecordSet->SetAdoConnection(m_pAdoConnection);
		m_pAdoRecordSet->SetCursorLocation();
		if(!m_pAdoRecordSet->Open(m_sysConfig.selectsql)){
			Output(_T("Ado recordset error!"));
			return FALSE;
		}
		
		m_pAdoCommand = new CAdoCommand;
		m_pAdoCommand->SetConnection(m_pAdoConnection);
		m_pAdoCommand->SetCommandText(_T("prUpdateDtuState"));
		_ParameterPtr paramPtr = NULL;
		
		
		_variant_t var;
		var.vt = VT_EMPTY;
		paramPtr = m_pAdoCommand->CreateParameter(_T("CezhanID"),adVarChar,adParamInput,30,var);
		m_pAdoCommand->Append(paramPtr);
		paramPtr = m_pAdoCommand->CreateParameter(_T("NetState"),adInteger,adParamInput,4,var);
		m_pAdoCommand->Append(paramPtr);
	
		//启动服务
		m_pServer = new CMyServer;
		if(!m_pServer->Start(m_sysConfig.serverport))
		{
			::Output(_T("Server start failed!"));
			break;
		}
		
		//启动线程
		m_thrdDataProc.m_pMainProc = this;
		m_thrdDataProc.Start();
		
		//启动宏电服务
		if(!StartDtu()) break;
		
		//状态刷新定时器
		m_iTimeID = ::timeSetEvent(1000, 1, CallBackFunc, (DWORD)this, TIME_PERIODIC); //开启计时器
		
		
		return TRUE;
	} while (1);
	
	return FALSE;
}

void CMainProc::CoUnInit()
{
	//关闭服务
	if(m_pServer)
	{
		m_pServer->Shutdown();
		TRACE("Server exist!");
		delete m_pServer;
		m_pServer = NULL;
	}
	//关闭定时器
	timeKillEvent(m_iTimeID);
	//关闭DTU
	StopDtu();
	//关闭线程
	m_thrdDataProc.Stop();

	if(m_pAdoRecordSet)
	{
		delete m_pAdoRecordSet;
		m_pAdoRecordSet = NULL;
	}
	
	if(m_pAdoCommand)
	{
		delete m_pAdoCommand;
		m_pAdoCommand = NULL;
	}
	
	if(m_pAdoConnection)
	{
		m_pAdoConnection->Close();
		delete m_pAdoConnection;
		m_pAdoConnection = NULL;
	}
	CoUninitialize();
}

BOOL CMainProc::StartDtu()
{
	char mess[512];
	//开发包函数,指定服务IP
	//SetCustomIP(inet_addr((char *)LPCTSTR()));
	//开发包函数,设置服务模式:2-消息,1-非阻塞,0-阻塞
	SetWorkMode(1);
	//开发包函数,设置服务类型:0-UDP,1-TCP
	SelectProtocol(0);
	//开发包函数,启动服务
	int iResult = start_net_service(NULL,0,m_sysConfig.hdport,mess);
	cout<<mess<<endl;
	//启动服务失败
	if (iResult!=0)
	{
		return FALSE;
	}
	return TRUE;
}

void CMainProc::StopDtu()
{
	stop_net_service(NULL);
}

void CMainProc::Run()
{
	if(!Init()) 
	{
		CoUnInit();
		return;
	}
	//命令行
	TCHAR command[100] = {0};
	while(gets(command))
	{
		CString strCode(command);
		if(strCode.IsEmpty()) continue;
		CString strCommand;
		CStringArray arrCommand;
		int iParams = 1;
		if(strCode.Find(_T(" "),0)<0) 
		{
			strCommand = strCode;
		}else{
			iParams = ::SplitString(strCode,_T(" "),arrCommand);
			strCommand = arrCommand[0];
		}
		
		if(strCommand== "exit")
		{
			break;
		}else if(strCommand == "kill_proc")
		{
			m_thrdDataProc.Stop();Output("ThreadDataProc have exited!!");
		}else if(strCommand == "start_proc")
		{
			if(!m_thrdDataProc.m_bExit)
			{
				Output("Thread already start.");
			}else{
				m_thrdDataProc.Start();
				Output("ThreadDataProc have started!!");
			}
		}else if(strCommand == "print_online")
		{
			PrintOnline();
		}else if(strCommand == "test")
		{
			if(iParams!=2)
				Output("test command need one param.Please use help command.");
			else
				PrintOnline(arrCommand[1]);
		}
		else if(strCommand == "help")
		{
			PrintCommand();
		}
		else
		{
			Output("error command!!");
		}
		
		memset(command,0,100);
	} 
	
	CoUnInit();
}

void CMainProc::PrintOnline()
{
	Output(_T("================================"));
	int i,iMaxDTUAmount;
	user_info ui;
	memset(&ui,0,sizeof(ui));
	time_t t_now,t_update;
	//获取开发包支持的最大连接数
	CString strSql;
	iMaxDTUAmount=get_max_user_amount();
	for (i=0;i<iMaxDTUAmount;i++)
	{
		ui.m_status=0;
		//获取指定位置的DTU信息
		get_user_at(i,&ui);
		if (1==ui.m_status)
		{			
			//取当前系统时间
			t_now=time(NULL);
			//取m_update_time的时间
			t_update=*((time_t *)ui.m_update_time);
			//用当前时间比较最后一次更新的时间
			if ((t_now-t_update)>=m_sysConfig.outlinetime*60)
			{
				do_close_one_user((unsigned char *)ui.m_userid,NULL);
			}else
			{
				Output(ui.m_userid);
			}
		}
	}
}

void CMainProc::PrintOnline(LPCTSTR lpszdtuid)
{
	ASSERT(lpszdtuid);
	Output(lpszdtuid,FALSE);
	user_info ui;
	memset(&ui,0,sizeof(ui));
	time_t t_now,t_update;
	ui.m_status=0;
	//获取指定位置的DTU信息
	get_user_info((uint8*)lpszdtuid,&ui);
	
	if (1==ui.m_status)
	{			
		//取当前系统时间
		t_now=time(NULL);
		//取m_update_time的时间
		t_update=*((time_t *)ui.m_update_time);
		//用当前时间比较最后一次更新的时间
		if ((t_now-t_update)>=m_sysConfig.outlinetime*60)
		{
			do_close_one_user((unsigned char *)ui.m_userid,NULL);
		}else
		{
			cout<<_T(" online.")<<endl;
			return;
		}
	}
	cout<<_T(" outline.")<<endl;
}

void CMainProc::PrintCommand()
{
	Output(_T("================================"));
	Output(_T("print_online -- 打印在线DTU号码."));
	Output(_T("print_online dtuid -- 测试dtuid是否在线."));
	Output(_T("start_proc -- 启动接收线程."));
	Output(_T("kill_proc -- 关闭接收线程."));
	Output(_T("exit -- 退出."));
}