/*
 * SPI NOR Flash Unique ID (UID) utility header
 * 
 * Provides functions for reading and parsing SPI NOR Flash Unique ID
 * Supports multiple chip manufacturers: Winbond, Macronix, GigaDevice, Eon, etc.
 */

#ifndef __SPI_UID_UTIL_H__
#define __SPI_UID_UTIL_H__

#include <stdint.h>

/* Maximum UID length supported */
#define MAX_UID_LENGTH 32

/* UID read status codes */
typedef enum {
    UID_READ_SUCCESS = 0,
    UID_READ_ERROR_INVALID_PARAMS = -1,
    UID_READ_ERROR_DEVICE_NOT_READY = -2,
    UID_READ_ERROR_TRANSFER_FAILED = -3,
    UID_READ_ERROR_LENGTH_EXCEEDED = -4
} uid_read_status_t;

/* Chip manufacturer IDs */
typedef enum {
    CHIP_MFR_UNKNOWN = 0x00,
    CHIP_MFR_WINBOND = 0xEF,
    CHIP_MFR_MACRONIX = 0xC2,
    CHIP_MFR_GIGADEVICE = 0xC8,
    CHIP_MFR_EON = 0x1C,
    CHIP_MFR_ZETTA = 0xBA,
    CHIP_MFR_SPANSION = 0x01,
    CHIP_MFR_ATMEL = 0x1F,
    CHIP_MFR_MICRON = 0x2C
} chip_manufacturer_t;

/* UID information structure */
typedef struct {
    uint8_t raw_data[MAX_UID_LENGTH];
    int length;
    chip_manufacturer_t manufacturer;
    uint32_t uid_32bit;  /* First 4 bytes as 32-bit value */
    int valid_bits;      /* Number of non-trivial bytes */
} spi_uid_info_t;

/**
 * Read SPI NOR Flash Unique ID
 * 
 * @param uid       Buffer to store UID data
 * @param uid_len   Number of bytes to read (max 32)
 * @return          Number of bytes read on success, negative value on error
 */
int snor_read_uid(unsigned char *uid, int uid_len);

/**
 * Parse UID information
 * 
 * @param uid       Raw UID data
 * @param uid_len   Length of UID data
 * @param info      Pointer to UID info structure to fill
 * @return          0 on success, -1 on error
 */
int snor_parse_uid(const unsigned char *uid, int uid_len, spi_uid_info_t *info);

/**
 * Get manufacturer name from ID
 * 
 * @param mfr_id    Manufacturer ID
 * @return          Pointer to manufacturer name string
 */
const char *snor_get_manufacturer_name(uint8_t mfr_id);

/**
 * Display UID in formatted output
 * 
 * @param uid       UID data
 * @param uid_len   Length of UID
 * @param verbose   If true, show additional analysis
 */
void snor_display_uid(const unsigned char *uid, int uid_len, int verbose);

#endif /* __SPI_UID_UTIL_H__ */
