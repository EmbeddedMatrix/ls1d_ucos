/*
 * Interrupt.h
 *
 *  Created on: 2013-1-16
 *      Author: liming
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

/*
 * SOFT_INT、TIMER1_INT、WAKER_INT、KEY_INT、BAT_FAIL、RES_VALID、INTC、EXINT
 * 分别是8种中断的处理函数。
 */
void SOFT_INT();
void TIMER1_INT();
void WAKER_INT();
void KEY_INT();
void BAT_FAIL();
void RES_VALID();
void INTC();
void EXINT();

/*
 * Schedule
 * 任务调度函数。
 */
void Schedule();

/*
 * IRQ_Exception
 * 中断处理函数的总入口。
 */
void IRQ_Exception();   //中断处理函数

/*
 * ls1d_waker_init
 * 初始化wake定时器，使能waker中断。
 */
void ls1d_waker_init(_u32 wake_delay);

/*
 * ls1d_timer_init
 * 初始化8M定时器。
 *
 * ls1d_disable_timer
 * 关闭8M定时器。
 */
void ls1d_timer_init(_u32 timer_delay);
void ls1d_disable_timer();

/*
 * ls1d_set_soft_int
 * 软件主动触发一次soft中断。
 */
void ls1d_set_soft_int();


#endif /* INTERRUPT_H_ */
