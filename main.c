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


/* 	在os_cfg.h文件里面定义栈的地址，用数字固定住，放在掉电 ram 中。  */

/***************************************************
 *不应该调用 DisWake，因为不确定用户是否按下按键。    不可能不按键
 *就不让cpu休眠，实际上这样会导致这个任务在 pend 的时候 进入空闲进程导致cpu休眠！！
 *几乎不可能发生 pend  1分钟而不休眠的~
 ****************************************************/
void  TaskKey (void *pdata)
{
	while(1) {
		KeyTime		= KeyProc(pGMboxKey,KeyTime);
		OSTimeDly(50 * 8000 / TIMERDLY );		/// 50ms
	}
}


/**TaskLcd 跟  TaskTDC 任务优先级 和执行流 需要仔细安排下，
 * 在  TaskTDC 里面补偿了全局变量  OSTime 之后  ，还需要SystemClock 更新下
 *  才能在lcd上显示    **/
void  TaskLcd (void *pdata)
{
	INT8U	err;
	LcdPoweron();
	OSSemPend(pGSemSynch,0,&err);			/// lcd 进程优先级高所以需要等待 tdc 进程 补偿了时间之后才做 SystemClock 操作。
	while(1) {
		while(!SystemClock());
		OSTimeDly(100 * 8000/TIMERDLY);
	}
}


/** 这个任务  要做大量的浮点计算， 整个系统只有一个任务可以使用浮点寄存器 **/
void  TaskTDC (void *pdata)
{
	DisSleep();
	WakeCompen();
	OSSemPost(pGSemSynch);
	//if(CapaciDetec(pGSemTdcInt,CAPADETEC)){
		////		这个绝大部分检测都是错误的！！！	TODO
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

	/** 后面的  InitWaker  用到了 pGMboxKey  pGSemTdcInt 所以需要提前创建！**/
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


/* 每次 系统来到空闲任务都会执行这个函数！  如何处理？ 在这里加入cpu掉电是可以的，但不可能 只要进入 函数就 掉电需要有些判断条件
 * 这个函数被空闲任务所调用，因此肯定是无法被阻塞的。 OSTimeDly 和 Pend 之类的函数无法调用！
 * 调用这个函数的时候 已经进入 临界区了，因此无需再  ENTER_CRITICAL 了，这个应当尽量简单，
 * 这个 函数仅仅判断一下全局变量 WakeEn 如果不符合条件就很快退出了，如果符合就把cpu休眠。
 * */
void	OSTaskIdleHook()
{
    if( SleepEn == 4){
		if( CASE_ISSET(PMUWAKE) || CASE_ISSET(POWERUP) ){					// 只要是被wake唤醒的，或者上电启动的一次。就睡眠足够时间
			if(SampingMode	== USER){										// 被  key 唤醒,被 res 唤醒的cpu，并且在这次执行期间发生了 wake 中断！
				SetPDTime(USERWAKE);										// 也在这里面考虑之内的
			}else if(SampingMode	== DOCIM){
				SetPDTime(DOCIMWAKE);
			}
		}else if( CASE_ISSET(KEYWAKE) ||  CASE_ISSET(TIMEWAKE) || CASE_ISSET(THSENSWAKE) ){
				/// DO Nothing
		}
		CpuSleep();
	}
}

/**main 函数 一般不用理会，任务的创建都是在 TaskStartCreateTasks 里面完成的**/
int main()
{
	WakeUpPmuCnt	= ChkWakeUp();

	OSInit();

	WakeWaitInt();

	OSTaskCreate(TaskStart, (void *)0, (OS_STK *)OSTASKSTARTSTK , 0);

	OSStart();                                             /* Start multitasking */

	/*主循环*/
	while(1)
	{
		/// printf("can't reach here!!\n");
	}
	return 0;
}

