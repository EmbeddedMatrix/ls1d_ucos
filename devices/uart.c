
#include "includes.h"

void Uart0_38400B(void)
{
#if OS_CRITICAL_METHOD == 3
    OS_CPU_SR     cpu_sr;
#endif
	OS_ENTER_CRITICAL();
	PMU_SecToken = PMU_SecToken_chipctrl;
	PMU_ChipCtrl &= ~(1<<2);
	OS_EXIT_CRITICAL();
	/*
	 8bit  1stop  38400Baut  odd parity
	*/

	Uart0_FCR = FIFO_ENABLE|FIFO_RCV_RST|FIFO_XMT_RST|FIFO_TRIGGER_1;
	Uart0_LCR = CFCR_DLAB;
	Uart0_BaudL = 13;
	Uart0_LCR = CFCR_8BITS|CFCR_PENAB|CFCR_PODD;  //8bit, 1stop, odd parity
	Uart0_MCR = MCR_DTR|MCR_RTS;
	Uart0_IER = 0;
}


void tgt_putchar(_u8 chr)
{
	while(!(Uart0_LSR & 0x20)) ;
	Uart0_TxData = chr;
}

int putchar(long c)
{
	tgt_putchar(c);
	return 0;
}


int puts(char *s)
{
	char c;
	while((c=*s))
	{
		if(c == '\n') putchar('\r');
			putchar(c);
		s++;
	}
	return 0;
}








