/*
 * main.c
 *
 *  Created on: 2014-5-26
 *      Author: xushichao
 */


#include <ls1d.h>
#include "source/os_cfg.h"

#define		OS_GLOBALS
#include "source/ucos_II.h"
#include "devices/device.h"
#include "Func/func.h"


/* 	��os_cfg.h�ļ����涨��ջ�ĵ�ַ�������̶ֹ�ס�����ڵ��� ram �С�  */

/***************************************************
 *��Ӧ�õ��� DisWake����Ϊ��ȷ���û��Ƿ��°�����    �����ܲ�����
 *�Ͳ���cpu���ߣ�ʵ���������ᵼ����������� pend ��ʱ�� ������н��̵���cpu���ߣ���
 *���������ܷ��� pend  1���Ӷ������ߵ�~
 ****************************************************/
void  TaskKey (void *pdata)
{
	while(1) {
		KeyTime		= KeyProc(pGMboxKey,KeyTime);
		OSTimeDly(50 * 8000 / TIMERDLY );		/// 50ms
	}
}


/**TaskLcd ��  TaskTDC �������ȼ� ��ִ���� ��Ҫ��ϸ�����£�
 * ��  TaskTDC ���油����ȫ�ֱ���  OSTime ֮��  ������ҪSystemClock ������
 *  ������lcd����ʾ    **/
void  TaskLcd (void *pdata)
{
	INT8U	err;
	LcdPoweron();
	OSSemPend(pGSemSynch,0,&err);			/// lcd �������ȼ���������Ҫ�ȴ� tdc ���� ������ʱ��֮����� SystemClock ������
	while(1) {
		while(!SystemClock());
		OSTimeDly(100 * 8000/TIMERDLY);
	}
}


/** �������  Ҫ�������ĸ�����㣬 ����ϵͳֻ��һ���������ʹ�ø���Ĵ��� **/
void  TaskTDC (void *pdata)
{
	DisSleep();
	WakeCompen();
	OSSemPost(pGSemSynch);
	//if(CapaciDetec(pGSemTdcInt,CAPADETEC)){
		////		������󲿷ּ�ⶼ�Ǵ���ģ�����	TODO
	//}
	while(1) {
		TdcSampProc(pGSemTdcInt);

		Float2LCDBuf();
		PrintLcd();

		EnSleep();
		OSTimeDly(20 * 8000/TIMERDLY);
		DisSleep();
	}
}

void TaskInfra (void *pdata)
{
	INT8U		err;

	OSSemPend(pGSemKeyToInfra,0,&err);
	while(1){
		DisSleep();
		while(1){
			Infrared1Init();
			OSSemPend(pGSemInfraRev,30 * 8000000/ TIMERDLY,&err);
			if(err == OS_TIMEOUT){
				puts("tout");
				Infrared1Disable();
				EnSleep();
				break;
			}else if(err == OS_NO_ERR){
				//InfraProc();
				//puts("received");
				InfraredBuf[19] = 0;
				puts((char *)InfraredBuf);
			}
		}
		OSTimeDly(300 * 8000/TIMERDLY);		/// 300ms
	}
}

static  void  TaskStartCreateTasks (void)
{
    OSTaskCreate(TaskKey, (void *)0, (OS_STK *)OSTASKSKEY, 5);
    OSTaskCreate(TaskLcd, (void *)0, (OS_STK *)OSTASKSLCD, 6);
    OSTaskCreate(TaskTDC, (void *)0, (OS_STK *)OSTASKSTDC, 10);
    OSTaskCreate(TaskInfra, (void *)0, (OS_STK *)OSTASKSINFRA, 9);
}


/**********************************************************************************************************
*                                              STARTUP TASK
**********************************************************************************************************/
void  TaskStart (void *pdata)
{
	Lcd_HW_Init();
	Uart0_38400B();

	/** �����  InitWaker  �õ��� pGMboxKey  pGSemTdcInt ������Ҫ��ǰ������**/
	pGMboxKey		= OSMboxCreate(NULL);
	pGSemTdcInt		= OSSemCreate(0);

	InitWaker();

	pGSemKeyToInfra	= OSSemCreate(0);
	pGSemInfraRev	= OSSemCreate(0);
	pGSemSynch		= OSSemCreate(0);

    TaskStartCreateTasks();                                /* Create all the application tasks         */

    for (;;) {
    	//if(BatFailErased <8){
    	//	Flash_Erase_page((_u32)(BATFailAddr + (BatFailErased << 7)));
    	//	BatFailErased ++;
    	//}
    	OSTimeDly(300 * 8000/TIMERDLY );
    }
}


/* ÿ�� ϵͳ�����������񶼻�ִ�����������  ��δ��� ���������cpu�����ǿ��Եģ��������� ֻҪ���� ������ ������Ҫ��Щ�ж�����
 * ����������������������ã���˿϶����޷��������ġ� OSTimeDly �� Pend ֮��ĺ����޷����ã�
 * �������������ʱ�� �Ѿ����� �ٽ����ˣ����������  ENTER_CRITICAL �ˣ����Ӧ�������򵥣�
 * ��� ���������ж�һ��ȫ�ֱ��� WakeEn ��������������ͺܿ��˳��ˣ�������ϾͰ�cpu���ߡ�
 * */
void	OSTaskIdleHook()
{
    if( SleepEn == 4){
		if( CASE_ISSET(PMUWAKE) || CASE_ISSET(POWERUP) ){					// ֻҪ�Ǳ�wake���ѵģ������ϵ�������һ�Ρ���˯���㹻ʱ��
			if(SampingMode	== USER){										// ��  key ����,�� res ���ѵ�cpu�����������ִ���ڼ䷢���� wake �жϣ�
				SetPDTime(USERWAKE);										// Ҳ�������濼��֮�ڵ�
			}else if(SampingMode	== DOCIM){
				SetPDTime(DOCIMWAKE);
			}
		}else if( CASE_ISSET(KEYWAKE) ||  CASE_ISSET(TIMEWAKE) || CASE_ISSET(THSENSWAKE) ){
				/// DO Nothing
		}
		CpuSleep();
	}
}

/**main ���� һ�㲻����ᣬ����Ĵ��������� TaskStartCreateTasks ������ɵ�**/
int main()
{
	WakeUpPmuCnt	= ChkWakeUp();

	OSInit();

	WakeWaitInt();

	OSTaskCreate(TaskStart, (void *)0, (OS_STK *)OSTASKSTARTSTK , 0);

	OSStart();                                             /* Start multitasking */

	/*��ѭ��*/
	while(1)
	{
		/// printf("can't reach here!!\n");
	}
	return 0;
}

