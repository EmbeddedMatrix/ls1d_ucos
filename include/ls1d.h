/*
 * ls1d.h
 *	Only used for LS1D chip.
 *
 *  Created on: 2013-1-10
 *      Author: liming
 */

#ifndef LS1D_H_
#define LS1D_H_

/*
*********************************************************************************************************
*                                               uC/OS-II
*                                        The Real-Time Kernel
*
*                         (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
*                                          All Rights Reserved
*
*                                       80x86/80x88 Specific code
*                                          LARGE MEMORY MODEL
*
*                                          Borland C/C++ V4.51
*
* File         : OS_CPU.H
* By           : Jean J. Labrosse
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                              DATA TYPES
*                                         (Compiler Specific)
*********************************************************************************************************
*/
typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;                    /* Unsigned  8 bit quantity                           */
typedef signed   char  INT8S;                    /* Signed    8 bit quantity                           */
typedef unsigned short int   INT16U;                   /* Unsigned 16 bit quantity                           */
typedef signed   short int   INT16S;                   /* Signed   16 bit quantity                           */
typedef unsigned int  	INT32U;                   /* Unsigned 32 bit quantity                           */
typedef signed   int  	INT32S;                   /* Signed   32 bit quantity                           */
typedef float          FP32;                     /* Single precision floating point                    */
typedef double         FP64;                     /* Double precision floating point                    */

typedef INT32U   		OS_STK;                   /* Each stack entry is 32-bit wide                    */
typedef INT32U 			OS_CPU_SR;                /* Define size of CPU status register 32 bits			*/

#define BYTE           INT8S                     /* Define data types for backward compatibility ...   */
#define UBYTE          INT8U                     /* ... to uC/OS V1.xx.  Not actually needed for ...   */
#define WORD           INT16S                    /* ... uC/OS-II.                                      */
#define UWORD          INT16U
#define LONG           INT32S
#define ULONG          INT32U

typedef char _s8;
typedef unsigned char _u8;

typedef short _s16;
typedef unsigned short _u16;

typedef int _s32;
typedef unsigned int _u32;

typedef float _fp32;

void EnableWake(void);
void EnableInt(void);
void EnableIntC(void);

#define  OS_STK_GROWTH        1
#define  OS_TASK_SW()			OSCtxSw()

INT8U  OSTickDOSCtr;

#define  OS_CRITICAL_METHOD    3

#if      OS_CRITICAL_METHOD == 3
#define  OS_ENTER_CRITICAL()  (cpu_sr = OSCPUSaveSR())    /* Disable interrupts                        */
#define  OS_EXIT_CRITICAL()   (OSCPURestoreSR(cpu_sr))    /* Enable  interrupts                        */
#endif

#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
OS_CPU_SR  OSCPUSaveSR(void);
void       OSCPURestoreSR(OS_CPU_SR cpu_sr);
#endif

typedef	enum	{USER, DOCIM }  SAMPMODE;

typedef	enum	{UNKNOWN = (1 << 0),POWERUP = (1 << 1),PMUWAKE = (1 << 2), KEYWAKE = (1 << 3),
				TIMEWAKE = (1 << 4),THSENSWAKE = (1 << 5)  } RUNCASE;

#define			CASE_SET(x)		(RunReason |= x)
#define			CASE_ISSET(x)	(RunReason & x)
#define			CASE_ZERO()		(RunReason = 0)


typedef		INT8U	(*pFuncFloat)(void);


//中断处理函数类型定义
typedef void (*IRQ_HAND) (void);

#define WAKEINT_ENABLE	\
	do{  												\
		__asm__ volatile(								\
					"mfc0	$8, $12;\n"					\
					"li		$9, 0xffff00fe;\n"			\
					"and	$10, $8, $9;\n"				\
					"li		$9, 0x2c01;\n"				\
					"or		$8, $10, $9;\n"				\
					"mtc0	$8, $12;\n"					\
														\
					:::"$8","$9","$10"					\
					);									\
}while(0);

#define WAKEINT_DISABLE	\
	do{ 											\
		__asm__ volatile(							\
				"mfc0	$8, $12;\n"					\
				"li		$9, 0xffffd3fe;\n"			\
				"and	$10, $8, $9;\n"				\
				"mtc0	$10, $12;\n"				\
													\
				:::"$8","$9","$10"					\
				);									\
}while(0);


void SOFT_INT();
void TIMER1_INT();
void WAKER_INT();
void KEY_INT();
void BAT_FAIL();
void RES_VALID();
void INTC();
void EXINT();

/********************************LS1D Chip Option*****************************************************/
#define		PMU		((INT32U*)0xbfeb0000)
#define		Sonar	((INT32U*)0xbfeb4000)
#define		Thsens	((INT32U*)0xbfeb8000)

#define S_TDC	2
#define R_TDC	1
#define H_TDC	0

#define mode_0   0    //once   time   measure
#define mode_1   1    //once   thsens measure
#define mode_2   2    //twice  time   measure
#define mode_3   3    //twice  thsens measure
#define mode_4   4    //tdc    cal
#define mode_5   5    //clkhs  cal
#define mode_6   6    //cap    detection

#define	SINGLETIME		(0 << 2)
#define	SINGLETHSENS	(1 << 2)

#define	DOUBLETIME		(2 << 2)
#define	DOUBLETHSENS	(3 << 2)

#define	CAPADETEC		(6 << 2)

typedef struct TDC_DATA{
	_fp32 ptime[4];  //pt1~4
	_fp32 ftime1;  //fire_up
	_fp32 ftime2;  //fire_down
	_fp32 time_delta;
	_u32 cal;    //calibration
} TDCDATA;

