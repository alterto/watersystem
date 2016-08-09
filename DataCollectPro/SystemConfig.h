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
	//超时判断间隔,单位分钟
	int outlinetime;
	//数据采集间隔
	int requiresecond;
	//宏电绑定端口号
	int hdport;
	//服务端口号
	int serverport;
	//服务器IP
	CString ip;
	//数据库名称
	CString dbname;
	//用户名
	CString username;
	//密码
	CString passwd;
	//查询语句
	CString selectsql;
};

#endif // !defined(AFX_SYSTEMCONFIG_H__3AA14E4F_FAD6_4A22_8160_331D149444F1__INCLUDED_)
