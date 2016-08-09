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
	//��ʱ�жϼ��,��λ����
	outlinetime = 0;
	//���ݲɼ����
	requiresecond = 0;
	//���󶨶˿ں�
	hdport = 0;
	//����˿ں�
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
	cout<<"outlinetime is "<<outlinetime<<endl;//����ʱ���жϣ�����
	cout<<"data requiresecond is "<<requiresecond<<endl;//��������ʱ��
	cout<<"hdport is "<<hdport<<endl;//���˿ں�
	cout<<"serverport is "<<serverport<<endl;//�󶨶˿ں�
	cout<<"ip is "<<ip.GetBuffer(0)<<endl;//Ip��ַ
	cout<<"dbname is "<<dbname.GetBuffer(0)<<endl;//���ݿ�����
	cout<<"================================================================="<<endl;
}