typedef struct _TDCINFO_{
	_u32 tdc_mode;
	_u32 mrange;
	_u32 disautocal;
	_u32 hitin1;
	_u32 tdc_opt;
	TDCDATA tdc_data;
	_u32 result[16];   				//原始数据
	_fp32 res[16];      			//处理后的数据
	_u32 current_ptr0;
	_u32 current_ptr1;
	_u32 last_ptr0;
	_u32 last_ptr1;
	_u32 res_num;    				//正确的结果个数
	_u32 change_flow_direction;  	//交换流量测量方向
	_u32 sonar_PulseDef;
	_u32 sonar_ResPtr;
	_u32 mop;
	_u32 err;
} TDCINFO;


/*** 这个结构体数据成员是不是还不够？  对应文档上如何扩充？ **/
typedef	struct	RecordInfo_{
	INT8U	year;
	INT8U	month;
	_fp32	heat;
	_fp32	flux;
}RecordInfo;


#define FLASH_PAGE_SIZE		128

/********************************LS1D Address Space********************************/

#define UNCACHED_MEMORY_ADDR 	0xa0000000
#define UNCACHED_TO_PHYS(x)     ((x) & 0x1fffffff)
#define PHYS_TO_UNCACHED(x)     ((x) | UNCACHED_MEMORY_ADDR)

#define SRAM1_BASEADDR			PHYS_TO_UNCACHED(0x0)          //sram, sleep
#define SRAM2_BASEADDR			PHYS_TO_UNCACHED(0x1000)          //sram, no sleep
#define SPI_FLASH_BASEADDR		PHYS_TO_UNCACHED(0x1e000000)    //spi flash
#define FLASH_BASEADDR			PHYS_TO_UNCACHED(0x1f000000)   //spi/nand/lpc
#define BOOT_ADDR				PHYS_TO_UNCACHED(0x1fc00000)    //spi/flash
#define FLASH_REG_BASEADDR		PHYS_TO_UNCACHED(0x1fe60000)     //flash regs
#define SPI_REG_BASEADDR		PHYS_TO_UNCACHED(0x1fe70000)     //spi regs
#define UART0_BASEADDR  		PHYS_TO_UNCACHED(0x1fe80000)
#define UART1_BASEADDR			PHYS_TO_UNCACHED(0x1fe88000)
#define I2C_BASEADDR			PHYS_TO_UNCACHED(0x1fe90000)
#define INTC_REGS_BASEADDR		PHYS_TO_UNCACHED(0x1fea0000)   //Interrupt_Regs_Baseaddr
#define ADC_BASEADDR			PHYS_TO_UNCACHED(0x1fea8000)   //ADC regs

#define PMU_BASEADDR			PHYS_TO_UNCACHED(0x1feb0000)
#define SONAR_BASEADDR			PHYS_TO_UNCACHED(0x1feb4000)
#define THSENS_BASEADDR			PHYS_TO_UNCACHED(0x1feb8000)

#define SLCD_REG_BASEADDR		PHYS_TO_UNCACHED(0x1febc000)

/******************************** REGS ********************************/
#define SET_REG(REG, VAL)	(REG = VAL)	
#define READ_REG(REG, VAL)	(VAL = RAG)	


/********************************PMU REGS********************************/
#define PMU_Timing    	*(volatile _u32 *)(PMU_BASEADDR)
#define PMU_Command    	*(volatile _u32 *)(PMU_BASEADDR+0x04)
#define PMU_Compare    	*(volatile _u32 *)(PMU_BASEADDR+0x08)

#define PMU_ChipCtrl  	*(volatile _u32 *)(PMU_BASEADDR+0x0C)
#define PMU_GPIO_OE    	*(volatile _u32 *)(PMU_BASEADDR+0x10)
#define PMU_GPIO_O  	*(volatile _u32 *)(PMU_BASEADDR+0x14)
#define PMU_GPIO_I   	*(volatile _u32 *)(PMU_BASEADDR+0x18)

#define PMU_Count 		*(volatile _u32 *)(PMU_BASEADDR+0x1c)

#define PMU_UserDat0	*(volatile _u32 *)(PMU_BASEADDR+0x20)
#define PMU_UserDat1    *(volatile _u32 *)(PMU_BASEADDR+0x24)
#define PMU_UserDat2    *(volatile _u32 *)(PMU_BASEADDR+0x28)
#define PMU_UserDat3    *(volatile _u32 *)(PMU_BASEADDR+0x2c)
#define PMU_UserDat4    *(volatile _u32 *)(PMU_BASEADDR+0x30)

#define PMU_AutoSave    *(volatile _u32 *)(PMU_BASEADDR+0x34)
#define PMU_PowerCfg    *(volatile _u32 *)(PMU_BASEADDR+0x38)
#define PMU_CommandW 	*(volatile _u32 *)(PMU_BASEADDR+0x3c)

#define PMU_Exint0 		*(volatile _u32 *)(PMU_BASEADDR+0x40)   //external interrupt
#define PMU_Exint1 		*(volatile _u32 *)(PMU_BASEADDR+0x44)   //external interrupt

#define PMU_Pulse0 		*(volatile _u32 *)(PMU_BASEADDR+0x48)   //pulse output
#define PMU_Pulse1 		*(volatile _u32 *)(PMU_BASEADDR+0x4c)   //pulse output
#define PMU_WdtFeed 	*(volatile _u32 *)(PMU_BASEADDR+0x50)   
#define PMU_SecToken 	*(volatile _u32 *)(PMU_BASEADDR+0x7c)   

