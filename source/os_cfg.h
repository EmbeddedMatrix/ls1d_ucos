/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                           (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
*                                       uC/OS-II Configuration File
*
* File : OS_CFG.H
* By   : Jean J. Labrosse
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                         uC/OS-II CONFIGURATION
*********************************************************************************************************
*/

                                       /* ---------------------- MISCELLANEOUS ----------------------- */
#define OS_ARG_CHK_EN             0    /* Enable (1) or Disable (0) argument checking                  */

#define OS_CPU_HOOKS_EN           0    /* uC/OS-II hooks are found in the processor port files         */

#define OS_LOWEST_PRIO           63    /* Defines the lowest priority that can be assigned ...         */
                                       /* ... MUST NEVER be higher than 63!                            */

#define OS_MAX_EVENTS             5    /* Max. number of event control blocks in your application ...  */
                                       /* ... MUST be > 0                                              */
#define OS_MAX_FLAGS              1    /* Max. number of Event Flag Groups    in your application ...  */
                                       /* ... MUST be > 0                                              */
#define OS_MAX_MEM_PART           1    /* Max. number of memory partitions ...                         */
                                       /* ... MUST be > 0                                              */
#define OS_MAX_QS                 1    /* Max. number of queue control blocks in your application ...  */
                                       /* ... MUST be > 0                                              */
#define OS_MAX_TASKS              6    /* Max. number of tasks in your application ...                 */
                                       /* ... MUST be >= 2                                             */

#define OS_SCHED_LOCK_EN          1    /*     Include code for OSSchedLock() and OSSchedUnlock()       */

#define OS_TASK_IDLE_STK_SIZE   512    /* Idle task stack size (# of OS_STK wide entries)              */

#define OS_TASK_STAT_EN           0    /* Enable (1) or Disable(0) the statistics task                 */
#define OS_TASK_STAT_STK_SIZE   512    /* Statistics task stack size (# of OS_STK wide entries)        */

#define OS_TICKS_PER_SEC        200    /* Set the number of ticks in one second                        */


                                       /* ----------------------- EVENT FLAGS ------------------------ */
#define OS_FLAG_EN                0    /* Enable (1) or Disable (0) code generation for EVENT FLAGS    */
#define OS_FLAG_WAIT_CLR_EN       1    /* Include code for Wait on Clear EVENT FLAGS                   */
#define OS_FLAG_ACCEPT_EN         0    /*     Include code for OSFlagAccept()                          */
#define OS_FLAG_DEL_EN            0    /*     Include code for OSFlagDel()                             */
#define OS_FLAG_QUERY_EN          0    /*     Include code for OSFlagQuery()                           */


                                       /* -------------------- MESSAGE MAILBOXES --------------------- */
#define OS_MBOX_EN                1    /* Enable (1) or Disable (0) code generation for MAILBOXES      */
#define OS_MBOX_ACCEPT_EN         0    /*     Include code for OSMboxAccept()                          */
#define OS_MBOX_DEL_EN            0    /*     Include code for OSMboxDel()                             */
#define OS_MBOX_POST_EN           1    /*     Include code for OSMboxPost()                            */
#define OS_MBOX_POST_OPT_EN       0    /*     Include code for OSMboxPostOpt()                         */
#define OS_MBOX_QUERY_EN          0    /*     Include code for OSMboxQuery()                           */


                                       /* --------------------- MEMORY MANAGEMENT -------------------- */
#define OS_MEM_EN                 0    /* Enable (1) or Disable (0) code generation for MEMORY MANAGER */
#define OS_MEM_QUERY_EN           1    /*     Include code for OSMemQuery()                            */


                                       /* ---------------- MUTUAL EXCLUSION SEMAPHORES --------------- */
#define OS_MUTEX_EN               0    /* Enable (1) or Disable (0) code generation for MUTEX          */
#define OS_MUTEX_ACCEPT_EN        1    /*     Include code for OSMutexAccept()                         */
#define OS_MUTEX_DEL_EN           1    /*     Include code for OSMutexDel()                            */
#define OS_MUTEX_QUERY_EN         1    /*     Include code for OSMutexQuery()                          */


                                       /* ---------------------- MESSAGE QUEUES ---------------------- */
#define OS_Q_EN                   0    /* Enable (1) or Disable (0) code generation for QUEUES         */
#define OS_Q_ACCEPT_EN            1    /*     Include code for OSQAccept()                             */
#define OS_Q_DEL_EN               1    /*     Include code for OSQDel()                                */
#define OS_Q_FLUSH_EN             1    /*     Include code for OSQFlush()                              */
#define OS_Q_POST_EN              1    /*     Include code for OSQPost()                               */
#define OS_Q_POST_FRONT_EN        1    /*     Include code for OSQPostFront()                          */
#define OS_Q_POST_OPT_EN          1    /*     Include code for OSQPostOpt()                            */
#define OS_Q_QUERY_EN             1    /*     Include code for OSQQuery()                              */


                                       /* ------------------------ SEMAPHORES ------------------------ */
#define OS_SEM_EN                 1    /* Enable (1) or Disable (0) code generation for SEMAPHORES     */
#define OS_SEM_ACCEPT_EN          0    /*    Include code for OSSemAccept()                            */
#define OS_SEM_DEL_EN             0    /*    Include code for OSSemDel()                               */
#define OS_SEM_QUERY_EN           1    /*    Include code for OSSemQuery()                             */


                                       /* --------------------- TASK MANAGEMENT ---------------------- */
#define OS_TASK_CHANGE_PRIO_EN    0    /*     Include code for OSTaskChangePrio()                      */
#define OS_TASK_CREATE_EN         1    /*     Include code for OSTaskCreate()                          */
#define OS_TASK_CREATE_EXT_EN     0    /*     Include code for OSTaskCreateExt()                       */
#define OS_TASK_DEL_EN            0    /*     Include code for OSTaskDel()                             */
#define OS_TASK_SUSPEND_EN        0    /*     Include code for OSTaskSuspend() and OSTaskResume()      */
#define OS_TASK_QUERY_EN          0    /*     Include code for OSTaskQuery()                           */


                                       /* --------------------- TIME MANAGEMENT ---------------------- */
#define OS_TIME_DLY_HMSM_EN       0    /*     Include code for OSTimeDlyHMSM()                         */
#define OS_TIME_DLY_RESUME_EN     0    /*     Include code for OSTimeDlyResume()                       */
#define OS_TIME_GET_SET_EN        1    /*     Include code for OSTimeGet() and OSTimeSet()             */

typedef INT16U             OS_FLAGS;   	/* Date type for event flag bits (8, 16 or 32 bits)             */


#define				OSTASKIDLESTK					0xa0000F00
#define				OSTASKSTARTSTK					0Xa0000D00
#define				OSTASKSKEY						0Xa0000A00
#define				OSTASKSLCD						0Xa0000800
#define				OSTASKSTDC						0Xa0000600
#define				OSTASKSINFRA					0Xa0000400


/// sizeof(OSTCBPrioTbl_BASEADDR) = (OS_LOWEST_PRIO + 1) * 4 = 256 = 0x100 个字节
/// 这个长度是固定的因为 不管多少个进程最低优先级进程总是  63
#define 			OSTCBPrioTbl_BASEADDR			0Xa0000000
#define				OSTCBTbl_BASEADDR				0Xa0000100


/**** 表示用户模式下 换能器2秒采样一次，
 *  温度采样 是 2*6  = 12秒 一次 ****/
#define				USERWAKE						32
#define				USERMULTI						6


/**** 表示用户模式下 换能器 250ms 采样一次，
 *  温度采样 是 250ms * 8 = 2秒 一次 ****/
#define				DOCIMWAKE						4
#define				DOCIMMULTI						8

#define				WAKETIME						16


/***指定系统时钟  10ms 休眠一次0x13880 = 80000 系统是8M时钟
 *  如果是 1ms 休眠一次 那么 这个宏的值就是  0x1F40 = 8000
 * ***/
#define				TIMERDLY						0x3E80	///0x13880


/**前十八个月每月（记录年月）消耗的热量值、累积流量值以及相对应的时间数据 放在 flash 中 ,
 * 这里定义是从 62K 开始的地方，估计最大也就占用1K
 * 这里不可以定义成const变量，否则这个变量跟代码混在一块，擦除时候也会把代码擦除**/
#define				RecodrFlashAddr					0xbfc0F800


/**  掉电的时候保存的空间，1K应该足够用了 。每次启动的时候首先把这些空间擦除，掉电保存的时候就容易了。
 * 所用时间也就少了  **/
#define				BATFailAddr						0xbfc0FC00

/**返回 通道 0 的电压值，用16 进制返回   0x8BA  对应着 1.8V ; 因为0通道  是  2/3 分频 对应着 2.7V  **/
#define				StandVolt						0x8BA


