
#include "includes.h"

void ls1d_key_init()
{
	PMU_SecToken = PMU_SecToken_chipctrl;
	PMU_ChipCtrl &= ~0x480;
	PMU_SecToken = PMU_SecToken_command;
	PMU_Command |= 0x2000;
}