/*** PMU_Timing ***/
#define PMU_Timing_StartCLKHS_OFFSET		2
#define PMU_Timing_StartCLKHS_MASK		0x7
#define PMU_Timing_HZ60_OFFSET			5
#define PMU_Timing_HZ60_MASK			0x1
#define PMU_Timing_CalResNum_OFFSET		6
#define PMU_Timing_CalResNum_MASK		0x3
#define PMU_Timing_CycleTOF_OFFSET		8
#define PMU_Timing_CycleTOF_MASK		0x3
#define PMU_Timing_CyleTemp_OFFSET		10
#define PMU_Timing_CyleTemp_MASK		0x3
#define PMU_Timing_CruiseTOF_OFFSET		12
#define PMU_Timing_CruiseTOF_MASK		0x3
#define PMU_Timing_CruiseTemp_OFFSET		14
#define PMU_Timing_CruiseTemp_MASK		0x3
#define PMU_Timing_CruiseX_OFFSET		16
#define PMU_Timing_CruiseX_MASK			0x1
#define PMU_Timing_CycleZero_OFFSET		17
#define PMU_Timing_CycleZero_MASK		0x1

/*** PMU_Command ***/
#define PMU_Command_Sleep_OFFSET		0
#define PMU_Command_Sleep_MASK			0x1
#define PMU_Command_StartOP_OFFSET		1
#define PMU_Command_StartOP_MASK		0x1
#define PMU_Command_MOP_OFFSET			2
#define PMU_Command_MOP_MASK			0x7
#define PMU_Command_WatchEn_OFFSET		5
#define PMU_Command_WatchEn_MASK		0x1
#define PMU_Command_Cruisemode_OFFSET		6
#define PMU_Command_Cruisemode_MASK		0x3
#define PMU_Command_WakeEn_OFFSET		8
#define PMU_Command_WakeEn_MASK			0x1
#define PMU_Command_COP_OFFSET			9
#define PMU_Command_COP_MASK			0x7
#define PMU_Command_IntEn_OFFSET		12
#define PMU_Command_IntEn_MASK			0xf
#define PMU_Command_IntSrc_OFFSET		16
#define PMU_Command_IntSrc_MASK			0x1f
#define PMU_Command_ExtintEn_OFFSET		21
#define PMU_Command_ExtintEn_MASK		0x1

#define PMU_Command_WdtEn_OFFSET		29
#define PMU_Command_WdtEn_MASK			0x1

#define PMU_Command_ResRead_OFFSET		31
#define PMU_Command_ResRead_MASK		0x1

/*** PMU_Compare ***/
#define PMU_Compare_WakeCmp_MASK	0xfffff

/*** PMU_ChipCtrl ***/
#define PMU_ChipCtrl_spi_gpio		(1<<0)
#define PMU_ChipCtrl_i2c_gpio		(1<<1)
#define PMU_ChipCtrl_uart0_gpio		(1<<2)
#define PMU_ChipCtrl_uart1_gpio		(1<<3)
#define PMU_ChipCtrl_pt34_gpio		(1<<4)
#define PMU_ChipCtrl_adci0_ien		(1<<5)
#define PMU_ChipCtrl_empi_gpio		(1<<6)
#define PMU_ChipCtrl_keyi_gpio		(1<<7)
#define PMU_ChipCtrl_lcd_en		(1<<8)
#define PMU_ChipCtrl_lcd_pd		(1<<9)
#define PMU_ChipCtrl_debug_out		(1<<10)
#define PMU_ChipCtrl_spi_startu	p	(1<<11)
#define PMU_ChipCtrl_empi_pd		(1<<12)
#define PMU_ChipCtrl_empi_pu		(1<<13)
#define PMU_ChipCtrl_adci0_pd		(1<<14)
#define PMU_ChipCtrl_adci0_pu		(1<<15)

#define PMU_ChipCtrl_sonar_pd		(1<<17)
#define PMU_ChipCtrl_spilp_off		(1<<18)
#define PMU_ChipCtrl_batdet_sel_MASK	0x3
#define PMU_ChipCtrl_batdet_sel_OFFSET	19
#define PMU_ChipCtrl_exstart_sel	(1<<21)
#define PMU_ChipCtrl_lcd_gpio_MASK	0xf
#define PMU_ChipCtrl_lcd_gpio_OFFSET	22
#define PMU_ChipCtrl_adc_lp0en		(1<<26)

#define PMU_ChipCtrl_soft_flag_MASK	0x1f
#define PMU_ChipCtrl_soft_flag_OFFSET	27

/*** PMU_GPIO_OE ***/

/*** PMU_GPIO_O ***/

/*** PMU_GPIO_I ***/

/*** PMU_Count ***/
#define PMU_Count_RTC_MASK	0xfffff
#define PMU_Compare_RTC_MASK	0xfffff

#define PMU_Count_Wdt_MASK	0xffff

/*** PMU_UserDatx ***/

/*** PMU_AutoSave ***/
#define PMU_AutoSave_fmback_en		0x1
#define PMU_AutoSave_fmaddrp_MASK	0xfff0
#define PMU_AutoSave_fmaddrn_MASK	0xfff00000

/*** PMU_PowerCfg ***/
#define PMU_PowerCfg_force_stb		0x1
#define PMU_PowerCfg_abstrim_MASK	0x1f
#define PMU_PowerCfg_abstrim_OFFSET	24
#define PMU_PowerCfg_tctrim_MASK	0x7
#define PMU_PowerCfg_tctrim_OFFSET	28

/*** PMU_Exint0 ***/
#define PMU_Exint0_en_MASK		0xffff
#define PMU_Exint0_pol_OFFSET		16
#define PMU_Exint0_pol_MASK		0xffff

/*** PMU_Exint1 ***/
#define PMU_Exint1_src_MASK		0xffff
#define PMU_Exint1_edge_OFFSET		16
#define PMU_Exint1_edge_MASK		0xffff

/*** PMU_Pulse0 ***/
#define PMU_Pulse0_pulse_div_MASK		0xffff
#define PMU_Pulse0_clk_sel_OFFSET		16
#define PMU_Pulse0_enable_OFFSET		17

