/*
 * GENERATED FILE - DO NOT EDIT
 * Copyright (c) 2008-2013 Code Red Technologies Ltd,
 * Copyright 2015, 2018-2019 NXP
 * (c) NXP Semiconductors 2013-2022
 * Generated linker script file for MKL25Z128xxx4
 * Created from memory.ldt by FMCreateLinkMemory
 * Using Freemarker v2.3.30
 * MCUXpresso IDE v11.5.0 [Build 7232] [2022-01-11] on 02-May-2022, 10:19:46 PM
 */

MEMORY
{
  /* Define each memory region */
  PROGRAM_FLASH (rx) : ORIGIN = 0x0, LENGTH = 0x20000 /* 128K bytes (alias Flash) */  
  SRAM (rwx) : ORIGIN = 0x1ffff000, LENGTH = 0x4000 /* 16K bytes (alias RAM) */  
}

  /* Define a symbol for the top of each memory region */
  __base_PROGRAM_FLASH = 0x0  ; /* PROGRAM_FLASH */  
  __base_Flash = 0x0 ; /* Flash */  
  __top_PROGRAM_FLASH = 0x0 + 0x20000 ; /* 128K bytes */  
  __top_Flash = 0x0 + 0x20000 ; /* 128K bytes */  
  __base_SRAM = 0x1ffff000  ; /* SRAM */  
  __base_RAM = 0x1ffff000 ; /* RAM */  
  __top_SRAM = 0x1ffff000 + 0x4000 ; /* 16K bytes */  
  __top_RAM = 0x1ffff000 + 0x4000 ; /* 16K bytes */  
