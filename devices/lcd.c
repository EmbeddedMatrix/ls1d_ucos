/*
 * lcd.c
 *	Lcd print testing.
 *
 *  Created on: 2013-1-10
 *      Author: liming
 */

#include "includes.h"


void LcdPoweron()
{
	//INTERRUPT_DISABLE;
	PMU_SecToken = PMU_SecToken_chipctrl;
	PMU_ChipCtrl &= ~(1<<9);
	//INTERRUPT_ENABLE;
}

void LcdPowerdown()
{
	//INTERRUPT_DISABLE;
	PMU_SecToken = PMU_SecToken_chipctrl;
	PMU_ChipCtrl |= (1<<9);
	//INTERRUPT_ENABLE;
}

void WriteLcdRegs(INT8U *buf)
{
	INT32U *data = (INT32U *)buf;

    LCD_FB_0 = (data[0]);
    LCD_FB_1 = (data[1]);
    LCD_FB_2 = (data[2]);
}

void Lcd_HW_Init()
{
	LCD_RefeshRate = 0x10;

	//IO init
	//INTERRUPT_DISABLE;
	PMU_SecToken = PMU_SecToken_chipctrl;
	PMU_ChipCtrl &= ~0x3c00200; //lcd power on
	PMU_SecToken = PMU_SecToken_chipctrl;
	PMU_ChipCtrl |= 0x100;  //GPIO->lcd
	//INTERRUPT_ENABLE;
}

void WriteLcdAll(void )
{
    LCD_FB_0 = 0xf0f0f0f0;
    LCD_FB_1 = 0xf0f0f0f0;
    LCD_FB_2 = 0xf0f0f0f0;
}





