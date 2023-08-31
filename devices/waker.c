#include "includes.h"
#include "device.h"


/**	在  OSInit 之前调用 ！  **/
INT32U  ChkWakeUp(void)
{
	INT32U count;
	if(!(PMU_ChipCtrl & 0xf8000000))			//// 系统启动 非唤醒！！
	{
		CASE_SET(POWERUP);
		PMU_SecToken = PMU_SecToken_chipctrl;
		PMU_ChipCtrl |= 0xf8000000;
		return 0;
	}else{										/// 系统被唤醒的
		count 		= PMU_Count;						/// 时间计数器，每 1/16 秒加  1
		count 		&= PMU_Count_RTC_MASK;
		CASE_SET(UNKNOWN);
		return count;
	}
}



/******************************************
 * 系统休眠的并被唤醒的时候需要触发一次中断，在这个函数里面等待中断完成，
 * 调用 OSInit 之后立即调用这个函数，此时status寄存器  在被唤醒之后没有什么修改
 * timer 的mask还未置位，IE位还为0.
 * *************************************/
void	WakeWaitInt()
{
	if(CASE_ISSET(POWERUP)){
		 ;
	}else if(CASE_ISSET(UNKNOWN)){
		WAKEINT_ENABLE;							/// 使能 IE ,wake ,key ,res  的 mask 位.  切记不可使能timer 的mask位!!
												/// 刚刚使能上面的 中断位，中断会立即产生。
		WAKEINT_DISABLE;							/// 禁止 IE ,wake ,key ,res  的 mask 位
	}
}

/*****************************************************
 * 从  TaskStart 调用而来，这个是任务上下文中。 这里需要暂停一下，
 * 等待 唤醒的中断 产生之后才能继续往前走  比如说  key_int 或者 wake_int，因为要涉及到关中断
 * 如果继续往前走可能会因为  OS_ENTER_CRITICAL 导致 中断  丢失。
 * 现在处于任务上下文中，这里不会因为循环丢失 timer 中断！
*****************************************************/
INT8U InitWaker()
{
    OS_CPU_SR  cpu_sr;

    SleepEn	= 4;
	if(CASE_ISSET(POWERUP) )					//// 系统启动 非唤醒！！
	{
		SampingMode	= USER;
		MajorMenu	= 1;
		MinorMenu	= 0;
		//InitTimeDate();
		OS_ENTER_CRITICAL();
#if OS_TIME_GET_SET_EN > 0
		OSTime        = 0L;						/// 这是一个全局的时钟
#endif
		PMU_SecToken = PMU_SecToken_command;
		PMU_Command |= 0x1100;					/// 打开定时唤醒，使能 e_Wake 中断
		ls1d_key_init();
		OS_EXIT_CRITICAL();
		InitTdc();
		SelectTdc(S_TDC);
		puts("power\n");
	}else {										/// 系统被唤醒的
		if(CASE_ISSET(PMUWAKE)){
			puts("pmu\n");
		}
		if(CASE_ISSET(KEYWAKE) ){				///这里  status寄存器并没有 打开 key 中断
			OSMboxPost(pGMboxKey,&MboxKey);
			puts("key\n");
		}
		if( CASE_ISSET(TIMEWAKE)  ||  CASE_ISSET(THSENSWAKE) ){
			OSSemPost(pGSemTdcInt);
			if(CASE_ISSET(TIMEWAKE)){
				puts("res1\n");
			}
			if(CASE_ISSET(THSENSWAKE)){
				puts("res2\n");
			}
		}
		if( CASE_ISSET(PMUWAKE)  && CASE_ISSET(KEYWAKE) ){          /// 应当考虑这种同时唤醒额情况！！ 一般不会引起系统错误！
																	/// 这样就把这次唤醒作为 PMUWAKE 了。
		}
	}
	return 0;
}


/* 补偿因为  cpu 休眠导致的  OSTime 停止的时间
 * 仅仅只能补偿这个变量 -- OSTime
 * 对于当前正处于  ODTimeDly 状态的任务，并不会减少他们的等待时间
 * 这里  OSTime 不是浮点类型,会导致比较大的误差，
 * 因此还需要有一个校正过程
 * */
void	WakeCompen()
{
	///通过反汇编分析这里进行了 浮点计算！！
	OSTimeSet(OSTimeGet() + (INT32U)(62.5f * 8000 / TIMERDLY * (WakeUpPmuCnt - SleepPmuCnt)));
}

/// 在临界区调用的 的！
INT32U	SetPDTime(INT32U PDTime)
{
	INT32U count;
	count = PMU_Count;				/// 时间计数器，每 1/16 秒加  1
	count &= PMU_Count_RTC_MASK;
	PMU_Compare = count + PDTime;  	/* 每 1s 产生一个BT中断  PDTime = PowerDownTime */
	return count;
}

/// 在临界区调用的 的！
void  CpuSleep()
{
	SleepPmuCnt		= PMU_Count;
	SleepPmuCnt		&= PMU_Count_RTC_MASK;
	CASE_ZERO();

	PMU_SecToken = PMU_SecToken_command;
	PMU_CommandW = 0x1;  					//cpu power down
}


void	EnSleep()
{
    OS_CPU_SR  cpu_sr;

	OS_ENTER_CRITICAL();
	SleepEn ++;
	OS_EXIT_CRITICAL();
}


void	DisSleep()
{
    OS_CPU_SR  cpu_sr;

	OS_ENTER_CRITICAL();
	SleepEn --;
	OS_EXIT_CRITICAL();
}





