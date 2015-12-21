OUTPUT_ARCH(arm)

ENTRY(_start)

romBase = 0x00201000;
ramBase = 0x00102180;
pll_init_baseaddr = 0x0010DC00;

MEMORY {
    ram : ORIGIN = ramBase, LENGTH = 0xBA80
    rom : ORIGIN = romBase, LENGTH = 0x1F000
}

SECTIONS {

    . = romBase;
    .start ALIGN(4) : {	
        *(.text.start)
    } >rom
    
    . = romBase + 0x01FC;
    .rom_info ALIGN(4) : {	    	    	    	     
    	*(.data.rom_info)
    } >rom 

    .text ALIGN(4) : {
        *(.text)
/*        *(.text.*)     for get .text.mt_pll_init   */
    } >rom

    .rodata ALIGN(4) : {
        *(.rodata)
        *(.rodata.*)        
    } >rom

    .data ALIGN(4) : {
        *(.data)
        *(.data.*)        
    } >rom

    .got ALIGN(4) : {
        *(.got)
        *(.got.*)        
    } >rom

    .pll_text pll_init_baseaddr : AT (ADDR(.got) + SIZEOF(.got))
    {
        Image$$PLL_INIT$$Base = . ;
        *(.text.mt_pll_init)
        Image$$PLL_INIT$$Limit = . ;
    } >rom

    __load_start_pll_text = LOADADDR (.pll_text);
    __load_stop_pll_text = LOADADDR (.pll_text) + SIZEOF (.pll_text);

    __boot_end = .;

    . = ramBase;
    .bss ALIGN(16) : {
        _bss_start = .;
        *(.bss)
        *(.bss.*)
        *(COMMON)
        _bss_end = .;
    } >ram
	
	.secbuf ALIGN(4) : {
		_secbuf_start = .;
		*(.secbuf)
		_secbuf_end = .;
	} >ram

}
