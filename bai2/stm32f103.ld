MEMORY
{
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 64K
  RAM  (rwx) : ORIGIN = 0x20000000, LENGTH = 20K
}

__stack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
  .text :
  {
    KEEP(*(.vectors))
    *(.text*)
    *(.rodata*)
    . = ALIGN(4);
  } > FLASH

  .data :
  {
    *(.data*)
    . = ALIGN(4);
  } > RAM AT > FLASH

  .bss :
  {
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
  } > RAM
}
