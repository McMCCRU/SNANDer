/*
 * SPI NOR Flash Unique ID (UID) utility implementation
 * 
 * Provides utility functions for reading and parsing SPI NOR Flash Unique ID
 * Supports multiple chip manufacturers
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "spi_uid_util.h"

/**
 * Get manufacturer name from ID
 */
const char *snor_get_manufacturer_name(uint8_t mfr_id)
{
    switch (mfr_id) {
        case CHIP_MFR_WINBOND:
            return "Winbond";
        case CHIP_MFR_MACRONIX:
            return "Macronix";
        case CHIP_MFR_GIGADEVICE:
            return "GigaDevice";
        case CHIP_MFR_EON:
            return "Eon";
        case CHIP_MFR_ZETTA:
            return "Zetta";
        case CHIP_MFR_SPANSION:
            return "Spansion";
        case CHIP_MFR_ATMEL:
            return "Atmel";
        case CHIP_MFR_MICRON:
            return "Micron";
        default:
            return "Unknown";
    }
}

/**
 * Parse UID information and extract chip details
 */
int snor_parse_uid(const unsigned char *uid, int uid_len, spi_uid_info_t *info)
{
    if (uid == NULL || uid_len <= 0 || uid_len > MAX_UID_LENGTH || info == NULL) {
        return -1;
    }

    /* Initialize structure */
    memset(info, 0, sizeof(spi_uid_info_t));
    memcpy(info->raw_data, uid, uid_len);
    info->length = uid_len;

    /* Calculate manufacturer ID (usually first byte of device ID in chip_prob) */
    info->manufacturer = CHIP_MFR_UNKNOWN;

    /* Extract first 4 bytes as 32-bit value */
    if (uid_len >= 4) {
        info->uid_32bit = ((uint32_t)uid[0] << 24) | ((uint32_t)uid[1] << 16) | 
                         ((uint32_t)uid[2] << 8) | (uint32_t)uid[3];
    }

    /* Count valid (non-zero, non-0xFF) bytes */
    int i;
    for (i = 0; i < uid_len; i++) {
        if (uid[i] != 0x00 && uid[i] != 0xFF) {
            info->valid_bits++;
        }
    }

    return 0;
}

/**
 * Display UID in formatted output
 */
void snor_display_uid(const unsigned char *uid, int uid_len, int verbose)
{
    if (uid == NULL || uid_len <= 0) {
        printf("Error: Invalid UID data\n");
        return;
    }

    printf("\n========== SPI NOR Flash Unique ID ==========\n\n");

    /* Display header information */
    printf("UID Length: %d bytes\n", uid_len);
    printf("Manufacturer: %s (0x%02X)\n\n", 
           snor_get_manufacturer_name(0x00), 0x00);

    /* Display UID in hexadecimal format */
    printf("UID Data (Hex):\n");
    printf("  ");
    int i;
    for (i = 0; i < uid_len; i++) {
        printf("%02X ", uid[i]);
        if ((i + 1) % 16 == 0 && i + 1 < uid_len) {
            printf("\n  ");
        }
    }
    printf("\n\n");

    if (verbose) {
        /* Parse and display additional information */
        spi_uid_info_t uid_info;
        if (snor_parse_uid(uid, uid_len, &uid_info) == 0) {
            printf("Analysis:\n");
            
            if (uid_len >= 4) {
                printf("  First 4 bytes as U32: 0x%08X\n", uid_info.uid_32bit);
            }
            
            printf("  Valid bits: %d/%d bytes\n", uid_info.valid_bits, uid_len);
            
            /* Display in different formats for analysis */
            printf("\nAlternative Formats:\n");
            
            /* As pairs */
            printf("  16-bit pairs: ");
            for (i = 0; i < uid_len; i += 2) {
                if (i + 1 < uid_len) {
                    uint16_t val = ((uint16_t)uid[i] << 8) | uid[i + 1];
                    printf("0x%04X ", val);
                }
            }
            printf("\n");
            
            /* As 32-bit values */
            printf("  32-bit values: ");
            for (i = 0; i + 3 < uid_len; i += 4) {
                uint32_t val = ((uint32_t)uid[i] << 24) | ((uint32_t)uid[i+1] << 16) |
                              ((uint32_t)uid[i+2] << 8) | uid[i+3];
                printf("0x%08X ", val);
            }
            printf("\n");
        }
    }

    printf("\n===========================================\n\n");
    printf("Tip: Use this UID for chip identification and tracking purposes.\n");
}

/**
 * Read SPI NOR Flash Unique ID (stub implementation)
 * The actual implementation is in spi_nor_flash.c
 */
int snor_read_uid(unsigned char *uid, int uid_len)
{
    /* This function is implemented in spi_nor_flash.c */
    /* This is just a forward declaration for the header */
    return -1;
}
