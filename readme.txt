SNANDer - Serial Nor/nAND/Eeprom programmeR v.1.7.5 by McMCC <mcmcc_at_mail.ru>

  Usage:
 -h             display this message
 -d             disable internal ECC(use read and write page size + OOB size)
 -I             ECC ignore errors(for read test only)
 -L             print list support chips
 -i             read the chip ID info
 -E             select I2C EEPROM {24c01|24c02|24c04|24c08|24c16|24c32|24c64|24c128|24c256|24c512|24c1024}
                select Microwire EEPROM {93c06|93c16|93c46|93c56|93c66|93c76|93c86|93c96} (need SPI-to-MW adapter)
 -8             set organization 8-bit for Microwire EEPROM(default 16-bit) and set jumper on SPI-to-MW adapter
 -f <addr len>  set manual address size in bits for Microwire EEPROM(default auto)
 -e             erase chip(full or use with -a [-l])
 -l <bytes>     manually set length
 -a <address>   manually set address
 -w <filename>  write chip with data from filename
 -r <filename>  read chip and save data to filename
 -v             verify after write on chip

Examples:

1. Get info Flash.

igor@mcmcc-GL553VE:~/Soft/SNANDer-bin/Linux$ ./SNANDer -i

SNANDer - Serial Nor/nAND/Eeprom programmeR v.1.7.5 by McMCC <mcmcc_at_mail.ru>

Found programmer device: WinChipHead (WCH) - CH341A
Device revision is 3.0.4
spi_nand_probe: mfr_id = 0xc2, dev_id = 0x22
Get Status Register 1: 0x38
Get Status Register 2: 0x10
Using Flash ECC.
Detected SPI NAND Flash: MXIC MX35LF2G, Flash Size: 256 MB

or

SNANDer - Serial Nor/nAND/Eeprom programmeR v.1.7.5 by McMCC <mcmcc_at_mail.ru>

Found programmer device: Winchiphead (WCH) - CH341A
Device revision is 3.0.4
spi_nand_probe: mfr_id = 0xef, dev_id = 0xaa
Get Status Register 1: 0x81
Get Status Register 2: 0x18
Using Flash ECC.
Detected SPI NAND Flash: WINBOND W25N01G, Flash Size: 128 MB

2. Full erase flash with disable internal ECC check. Without OOB, page size 2112 bytes.

igor@mcmcc-GL553VE:~/Soft/SNANDer-bin/Linux$ ./SNANDer -d -e

SNANDer - Serial Nor/nAND/Eeprom programmeR v.1.7.5 by McMCC <mcmcc_at_mail.ru>

Found programmer device: WinChipHead (WCH) - CH341A
Device revision is 3.0.4
spi_nand_probe: mfr_id = 0xc2, dev_id = 0x22
Get Status Register 1: 0x00
Get Status Register 2: 0x11
Disable Flash ECC.
Detected SPI NAND Flash: MXIC MX35LF2G, Flash Size: 256 MB
ERASE:
Set full erase chip!
Erase addr = 0x0000000000000000, len = 0x0000000010800000
Erase 100% [276824064] of [276824064] bytes
Elapsed time: 3 seconds
Status: OK
igor@mcmcc-GL553VE:~/Soft/SNANDer-bin/Linux$

3. Write and verify flash with disable internal ECC check. Without OOB, page size 2112 bytes.

igor@mcmcc-GL553VE:~/Soft/SNANDer-bin/Linux$ ./SNANDer -d -v -w ecc_2Gb_2K_64_flashimage_rfb1_ac2600.bin 

SNANDer - Serial Nor/nAND/Eeprom programmeR v.1.7.5 by McMCC <mcmcc_at_mail.ru>

Found programmer device: WinChipHead (WCH) - CH341A
Device revision is 3.0.4
spi_nand_probe: mfr_id = 0xc2, dev_id = 0x22
Get Status Register 1: 0x00
Get Status Register 2: 0x11
Disable Flash ECC.
Detected SPI NAND Flash: MXIC MX35LF2G, Flash Size: 256 MB
WRITE:
Write addr = 0x0000000000000000, len = 0x000000000E5A9D6F
Written 100% [240819567] of [240819567] bytes
Elapsed time: 4184 seconds
Status: OK
VERIFY:
Read addr = 0x0000000000000000, len = 0x000000000E5A9D6F
Read 100% [240819567] of [240819567] bytes
Elapsed time: 2047 seconds
Status: OK

4. Read Microwire EEPROM Atmel 93C46 and save file.

igor@igor-GL553VE:~/Soft/SNANDer-bin/Linux$ ./SNANDer -E 93c46 -r test.bin

SNANDer - Serial Nor/nAND/Eeprom programmeR v.1.7.5 by McMCC <mcmcc_at_mail.ru>

Found programmer device: WinChipHead (WCH) - CH341A
Device revision is 3.0.4
Microwire EEPROM chip: 93c46, Size: 64 bytes, Org: 16 bits, fix addr len: Auto
READ:
Read addr = 0x0000000000000000, len = 0x0000000000000080
Read_EEPROM_3wire: Set address len 6 bits
Read 100% [64] of [64] bytes
Read [128] bytes from [93c46] EEPROM address 0x00000000
Elapsed time: 0 seconds
Status: OK

5. Write and verify Microwire EEPROM Atmel 93C46 from file.

igor@igor-GL553VE:~/Soft/SNANDer-bin/Linux$ ./SNANDer -E 93c46 -w test.bin -v

SNANDer - Serial Nor/nAND/Eeprom programmeR v.1.7.5 by McMCC <mcmcc_at_mail.ru>

Found programmer device: WinChipHead (WCH) - CH341A
Device revision is 3.0.4
Microwire EEPROM chip: 93c46, Size: 64 bytes, Org: 16 bits, fix addr len: Auto
WRITE:
Write addr = 0x0000000000000000, len = 0x0000000000000080
Erase_EEPROM_3wire: Set address len 6 bits
Write_EEPROM_3wire: Set address len 6 bits
Written 100% [64] of [64] bytes
Wrote [128] bytes to [93c46] EEPROM address 0x00000000
Elapsed time: 1 seconds
Status: OK
VERIFY:
Read addr = 0x0000000000000000, len = 0x0000000000000080
Read_EEPROM_3wire: Set address len 6 bits
Read 100% [64] of [64] bytes
Read [128] bytes from [93c46] EEPROM address 0x00000000
Elapsed time: 1 seconds
Status: OK
