// SystemConfig.h: interface for the CSystemConfig class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSTEMCONFIG_H__3AA14E4F_FAD6_4A22_8160_331D149444F1__INCLUDED_)
#define AFX_SYSTEMCONFIG_H__3AA14E4F_FAD6_4A22_8160_331D149444F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSystemConfig  
{
public:
	CSystemConfig();
	virtual ~CSystemConfig();

	BOOL UpdateData();
	void Output();

public:
	//��ʱ�жϼ��,��λ����
	int outlinetime;
	//���ݲɼ����
	int requiresecond;
	//���󶨶˿ں�
	int hdport;
	//����˿ں�
	int serverport;
	//������IP
	CString ip;
	//���ݿ�����
	CString dbname;
	//�û���
	CString username;
	//����
	CString passwd;
	//��ѯ���
	CString selectsql;
};

#endif // !defined(AFX_SYSTEMCONFIG_H__3AA14E4F_FAD6_4A22_8160_331D149444F1__INCLUDED_)
