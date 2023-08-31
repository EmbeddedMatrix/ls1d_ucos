/*
 * lcd-test.h
 *
 *  Created on: 2013-11-6
 *      Author: liming
 */

#ifndef LCDTEST_H_
#define LCDTEST_H_


	struct lcd_hw_interface {
		_u8 buf[12];
		_u32 num_length;
		const _u8 *number;
		const _u8 *word;
		_u8 minus;
		_u8 dot;
	};
	struct lcd_interface {
		struct lcd_hw_interface hw;
		_u32 (*lcd_display)();
		void (*poweron)();
		void (*powerdown)();
		void (*clear)();
		void (*print_all)();
		void (*write_reg)();
		_u32 option;
		void (*init)();
		void (*process)();
	};


#define LCD_NUM_LENGTH	8
void lcd_print(struct lcd_interface *lcd);

#endif /* LCDTEST_H_ */
