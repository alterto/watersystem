#ifndef HZ_PACKET
#define HZ_PACKET

typedef struct hz_sdpacket01
{
	unsigned char a0;//����ͷ1
	unsigned char a1;//����ͷ1
	unsigned char addrH;//������Ÿ�λ�ֽ�  1
	unsigned char addrL;//������ŵ�λ�ֽ�  1
	unsigned char con;//����λ1
	unsigned char jiaoyan;//crcУ��  1
}hz_sdpacket01;

#define HZ_RECVPACKET_LEN01 27

typedef struct hz_rcpacket01
{
	unsigned char addrH;//������Ÿ�λ    1
	unsigned char addrL;//������ŵ�λ   1
	unsigned char a0;//��ַ��֤      1
	unsigned char a1;//��ַ��֤      1
	unsigned char yiaoce;//ң��λ
	unsigned char open;//���ػ�   1
	unsigned char userno;//�û���  1
	//��ǰ��ˮ�����е�λ
	unsigned char waterH;
	unsigned char waterM;
	unsigned char waterL;
	//��ǰˮ��������е�λ
	unsigned char WtNumH;
	unsigned char WtNumM;
	unsigned char WtNumL;
	//ʣ��ˮ�����е�λ
	unsigned char LeftWaterH;
	unsigned char LeftWaterM;
	unsigned char LeftWaterL;
	//δ��λ
	unsigned char AIH0;
	unsigned char AIL0;
	//δ��λ
	unsigned char AIH1;
	unsigned char AIL1;
	//δ��λ
	unsigned char AIH2;
	unsigned char AIL2;
	//δ��λ
	unsigned char AIH3;
	unsigned char AIL3;
	//δ��λ
	unsigned char AIH4;
	unsigned char AIL4;
	//У��λ
	unsigned char jiaoyan;
	
}hz_rcpacket01;

#define HZ_RECVPACKET_LEN02 50

