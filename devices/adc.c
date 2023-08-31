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
	/**������ѯ adc �����Ƿ���ϣ� ������β���  lcd��ӡ ������ѯ����Ҳ���� 2������ ����˲���Ҫʲô�ر�Ĵ����� ע���ٽ��� ***/
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

/**���� ͨ��0 �ĵ�ѹֵ����16���Ʒ���  0xAAB ��Ӧ�� 2.2V**/
INT32U	adc_test()
{
	INT32U		ret;
	adc_init0();

	ret	= adc_measure0();

	adc_disable0();

	return ret;
}

/**  ����ʱ�̣���ѹ���� 2.7V ��15λ ��λ��   2���Ӻ���ֵ͵�ѹ��־ 14λ��λ�� 16���Ӻ�͵�ѹ��־��ʧ14λ���㡣**/
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



