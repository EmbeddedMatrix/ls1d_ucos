/********************************************************************************************************
;                                               uC/OS-II
;                                         The Real-Time Kernel
;
;                          (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
;                                          All Rights Reserved
;
;
;                                       80x86/80x88 Specific code
;                                          LARGE MEMORY MODEL
;
;                                           Borland C/C++ V4.51
;                                       (IBM/PC Compatible Target)
;
; File         : OS_CPU_A.ASM
; By           : Jean J. Labrosse
;********************************************************************************************************/
#include "../include/asm/mipsregs.h"
#include "../include/asm/interrupt.h"


.section .text
            .globl OSTickISR
            .globl OSStartHighRdy
            .globl OSCtxSw
            .globl OSIntCtxSw
            .globl OS_InitMips
            .globl OSCPURestoreSR
            .globl OSCPUSaveSR
			.globl EnableWake
			.globl EnableInt
			.globl EnableIntC
.set noreorder

/*********************************************************************************************************
;                                          START MULTITASKING
;                                       void OSStartHighRdy(void)
;
; The stack frame is assumed to look as follows:
;

;
; Note : OSStartHighRdy() MUST:
;           a) Call OSTaskSwHook() then,
;           b) Set OSRunning to TRUE,
;           c) Switch to the highest priority task.
;*********************************************************************************************************/
OSStartHighRdy:
	la t0,OSRunning             /* Indicate that multitasking has started */
	li t1,1
	sb t1,0(t0)                 /* OSRunning is a boolean */

	la t0,OSTCBHighRdy          /* Get highest priority task TCB address */
	lw t0,0(t0)                 /* get stack pointer */
    lw sp,0(t0)                 /* switch to the new stack */

	RESTORE_REG_RET()			/* pop new task's at, v0-v1,a0-a3,t0-t9,s0-s7,fp,ra, & pc */

/*********************************************************************************************************
;                                PERFORM A CONTEXT SWITCH (From task level)
;                                           void OSCtxSw(void)
;
; Note(s): 1) Upon entry,
;             OSTCBCur     points to the OS_TCB of the task to suspend
;             OSTCBHighRdy points to the OS_TCB of the task to resume
;
;          2) The stack frame of the task to suspend looks as follows:
;
;											TODO
;
;
;
;          3) The stack frame of the task to resume looks as follows:
;
;                                          	TODO
;
;
;*********************************************************************************************************/
OSCtxSw:
	/* Special optimised code below: */
	STORE_REG_RET(ra)               /* push task's at, v0-v1,a0-a3,t0-t9,s0-s7,fp,ra, & pc */

	/* OSPrioCur = OSPrioHighRdy */
	la t0,OSPrioCur
	la t1,OSPrioHighRdy
	lb t2,0(t1)
	sb t2,0(t0)

	/* Get current task TCB address */
	la t0,OSTCBCur
	lw t1,0(t0)
	sw sp,0(t1)                 /* store sp in preempted tasks's TCB */

	/* Get highest priority task TCB address */
	la t1,OSTCBHighRdy
	lw t1,0(t1)
	lw sp,0(t1)                 /* get new task's stack pointer */

	/* OSTCBCur = OSTCBHighRdy */
	sw t1,0(t0)                /* set new current task TCB address */

	RESTORE_REG_RET()        /* TBI */     /* pop new task's at, v0-v1,a0-a3,t0-t9,s0-s7,fp,ra, & pc */

/*********************************************************************************************************
;                                PERFORM A CONTEXT SWITCH (From an ISR)
;                                        void OSIntCtxSw(void)
;
; Note(s): 1) Upon entry,
;             OSTCBCur     points to the OS_TCB of the task to suspend
;             OSTCBHighRdy points to the OS_TCB of the task to resume
;
;          2) The stack frame of the task to suspend looks as follows:
;
;                                            OFFSET  of task code address
;                                            SEGMENT of task code address
;                                            Flags to load in PSW            (High memory)
;
;
;          3) The stack frame of the task to resume looks as follows:
;
;                                           OFFSET  of task code address
;                                           SEGMENT of task code address
;                                           Flags to load in PSW             (High memory)
;*********************************************************************************************************/
OSIntCtxSw:

	/* OSPrioCur = OSPrioHighRdy */
	la t0,OSPrioCur
	la t1,OSPrioHighRdy
	lb t1,0(t1)
	sb t1,0(t0)

	/* Get current task TCB address */
	la t0,OSTCBCur

	/* Get highest priority task TCB address */
	la t2,OSTCBHighRdy
	lw t2,0(t2)
	lw sp,0(t2)                 /* get new task's stack pointer*/

	/* OSTCBCur = OSTCBHighRdy */
	sw t2,0(t0)                 /* set new current task TCB address */

	RESTORE_REG_ERET()


OSCPUSaveSR:
	MFC0  v0,CP0_STATUS
	li	v1,0xfffffffe
	and v1,v0,v1
	MTC0  v1,CP0_STATUS
	jr ra
	nop

OSCPURestoreSR:
	MTC0  a0,CP0_STATUS
	jr ra
    nop


OS_InitMips:
	la 	t0,wGP
	sw	gp,0(t0)

	MFC0 t0,CP0_STATUS
	ori  t0,t0,0x201			//// 使能IE 位，使能 timer 的 mask
	la	t1,wSR
	sw 	t0,0(t1)
	jr	ra
	nop

EnableWake:
	MFC0  	t0,CP0_STATUS
	ori	t0,t0,0x00000400
	MTC0	t0,CP0_STATUS
	jr ra
    nop

EnableInt:
	MFC0  	t0,CP0_STATUS
	ori	t0,t0,0x00000401
	MTC0	t0,CP0_STATUS
	jr ra
    nop

EnableIntC:
	MFC0  	t0,CP0_STATUS
	ori	t0,t0,0x00004000
	MTC0	t0,CP0_STATUS
	jr ra
    nop
