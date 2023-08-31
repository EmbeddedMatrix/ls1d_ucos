

/*** ���� lcd ���̺� tdc����ͬ�� ***/
OS_EXT	OS_EVENT	*pGSemSynch;


/** ��Ϣ���� �����жϰ����жϣ������жϰ���  ���� ����  �ɿ� **/
OS_EXT	OS_EVENT	*pGMboxKey;
OS_EXT	INT8U		MboxKey;


/** �ź����� �������� tdc �Ƿ������жϣ� **/
OS_EXT	OS_EVENT	*pGSemTdcInt;

/** �ź���������  ��һ���̰��� ������ ����ͨ�� **/
OS_EXT	OS_EVENT	*pGSemKeyToInfra;

/** �ź���������  ���������� **/
OS_EXT	OS_EVENT	*pGSemInfraRev;


/** ��ʾ��  ˯��֮ǰ  �� cpu ������ ʱ��� PMU_COUNT ��ֵ**/
OS_EXT	INT32U		SleepPmuCnt;
OS_EXT	INT32U		WakeUpPmuCnt;

OS_EXT	INT32U		KeyTime;

 /** ��ǰ �� �ϵ�����    ���� ��wake����    ���Ǳ�key����  **/
OS_EXT	volatile	RUNCASE		RunReason;

/** ��ǰ������ģʽ���Ǵ����û�ģʽ���Ǽ춨ģʽ **/
OS_EXT	volatile	SAMPMODE	SampingMode;


/**  ����ʱ�̣���ѹ���� 2.7V ��15λ ��λ��   2���Ӻ���ֵ͵�ѹ��־ 14λ��λ�� 16���Ӻ�͵�ѹ��־��ʧ14λ���㡣**/
OS_EXT	volatile	INT16U		VoltageFlag;
/*** ��ѹֵ ***/
OS_EXT	INT32U		Voltage;

OS_EXT	INT8U		InfraredBuf[20];
OS_EXT	INT8U		InfraredCnt;

/// ���β˵��� MajorMenu �� A1,A2,A3,A4 ֮���л� ȡֵ 1 2 3 4
/// MinorMenu�ڲ˵��ڲ�֮���������0��ʼȡֵ
OS_EXT	volatile	INT8U	MajorMenu,MinorMenu;

OS_EXT	volatile	INT8U	SleepEn;

OS_EXT	volatile	INT8U		year,month,day,hour,minute,second;

////  ��ǰ�ѵ��籣���flash ��ַ�������
OS_EXT		INT8U	BatFailErased;

//// 18�����½�� ���� �±�
OS_EXT		INT8U	MonthlySaveIndex;


/// �ϴΰ������������������̰�����ʱ��
OS_EXT		INT32U	LASTKEYPress;
OS_EXT 	volatile	INT8U	KeyTimeOutCnt;


OS_EXT	TDCINFO		GTdcInfo;

///�ڡ��춨ģʽ���£�ÿ������������� 10S �����¿�ʼ����ʱ�������ۻ�ֵ�Զ����㡣 TODO  �ۼ������ǲ���Ҫ����2������
OS_EXT	_fp32		AccumHeat;					///	�ۻ�������KW��h��
OS_EXT	_fp32		Power;						/// ����
OS_EXT	_fp32		DammingTemp;				/// ��ˮ�¶�
OS_EXT	_fp32		DiffTemp;					/// �²�棩
OS_EXT	_fp32		InteFlux;					///	�ۻ�������m3��	integrated flux
OS_EXT	_fp32		InstantFlux;				/// ˲ʱ������m3/h��instantaneous flux
OS_EXT	_fp32		InstantHeat;				/// ˲ʱ����		instantaneous heat

OS_EXT	_fp32		InTemp;						/// ���� �¶�
OS_EXT	_fp32		OutTemp;					/// �����¶�

//// ���ۻ�����ʱ�䣨h��������ʱ�䣨h������������ʲô���壿		TODO

/** ������� BCD ���� LCD Ҫ ��ʾ ������  ��� ������ ram ���� ���Բ�ȡѹ�� BCD �����ʽ **/
OS_EXT	INT8U		LCDContent[10];


#define		RecodrFlashTbl	((RecordInfo*)RecodrFlashAddr)




/***����� һЩ�����Ƿ��� flash ����� ��***/
#ifdef		OS_GLOBALS
const INT8U LCDNumber [] = {
	    0xeb, /* "0" */
	    0X60, /* "1" */
	    0Xc7, /* "2" */
	    0Xe5, /* "3" */
	    0X6c, /* "4" */
	    0Xad, /* "5" */
	    0Xaf, /* "6" */
	    0Xe0, /* "7" */
	    0Xef, /* "8" */
	    0Xed  /* "9" */
};
const 	INT8U MonthDays 	[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
const 	INT8U USERNumber 	[] = {1,8,2,5,3,7,1,4};
const 	INT8U VERNumber 	[] = {1,4,6,1,3,7,1,4};
const	IRQ_HAND MY_INT		[] = {SOFT_INT, TIMER1_INT, WAKER_INT, KEY_INT, BAT_FAIL, RES_VALID, INTC, EXINT};


#else
extern 	const 	INT8U 		LCDNumber[];
extern	const 	INT8U 		VERNumber[];
extern	const 	INT8U 		USERNumber[];
extern	const 	INT8U 		MonthDays[];
extern	const	IRQ_HAND	MY_INT[];
#endif






