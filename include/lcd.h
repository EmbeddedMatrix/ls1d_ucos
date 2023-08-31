/*
 * lcd.h
 *
 *  Created on: 2013-1-10
 *      Author: liming
 */

#ifndef LCD_H_
#define LCD_H_

/*
 * ls1d_lcd_poweron
 * 打开lcd电源
 *
 * ls1d_lcd_powerdown
 * 关闭lcd电源
 */
void ls1d_lcd_poweron();
void ls1d_lcd_powerdown();

/*
 * LcdClear
 * 将lcd驱动的缓冲区清0。
 */
void LcdClear(_u8 *buf);

/*
 * LcdPrintAll
 * 将lcd驱动的缓冲区写全“1”。
 */
void LcdPrintAll(_u8 *buf);

/*
 * WriteLcdRegs
 * 将lcd驱动的缓冲区内的数据写入lcd控制器的显示buffer。
 */
void WriteLcdRegs(_u8 *buf);

/*
 * Lcd_HW_Init
 * 初始化lcd控制器。
 */
void Lcd_HW_Init();


#endif /* LCD_H_ */
