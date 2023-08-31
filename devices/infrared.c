#include "includes.h"


void Uart1_2400B(void)
{
#if OS_CRITICAL_METHOD == 3
    OS_CPU_SR     cpu_sr;
#endif
    OS_ENTER_CRITICAL();
	PMU_SecToken = PMU_SecToken_chipctrl;
	PMU_ChipCtrl &= ~(1<<3);
	OS_EXIT_CRITICAL();
	/*
	 8bit  1stop  2400Baut  odd parity
	*/

	Uart1_FCR = FIFO_ENABLE|FIFO_RCV_RST|FIFO_XMT_RST|FIFO_TRIGGER_1;
	Uart1_LCR = CFCR_DLAB;
	Uart1_BaudL = 208;  //set 2400Baut
	Uart1_LCR = CFCR_8BITS|CFCR_PENAB|CFCR_PODD;  //8bit, 1stop, odd parity
	Uart1_MCR = MCR_DTR|MCR_RTS;
	Uart1_IER = 0;

	INT_POL    |= 0x04;    //low available
	INT_EN     |= 0x04;		//enable uart1 int

	//debugX(2, "Uart1 init...\n");

	_u8 data;
	data = Uart1_RxData;   //clear the receive fifo
	Uart1_IER  |= 0x01;    //enable uart1 rx int
}


void Infrared1Init( )
{
    PMU_GPIO_OE |= (1 << 10)|(1<<2);  //IRPWR -> GPIO output
    PMU_GPIO_O |= (1 << 10)|(1<<2);  //IRPWR -> GPIO output
	//IR_PWR_ON();

    Uart1_2400B();
    Uart1_MCR |= 0x80; //0xa0;  	//MCR bit7: enable 38k;  bit6: Rx¼«ÐÔ;  bit5: Tx¼«ÐÔ

	memset1d(InfraredBuf,16,0);
	InfraredCnt	= 0;
}


void Infrared1Disable( )
{
    PMU_GPIO_O &= ~((1 << 10)|(1<<2));  //IRPWR -> GPIO output
	//IR_PWR_OFF();

	INT_EN     &= ~0x08;		//disable uart0 int
}

/*void	InfraProc()
{


}*/
