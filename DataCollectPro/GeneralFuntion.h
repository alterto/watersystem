#ifndef GENERALFUNCTION
#define GENERALFUNCTION


extern void Output(string strInfo,BOOL bNextLine  = TRUE ,BOOL bSave = FALSE);
extern CString GetSysPath();
extern CAdoConnection *GetConnect(CString strIp,CString strDbName,CString strUserName,CString strPasswd);
extern bool IsValidateStr16(char* str);
extern int StrToHex(char *str,char *hex);
extern int HexToStr(char *hex,char *str);
extern int SplitString(const CString& input, 
							 const CString& delimiter, CStringArray& results);
extern CString GetSystemPath();
extern BOOL GetConfig(CString strSection,CString strKey,CString &strValue,CString strPathName = _T("\\system.cfg"));
extern BOOL GetConfig(CString strSection,CString strKey,int &iValue,CString strPathName = _T("\\system.cfg"));
extern BOOL SetConfig(CString strSection,CString strKey,CString strValue,CString strPathName = _T("\\system.cfg"));
extern BOOL SetConfig(CString strSection,CString strKey,int iValue,CString strPathName = _T("\\system.cfg"));


#endif // !defined(AFX_GENERALFUNTION_H__7C86CEAF_9BA5_4592_9C8F_8318A2A0715D__INCLUDED_)