/*** PMU_Pulse1 ***/
#define PMU_Pulse1_pulse_div_MASK		0xffff
#define PMU_Pulse1_clk_sel_OFFSET		16
#define PMU_Pulse1_enable_OFFSET		17

/*** PMU_WdtFeed ***/
#define PMU_WdtFeed_cmd		0xa55a55aa

/*** PMU_SecToken ***/
#define PMU_SecToken_timing		0x5f439e29
#define PMU_SecToken_command	0xcf216e95
#define PMU_SecToken_chipctrl	0x15daed27
#define PMU_SecToken_wdtfeed	0x69e33916


/******************************SONAR REGS********************************/
#define SONAR_PulseDef        *(volatile _u32 *)(SONAR_BASEADDR)
#define SONAR_SonarCtrl0      *(volatile _u32 *)(SONAR_BASEADDR+0x04)
#define SONAR_SonarCtrl1      *(volatile _u32 *)(SONAR_BASEADDR+0x08)
#define SONAR_GateDef1        *(volatile _u32 *)(SONAR_BASEADDR+0x0C)
#define SONAR_GateDef2        *(volatile _u32 *)(SONAR_BASEADDR+0x10)
#define SONAR_GateDef3        *(volatile _u32 *)(SONAR_BASEADDR+0x14)
#define SONAR_WaveW_Cur       *(volatile _u32 *)(SONAR_BASEADDR+0x18)
#define SONAR_WaveW_Last      *(volatile _u32 *)(SONAR_BASEADDR+0x1C)
#define SONAR_ResPtr          *(volatile _u32 *)(SONAR_BASEADDR+0x20)

#define SONAR_Result          *(volatile _u32 *)(SONAR_BASEADDR+0x40)
//Result[15:0]	SONAR_BASEADDR + [0x40 ~ 0x7c]

/*** SONAR_PulseDef ***/
#define SONAR_PulseDef_FireNum_MASK		0x7f
#define SONAR_PulseDef_FireNum_OFFSET		0
#define SONAR_PulseDef_FireDiv_MASK		0x3f
#define SONAR_PulseDef_FireDiv_OFFSET		7
#define SONAR_PulseDef_FireCurr_MASK		0x1
#define SONAR_PulseDef_FireCurr_OFFSET		13
#define SONAR_PulseDef_FireSel_MASK		0x1
#define SONAR_PulseDef_FireSel_OFFSET		14
#define SONAR_PulseDef_StopSel_MASK		0x1
#define SONAR_PulseDef_StopSel_OFFSET		15
#define SONAR_PulseDef_FirePhase_MASK		0x7fff
#define SONAR_PulseDef_FirePhase_OFFSET		16
#define SONAR_PulseDef_Fire0Def_MASK		0x1
#define SONAR_PulseDef_Fire0Def_OFFSET		31

#define SONAR_PulseDef_Fire_48mA		0x0000
#define SONAR_PulseDef_Fire_96mA		0x2000
#define SONAR_PulseDef_Fire_up		0x0000
#define SONAR_PulseDef_Fire_down	0x4000
#define SONAR_PulseDef_stop_1		0x0000
#define SONAR_PulseDef_stop_2		0x8000

/*** SONAR_Ctrl0 ***/
#define SONAR_Ctrl0_StartDly_MASK		0x7f
#define SONAR_Ctrl0_StartDly_OFFSET		0
#define SONAR_Ctrl0_Offset_MASK			0x1f
#define SONAR_Ctrl0_Offset_OFFSET		7
#define SONAR_Ctrl0_Offset20p_MASK		0x1
#define SONAR_Ctrl0_Offset20p_OFFSET		12
#define SONAR_Ctrl0_Offset20m_MASK		0x1
#define SONAR_Ctrl0_Offset20m_OFFSET		13
#define SONAR_Ctrl0_NegStop1_MASK		0x1
#define SONAR_Ctrl0_NegStop1_OFFSET		14
#define SONAR_Ctrl0_NegStop2_MASK		0x1
#define SONAR_Ctrl0_NegStop2_OFFSET		15
#define SONAR_Ctrl0_RFEdge1_MASK		0x1
#define SONAR_Ctrl0_RFEdge1_OFFSET		16
#define SONAR_Ctrl0_RFEdge2_MASK		0x1
#define SONAR_Ctrl0_RFEdge2_OFFSET		17
#define SONAR_Ctrl0_MRange2_MASK		0x1
#define SONAR_Ctrl0_MRange2_OFFSET		18

#define SONAR_Ctrl0_DisAutocal_MASK		0x1
#define SONAR_Ctrl0_DisAutocal_OFFSET		20
#define SONAR_Ctrl0_HitIn1_MASK			0x7
#define SONAR_Ctrl0_HitIn1_OFFSET		21
#define SONAR_Ctrl0_HitIn2_MASK			0x7
#define SONAR_Ctrl0_HitIn2_OFFSET		24
#define SONAR_Ctrl0_TDCEN_MASK			0x1
#define SONAR_Ctrl0_TDCEN_OFFSET		27
#define SONAR_Ctrl0_TRefChrg_MASK		0x3
#define SONAR_Ctrl0_TRefChrg_OFFSET		28
#define SONAR_Ctrl0_TimeOutSel_MASK		0x3
#define SONAR_Ctrl0_TimeOutSel_OFFSET		30

#define SONAR_Ctrl0_Edge1_single	(0x0)
#define SONAR_Ctrl0_Edge1_double	(0x1<<SONAR_Ctrl0_RFEdge1_OFFSET)		
#define SONAR_Ctrl0_Edge2_single	(0x0)
#define SONAR_Ctrl0_Edge2_double	(0x1<<SONAR_Ctrl0_RFEdge2_OFFSET)		
#define SONAR_Ctrl0_MRange1		0x0
#define SONAR_Ctrl0_MRange2		(0x1<<SONAR_Ctrl0_MRange2_OFFSET)

