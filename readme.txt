SNANDer - Serial Nor/nAND/Eeprom programmeR v.1.7.9.4 by McMCC <mcmcc_at_mail.ru>

  Usage:
 -h             display this message
 -d             disable internal ECC(use read and write page size + OOB size - On-die ECC)
 -o <bytes>     manual set OOB size with disable internal ECC(default 0)
 -I             ECC ignore errors(for read test only)
 -k             Skip BAD pages, try read or write in to next page
 -L             print list support chips
 -i             read the chip ID info
 -u             read Unique ID (UID) of SPI NOR Flash
 -E             select I2C EEPROM {24c01|24c02|24c04|24c08|24c16|24c32|24c64|24c128|24c256|24c512|24c1024|24c2048}
                select Microwire EEPROM {93c06|93c16|93c46|93c56|93c66|93c76|93c86|93c96} (need SPI-to-MW adapter)
                select SPI EEPROM 25xxx {25010|25020|25040|25080|25160|25320|25640|25128|25256|25512|251024}
 -8             set organization 8-bit for Microwire EEPROM(default 16-bit) and set jumper on SPI-to-MW adapter
 -f <addr len>  set manual address size in bits for Microwire EEPROM(default auto)
 -s <bytes>     set page size from datasheet for fast write SPI EEPROM(default not usage)
 -e             erase chip(full or use with -a [-l])
 -l <bytes>     manually set length
 -a <address>   manually set address
 -w <filename>  write chip with data from filename
 -r <filename>  read chip and save data to filename
 -v             verify after write on chip

Examples:

1. Get info Flash.

igor@mcmcc-GL553VE:~/Soft/SNANDer-bin/Linux$ ./SNANDer -i

SNANDer - Serial Nor/nAND/Eeprom programmeR v.1.7.9.3 by McMCC <mcmcc_at_mail.ru>

Found programmer device: WinChipHead (WCH) - CH341A
Device revision is 3.0.4
spi_nand_probe: mfr_id = 0xcd, dev_id = 0x70, dev_id_2 = 0x70
Get Status Register 1: 0x7c
Get Status Register 2: 0x10
Using Flash ECC.
Detected SPI NAND Flash: FORESEE F35SQA512M, Flash Size: 64MB, OOB Size: 64B
Set Status Register 1: 0x00
Set Status Register 2: 0x11

or

SNANDer - Serial Nor/nAND/Eeprom programmeR v.1.7.9.3 by McMCC <mcmcc_at_mail.ru>

Found programmer device: WinChipHead (WCH) - CH341A
Device revision is 3.0.4
spi_nand_probe: mfr_id = 0xb0, dev_id = 0x1c, dev_id_2 = 0xb0
Get Status Register 1: 0x3e
Get Status Register 2: 0x10
Using Flash ECC.
Detected SPI NAND Flash: UNIM UM19C0HISW, Flash Size: 128MB, OOB Size: 64B
Set Status Register 1: 0x06
Set Status Register 2: 0x11

2. Full erase flash with disable internal ECC check. Without OOB, page size 2112 bytes.

igor@mcmcc-GL553VE:~/Soft/SNANDer-bin/Linux$ ./SNANDer -d -e

SNANDer - Serial Nor/nAND/Eeprom programmeR v.1.7.9.3 by McMCC <mcmcc_at_mail.ru>

Found programmer device: WinChipHead (WCH) - CH341A
Device revision is 3.0.4
spi_nand_probe: mfr_id = 0xcd, dev_id = 0x70, dev_id_2 = 0x70
Get Status Register 1: 0x00
Get Status Register 2: 0x11
Disable Flash ECC.
Detected SPI NAND Flash: FORESEE F35SQA512M, Flash Size: 64MB, OOB Size: 64B
Set Status Register 1: 0x00
Set Status Register 2: 0x01
ERASE:
Set full erase chip!
Erase addr = 0x0000000000000000, len = 0x0000000004200000
Erase 100% [69206016] of [69206016] bytes
Elapsed time: 1 seconds
Status: OK

