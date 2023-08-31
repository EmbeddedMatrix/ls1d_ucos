#include "includes.h"


/// ���β˵��� MajorMenu ��A1,A2,A3,A4֮���л���  MinorMenu�ڲ˵��ڲ�֮�������
//OS_EXT	INT8U	MajorMenu,MinorMenu;
//OS_EXT	INT8U		LCDContent[8];
//int ftoa(char *str, float num, int n)


extern int ftoa(char *str, _fp32 num, int n);

//// ����û��ͳһ���ĵ�˵����������lcd��8�� λ��ǰ���λ��ʾ������������ʾ��֮����ʾС��
/// ����û�и�ֵ����ʵ��Ӧ����Ҳ�����ܳ��ָ�ֵ��  ����п����� abs �괦��һ��
/// �����С������С����ռ��LCDContentһ��λ����ʵ������lcd��С���㲻ռ��һ��λ��
/// ע����� buf ջ����Ĵ��󣬷��򣡣���������ftoa ����Ҳ���� ջ��������ơ� TODO
void	Float2LCD(_fp32 AccumHeat)
{
	INT8U	buf[32];

	ftoa((char *)buf, AccumHeat, 8);
	memcpy1d(LCDContent,buf, 9);
}


void	TimeDate2LCD(INT8U	first,INT8U second,INT8U third)
{
	memset1d(LCDContent,0,8);

	LCDContent[6]	= LCDNumber[third/10];
	LCDContent[7]	= LCDNumber[third%10];

	LCDContent[3]	= LCDNumber[second/10];
	LCDContent[4]	= LCDNumber[second%10];

	LCDContent[0]	= LCDNumber[first/10];
	LCDContent[1]	= LCDNumber[first%10];
}

void	Number2LCD(const INT8U *pNumber)
{
	INT8U	i;
	for(i = 0;i < 8;i ++){
		LCDContent[i]	= LCDNumber[pNumber[i]];
	}
}

/// ������� �漰���ܶม����㣬  ֻ�ܱ� TaskTDC ������ã�
void	Float2LCDBuf(void)
{
	if( (MajorMenu == 1) || (MajorMenu == 4) ){
		switch(MinorMenu) {
			case 0:
				Float2LCD(AccumHeat);
				break;
			case 1:
				Float2LCD(Power);
				break;
			case 2:
				Float2LCD(DammingTemp);
				break;
			case 3:
				Float2LCD(DiffTemp);
				break;
			case 4:
				Float2LCD(InteFlux);
				break;
			case 5:
				Float2LCD(InstantFlux);
				break;
		}
		if( (MajorMenu == 4) && (MinorMenu == 6) ){
			Float2LCD(InstantHeat);
		}
	}else if(MajorMenu == 2){
		switch(MinorMenu) {
			case 0:
				TimeDate2LCD(second,minute,hour);
				break;
			case 1:
				TimeDate2LCD(day,month,year);
				break;
			case 2:							/// �û���ţ�����flash��
				Number2LCD(USERNumber);
				break;
			case 3:							///�汾�� ������flash��
				Number2LCD(VERNumber);
				break;
			case 4:
				Float2LCD(1.5f * Voltage * 3.3f / 4096.0f );		/// ��ʾ��ѹ��ֵ����
				break;
			case 5:			///	��Ļȫ��
				break;
		}
	}else if( (MajorMenu == 3) && (MinorMenu != 0xff)){						/// ��ο��ƣ�һ������ʾ���µġ�
		INT8U	index;
		if(MonthlySaveIndex == 0){
			index	= 17 - MinorMenu/3;
		}else if(MonthlySaveIndex -1 >= MinorMenu/3){
			index = MonthlySaveIndex -1 - MinorMenu/3;
		}else {
			index = MonthlySaveIndex -1 - MinorMenu/3 + 18;
		}
		if((MinorMenu %3) == 0){
			TimeDate2LCD(28,RecodrFlashTbl[index].month,RecodrFlashTbl[index].year);
		}else if((MinorMenu %3) == 1){
			Float2LCD(RecodrFlashTbl[index].heat);
		}else if((MinorMenu %3) == 2){
			Float2LCD(RecodrFlashTbl[index].flux);
		}
	}
	if(MinorMenu == 0xff){
		memset1d(LCDContent,LCDNumber[MajorMenu],8);
	}
}






