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
 * ע�� ��Ϊ��Ƶ�  ���ѵĴ���ʽ��һ�� cpu ����֮����֮�� �� main
 * �������õ�  WakeWaitInt  WAKEINT_ENABLE֮����ֻ����ж�
 * �Żᷢ��������˵ wake��key��res�ȡ���ʱ�д��ڲ���ϵͳû������������
 * �׶Ρ����ʱ����� puts �ǻ��ó�������ģ���Ȼ��ʼ���˴��ڣ��мǣ���
 * **********************************************/
void WAKER_INT()
{
	/* IntClr of PMU_Command */
	PMU_SecToken = PMU_SecToken_command;
	PMU_CommandW = 0x10000;

	CASE_SET(PMUWAKE);
	return ;
}

/*  key ��Ҫ ��� �жϡ� �������ж�������������Ҫ���� �ٽ�  ����   */
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


/// �޸� Flash_Write �����жϵģ������Ѿ������ĵ��籣�� fllash  �������ٱ����ˡ� TODO
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