typedef struct hz_rcpacket02
{
	unsigned char addrH;//������Ÿ�λ		1
	unsigned char addrL;//������ŵ�λ		2
	unsigned char a0;//��ַ��֤				3
	unsigned char a1;//��ַ��֤				4
	unsigned char yiaoce;//ң��λ			5
	unsigned char open;//������״̬			6
	unsigned char userno;//�û���			7
	//��ǰ��ˮ�����е�λ
	unsigned char waterH;//					8
	unsigned char waterM;//					9
	unsigned char waterL;//					10
	//��ǰˮ��������е�λ
	unsigned char WtNumH;//					11
	unsigned char WtNumM;//					12
	unsigned char WtNumL;//					13
	//ʣ��ˮ�����е�λ
	unsigned char LeftWtH;//				14
	unsigned char LeftWtM;//				15
	unsigned char LeftWtL;//				16
	//δ��λ
	unsigned char AIH0;//					17
	unsigned char AIL0;//					18
	//δ��λ
	unsigned char AIH1;//					19
	unsigned char AIL1;//					20
	//δ��λ				
	unsigned char AIH2;//					21
	unsigned char AIL2;//					22
	//δ��λ
	unsigned char AIH3;//					23
	unsigned char AIL3;//					24
	//δ��λ
	unsigned char AIH4;//					25
	unsigned char AIL4;//					26
	//��ǰ��ˮ�����ֽ�
	unsigned char waterHH;//				27
	//��ǰˮ��������ֽ�
	unsigned char WtNumHH;//				28
	//ʣ��ˮ�����ֽ�
	unsigned char LeftWtHH;//				29
	//���
	unsigned char PowerL;//					33
	unsigned char PowerM;//					32
	unsigned char PowerH;//					31
	unsigned char PowerHH;//				30
	//A���ѹ	
	unsigned char AVL;//					35
	unsigned char AVH;//					34
	//B���ѹ
	unsigned char BVL;//					37
	unsigned char BVH;//					36
	//C���ѹ
	unsigned char CVL;//					39
	unsigned char CVH;//					38
	//A�����
	unsigned char AAL;//					42
	unsigned char AAM;//					41
	unsigned char AAH;//					40
	//B�����
	unsigned char BAL;//					45
	unsigned char BAM;//					44
	unsigned char BAH;//					43
	//C�����
	unsigned char CAL;//					48
	unsigned char CAM;//					47
	unsigned char CAH;//					46
	//A��״̬��
	unsigned char ASH;//					49
	unsigned char ASL;//					50
	//B��״̬��
	unsigned char BSH;//					51
	unsigned char BSL;//					52
	//C��״̬��
	unsigned char CSH;//					53
	unsigned char CSL;//					54
	//У��λ
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
	//��ȡ�豸��
	long GetDev(){return u.rc2.addrH*256+u.rc2.addrL;}
	//��ȡ�û���
	long GetUserNum(){return u.rc2.userno<=10&&u.rc2.userno>0 ? u.rc2.userno:0;}
	//��·��״̬
	long IsRuning(){return u.rc2.open&0x01;}
	//��������״̬
	long ASwitchState(){return (u.rc2.open&0x08)!=NULL;}
	//��״̬
	long IsDoolClosed(){return (u.rc2.open&0x10)==NULL;}
	//���������
	bool IsError(){return (u.rc2.open&0x20)!=NULL;}
	//��ȡ��ˮ��
	double GetUseWt(){return (u.rc2.waterHH*16777216+u.rc2.waterH*65536+u.rc2.waterM*256+u.rc2.waterL)/10.0;}
	//��ȡ���ۼ�
	double GetTotalUse(){return (u.rc2.WtNumHH*16777216+u.rc2.WtNumH*65536+u.rc2.WtNumM*256+u.rc2.WtNumL)/10.0;}
	//��ȡʣ��ˮ��
	double GetLeftWt(){return (u.rc2.LeftWtHH*16777216+u.rc2.LeftWtH*65536+u.rc2.LeftWtM*256+u.rc2.LeftWtL)/10.0;}
	//��ȡ�õ���
	double GetPower(){return bcd_decimal(u.rc2.PowerHH)*10000+bcd_decimal(u.rc2.PowerH)*100+bcd_decimal(u.rc2.PowerM)+bcd_decimal(u.rc2.PowerL)/100.0;}
	//��ȡA���ѹ
	double GetAV(){return bcd_decimal(u.rc2.AVH)*10+bcd_decimal(u.rc2.AVL)/10.0;}
	//��ȡB���ѹ
	double GetBV(){return bcd_decimal(u.rc2.BVH)*10+bcd_decimal(u.rc2.BVL)/10.0;}
	//��ȡC���ѹ
	double GetCV(){return bcd_decimal(u.rc2.CVH)*10+bcd_decimal(u.rc2.CVL)/10.0;}
	//��ȡA�����
	double GetAA(){
		if(u.rc2.AAH & 0x800000)
		{
			u.rc2.AAH &= 0x800000;
			return -(bcd_decimal(u.rc2.AAH)*100+bcd_decimal(u.rc2.AAM)/10.0+bcd_decimal(u.rc2.AAL)/1000.0);
		}
		return bcd_decimal(u.rc2.AAH)*100+bcd_decimal(u.rc2.AAM)/10.0+bcd_decimal(u.rc2.AAL)/1000.0;}
	//��ȡB�����
	double GetBA(){
		if(u.rc2.BAH & 0x800000)
		{
			u.rc2.BAH &= 0x800000;
			return -(bcd_decimal(u.rc2.BAH)*100+bcd_decimal(u.rc2.BAM)/10.0+bcd_decimal(u.rc2.BAL)/1000.0);
		}
		return bcd_decimal(u.rc2.BAH)*100+bcd_decimal(u.rc2.BAM)/10.0+bcd_decimal(u.rc2.BAL)/1000.0;
	}
	//��ȡC�����
	double GetCA(){
		if(u.rc2.CAH & 0x800000)
		{
			u.rc2.CAH &= 0x800000;
			return -(bcd_decimal(u.rc2.CAH)*100+bcd_decimal(u.rc2.CAM)/10.0+bcd_decimal(u.rc2.CAL)/1000.0);
		}
		return bcd_decimal(u.rc2.CAH)*100+bcd_decimal(u.rc2.CAM)/10.0+bcd_decimal(u.rc2.CAL)/1000.0;
	}
	//��ȡA��״̬��
	double GetAS(){return (u.rc2.ASH<<8)+u.rc2.ASL;}
	//��ȡB��״̬��
	double GetBS(){return (u.rc2.BSH<<8)+u.rc2.BSL;}
	//��ȡC��״̬��
	double GetCS(){return (u.rc2.CSH<<8)+u.rc2.CSL;}
	//ģ����0
	long GetAI0(){return (u.rc2.AIH0<<8)+u.rc2.AIL0;}
	//ģ����1
	long GetAI1(){return (u.rc2.AIH1<<8)+u.rc2.AIL1;}
	//ģ����2
	long GetAI2(){return (u.rc2.AIH2<<8)+u.rc2.AIL2;}
	//ģ����3
	long GetAI3(){return (u.rc2.AIH3<<8)+u.rc2.AIL3;}
	//ģ����4
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