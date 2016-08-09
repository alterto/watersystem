#pragma once
struct HZFORMAT
{
	HZFORMAT()
	{
		memset(buf,0,1024);
		iLen = 0;
	}
	int GetLength()
	{
		return iLen;
	}
	//获取数据
	char *GetData()
	{
		return buf;
	}
	//校验
	void JianYan(char *data,int ilen)
	{
		data[ilen-1] = 0;
		for(int iIndex = 0;iIndex<ilen-1;iIndex++)
		{
			data[ilen-1]^=data[iIndex];
		}
	}
	//创建关机命令包
	bool CreateOpenCloseCode(bool bShutdown = 1)
	{
		int iIndex = 0;
		char data[] = {0x00,0x00,0x00,0x00,0x07,0x01,0x00,0xA0,bShutdown,0x00};
		JianYan(data,10);
		for(iIndex = 0;iIndex<10;iIndex++)
		{
			buf[iIndex] = data[iIndex];
		}
		iLen = 10;
		return true;
	}
	//创建流量设定
	bool CreateFlowSetCode(short iFlow = 0)
	{
		int iIndex = 0;
		union{
			short data;
			char chdata[2];
		}u;

		u.data = iFlow;

		char data[] = {00, 00, 00, 00, 07, 02, 00, 0xB4, u.chdata[0], 00, 0xB5, u.chdata[1], 00};
		JianYan(data,13);

		for(iIndex = 0;iIndex<13;iIndex++)
		{
			buf[iIndex] = data[iIndex];
		}

		iLen = 13;
		return true;
	}

	//创建流量获取
	bool CreateFlowGetCode()
	{
		int iIndex = 0;
		char data[] = {00, 00, 00, 00, 06, 02, 00, 0xB4,00, 0xB5,00};
		JianYan(data,11);
		
		for(iIndex = 0;iIndex<11;iIndex++)
		{
			buf[iIndex] = data[iIndex];
		}
		
		iLen = 11;
		return true;
	}
	
	char buf[1024];
	int iLen;
};