/*** SONAR_Ctrl1 ***/
#define SONAR_Ctrl1_EchoEn_MASK			0x1
#define SONAR_Ctrl1_EchoEn_OFFSET		0
#define SONAR_Ctrl1_EchoOffs_MASK		0x1f
#define SONAR_Ctrl1_EchoOffs_OFFSET		1
#define SONAR_Ctrl1_EchoOffs20p_MASK		0x1
#define SONAR_Ctrl1_EchoOffs20p_OFFSET		6
#define SONAR_Ctrl1_EchoOffs20m_MASK		0x1
#define SONAR_Ctrl1_EchoOffs20m_OFFSET		7
#define SONAR_Ctrl1_EchoPrd_MASK		0x7f
#define SONAR_Ctrl1_EchoPrd_OFFSET		8
#define SONAR_Ctrl1_TDCpwropt_MASK		0x1
#define SONAR_Ctrl1_TDCpwropt_OFFSET		15
#define SONAR_Ctrl1_Dbg0sel_MASK		0x3
#define SONAR_Ctrl1_Dbg0sel_OFFSET		16
#define SONAR_Ctrl1_Dbg1sel_MASK		0x3
#define SONAR_Ctrl1_Dbg1sel_OFFSET		18
#define SONAR_Ctrl1_AnalogEn_MASK		0x1
#define SONAR_Ctrl1_AnalogEn_OFFSET		20
#define SONAR_Ctrl1_ChopEn_MASK			0x1
#define SONAR_Ctrl1_chopEn_OFFSET		21
#define SONAR_Ctrl1_TDCOpt_MASK			0xff
#define SONAR_Ctrl1_TDCOpt_OFFSET		22
#define SONAR_Ctrl1_AutoFW_MASK			0x1
#define SONAR_Ctrl1_AutoFW_OFFSET		30
#define SONAR_Ctrl1_WaveEn_MASK			0x1
#define SONAR_Ctrl1_WaveEn_OFFSET		31

#define SONAR_Ctrl1_Echo_ON			0x1
#define SONAR_Ctrl1_Echo_CLOSE			0x0
#define SONAR_Ctrl1_TDC_lowpower		0x8000
#define SONAR_Ctrl1_Dbg0_start		(0x0<<SONAR_Ctrl1_Dbg0sel_OFFSET)
#define SONAR_Ctrl1_Dbg0_stop		(0x1<<SONAR_Ctrl1_Dbg0sel_OFFSET)
#define SONAR_Ctrl1_Dbg0_comp		(0x2<<SONAR_Ctrl1_Dbg0sel_OFFSET)
#define SONAR_Ctrl1_Dbg0_window		(0x3<<SONAR_Ctrl1_Dbg0sel_OFFSET)
#define SONAR_Ctrl1_Dbg1_stop		(0x0<<SONAR_Ctrl1_Dbg1sel_OFFSET)
#define SONAR_Ctrl1_Dbg1_comp		(0x1<<SONAR_Ctrl1_Dbg1sel_OFFSET)
#define SONAR_Ctrl1_Dbg1_window		(0x2<<SONAR_Ctrl1_Dbg1sel_OFFSET)
#define SONAR_Ctrl1_Dbg1_ldsenset	(0x3<<SONAR_Ctrl1_Dbg1sel_OFFSET)
#define SONAR_Ctrl1_AnalogEn		(0x1<<SONAR_Ctrl1_AnalogEn_OFFSET)
#define SONAR_Ctrl1_chopEn		(0x1<<SONAR_Ctrl1_chopEn_OFFSET)
#define TDC_dres_single		(0x0<<SONAR_Ctrl1_TDCOpt_OFFSET)
#define TDC_dres_double		(0x1<<SONAR_Ctrl1_TDCOpt_OFFSET)
#define TDC_nsmp_1		(0x0<<(SONAR_Ctrl1_TDCOpt_OFFSET+5))
#define TDC_nsmp_2		(0x1<<(SONAR_Ctrl1_TDCOpt_OFFSET+5))
#define TDC_nsmp_4		(0x3<<(SONAR_Ctrl1_TDCOpt_OFFSET+5))
#define SONAR_Ctrl1_AutoFW		0x40000000
#define SONAR_Ctrl1_WaveEn		0x80000000

/*** SONAR_GateDef ***/
#define SONAR_GateDefx_DelayVal_OFFSET		0
#define SONAR_GateDefx_DelayVal_MASK		0xfffff
#define SONAR_GateDefx_GateLen_OFFSET		20
#define SONAR_GateDefx_GateLen_MASK		0xff
#define SONAR_GateDefx_Oneshot_OFFSET		30
#define SONAR_GateDefx_Oneshot_MASK		0x1
#define SONAR_GateDefx_GateKeep_OFFSET		31
#define SONAR_GateDefx_GateKeep_MASK		0x1

#define SONAR_GateDefx_Oneshot			0x40000000
#define SONAR_GateDefx_GateKeepOpen		0x80000000

/*** SONAR_WaveW_Cur, WaveW_Last ***/
#define SONAR_WaveW_FirstWW_OFFSET		0
#define SONAR_WaveW_FirstWW_MASK		0xffff
#define SONAR_WaveW_SecondWW_OFFSET		16
#define SONAR_WaveW_SecondWW_MASK		0xffff

