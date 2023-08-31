#include "includes.h"


void InitTdc(void)
{
#if OS_CRITICAL_METHOD == 3
    OS_CPU_SR  cpu_sr;
#endif
	Sonar[0] = 0x80008210;    //8M div 8, 48mA, Fireup, stop2
	Sonar[1] = 0xf0c407a8;     //use Stdc, measure 2, single rise-edge

	//���ײ�
	Sonar[2] = 0x40500000;    //detect first wave, double, single-sample
	Sonar[3] =  0x8003a00;    //set delay, detect first wave
	Sonar[4] =  0x40006a0;    //set delay, from first wave to the first stop

	Thsens[0] = 0xf7;

	//_u32 i;
	//for(i=0; i<6; i++)
	//	printf("sonar[%d] = 0x%x\n", i, Sonar[i]);

	OS_ENTER_CRITICAL();
	PMU_SecToken = PMU_SecToken_chipctrl;
	PMU_ChipCtrl &= ~(1<<4);
	PMU_SecToken = PMU_SecToken_command;
	PMU_Command |= 0x8000;
	OS_EXIT_CRITICAL();
}

void SelectTdc(INT32U mode)
{
	switch(mode){
	case R_TDC:  //rtdc
		break;
	case H_TDC:  //htdc
		break;
	case S_TDC:  //stdc
	default:
		Sonar[1] &= ~0x08080000;
		Sonar[1] |=  0x08000000;  //stdc
		break;
	}
}


void SetTdcMop(INT32U mode)
{
#if OS_CRITICAL_METHOD == 3
    OS_CPU_SR  cpu_sr;
#endif
    OS_ENTER_CRITICAL();
	//cruise mode
	PMU_SecToken = PMU_SecToken_timing;
	PMU_Timing = 0x21014;

	INT32U tmp = PMU_Command & 0xffffff20;
	PMU_SecToken = PMU_SecToken_command;
	PMU_Command = tmp|mode;   		//res
	/// ��ʱ tmp=0x8000 ��mode = 0x10
	/// modeλһ���Ӧ��Command�Ĵ����� MOP λ�� һ��mop����С��7 �ġ�
	/// ���ռĴ���˵������ֻ���� res_valid �ж�ʹ�ܣ� û�д�Ѳ��ģʽ����
	OS_EXIT_CRITICAL();
}



void StartTdc()
{
#if OS_CRITICAL_METHOD == 3
    OS_CPU_SR  cpu_sr;
#endif
	OS_ENTER_CRITICAL();
	PMU_SecToken = PMU_SecToken_command;
	PMU_CommandW = 0x2;
	OS_EXIT_CRITICAL();
}



