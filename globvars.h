

/*** 用来 lcd 进程和 tdc进程同步 ***/
OS_EXT	OS_EVENT	*pGSemSynch;


/** 消息邮箱 用来判断按键中断，并且判断按键  按下 还是  松开 **/
OS_EXT	OS_EVENT	*pGMboxKey;
OS_EXT	INT8U		MboxKey;


/** 信号量， 用来表明 tdc 是否发生了中断！ **/
OS_EXT	OS_EVENT	*pGSemTdcInt;

/** 信号量，表明  有一个短按键 ，启动 红外通信 **/
OS_EXT	OS_EVENT	*pGSemKeyToInfra;

/** 信号量，表明  红外接受完毕 **/
OS_EXT	OS_EVENT	*pGSemInfraRev;


/** 表示在  睡眠之前  和 cpu 被唤醒 时候的 PMU_COUNT 数值**/
OS_EXT	INT32U		SleepPmuCnt;
OS_EXT	INT32U		WakeUpPmuCnt;

OS_EXT	INT32U		KeyTime;

 /** 当前 是 上电运行    还是 被wake唤醒    还是被key唤醒  **/
OS_EXT	volatile	RUNCASE		RunReason;

/** 当前采样的模式，是处于用户模式还是检定模式 **/
OS_EXT	volatile	SAMPMODE	SampingMode;


/**  任意时刻，电压低于 2.7V 第15位 置位。   2分钟后出现低电压标志 14位置位。 16分钟后低电压标志消失14位清零。**/
OS_EXT	volatile	INT16U		VoltageFlag;
/*** 电压值 ***/
OS_EXT	INT32U		Voltage;

OS_EXT	INT8U		InfraredBuf[20];
OS_EXT	INT8U		InfraredCnt;

/// 主次菜单， MajorMenu 在 A1,A2,A3,A4 之间切换 取值 1 2 3 4
/// MinorMenu在菜单内部之间分屏。从0开始取值
OS_EXT	volatile	INT8U	MajorMenu,MinorMenu;

OS_EXT	volatile	INT8U	SleepEn;

OS_EXT	volatile	INT8U		year,month,day,hour,minute,second;

////  提前把掉电保存的flash 地址拆除掉。
OS_EXT		INT8U	BatFailErased;

//// 18个月月结的 数组 下标
OS_EXT		INT8U	MonthlySaveIndex;


/// 上次按键（包括长按键，短按键）时刻
OS_EXT		INT32U	LASTKEYPress;
OS_EXT 	volatile	INT8U	KeyTimeOutCnt;


OS_EXT	TDCINFO		GTdcInfo;

///在“检定模式”下，每个流量点检测完毕 10S 后，重新开始计量时，热量累积值自动清零。 TODO  累计热量是不是要定义2个变量
OS_EXT	_fp32		AccumHeat;					///	累积热量（KW·h）
OS_EXT	_fp32		Power;						/// 功率
OS_EXT	_fp32		DammingTemp;				/// 回水温度
OS_EXT	_fp32		DiffTemp;					/// 温差（℃）
OS_EXT	_fp32		InteFlux;					///	累积流量（m3）	integrated flux
OS_EXT	_fp32		InstantFlux;				/// 瞬时流量（m3/h）instantaneous flux
OS_EXT	_fp32		InstantHeat;				/// 瞬时热量		instantaneous heat

OS_EXT	_fp32		InTemp;						/// 进入 温度
OS_EXT	_fp32		OutTemp;					/// 出来温度

//// 　累积运行时间（h）、报警时间（h）；这两个有什么含义？		TODO

/** 这里采用 BCD 码存放 LCD 要 显示 的内容  如果 不掉电 ram 紧张 可以采取压缩 BCD 码的形式 **/
OS_EXT	INT8U		LCDContent[10];


#define		RecodrFlashTbl	((RecordInfo*)RecodrFlashAddr)




/***下面的 一些变量是放在 flash 里面的 ！***/
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






