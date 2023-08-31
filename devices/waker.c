#include "includes.h"
#include "device.h"


/**	��  OSInit ֮ǰ���� ��  **/
INT32U  ChkWakeUp(void)
{
	INT32U count;
	if(!(PMU_ChipCtrl & 0xf8000000))			//// ϵͳ���� �ǻ��ѣ���
	{
		CASE_SET(POWERUP);
		PMU_SecToken = PMU_SecToken_chipctrl;
		PMU_ChipCtrl |= 0xf8000000;
		return 0;
	}else{										/// ϵͳ�����ѵ�
		count 		= PMU_Count;						/// ʱ���������ÿ 1/16 ���  1
		count 		&= PMU_Count_RTC_MASK;
		CASE_SET(UNKNOWN);
		return count;
	}
}



/******************************************
 * ϵͳ���ߵĲ������ѵ�ʱ����Ҫ����һ���жϣ��������������ȴ��ж���ɣ�
 * ���� OSInit ֮���������������������ʱstatus�Ĵ���  �ڱ�����֮��û��ʲô�޸�
 * timer ��mask��δ��λ��IEλ��Ϊ0.
 * *************************************/
void	WakeWaitInt()
{
	if(CASE_ISSET(POWERUP)){
		 ;
	}else if(CASE_ISSET(UNKNOWN)){
		WAKEINT_ENABLE;							/// ʹ�� IE ,wake ,key ,res  �� mask λ.  �мǲ���ʹ��timer ��maskλ!!
												/// �ո�ʹ������� �ж�λ���жϻ�����������
		WAKEINT_DISABLE;							/// ��ֹ IE ,wake ,key ,res  �� mask λ
	}
}

/*****************************************************
 * ��  TaskStart ���ö���������������������С� ������Ҫ��ͣһ�£�
 * �ȴ� ���ѵ��ж� ����֮����ܼ�����ǰ��  ����˵  key_int ���� wake_int����ΪҪ�漰�����ж�
 * ���������ǰ�߿��ܻ���Ϊ  OS_ENTER_CRITICAL ���� �ж�  ��ʧ��
 * ���ڴ��������������У����ﲻ����Ϊѭ����ʧ timer �жϣ�
*****************************************************/
INT8U InitWaker()
{
    OS_CPU_SR  cpu_sr;

    SleepEn	= 4;
	if(CASE_ISSET(POWERUP) )					//// ϵͳ���� �ǻ��ѣ���
	{
		SampingMode	= USER;
		MajorMenu	= 1;
		MinorMenu	= 0;
		//InitTimeDate();
		OS_ENTER_CRITICAL();
#if OS_TIME_GET_SET_EN > 0
		OSTime        = 0L;						/// ����һ��ȫ�ֵ�ʱ��
#endif
		PMU_SecToken = PMU_SecToken_command;
		PMU_Command |= 0x1100;					/// �򿪶�ʱ���ѣ�ʹ�� e_Wake �ж�
		ls1d_key_init();
		OS_EXIT_CRITICAL();
		InitTdc();
		SelectTdc(S_TDC);
		puts("power\n");
	}else {										/// ϵͳ�����ѵ�
		if(CASE_ISSET(PMUWAKE)){
			puts("pmu\n");
		}
		if(CASE_ISSET(KEYWAKE) ){				///����  status�Ĵ�����û�� �� key �ж�
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
		if( CASE_ISSET(PMUWAKE)  && CASE_ISSET(KEYWAKE) ){          /// Ӧ����������ͬʱ���Ѷ�������� һ�㲻������ϵͳ����
																	/// �����Ͱ���λ�����Ϊ PMUWAKE �ˡ�
		}
	}
	return 0;
}


/* ������Ϊ  cpu ���ߵ��µ�  OSTime ֹͣ��ʱ��
 * ����ֻ�ܲ���������� -- OSTime
 * ���ڵ�ǰ������  ODTimeDly ״̬�����񣬲�����������ǵĵȴ�ʱ��
 * ����  OSTime ���Ǹ�������,�ᵼ�±Ƚϴ����
 * ��˻���Ҫ��һ��У������
 * */
void	WakeCompen()
{
	///ͨ������������������� ������㣡��
	OSTimeSet(OSTimeGet() + (INT32U)(62.5f * 8000 / TIMERDLY * (WakeUpPmuCnt - SleepPmuCnt)));
}

/// ���ٽ������õ� �ģ�
INT32U	SetPDTime(INT32U PDTime)
{
	INT32U count;
	count = PMU_Count;				/// ʱ���������ÿ 1/16 ���  1
	count &= PMU_Count_RTC_MASK;
	PMU_Compare = count + PDTime;  	/* ÿ 1s ����һ��BT�ж�  PDTime = PowerDownTime */
	return count;
}

/// ���ٽ������õ� �ģ�
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





