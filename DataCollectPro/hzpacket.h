#ifndef HZ_PACKET
#define HZ_PACKET

typedef struct hz_sdpacket01
{
	unsigned char a0;//数据头1
	unsigned char a1;//数据头1
	unsigned char addrH;//机井编号高位字节  1
	unsigned char addrL;//机井编号低位字节  1
	unsigned char con;//控制位1
	unsigned char jiaoyan;//crc校验  1
}hz_sdpacket01;

#define HZ_RECVPACKET_LEN01 27

typedef struct hz_rcpacket01
{
	unsigned char addrH;//机井编号高位    1
	unsigned char addrL;//机井编号低位   1
	unsigned char a0;//地址验证      1
	unsigned char a1;//地址验证      1
	unsigned char yiaoce;//遥测位
	unsigned char open;//开关机   1
	unsigned char userno;//用户号  1
	//当前用水量高中低位
	unsigned char waterH;
	unsigned char waterM;
	unsigned char waterL;
	//当前水表底数高中低位
	unsigned char WtNumH;
	unsigned char WtNumM;
	unsigned char WtNumL;
	//剩余水量高中低位
	unsigned char LeftWaterH;
	unsigned char LeftWaterM;
	unsigned char LeftWaterL;
	//未用位
	unsigned char AIH0;
	unsigned char AIL0;
	//未用位
	unsigned char AIH1;
	unsigned char AIL1;
	//未用位
	unsigned char AIH2;
	unsigned char AIL2;
	//未用位
	unsigned char AIH3;
	unsigned char AIL3;
	//未用位
	unsigned char AIH4;
	unsigned char AIL4;
	//校验位
	unsigned char jiaoyan;
	
}hz_rcpacket01;

#define HZ_RECVPACKET_LEN02 50

typedef struct hz_rcpacket02
{
	unsigned char addrH;//机井编号高位		1
	unsigned char addrL;//机井编号低位		2
	unsigned char a0;//地址验证				3
	unsigned char a1;//地址验证				4
	unsigned char yiaoce;//遥测位			5
	unsigned char open;//开关量状态			6
	unsigned char userno;//用户号			7
	//当前用水量高中低位
	unsigned char waterH;//					8
	unsigned char waterM;//					9
	unsigned char waterL;//					10
	//当前水表底数高中低位
	unsigned char WtNumH;//					11
	unsigned char WtNumM;//					12
	unsigned char WtNumL;//					13
	//剩余水量高中低位
	unsigned char LeftWtH;//				14
	unsigned char LeftWtM;//				15
	unsigned char LeftWtL;//				16
	//未用位
	unsigned char AIH0;//					17
	unsigned char AIL0;//					18
	//未用位
	unsigned char AIH1;//					19
	unsigned char AIL1;//					20
	//未用位				
	unsigned char AIH2;//					21
	unsigned char AIL2;//					22
	//未用位
	unsigned char AIH3;//					23
	unsigned char AIL3;//					24
	//未用位
	unsigned char AIH4;//					25
	unsigned char AIL4;//					26
	//当前用水量高字节
	unsigned char waterHH;//				27
	//当前水表底数高字节
	unsigned char WtNumHH;//				28
	//剩余水量高字节
	unsigned char LeftWtHH;//				29
	//电度
	unsigned char PowerL;//					33
	unsigned char PowerM;//					32
	unsigned char PowerH;//					31
	unsigned char PowerHH;//				30
	//A相电压	
	unsigned char AVL;//					35
	unsigned char AVH;//					34
	//B相电压
	unsigned char BVL;//					37
	unsigned char BVH;//					36
	//C相电压
	unsigned char CVL;//					39
	unsigned char CVH;//					38
	//A相电流
	unsigned char AAL;//					42
	unsigned char AAM;//					41
	unsigned char AAH;//					40
	//B相电流
	unsigned char BAL;//					45
	unsigned char BAM;//					44
	unsigned char BAH;//					43
	//C相电流
	unsigned char CAL;//					48
	unsigned char CAM;//					47
	unsigned char CAH;//					46
	//A相状态字
	unsigned char ASH;//					49
	unsigned char ASL;//					50
	//B相状态字
	unsigned char BSH;//					51
	unsigned char BSL;//					52
	//C相状态字
	unsigned char CSH;//					53
	unsigned char CSL;//					54
	//校验位
	unsigned char jiaoyan;//				55
	
}hz_rcpacket02;