3. Write and verify flash with disable internal ECC check. Without OOB, page size 2112 bytes.

igor@mcmcc-GL553VE:~/Soft/SNANDer-bin/Linux$ ./SNANDer -d -v -w ecc_512M_2K_64_flashimage_rfb1_ac2600.bin 

SNANDer - Serial Nor/nAND/Eeprom programmeR v.1.7.9.3 by McMCC <mcmcc_at_mail.ru>

Found programmer device: WinChipHead (WCH) - CH341A
Device revision is 3.0.4
spi_nand_probe: mfr_id = 0xcd, dev_id = 0x70, dev_id_2 = 0x70
Get Status Register 1: 0x00
Get Status Register 2: 0x01
Disable Flash ECC.
Detected SPI NAND Flash: FORESEE F35SQA512M, Flash Size: 64MB, OOB Size: 64B
Set Status Register 1: 0x00
Set Status Register 2: 0x01
WRITE:
Write addr = 0x0000000000000000, len = 0x00000000017200C0
Written 100% [24248512] of [24248512] bytes
Elapsed time: 405 seconds
Status: OK
VERIFY:
Read addr = 0x0000000000000000, len = 0x00000000017200C0
Read 100% [24248512] of [24248512] bytes
Elapsed time: 193 seconds
Status: OK

4. Read Microwire EEPROM Atmel 93C46 and save file.

igor@igor-GL553VE:~/Soft/SNANDer-bin/Linux$ ./SNANDer -E 93c46 -r test.bin

SNANDer - Serial Nor/nAND/Eeprom programmeR v.1.7.9.3 by McMCC <mcmcc_at_mail.ru>

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

SNANDer - Serial Nor/nAND/Eeprom programmeR v.1.7.9.3 by McMCC <mcmcc_at_mail.ru>

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

6. Write and verify SPI EEPROM Atmel AT25640B from file.

igor@igor-GL553VE:~/Soft/SNANDer-bin/Linux$ ./SNANDer -E 25640 -v -w test.bin

SNANDer - Serial Nor/nAND/Eeprom programmeR v.1.7.9.3 by McMCC <mcmcc_at_mail.ru>

Found programmer device: WinChipHead (WCH) - CH341A
Device revision is 3.0.4
SPI EEPROM chip: 25640, Size: 8192 bytes
WRITE:
Write addr = 0x0000000000000000, len = 0x0000000000002000
Written 100% [8192] bytes to [25640] EEPROM address 0x00000000
Elapsed time: 22 seconds
Status: OK
VERIFY:
Read addr = 0x0000000000000000, len = 0x0000000000002000
Read 100% [8192] bytes from [25640] EEPROM address 0x00000000
Elapsed time: 2 seconds
Status: OK

7. Fast write and verify SPI EEPROM Atmel AT25640B from file with use page size.
   (Find out page size from datasheet on chip!!!)

igor@igor-GL553VE:~/Soft/SNANDer-bin/Linux$ ./SNANDer -E 25640 -v -w test.bin -s 32

SNANDer - Serial Nor/nAND/Eeprom programmeR v.1.7.9.3 by McMCC <mcmcc_at_mail.ru>

Found programmer device: WinChipHead (WCH) - CH341A
Device revision is 3.0.4
SPI EEPROM chip: 25640, Size: 8192 bytes
Setting page size 32B for write.
WRITE:
Write addr = 0x0000000000000000, len = 0x0000000000002000
Written 100% [8192] bytes to [25640] EEPROM address 0x00000000
Elapsed time: 1 seconds
Status: OK
VERIFY:
Read addr = 0x0000000000000000, len = 0x0000000000002000
Read 100% [8192] bytes from [25640] EEPROM address 0x00000000
Elapsed time: 2 seconds
Status: OK
