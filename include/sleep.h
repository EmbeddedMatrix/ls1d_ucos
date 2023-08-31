/*
 * sleep.h
 *
 *  Created on: 2014-3-4
 *      Author: liming
 */

#ifndef SLEEP_H_
#define SLEEP_H_

/*
 * ls1d_sleep
 * 将io_select写入PMU_ChipCtrl，配置复用关系，
 * 根据io_direction的值配置GPIO的输入输出方向，
 * 根据io_status的值配置GPIO的输出电平。
 */
void ls1d_sleep(_u32 io_select, _u32 io_direction, _u32 io_status);

/*
 * Check_Data
 * 对于一个数据采集系统来说，系统复位操作之后，紧接着需要恢复历史数据；
 * 一般地，需要从一个非易失的记忆体中的特定位置读出历史数据，
 * 比如从ls1d芯片的片内flash中，或者1KB不掉电的SRAM中读出历史数据；
 * 请用户根据需求自行定义该函数的实现。
 */
void Check_Data();

#endif /* SLEEP_H_ */
