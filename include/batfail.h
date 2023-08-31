/*
 * batfail.h
 *
 *  Created on: 2014-2-18
 *      Author: liming
 */

#ifndef BATFAIL_H_
#define BATFAIL_H_

/*
 * ls1d_batfail_test
 * 执行掉电中断测试，如果芯片产生掉电中断，将执行这个函数。
 * 默认向0xbfc0ff00地址写一个字符串"abcdefghijk"。
 */
void ls1d_batfail_test();

#endif /* BATFAIL_H_ */
