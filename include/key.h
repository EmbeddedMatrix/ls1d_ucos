/*
 * key.h
 *
 *  Created on: 2014-3-4
 *      Author: liming
 */

#ifndef KEY_H_
#define KEY_H_

/*
 * ls1d_key_open
 * 配置key引脚状态为输入，使能key中断。
 *
 * ls1d_key_debug
 * 配置key引脚状态，输出芯片内部tdc信号。
 *
 * ls1d_key_close
 * 关闭key中断。
 */
void ls1d_key_open();
void ls1d_key_debug();
void ls1d_key_close();


#endif /* KEY_H_ */
