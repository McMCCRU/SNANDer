SNANDer - Serial Nor/nAND/Eeprom programmeR v.1.6 by McMCC <mcmcc_at_mail.ru>

  Usage:
 -h             display this message
 -d             disable internal ECC(use read and write page size + OOB size)
 -I             ECC ignore errors(for read test only)
 -L             print list support chips
 -i             read the chip ID info
 -E             select I2C EEPROM {24c01|24c02|24c04|24c08|24c16|24c32|24c64|24c128|24c256|24c512|24c1024}
                select Microwire EEPROM {93c06|93c16|93c46|93c56|93c66|93c76|93c86|93c96} (need SPI-to-MW adapter)
 -8             set organization 8-bit for Microwire EEPROM(default 16-bit) and set jumper on SPI-to-MW adapter
 -e             erase chip(full or use with -a [-l])
 -l <bytes>     manually set length
 -a <address>   manually set address
 -w <filename>  write chip with data from filename
 -r <filename>  read chip and save data to filename
 -v             verify after write on chip

Examples:

1. Get info Flash.

igor@mcmcc-GL553VE:~/Soft/SNANDer-bin/Linux$ ./SNANDer -i

Spi NAND programmER v.1.0 by McMCC <mcmcc_at_mail.ru>

Found programmer device: Winchiphead (WCH) - CH341A
Device revision is 3.0.4
spi_nand_probe: mfr_id = 0xef, dev_id = 0xaa
Get Status Register 1: 0x81
Get Status Register 2: 0x18
Using Flash ECC.
Detected SPI NAND Flash: WINBOND W25N01G, Flash Size: 128 MB
igor@mcmcc-GL553VE:~/Soft/SNANDer-bin/Linux$

2. Full erase flash with disable internal ECC check. Without OOB, page size 2112 bytes.

igor@mcmcc-GL553VE:~/Soft/SNANDer-bin/Linux$ ./SNANDer -d -e

SNANDer - Serial Nor/nAND/Eeprom programmeR v.1.6 by McMCC <mcmcc_at_mail.ru>

Found programmer device: Winchiphead (WCH) - CH341A
Device revision is 3.0.4
spi_nand_probe: mfr_id = 0xef, dev_id = 0xaa
Get Status Register 1: 0x81
Get Status Register 2: 0x18
Disable Flash ECC.
Detected SPI NAND Flash: WINBOND W25N01G, Flash Size: 128 MB
ERASE:
Set full erase chip!
Erase addr = 0x0000000000000000, len = 0x0000000008400000
Status: OK
igor@mcmcc-GL553VE:~/Soft/SNANDer-bin/Linux$

3. Write and verify flash with disable internal ECC check. Without OOB, page size 2112 bytes.

igor@mcmcc-GL553VE:~/Soft/SNANDer-bin/Linux$ ./SNANDer -d -v -w ecc_1Gb_2K_64_flashimage_rfb1_ac2600.bin 

SNANDer - Serial Nor/nAND/Eeprom programmeR v.1.6 by McMCC <mcmcc_at_mail.ru>

Found programmer device: Winchiphead (WCH) - CH341A
Device revision is 3.0.4
spi_nand_probe: mfr_id = 0xef, dev_id = 0xaa
Get Status Register 1: 0x81
Get Status Register 2: 0x08
Disable Flash ECC.
Detected SPI NAND Flash: WINBOND W25N01G, Flash Size: 128 MB
WRITE:
Write addr = 0x0000000000000000, len = 0x0000000001080000
snand_erase_write: offs:0, count:1080000
..........................................................................................................
..........................................................................................................
..........................................................................................................
..........................................................................................................
........................................................................................................Done!
Status: OK
VERIFY:
Read addr = 0x0000000000000000, len = 0x0000000001080000
Status: OK
igor@mcmcc-GL553VE:~/Soft/SNANDer-bin/Linux$
