#include "includes.h"


void adc_init0( )
{
#if OS_CRITICAL_METHOD == 3
    OS_CPU_SR  cpu_sr;
#endif
	OS_ENTER_CRITICAL();
	PMU_SecToken = PMU_SecToken_chipctrl;
	PMU_ChipCtrl |= 0x40;
	OS_EXIT_CRITICAL();
	PMU_GPIO_OE |= 0x1000;  //empi output
	PMU_GPIO_O &= ~0x1000;  //empi output '0'
	return ;
}


_u32 adc_measure0()
{
	_u32 result = 0;
	/**这里轮询 adc 测试是否完毕， 经过多次测试  lcd打印 发现轮询次数也就在 2次左右 ，因此不需要什么特别的处理了 注意临界区 ***/
	/* set CR_sel, when a measure finish, the CR_stat should be set to "01" automatically, the sel remains the value */
	ADC_CR = ADC_CR_START;
	while(!(ADC_CR&ADC_CR_HALT));

	result = ((_u32)(ADC_D0&0xf0) >> 4) + ((_u32)ADC_D1 << 4);

	return result;
}


void adc_disable0()
{
	PMU_GPIO_O |= 0x1000;  //empi output '1'

	return ;
}

/**返回 通道0 的电压值，用16进制返回  0xAAB 对应着 2.2V**/
INT32U	adc_test()
{
	INT32U		ret;
	adc_init0();

	ret	= adc_measure0();

	adc_disable0();

	return ret;
}

/**  任意时刻，电压低于 2.7V 第15位 置位。   2分钟后出现低电压标志 14位置位。 16分钟后低电压标志消失14位清零。**/
void	VoltageCtrl(void)
{
	if( ( !(VoltageFlag & (1<<15)) ) && !(VoltageFlag & 1<< 14) ){
		if( (Voltage = adc_test())  < StandVolt){
			VoltageFlag |= 1<<15;
		}
	}else if( VoltageFlag & (1<<15) ){
		VoltageFlag	++;
		if((VoltageFlag & 0xfff) > 120){
			VoltageFlag	|= 1<< 14;
			VoltageFlag	&= ~(1<< 15);
			VoltageFlag	&= 0xf000;
		}
	}else if(VoltageFlag & (1<<14)){
		VoltageFlag	++;
		if((VoltageFlag & 0xfff) > 960){
			VoltageFlag	= 0;
		}
	}
}



