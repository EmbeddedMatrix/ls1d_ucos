/*
*********************************************************************************************************
*                                               uC/OS-II
*                                         The Real-Time Kernel
*
*                         (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
*                                          All Rights Reserved
*
*
*                                       80x86/80x88 Specific code
*                                          LARGE MEMORY MODEL
*
*                                          Borland C/C++ V4.51
*
* File         : OS_CPU_C.C
* By           : Jean J. Labrosse
*********************************************************************************************************
*/

#include "../include/ls1d.h"

/*
*********************************************************************************************************
*                                        INITIALIZE A TASK'S STACK
*
* Description: This function is called by either OSTaskCreate() or OSTaskCreateExt() to initialize the
*              stack frame of the task being created.  This function is highly processor specific.
*
* Arguments  : task          is a pointer to the task code
*
*              pdata         is a pointer to a user supplied data area that will be passed to the task
*                            when the task first executes.
*
*              ptos          is a pointer to the top of stack.  It is assumed that 'ptos' points to
*                            a 'free' entry on the task stack.  If OS_STK_GROWTH is set to 1 then 
*                            'ptos' will contain the HIGHEST valid address of the stack.  Similarly, if
*                            OS_STK_GROWTH is set to 0, the 'ptos' will contains the LOWEST valid address
*                            of the stack.
*
*              opt           specifies options that can be used to alter the behavior of OSTaskStkInit().
*                            (see uCOS_II.H for OS_TASK_OPT_???).
*
* Returns    : Always returns the location of the new top-of-stack' once the processor registers have
*              been placed on the stack in the proper order.
*
* Note(s)    : Interrupts are enabled when your task starts executing. You can change this by setting the
*              PSW to 0x0002 instead.  In this case, interrupts would be disabled upon task startup.  The
*              application code would be responsible for enabling interrupts at the beginning of the task
*              code.  You will need to modify OSTaskIdle() and OSTaskStat() so that they enable 
*              interrupts.  Failure to do this will make your system crash!
*********************************************************************************************************
*/
///  栈类型 需要十分的注意！！！
INT32U wSR=0;
INT32U wGP;

OS_STK  *OSTaskStkInit (void (*task)(void *pd), void *pdata, OS_STK *ptos, INT16U opt)
{
    INT32U *stk;

    opt    = opt;                           /* 'opt' is not used, prevent warning                      */
    stk    = (INT32U *)ptos;                /* Load stack pointer                                      */
    stk --;
    *stk-- = (INT32U)(task);         		/* pc: Entry Point                 		*/
    *stk-- = (INT32U)(wSR);					///C0_SR		IE	= 1
    *stk-- = (INT32U)0;						/// at
    *stk-- = (INT32U)0;						/// v0
    *stk-- = (INT32U)0;               		/// v1
    *stk-- = (INT32U)pdata;          		/// a0: argument
    *stk-- = (INT32U)0;						/// a1
    *stk-- = (INT32U)0;						/// a2
    *stk-- = (INT32U)0;						/// a3
    *stk-- = (INT32U)0;						/// t0
    *stk-- = (INT32U)0;						/// t1
    *stk-- = (INT32U)0;						/// t2
    *stk-- = (INT32U)0;						/// t3
    *stk-- = (INT32U)0;						/// t4
    *stk-- = (INT32U)0;						/// t5
    *stk-- = (INT32U)0;						/// t6
    *stk-- = (INT32U)0;						/// t7
    *stk-- = (INT32U)0;						/// s0
    *stk-- = (INT32U)0;						/// s1
    *stk-- = (INT32U)0;						/// s2
    *stk-- = (INT32U)0;						/// s3
    *stk-- = (INT32U)0;						/// s4
    *stk-- = (INT32U)0;						/// s5
    *stk-- = (INT32U)0;						/// s6
    *stk-- = (INT32U)0;						/// s7
    *stk-- = (INT32U)0;						/// s8 t8
    *stk-- = (INT32U)0;						/// s9 t9
    *stk-- = (INT32U)0;						/// gp
    *stk-- = (INT32U)0;						/// fp
    *stk   = (INT32U)(task);				/// ra

	return ((OS_STK *)stk);
}