/*** SONAR_ResPtr ***/
#define SONAR_ResPtr_wptr0_cur_OFFSET		0
#define SONAR_ResPtr_wptr0_cur_MASK		0xf
#define SONAR_ResPtr_wptr1_cur_OFFSET		4
#define SONAR_ResPtr_wptr1_cur_MASK		0xf
#define SONAR_ResPtr_wptr0_last_OFFSET		8
#define SONAR_ResPtr_wptr0_last_MASK		0xf
#define SONAR_ResPtr_wptr1_last_OFFSET		12
#define SONAR_ResPtr_wptr1_last_MASK		0xf
#define SONAR_ResPtr_echocnt_cur_OFFSET		16
#define SONAR_ResPtr_echocnt_cur_MASK		0xff
#define SONAR_ResPtr_echocnt_last_OFFSET	24
#define SONAR_ResPtr_echocnt_last_MASK		0xff


/*****************************THSENS REGS********************************/
#define ThsensCtrl      *(volatile _u32 *)(THSENS_BASEADDR)

/*** ThsensCtrl ***/
#define ThsensCtrl_FakeNum_OFFSET		0
#define ThsensCtrl_FakeNum_MASK			0x1
#define ThsensCtrl_PortNum_OFFSET		1
#define ThsensCtrl_PortNum_MASK			0x1
#define ThsensCtrl_PortDir_OFFSET		2
#define ThsensCtrl_PortDir_MASK			0x1
#define ThsensCtrl_PwrSave_OFFSET		3
#define ThsensCtrl_PwrSave_MASK			0x1
#define ThsensCtrl_Tload_OFFSET			4
#define ThsensCtrl_Tload_MASK			0x3
#define ThsensCtrl_Tunload_OFFSET		6
#define ThsensCtrl_Tunload_MASK			0x3
#define ThsensCtrl_FastMode_OFFSET		8
#define ThsensCtrl_FastMode_MASK		0x1

#define ThsensCtrl_FakeNum_2		0x0
#define ThsensCtrl_FakeNum_7		0x1
#define ThsensCtrl_PortNum_2		0x0
#define ThsensCtrl_PortNum_4		0x2
#define ThsensCtrl_PortDir_1234		0x0
#define ThsensCtrl_PortDir_4321		0x4
#define ThsensCtrl_PwrSave		0x8
#define ThsensCtrl_Tload_90us		(0x0<<ThsensCtrl_Tload_OFFSET)
#define ThsensCtrl_Tload_120us		(0x1<<ThsensCtrl_Tload_OFFSET)
#define ThsensCtrl_Tload_150us		(0x2<<ThsensCtrl_Tload_OFFSET)
#define ThsensCtrl_Tload_500us		(0x3<<ThsensCtrl_Tload_OFFSET)
#define ThsensCtrl_Tunload_90us		(0x0<<ThsensCtrl_Tunload_OFFSET)
#define ThsensCtrl_Tunload_120us		(0x1<<ThsensCtrl_Tunload_OFFSET)
#define ThsensCtrl_Tunload_150us		(0x2<<ThsensCtrl_Tunload_OFFSET)
#define ThsensCtrl_Tunload_500us		(0x3<<ThsensCtrl_Tunload_OFFSET)
#define ThsensCtrl_FastMode		0x80


/*****************************ADC REGS********************************/
#define ADC_CR	*(volatile _u8 *)(ADC_BASEADDR)
#define ADC_D0	*(volatile _u8 *)(ADC_BASEADDR+0x1)
#define ADC_D1	*(volatile _u8 *)(ADC_BASEADDR+0x2)

/*** ADC_CR ***/
#define ADC_CR_sel_OFFSET		0
#define ADC_CR_sel_MASK			0x3
#define ADC_CR_stat_OFFSET		6
#define ADC_CR_stat_MASK		0x3

#define ADC_CR_adci_0		0x0
#define ADC_CR_adci_1		0x1
#define ADC_CR_vcore		0x2
#define ADC_CR_1V			0x3

#define ADC_CR_BUSY		(0x0<<ADC_CR_stat_OFFSET)
#define ADC_CR_HALT		(0x1<<ADC_CR_stat_OFFSET)
#define ADC_CR_START		(0x2<<ADC_CR_stat_OFFSET)
#define ADC_CR_NOVALID		(0x3<<ADC_CR_stat_OFFSET)

/*** ADC_D0 ***/
#define ADC_D0_out_OFFSET		4
#define ADC_D0_out_MASK			0xf


/*******************************LCD REGS*********************************/
#define LCD_FB_0  	 	*(volatile _u32 *)(SLCD_REG_BASEADDR)
#define LCD_FB_1		*(volatile _u32 *)(SLCD_REG_BASEADDR+0x04)
#define LCD_FB_2		*(volatile _u32 *)(SLCD_REG_BASEADDR+0x08)
#define LCD_RefeshRate   	*(volatile _u32 *)(SLCD_REG_BASEADDR+0x0C)

/*** LCD_RefreshRate ***/
#define LCD_RefreshRate_OFFSET		0
#define LCD_RefreshRate_MASK		0xff


/*****************************FLASH REGS********************************/
#define FLASH_CMD_REG		*(volatile _u32 *)(FLASH_REG_BASEADDR)
#define FLASH_Caddr_h		*(volatile _u32 *)(FLASH_REG_BASEADDR+0x04)
#define FLASH_Caddr_l		*(volatile _u32 *)(FLASH_REG_BASEADDR+0x08)
#define FLASH_Verif_data	*(volatile _u32 *)(FLASH_REG_BASEADDR+0x10)
#define FLASH_Status		*(volatile _u32 *)(FLASH_REG_BASEADDR+0x14)
#define FLASH_PEctrl_REG	*(volatile _u32 *)(FLASH_REG_BASEADDR+0x24)

/*** FLASH_Command ***/
#define FLASH_Command_Daddr_OFFSET		0
#define FLASH_Command_Daddr_MASK		0xffff
#define FLASH_Command_Cmd_OFFSET		28
#define FLASH_Command_Cmd_MASK			0xf

