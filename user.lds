/*
 * test.lds
 *
 *  Created on: 2013-1-10
 *      Author: liming
 */

OUTPUT_FORMAT("elf32-tradlittlemips", "elf32-tradlittlemips", "elf32-tradlittlemips")

OUTPUT_ARCH(mips)
ENTRY(_start)
SECTIONS
{

	. = ALIGN(4);
	.text       :
	{
	  *(.text)
	}

	. = ALIGN(4);
	.rel.text  : { *(.rel.text) }

	. = ALIGN(4);
	.rodata  : { *(.rodata) }

	. = ALIGN(4);
	.data  : { *(.data) }

	. = ALIGN(4);
	.reginfo  : { *(.reginfo) }

	. = ALIGN(4);
	.ptr  : { *(.ptr) }

	. = ALIGN(4);
	.shstrtab  : { *(.shstrtab) }

	. = ALIGN(4);
	.strtab  : { *(.strtab) }

	. = 0xa0001000;
	. = ALIGN(4);
	.scommon : {  *(.scommon) }
	.sbss  : { *(.sbss) }
	.bss  : { *(.bss) . = ALIGN(4); }
}

