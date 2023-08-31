#include "includes.h"


/// 主次菜单， MajorMenu 在A1,A2,A3,A4之间切换；  MinorMenu在菜单内部之间分屏。
//OS_EXT	INT8U	MajorMenu,MinorMenu;
//OS_EXT	INT8U		LCDContent[8];
//int ftoa(char *str, float num, int n)


extern int ftoa(char *str, _fp32 num, int n);

//// 由于没有统一的文档说明，这里在lcd的8个 位上前面的位显示整数，整数显示完之后显示小数
/// 假设没有负值，在实际应用中也不可能出现负值。  如果有可以用 abs 宏处理一下
/// 如果是小数，则小数点占用LCDContent一个位，而实际上在lcd上小数点不占用一个位，
/// 注意变量 buf 栈溢出的错误，否则！！，另外在ftoa 里面也加入 栈溢出检测机制。 TODO
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

/// 这个函数 涉及到很多浮点计算，  只能被 TaskTDC 任务调用，
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
			case 2:							/// 用户编号，放在flash中
				Number2LCD(USERNumber);
				break;
			case 3:							///版本号 ，放在flash中
				Number2LCD(VERNumber);
				break;
			case 4:
				Float2LCD(1.5f * Voltage * 3.3f / 4096.0f );		/// 显示电压数值？？
				break;
			case 5:			///	屏幕全显
				break;
		}
	}else if( (MajorMenu == 3) && (MinorMenu != 0xff)){						/// 如何控制，一屏是显示不下的。
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