#define FLASH_Command_erase_page	(0xa<<FLASH_Command_Cmd_OFFSET)
#define FLASH_Command_prog_page		(0xe<<FLASH_Command_Cmd_OFFSET)
#define FLASH_Command_clr_page_latch	(0x4<<FLASH_Command_Cmd_OFFSET)
#define FLASH_Command_check_page	(0x1<<FLASH_Command_Cmd_OFFSET)
#define FLASH_Command_update_key	(0xf<<FLASH_Command_Cmd_OFFSET)
#define FLASH_Command_update_bound	(0x9<<FLASH_Command_Cmd_OFFSET)
#define FLASH_Command_clr_int		(0x3<<FLASH_Command_Cmd_OFFSET)

/*** FLASH_Status ***/
#define FLASH_Status_Verify_correct		0x1
#define FLASH_Status_Verify_end			0x2
#define FLASH_Status_int_pe_end			0x4
#define FLASH_Status_int_no_permission		0x8

/*** FLASH_PEctrl ***/
#define FLASH_PEctrl_MASK		0x7
#define FLASH_PEtime_1500us		0x0
#define FLASH_PEtime_2000us		0x1
#define FLASH_PEtime_2500us		0x2
#define FLASH_PEtime_3000us		0x3
#define FLASH_PEtime_3500us		0x4


/*****************************UART0 REGS********************************/
//NS16550
#define Uart0_RxData    *(volatile _u8 *)(UART0_BASEADDR)
#define Uart0_TxData    *(volatile _u8 *)(UART0_BASEADDR)
#define Uart0_IER   	*(volatile _u8 *)(UART0_BASEADDR+0x01)
#define Uart0_IIR   	*(volatile _u8 *)(UART0_BASEADDR+0x02)	 	//read only
#define Uart0_FCR   	*(volatile _u8 *)(UART0_BASEADDR+0x02)  		//write only
#define Uart0_LCR   	*(volatile _u8 *)(UART0_BASEADDR+0x03)
#define Uart0_MCR   	*(volatile _u8 *)(UART0_BASEADDR+0x04)
#define Uart0_LSR   	*(volatile _u8 *)(UART0_BASEADDR+0x05)
#define Uart0_MSR   	*(volatile _u8 *)(UART0_BASEADDR+0x06)

#define Uart0_BaudL   	*(volatile _u8 *)(UART0_BASEADDR)
#define Uart0_BaudH   	*(volatile _u8 *)(UART0_BASEADDR+0x01)

/*****************************UART1 REGS********************************/
//NS16550
#define Uart1_RxData    *(volatile _u8 *)(UART1_BASEADDR)
#define Uart1_TxData    *(volatile _u8 *)(UART1_BASEADDR)
#define Uart1_IER       *(volatile _u8 *)(UART1_BASEADDR+0x01)
#define Uart1_IIR       *(volatile _u8 *)(UART1_BASEADDR+0x02)	 	//read only
#define Uart1_FCR       *(volatile _u8 *)(UART1_BASEADDR+0x02)  		//write only
#define Uart1_LCR       *(volatile _u8 *)(UART1_BASEADDR+0x03)
#define Uart1_MCR       *(volatile _u8 *)(UART1_BASEADDR+0x04)
#define Uart1_LSR       *(volatile _u8 *)(UART1_BASEADDR+0x05)
#define Uart1_MSR       *(volatile _u8 *)(UART1_BASEADDR+0x06)

#define Uart1_BaudL     *(volatile _u8 *)(UART1_BASEADDR)
#define Uart1_BaudH     *(volatile _u8 *)(UART1_BASEADDR+0x01)

/*** UART controller ***/
#include "asm/ns16550-uart.h"
#define UART_MCR_infrared_on		0x80
#define UART_MCR_infrared_off		0x00
#define UART_MCR_Rx_pol			0x40
#define UART_MCR_Rx_pol_n		0x00
#define UART_MCR_infrared_pol		0x20
#define UART_MCR_infrared_pol_n		0x00


/*******************************SPI REGS********************************/
#define SPI_SPCR		*(volatile _u8 *)(SPI_REG_BASEADDR)
#define SPI_SPSR		*(volatile _u8 *)(SPI_REG_BASEADDR+0x01)
#define SPI_TxFIFO		*(volatile _u8 *)(SPI_REG_BASEADDR+0x02)
#define SPI_RxFIFO		*(volatile _u8 *)(SPI_REG_BASEADDR+0x02)
#define SPI_SPER		*(volatile _u8 *)(SPI_REG_BASEADDR+0x03)
#define SPI_SFC_PARAM	*(volatile _u8 *)(SPI_REG_BASEADDR+0x04)
#define SPI_SFC_SOFTCS	*(volatile _u8 *)(SPI_REG_BASEADDR+0x05)
#define SPI_SFC_TIMING	*(volatile _u8 *)(SPI_REG_BASEADDR+0x06)

/*** SPI controller ***/
#define SPI_SPCR_IE		0x80
#define SPI_SPCR_EN		0x40
#define SPI_SPCR_MASTER		0x10
#define SPI_SPCR_cpol		0x8
#define SPI_SPCR_cpha		0x4
#define SPI_SPCR_spr_mask	0x3

#define SPI_SPSR_INT		0x80
#define SPI_SPSR_wcol		0x40
#define SPI_SPSR_wfull		0x8
#define SPI_SPSR_wempty		0x4
#define SPI_SPSR_rfull		0x2
#define SPI_SPSR_rempty		0x1

#define SPI_SPER_icnt_mask	0xc0
#define SPI_SPER_icnt_1		0x00
#define SPI_SPER_icnt_2		0x40
#define SPI_SPER_icnt_3		0x80
#define SPI_SPER_icnt_4		0xc0
#define SPI_SPER_mode1		0x4   //same
#define SPI_SPER_mode2		0x0   //half
#define SPI_SPER_spre_mask	0x3

