// SystemConfig.cpp: implementation of the CSystemConfig class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SystemConfig.h"
#include "GeneralFuntion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSystemConfig::CSystemConfig()
{
	//超时判断间隔,单位分钟
	outlinetime = 0;
	//数据采集间隔
	requiresecond = 0;
	//宏电绑定端口号
	hdport = 0;
	//服务端口号
	serverport = 0;
}

CSystemConfig::~CSystemConfig()
{
}

BOOL CSystemConfig::UpdateData()
{
	do 
	{
		if(!GetConfig(_T("config"),_T("outlinetime"),outlinetime)) break;
		if(!GetConfig(_T("config"),_T("requiresecond"),requiresecond)) break;
		if(!GetConfig(_T("config"),_T("hdport"),hdport)) break;
		if(!GetConfig(_T("config"),_T("serverport"),serverport)) break;
		if(!GetConfig(_T("config"),_T("ip"),ip)) break;
		if(!GetConfig(_T("config"),_T("dbname"),dbname)) break;
		if(!GetConfig(_T("config"),_T("username"),username)) break;
		if(!GetConfig(_T("config"),_T("passwd"),passwd)) break;
		if(!GetConfig(_T("config"),_T("selectsql"),selectsql)) break;
		
		Output();
		return TRUE;
	} while (1);
	
	cout<<"config error!"<<endl;
	return FALSE;
	
}

void CSystemConfig::Output()
{
	cout<<"outlinetime is "<<outlinetime<<endl;//离线时间判断，分钟
	cout<<"data requiresecond is "<<requiresecond<<endl;//数据请求时间
	cout<<"hdport is "<<hdport<<endl;//宏电端口号
	cout<<"serverport is "<<serverport<<endl;//绑定端口号
	cout<<"ip is "<<ip.GetBuffer(0)<<endl;//Ip地址
	cout<<"dbname is "<<dbname.GetBuffer(0)<<endl;//数据库名称
	cout<<"================================================================="<<endl;
}