struct hz_rcpacket{
	hz_rcpacket()
	{
		memset(u.buffer,0,100);
	}
	static unsigned char bcd_decimal(unsigned char bcd)
	{
		int nRet=0,nX=0,nY=0,nData=0;
		int pow;
		
		pow = 1;

		nX=(bcd & 0x0f);
		nY=((bcd>>4) & 0x0f);
		nData=nY*10+nX;
		nRet+=nData*pow;
		pow*=100;

		return nRet;
	}
	BOOL IsInValid(){return (u.buffer[2]==0&&u.buffer[3]==0&&u.buffer[4]==1);}
	//获取设备号
	long GetDev(){return u.rc2.addrH*256+u.rc2.addrL;}
	//获取用户号
	long GetUserNum(){return u.rc2.userno<=10&&u.rc2.userno>0 ? u.rc2.userno:0;}
	//断路器状态
	long IsRuning(){return u.rc2.open&0x01;}
	//电流开关状态
	long ASwitchState(){return (u.rc2.open&0x08)!=NULL;}
	//门状态
	long IsDoolClosed(){return (u.rc2.open&0x10)==NULL;}
	//交流电故障
	bool IsError(){return (u.rc2.open&0x20)!=NULL;}
	//获取用水量
	double GetUseWt(){return (u.rc2.waterHH*16777216+u.rc2.waterH*65536+u.rc2.waterM*256+u.rc2.waterL)/10.0;}
	//获取总累计
	double GetTotalUse(){return (u.rc2.WtNumHH*16777216+u.rc2.WtNumH*65536+u.rc2.WtNumM*256+u.rc2.WtNumL)/10.0;}
	//获取剩余水量
	double GetLeftWt(){return (u.rc2.LeftWtHH*16777216+u.rc2.LeftWtH*65536+u.rc2.LeftWtM*256+u.rc2.LeftWtL)/10.0;}
	//获取用电量
	double GetPower(){return bcd_decimal(u.rc2.PowerHH)*10000+bcd_decimal(u.rc2.PowerH)*100+bcd_decimal(u.rc2.PowerM)+bcd_decimal(u.rc2.PowerL)/100.0;}
	//获取A相电压
	double GetAV(){return bcd_decimal(u.rc2.AVH)*10+bcd_decimal(u.rc2.AVL)/10.0;}
	//获取B相电压
	double GetBV(){return bcd_decimal(u.rc2.BVH)*10+bcd_decimal(u.rc2.BVL)/10.0;}
	//获取C相电压
	double GetCV(){return bcd_decimal(u.rc2.CVH)*10+bcd_decimal(u.rc2.CVL)/10.0;}
	//获取A相电流
	double GetAA(){
		if(u.rc2.AAH & 0x800000)
		{
			u.rc2.AAH &= 0x800000;
			return -(bcd_decimal(u.rc2.AAH)*100+bcd_decimal(u.rc2.AAM)/10.0+bcd_decimal(u.rc2.AAL)/1000.0);
		}
		return bcd_decimal(u.rc2.AAH)*100+bcd_decimal(u.rc2.AAM)/10.0+bcd_decimal(u.rc2.AAL)/1000.0;}
	//获取B相电流
	double GetBA(){
		if(u.rc2.BAH & 0x800000)
		{
			u.rc2.BAH &= 0x800000;
			return -(bcd_decimal(u.rc2.BAH)*100+bcd_decimal(u.rc2.BAM)/10.0+bcd_decimal(u.rc2.BAL)/1000.0);
		}
		return bcd_decimal(u.rc2.BAH)*100+bcd_decimal(u.rc2.BAM)/10.0+bcd_decimal(u.rc2.BAL)/1000.0;
	}
	//获取C相电流
	double GetCA(){
		if(u.rc2.CAH & 0x800000)
		{
			u.rc2.CAH &= 0x800000;
			return -(bcd_decimal(u.rc2.CAH)*100+bcd_decimal(u.rc2.CAM)/10.0+bcd_decimal(u.rc2.CAL)/1000.0);
		}
		return bcd_decimal(u.rc2.CAH)*100+bcd_decimal(u.rc2.CAM)/10.0+bcd_decimal(u.rc2.CAL)/1000.0;
	}
	//获取A相状态字
	double GetAS(){return (u.rc2.ASH<<8)+u.rc2.ASL;}
	//获取B相状态字
	double GetBS(){return (u.rc2.BSH<<8)+u.rc2.BSL;}
	//获取C相状态字
	double GetCS(){return (u.rc2.CSH<<8)+u.rc2.CSL;}
	//模拟量0
	long GetAI0(){return (u.rc2.AIH0<<8)+u.rc2.AIL0;}
	//模拟量1
	long GetAI1(){return (u.rc2.AIH1<<8)+u.rc2.AIL1;}
	//模拟量2
	long GetAI2(){return (u.rc2.AIH2<<8)+u.rc2.AIL2;}
	//模拟量3
	long GetAI3(){return (u.rc2.AIH3<<8)+u.rc2.AIL3;}
	//模拟量4
	long GetAI4(){return (u.rc2.AIH4<<8)+u.rc2.AIL4;}


	union
	{
		unsigned char buffer[100];
		hz_rcpacket01 rc1;
		hz_rcpacket02 rc2;
	}u;

	
	
	int iLength;
};

#endif