#include "stdafx.h"
#include "GeneralFuntion.h"

void Output(string strInfo,BOOL bNextLine /* = TRUE */,BOOL bSave)
{
	cout<<">>"<<strInfo;
	if(bNextLine)
	{
		cout<<endl;
	}
}

CString GetSysPath()
{
	CString strPath;
	TCHAR path[MAX_PATH];
	GetModuleFileName(NULL, path, MAX_PATH);
	strPath = path;
	int nPos = strPath.ReverseFind('\\');
	strPath = strPath.Left(nPos);
	return strPath;
}

CAdoConnection *GetConnect(CString strIp,CString strDbName,CString strUserName,CString strPasswd)
{
	CAdoConnection *pAdoConnection = NULL;
	CString strCfgFiePath;
	BOOL bRet = FALSE;
	pAdoConnection = new CAdoConnection;
	ASSERT(pAdoConnection);
	//连接数据库
	bRet = pAdoConnection->ConnectSQLServer(strIp, strDbName, strUserName, strPasswd);
	if (!bRet)
	{
		delete pAdoConnection;
		pAdoConnection = NULL;
	}
	
	return pAdoConnection;
}

bool IsValidateStr16(char* str)
{
       if (NULL == str)
       {
          return false;
       }

       int len = strlen(str);
       for (int i = 0;i < len;i++)
       {

              if (!(((str[i] >= '0') && (str[i] <= '9'))
                     || ((str[i] >= 'A') && (str[i] <= 'F'))
                     || ((str[i] >= 'a') && (str[i] <= 'f'))))

                     return false;

       }

       return true;
}

int StrToHex(char *str,char *hex)
{
	int i,slen;

	slen=strlen(str);
	for (i=0;i<slen;i++)
	{
		sprintf(hex + i * 3, "%02X ", str[i] & 0xFF);
	}
	return 0;
}

int HexToStr(char *hex,char *str)
{
	int i,hlen,iData,iFlag;
	char ch;
	int len = 0;

	iData=0;
	iFlag=0;
	hlen=strlen(hex);

	for (i=0;i<hlen;i++)
	{
		if (' '==hex[i]) 
			continue;

		ch=hex[i];

		if ((ch>='0') && (ch<='9'))
		{
			ch-=0x30;
		}
		else if ((ch>='a') && (ch<='f'))
		{
			ch-=0x57;
		}
		else if ((ch>='A') && (ch<='F'))
		{
			ch-=0x37;
		}
		else
		{
			*str='\0';
		    return -1;
		}

		if (0==iFlag)
		{
			iData=ch;
			iFlag=1;
		}
		else
		{
			iData=(iData<<4) +ch;
			iFlag=0;
			*str++=iData;
			len++;
		}
	}
	*str='\0';
	return len;
}

int SplitString(const CString& input, 
							 const CString& delimiter, CStringArray& results)
{
	int iPos = 0;
	int newPos = -1;
	int sizeS2 = delimiter.GetLength();
	int isize = input.GetLength();
	
	CArray<INT, INT> positions;
	
	newPos = input.Find (delimiter, 0);
	
	if( newPos < 0 ) { return 0; }
	
	int numFound = 0;
	
	while( newPos > iPos )
	{
		numFound++;
		positions.Add(newPos);
		iPos = newPos;
		newPos = input.Find (delimiter, iPos+sizeS2+1);
	}
	
	for( int i=0; i <= positions.GetSize(); i++ )
	{
		CString s;
		if( i == 0 )
			s = input.Mid( i, positions[i] );
		else
		{
			int offset = positions[i-1] + sizeS2;
			if( offset < isize )
			{
				if( i == positions.GetSize() )
					s = input.Mid(offset);
				else if( i > 0 )
					s = input.Mid( positions[i-1] + sizeS2, 
					positions[i] - positions[i-1] - sizeS2 );
			}
		}
		if( s.GetLength() > 0 )
			results.Add(s);
	}
	return results.GetSize();
}

CString GetSystemPath()
{
	CString strPath;
	TCHAR path[MAX_PATH];
	GetModuleFileName(NULL, path, MAX_PATH);
	strPath = path;
	int nPos = strPath.ReverseFind('\\');
	strPath = strPath.Left(nPos);
	return strPath;
}

BOOL GetConfig(CString strSection,CString strKey,CString &strValue,CString strPathName /* = _T("system.cfg") */)
{
	TCHAR buffer[100] = {0};
	if(GetPrivateProfileString(strSection,strKey,_T(""),buffer,MAX_PATH,GetSystemPath()+strPathName)<=0) return FALSE;
	strValue = buffer;
	return TRUE;
}

BOOL GetConfig(CString strSection,CString strKey,int &iValue,CString strPathName /* = _T("system.cfg") */)
{
	TCHAR buffer[100] = {0};
	GetPrivateProfileString(strSection,strKey,_T(""),buffer,MAX_PATH,GetSystemPath()+strPathName);
	if(GetPrivateProfileString(strSection,strKey,_T(""),buffer,MAX_PATH,GetSystemPath()+strPathName)<=0) return FALSE;
	iValue = _ttoi(buffer);
	return TRUE;
}

BOOL SetConfig(CString strSection,CString strKey,CString strValue,CString strPathName /* = _T("system.cfg") */)
{
	return WritePrivateProfileString(strSection,strKey,strValue,GetSystemPath()+strPathName)>0;
}

BOOL SetConfig(CString strSection,CString strKey,int iValue,CString strPathName /* = _T("system.cfg") */)
{
	CString strValue;
	strValue.Format(_T("%d"),iValue);
	return WritePrivateProfileString(strSection,strKey,strValue,GetSystemPath()+strPathName)>0;
}