#define SPI_PARAM_clk_div_mask	0xf0
#define SPI_PARAM_dual_io	0x8
#define SPI_PARAM_fast_read	0x4
#define SPI_PARAM_burst_en	0x2
#define SPI_PARAM_memory_en	0x1

#define SPI_SOFTCS_csn1_1	0x20
#define SPI_SOFTCS_csn1_0	0x00
#define SPI_SOFTCS_csn0_1	0x10
#define SPI_SOFTCS_csn0_0	0x00
#define SPI_SOFTCS_cs_en	0x1


/*******************************I2C REGS********************************/
#define IIC_PRER_L	*(volatile _u8 *)(I2C_BASEADDR+0x00)
#define IIC_PRER_H 	*(volatile _u8 *)(I2C_BASEADDR+0x01)
#define IIC_CTR   	*(volatile _u8 *)(I2C_BASEADDR+0x02)
#define IIC_TXR   	*(volatile _u8 *)(I2C_BASEADDR+0x03)     //w
#define IIC_RXR		*(volatile _u8 *)(I2C_BASEADDR+0x03)     //r
#define IIC_CR		*(volatile _u8 *)(I2C_BASEADDR+0x04)     //w
#define IIC_SR		*(volatile _u8 *)(I2C_BASEADDR+0x04)     //r
#define IIC_ADDR	*(volatile _u8 *)(I2C_BASEADDR+0x07)     // slave_addr

/*** IIC controller ***/
#define IIC_CTR_EN		0x80
#define IIC_CTR_IEN		0x40
#define IIC_CTR_MASTER		0x20
#define IIC_CTR_SLAVE		0x00
#define IIC_CTR_TXR		0x10
#define IIC_CTR_RXR		0x8

#define IIC_CR_START	0x80
#define IIC_CR_STOP	0x40
#define IIC_CR_RD	0x20
#define IIC_CR_WR	0x10
#define IIC_CR_NACK	0x8
#define IIC_CR_IACK	0x1

#define IIC_SR_RxACK		0x80
#define IIC_SR_BUSY		0x40
#define IIC_SR_AL		0x20
#define IIC_SR_S_ADDR_SED	0x10
#define IIC_SR_SLAVE_W		0x8
#define IIC_SR_SLAVE_R		0x0
#define IIC_SR_TIP		0x2
#define IIC_SR_IF		0x1

#define IIC_SLAVE_ADDR_MASK	0x7f
#define	LS1D_IIC_HZ100K		100000
#define LS1D_IIC_HZ50K		50000
#define LS1D_IIC_HZ10K		10000


/***************************INTERRUPT REGS******************************/
#define INT_EN	       *(volatile _u8 *)(INTC_REGS_BASEADDR)
#define INT_EGDE       *(volatile _u8 *)(INTC_REGS_BASEADDR+0x01)
#define INT_POL        *(volatile _u8 *)(INTC_REGS_BASEADDR+0x02)
#define INT_CLR        *(volatile _u8 *)(INTC_REGS_BASEADDR+0x03)
#define INT_SET        *(volatile _u8 *)(INTC_REGS_BASEADDR+0x04)
#define INT_OUT        *(volatile _u8 *)(INTC_REGS_BASEADDR+0x05)

/*** INT controller ***/
#define INTC_ADC	0x1
#define INTC_IIC	0x2
#define INTC_UART1	0x4
#define INTC_UART0	0x8
#define INTC_FLASH	0x10
#define INTC_SPI	0x20
#define INTC_MASK	0x3f

#if 0
#define INTERRUPT_ENABLE	\
	do{  												\
		__asm__ volatile(								\
					"mfc0	$8, $12;\n"					\
					"li		$9, 0xffff00fe;\n"			\
					"and	$10, $8, $9;\n"				\
					"li		$9, 0xff01;\n"				\
					"or		$8, $10, $9;\n"				\
					"mtc0	$8, $12;\n"					\
														\
					:::"$8","$9","$10"					\
					);									\
}while(0);


#define INTERRUPT_DISABLE	\
	do{ 											\
		__asm__ volatile(							\
				"mfc0	$8, $12;\n"					\
				"li		$9, 0xffff00fe;\n"			\
				"and	$10, $8, $9;\n"				\
				"mtc0	$10, $12;\n"				\
													\
				:::"$8","$9","$10"					\
				);									\
}while(0);
#endif

/**********************************************************************************************************************************************************************/

/******** GPIO setting ******/
#define GPIO_GPIO0		(1<<0)
#define GPIO_GPIO1		(1<<1)
#define GPIO_PULSE0		(1<<2)
#define GPIO_PULSE1		(1<<3)
#define GPIO_IIC_SCL		(1<<4)
#define GPIO_IIC_SDA		(1<<5)
#define GPIO_UART0_RX		(1<<6)
#define GPIO_UART0_TX		(1<<7)
#define GPIO_UART1_RX		(1<<8)
#define GPIO_UART1_TX		(1<<9)
#define GPIO_IR_PWR		(1<<10)
#define GPIO_KEY_I		(1<<11)
#define GPIO_EMP_I		(1<<12)
#define GPIO_SPI_CLK		(1<<13)
#define GPIO_SPI_MISO		(1<<14)
#define GPIO_SPI_MOSI		(1<<15)
#define GPIO_SPI_CSN0		(1<<16)
#define GPIO_SPI_CSN1		(1<<17)
#define GPIO_BS0		(1<<18)
#define GPIO_BS1		(1<<19)
#define GPIO_PT3		(1<<20)
#define GPIO_PT4		(1<<21)
#define GPIO_LCD_SEG20		(1<<22)
#define GPIO_LCD_SEG21		(1<<23)
#define GPIO_LCD_SEG22		(1<<24)
#define GPIO_LCD_SEG23		(1<<25)

#endif /* LS1D_H_ */
