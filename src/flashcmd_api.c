/*
 * Copyright (C) 2018 McMCC <mcmcc@mail.ru>
 * flashcmd_api.c
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <stdio.h>
#include "flashcmd_api.h"

#ifdef I2C_EEPROM_SUPPORT
#define __EEPROM___	"or EEPROM"
#else
#define __EEPROM___	""
#endif

long flash_cmd_init(struct flash_cmd *cmd)
{
	long flen = -1;

	if((flen = snand_init()) > 0) {
		cmd->flash_erase = snand_erase;
		cmd->flash_write = snand_write;
		cmd->flash_read  = snand_read;
	} else if ((flen = snor_init()) > 0) {
		cmd->flash_erase = snor_erase;
		cmd->flash_write = snor_write;
		cmd->flash_read  = snor_read;
#ifdef I2C_EEPROM_SUPPORT
	} else if ((flen = i2c_init()) > 0) {
		cmd->flash_erase = i2c_eeprom_erase;
		cmd->flash_write = i2c_eeprom_write;
		cmd->flash_read  = i2c_eeprom_read;
#endif
	} else
		printf("\nNot Flash" __EEPROM___ " detected!!!!\n\n");

	return flen;
}

void support_flash_list(void)
{
	support_snand_list();
	printf("\n");
	support_snor_list();
#ifdef I2C_EEPROM_SUPPORT
	printf("\n");
	support_i2c_eeprom_list();
#endif
}
