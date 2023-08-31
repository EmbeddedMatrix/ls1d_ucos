#include "includes.h"


/** 成功返回0，失败返回 1 **/
_u32 Flash_Erase_page(_u32 addr)  //erase a flash page, FLASH_PAGE_SIZE
{
	_u32 page_addr = (addr & (~(FLASH_PAGE_SIZE - 1))) ;
	FLASH_PEctrl_REG = 0x2;  // erase time, 2.5ms
	FLASH_CMD_REG = FLASH_Command_erase_page | (page_addr & FLASH_Command_Daddr_MASK);

	_u32 i;
	for(i=0; i<FLASH_PAGE_SIZE/4; i += 4)
	{
		if(*(volatile _u32 *)(page_addr+i) != 0x0)
			return 1;
	}

	return 0 ;
}


/*
	 This function will modify X(num) word, started from addr.
*/
_u32 Flash_Write(_u32 addr, _u32 *data, _u32 num)
{
	_u32 i, j, offset, write_addr;
	j = 0;
	write_addr = addr;
	_u32 flash_block_mask = FLASH_PAGE_SIZE - 1;
	_u32 old_data[FLASH_PAGE_SIZE/4];

	while(j<num)
	{
		for(i=0; i<FLASH_PAGE_SIZE/4; i++)
			old_data[i] = *(_u32 *)((write_addr & ~flash_block_mask)+i*4) ;

		FLASH_CMD_REG = FLASH_Command_clr_page_latch;
		/*wait interrupt ? Should I disable other int??????*/
		//debug("Page_latch is clean now...\n")

		_u32 *page_data = (_u32 *)(write_addr & ~flash_block_mask) ;

		offset = ((write_addr & flash_block_mask)>>2);  //页内字偏移
		for(i=0; i<(FLASH_PAGE_SIZE/4); i++)
		{
			if( (offset <= i)  && (j < num) )
				page_data[i] = data[j++];
			else
				page_data[i] = old_data[i];
		}

		_u32 err;
		err = Flash_Erase_page(write_addr);
		/*wait interrupt ? */
		/*Flash page is clean now*/
		if(err)
			return 1;

		FLASH_PEctrl_REG = 0x2;  // write time, 2.5ms
		FLASH_CMD_REG = FLASH_Command_prog_page | (write_addr & FLASH_Command_Daddr_MASK) ;
		/*wait interrupt ? */
		/*Flash page write is finish*/

		write_addr = (write_addr & ~flash_block_mask) + FLASH_PAGE_SIZE;  //待写数据超过当前页, 则继续写下一页
	}
	return 0;
}

//_u32 Flash_Write(_u32 addr, _u32 *data, _u32 num)
//RecodrFlashTbl
// MonthlySaveIndex
/**处理月结数据 */
void	MonthlySave(void)
{
	RecordInfo	temp;

	/*temp.heat	= AccumHeat;
	temp.flux	= InteFlux;
	temp.month	= month;
	temp.year	= year;*/
	temp.heat	= 13.4571f;
	temp.flux	= 4513.4f;
	temp.month	= 6;
	temp.year	= 14;
	Flash_Write((_u32)&RecodrFlashTbl[MonthlySaveIndex],(_u32 *)&temp,sizeof(RecordInfo));
	MonthlySaveIndex	++;
	if(MonthlySaveIndex >= 18)
		MonthlySaveIndex	= 0;
}

