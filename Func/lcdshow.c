#include "includes.h"


/****release ģʽ�� ������ ���Ż���һ�κ������� ���� ****/
void	PrintTime(INT8U *buf)
{
	memset1d((void *)buf,0,12);
/**/
	buf[7]	= LCDNumber[(Hex2Bcd(hour)&0xf0)>>4];
	buf[8]	= LCDNumber[Hex2Bcd(hour)&0x0f];

	buf[4]	= LCDNumber[(Hex2Bcd(minute)&0xf0)>>4];
	buf[5]	= LCDNumber[Hex2Bcd(minute)&0x0f];

	buf[1]	= LCDNumber[(Hex2Bcd(second)&0xf0)>>4];
	buf[2]	= LCDNumber[Hex2Bcd(second)&0x0f];
}


void	PrintLCDFloat(INT8U *lcdbuf)
{
	INT8U	DecimalIndex,i;

	DecimalIndex	= memchar1d(LCDContent,9,'.');
	if( (DecimalIndex == 0) || (DecimalIndex == 9) ){								/// û��С����
		for(i = 1;i <= 8;i ++){
			lcdbuf[i]	= LCDNumber[LCDContent[i-1] - '0'];			/// ��� '0' ����ȥ���� TODO
		}
	}else if(DecimalIndex == 2){						/// ����ֻ��1������ΪlcdС����С����ӵڶ�λ��ʼ���������λ�� 0
		lcdbuf[1]	= LCDNumber[0];
		lcdbuf[2]	= LCDNumber[LCDContent[0] - '0'];
		for(i = 3;i <= 8;i ++){
			lcdbuf[i]	=  LCDNumber[LCDContent[i-1] - '0'];
		}
	}else {
		for(i = 1;i <= DecimalIndex-1;i ++){
			lcdbuf[i]	= LCDNumber[LCDContent[i-1] - '0'];
		}
		for(i = DecimalIndex ; i <= 8; i ++){
			lcdbuf[i]	= LCDNumber[LCDContent[i] - '0'];
		}
	}
	if((DecimalIndex == 3) || (DecimalIndex == 2)){
		*((INT32U *)&lcdbuf[0])	|= 1<<12;
	}else if(DecimalIndex == 4){
		*((INT32U *)&lcdbuf[0])	|= 1<<28;
	}else if((DecimalIndex >= 5) && (DecimalIndex <= 8)){
		*((INT32U *)&lcdbuf[4])	|= 1 << (4  + ((DecimalIndex - 5) << 3) );
	}
}


/**����������֮����ܻ�ܴ� ��
 * ��Ȼ��������ܴ󣬵��Ƕ��Ƿ�ִ֧�е�  ������ʱ��Ӧ�ò��࣡
 * ���������Ӧ��ʹ�ø�����㣡 	**/
void PrintLcd(void)
{
	//INT8U	DecimalIndex,i;
	INT8U	lcdbuf[12];

	memset1d(lcdbuf,0,12);
	if( (MajorMenu == 1) || (MajorMenu == 4) ){
		PrintLCDFloat(lcdbuf);
		switch(MinorMenu) {
		case 0:
			*((INT32U *)&lcdbuf[0])	|= 1<<20;
			break;
		case 1:							/// ���� ��û�к���
			break;
		case 2:							/// ����ˮ�¶ȣ��棩
			*((INT32U *)&lcdbuf[8])	|= (1<<9) | (1<<15);
			break;
		case 3:							///	�²�棩
			*((INT32U *)&lcdbuf[8])	|= (1<<9) | (1<<15);
			break;
		case 4:							/// �ۻ�������m3��
			*((INT32U *)&lcdbuf[0])	|= 1<<20;
			*((INT32U *)&lcdbuf[8])	|= 1<<16;
			break;
		case 5:							/// ˲ʱ������m3/h��
			*((INT32U *)&lcdbuf[8])	|= (1<<8) | (1<<16) | (1<<17) | (1<<18);
			break;
		}
		if( (MajorMenu == 4) && (MinorMenu == 6) ){
			*((INT32U *)&lcdbuf[8])	|= 1<<8;
		}
	}else if(MajorMenu == 2){
		if( (MinorMenu >= 0) && (MinorMenu <= 3) ){
			memcpy1d(&lcdbuf[1],LCDContent, 8);
			if(MinorMenu == 0){
				*((INT32U *)&lcdbuf[8])	|= 1<<11;
			}else if(MinorMenu == 1)
				*((INT32U *)&lcdbuf[8])	|= 1<<10;
		}else if(MinorMenu == 4){
			PrintLCDFloat(lcdbuf);
		}else if(MinorMenu == 5){
			memset1d(lcdbuf,0xff,12);
		}
	}else if( (MajorMenu == 3) && (MinorMenu != 0xff)){
		if((MinorMenu %3) == 0){
			memcpy1d(&lcdbuf[1],LCDContent, 8);
			*((INT32U *)&lcdbuf[8])	|= 1<<10;
		}else if((MinorMenu %3) == 1){
			PrintLCDFloat(lcdbuf);
		}else if((MinorMenu %3) == 2){
			PrintLCDFloat(lcdbuf);
		}
	}
	if(MinorMenu == 0xff){
		memcpy1d(&lcdbuf[1],LCDContent, 8);
	}
	if(MajorMenu == 4){																/// ��ʾ�춨ģʽ
		*((INT32U *)&lcdbuf[8])	|= 1<<13;
	}
	if(VoltageFlag & (1<<14)){														/// ��ʾ�͵�ѹ
		*((INT32U *)&lcdbuf[8])	|= 1<<4;
	}
	WriteLcdRegs(lcdbuf);
}
