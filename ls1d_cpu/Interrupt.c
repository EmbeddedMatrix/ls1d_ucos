/*
 * Interrupt.c
 *
 *  Created on: 2014-5-27
 *      Author: xushichao
 */

#include "../include/ls1d.h"
#include "../source/os_cfg.h"
#include "../source/ucos_II.h"

#include "../devices/device.h"


void SOFT_INT()
{
	return ;
}


void TIMER1_INT()
{
	__asm__ volatile(
		"mfc0	$8, $11;\n"
		"li		$9, 0x3E80;\n"
	    "addu	$9, $8, $9;\n"
		"mtc0	$9, $11;\n"   /* write cp0_compare */

		 :::"$8","$9"
	);
	OSTimeTick();
}



/************************************************
 * 注意 因为设计的  唤醒的处理方式，一次 cpu 休眠之后唤醒之后 在 main
 * 函数调用的  WakeWaitInt  WAKEINT_ENABLE之后各种唤醒中断
 * 才会发生，比如说 wake，key，res等。此时尚处于操作系统没有运行起来的
 * 阶段。这个时候调用 puts 是会让程序崩溃的（虽然初始化了串口）切记！！
 * **********************************************/
void WAKER_INT()
{
	/* IntClr of PMU_Command */
	PMU_SecToken = PMU_SecToken_command;
	PMU_CommandW = 0x10000;

	CASE_SET(PMUWAKE);
	return ;
}

/*  key 需要 清除 中断。 这里在中断上下文中无需要进入 临界  区了   */
void KEY_INT()
{
	INT32U 	gpio_in;
	/* IntClr of PMU_Command */
	PMU_SecToken = PMU_SecToken_command;
	PMU_CommandW = 0x20000;

	gpio_in = PMU_GPIO_I;
	MboxKey	= 0;
	if(gpio_in & KEY_OFF)
	{
		MboxKey		|= 1 << 3;
	}
	CASE_SET(KEYWAKE);

	if(OSRunning)
		OSMboxPost(pGMboxKey,&MboxKey);
	return ;
}


/// 修改 Flash_Write 加入判断的，对于已经擦除的掉电保存 fllash  块无需再保存了。 TODO
void BAT_FAIL()
{
	Flash_Write(0xa0001000,	(_u32 *)0xa0001000,0x400);
	return ;
}


void RES_VALID()
{
	INT8U	cop;

	/* IntClr of PMU_Command */
	PMU_SecToken = PMU_SecToken_command;
	PMU_CommandW = 0x80000;

	cop = ((PMU[1]>>9)&0x7);  	//cop

	if(cop == mode_2){
		CASE_SET(TIMEWAKE);
	}else if(cop == mode_3){
		CASE_SET(THSENSWAKE);
	}
	if(OSRunning)
		OSSemPost(pGSemTdcInt);

	return ;
}


void INTC()
{
#if 1
	INT8U IntReg = INT_OUT;

	if(IntReg & 0x04)    					//Uart0
	{
		//debugX(4, "INTC from Uart1...\r\n");
		INT8U temp;
		temp = Uart1_IIR;
		if(temp & 0x02){
			//Tx Int
			//uart1_tx();
		}else if(temp & 0x04){ //Rx int
			//puts("Inf\n");
			InfraredBuf[InfraredCnt ++]	= Uart1_RxData;
			if(InfraredCnt == 19){
				OSSemPost(pGSemInfraRev);
			}
		}
		INT_CLR |= 0x04;
	}
#endif
	return ;
}

void EXINT()
{
	return ;
}

__attribute__((weak)) void IRQ_Exception()
{
	_u32 flag = 0;
	asm volatile("mfc0	%0, $13":"=r" (flag));

	_u32 i;
	for(i=8; i<16; i++)
	{
		if((flag>>i) & 0x1)
		{
			IRQ_HAND process = MY_INT[i-8];
			process();
		}
	}
}


void OS_InitTimer()
{
	__asm__ volatile(
			"mfc0	$8, $9;\n"
			"li		$9, 0x3E80;\n"
			"add	$9, $8, $9;\n"  /* 1 second */
			"mtc0	$9, $11;\n"   /* write cp0_compare */

			"mfc0	$8, $13;\n"
			"li		$9, 0xf7ffffff;\n"
			"and	$8, $8, $9;\n"
			"mtc0	$8, $13;\n"  /* enable cp0_count */

			:::"$8","$9"
			);
}



void ls1d_set_soft_int()
{
	__asm__ volatile(
		"mfc0	$8, $13;\n"
		"li		$9, 0x100;\n"
		"or		$9, $8, $9;\n"
		"mtc0	$9, $13;\n"   /* write cp0_cause */

		:::"$8","$9"
		);
}

