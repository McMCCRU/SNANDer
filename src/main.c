/*
 * Copyright (C) 2018-2021 McMCC <mcmcc@mail.ru>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include <stdio.h>

#include "flashcmd_api.h"
#include "ch341a_spi.h"
#include "spi_nand_flash.h"

struct flash_cmd prog;
extern unsigned int bsize;

#ifdef EEPROM_SUPPORT
#include "ch341a_i2c.h"
#include "bitbang_microwire.h"
extern struct EEPROM eeprom_info;
extern char eepromname[12];
extern int eepromsize;
extern int mw_eepromsize;
extern int org;
#define EHELP	" -E             select I2C EEPROM {24c01|24c02|24c04|24c08|24c16|24c32|24c64|24c128|24c256|24c512|24c1024}\n" \
		"                select Microwire EEPROM {93c06|93c16|93c46|93c56|93c66|93c76|93c86|93c96} (need SPI-to-MW adapter)\n" \
		" -8             set organization 8-bit for Microwire EEPROM(default 16-bit) and set jumper on SPI-to-MW adapter\n" \
		" -f <addr len>  set manual address size in bits for Microwire EEPROM(default auto)\n"
#else
#define EHELP	""
#endif

#define _VER	"1.7.4"

void title(void)
{
#ifdef EEPROM_SUPPORT
	printf("\nSNANDer - Serial Nor/nAND/Eeprom programmeR v." _VER " by McMCC <mcmcc@mail.ru>\n\n");
#else
	printf("\nSNANDer - Spi Nor/nAND programmER v." _VER " by McMCC <mcmcc@mail.ru>\n\n");
#endif
}

void usage(void)
{
	const char use[] =
		"  Usage:\n"\
		" -h             display this message\n"\
		" -d             disable internal ECC(use read and write page size + OOB size)\n"\
		" -I             ECC ignore errors(for read test only)\n"\
		" -L             print list support chips\n"\
		" -i             read the chip ID info\n"\
		"" EHELP ""\
		" -e             erase chip(full or use with -a [-l])\n"\
		" -l <bytes>     manually set length\n"\
		" -a <address>   manually set address\n"\
		" -w <filename>  write chip with data from filename\n"\
		" -r <filename>  read chip and save data to filename\n"\
		" -v             verify after write on chip\n";
	printf(use);
	exit(0);
}

int main(int argc, char* argv[])
{
	int c, vr = 0, svr = 0, ret = 0;
	char *str, *fname = NULL, op = 0;
	unsigned char *buf;
	int long long len = 0, addr = 0, flen = 0, wlen = 0;
	FILE *fp;

	title();

#ifdef EEPROM_SUPPORT
	while ((c = getopt(argc, argv, "diIhveLl:a:w:r:E:f:8")) != -1)
#else
	while ((c = getopt(argc, argv, "diIhveLl:a:w:r:")) != -1)
#endif
	{
		switch(c)
		{
#ifdef EEPROM_SUPPORT
			case 'E':
				if ((eepromsize = parseEEPsize(optarg, &eeprom_info)) > 0) {
					memset(eepromname, 0, sizeof(eepromname));
					strncpy(eepromname, optarg, 10);
					if (len > eepromsize) {
						printf("Error set size %lld, max size %d for EEPROM %s!!!\n", len, eepromsize, eepromname);
						exit(0);
					}
				} else if ((mw_eepromsize = deviceSize_3wire(optarg)) > 0) {
					memset(eepromname, 0, sizeof(eepromname));
					strncpy(eepromname, optarg, 10);
					org = 1;
					if (len > mw_eepromsize) {
						printf("Error set size %lld, max size %d for EEPROM %s!!!\n", len, mw_eepromsize, eepromname);
						exit(0);
					}
				} else {
					printf("Unknown EEPROM chip %s!!!\n", optarg);
					exit(0);
				}
				break;
			case '8':
				if (mw_eepromsize <= 0)
				{
					printf("-8 option only for Microwire EEPROM chips!!!\n");
					exit(0);
				}
				org = 0;
				break;
			case 'f':
				if (mw_eepromsize <= 0)
				{
					printf("-f option only for Microwire EEPROM chips!!!\n");
					exit(0);
				}
				str = strdup(optarg);
				fix_addr_len = strtoll(str, NULL, *str && *(str + 1) == 'x' ? 16 : 10);
				if (fix_addr_len > 32) {
						printf("Address len is very big!!!\n");
						exit(0);
				}
				break;
#endif
			case 'I':
				ECC_ignore = 1;
				break;
			case 'd':
				ECC_fcheck = 0;
				_ondie_ecc_flag = 0;
				break;
			case 'l':
				str = strdup(optarg);
				len = strtoll(str, NULL, *str && *(str + 1) == 'x' ? 16 : 10);
				break;
			case 'a':
				str = strdup(optarg);
				addr = strtoll(str, NULL, *str && *(str + 1) == 'x' ? 16 : 10);
				break;
			case 'v':
				vr = 1;
				break;
			case 'i':
			case 'e':
				if(!op)
					op = c;
				else
					op = 'x';
				break;
			case 'r':
			case 'w':
				if(!op) {
					op = c;
					fname = strdup(optarg);
				} else
					op = 'x';
				break;
			case 'L':
				support_flash_list();
				exit(0);
			case 'h':
			default:
				usage();
		}
	}

	if (op == 0) usage();

	if (op == 'x' || (ECC_ignore && !ECC_fcheck) || (op == 'w' && ECC_ignore)) {
		printf("Conflicting options, only one option at a time.\n\n");
		return -1;
	}

	if (ch341a_spi_init() < 0) {
		printf("Programmer device not found!\n\n");
		return -1;
	}

	if((flen = flash_cmd_init(&prog)) <= 0)
		goto out;

#ifdef EEPROM_SUPPORT
	if ((eepromsize || mw_eepromsize) && op == 'i') {
		printf("Programmer not supported auto detect EEPROM!\n\n");
		goto out;
	}
#else
	if (op == 'i') goto out;
#endif

	if (op == 'e') {
		printf("ERASE:\n");
		if(addr && !len)
			len = flen - addr;
		else if(!addr && !len) {
			len = flen;
			printf("Set full erase chip!\n");
		}
		if(len % bsize) {
			printf("Please set len = 0x%016llX multiple of the block size 0x%08X\n", len, bsize);
			goto out;
		}
		printf("Erase addr = 0x%016llX, len = 0x%016llX\n", addr, len);
		ret = prog.flash_erase(addr, len);
		if(!ret)
			printf("Status: OK\n");
		else
			printf("Status: BAD(%d)\n", ret);
		goto out;
	}

	if ((op == 'r') || (op == 'w')) {
		if(addr && !len)
			len = flen - addr;
		else if(!addr && !len) {
			len = flen;
		}
		buf = (unsigned char *)malloc(len + 1);
		if (!buf) {
			printf("Malloc failed for read buffer.\n");
			goto out;
		}
	}

	if (op == 'w') {
		printf("WRITE:\n");
		fp = fopen(fname, "rb");
		if (!fp) {
			printf("Couldn't open file %s for reading.\n", fname);
			free(buf);
			goto out;
		}
		wlen = fread(buf, 1, len, fp);
		if (ferror(fp)) {
			printf("Error reading file [%s]\n", fname);
			if (fp)
				fclose(fp);
			free(buf);
			goto out;
		}
		if(len == flen)
			len = wlen;
		printf("Write addr = 0x%016llX, len = 0x%016llX\n", addr, len);
		ret = prog.flash_write(buf, addr, len);
		if(ret > 0) {
			printf("Status: OK\n");
			if (vr) {
				op = 'r';
				svr = 1;
				printf("VERIFY:\n");
				goto very;
			}
		}
		else
			printf("Status: BAD(%d)\n", ret);
		fclose(fp);
		free(buf);
	}

very:
	if (op == 'r') {
		if (!svr) printf("READ:\n");
		else memset(buf, 0, len);
		printf("Read addr = 0x%016llX, len = 0x%016llX\n", addr, len);
		ret = prog.flash_read(buf, addr, len);
		if (ret < 0) {
			printf("Status: BAD(%d)\n", ret);
			free(buf);
			goto out;
		}
		if (svr) {
			unsigned char ch1;
			int i = 0;

			fseek(fp, 0, SEEK_SET);
			ch1 = (unsigned char)getc(fp);

			while ((ch1 != EOF) && (i < len - 1) && (ch1 == buf[i++]))
				ch1 = (unsigned char)getc(fp);

			if (ch1 == buf[i])
				printf("Status: OK\n");
			else
				printf("Status: BAD\n");
			fclose(fp);
			free(buf);
			goto out;
		}
		fp = fopen(fname, "wb");
		if (!fp) {
			printf("Couldn't open file %s for writing.\n", fname);
			free(buf);
			goto out;
		}
		fwrite(buf, 1, len, fp);
		if (ferror(fp))
			printf("Error writing file [%s]\n", fname);
		fclose(fp);
		free(buf);
		printf("Status: OK\n");
	}

out:
	ch341a_spi_shutdown();
	return 0;
}
