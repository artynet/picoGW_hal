/*
 / _____)             _              | |    
( (____  _____ ____ _| |_ _____  ____| |__  
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    �2013 Semtech-Cycleo

Description:
    Lora gateway Hardware Abstraction Layer
*/


#ifndef _LORAGW_HAL_H
#define _LORAGW_HAL_H

/* -------------------------------------------------------------------------- */
/* --- DEPENDANCIES --------------------------------------------------------- */

#include <stdint.h>     /* C99 types */
#include <stdbool.h>    /* bool type */

/* -------------------------------------------------------------------------- */
/* --- PUBLIC CONSTANTS ----------------------------------------------------- */

/* return status code */
#define LGW_HAL_SUCCESS     0
#define LGW_HAL_ERROR       -1

/* hardware characteristics */
#define LGW_RF_CHAIN_NB     2
#define LGW_IF_CHAIN_NB     10

#define LGW_PKT_FIFO_SIZE   8
#define LGW_DATABUFF_SIZE   1024
#define LGW_RF_BANDWIDTH    800000

/*
SX1275 frequency setting :
F_register(24bit) = F_rf (Hz) / F_step(Hz)
                  = F_rf (Hz) * 2^19 / F_xtal(Hz)
                  = F_rf (Hz) * 256/15625
*/
#define LGW_XTAL_FREQU      32000000
#define LGW_SW1257_DENUM    15625 /* pll settings denominator when the numerator is 2^8 */

/* to use those parameters, declare a local constant, and use 'rf_chain' as index */
#define LGW_RF_RX_LOWFREQ   {863000000, 863000000}
#define LGW_RF_RX_UPFREQ    {870000000, 870000000}
#define LGW_RF_TX_LOWFREQ   {863000000, 863000000}
#define LGW_RF_TX_UPFREQ    {870000000, 870000000}

/* type of if_chain */
#define UNDEFINED           0
#define DUMMY               0x01
#define LORA_STD            0x10    /* standard single-SF Lora modem */
#define LORA_MULTI          0x11    /* Lora receiver with multi-SF capability */
#define FSK_STD             0x20    /* standard FSK modem */

/* configuration of available IF chains on the hardware */
/* to use, declare a local constant, and use 'if_chain' as index */
#define LGW_IF_CONFIG {\
    LORA_MULTI, \
    LORA_MULTI, \
    LORA_MULTI, \
    LORA_MULTI, \
    DUMMY, \
    DUMMY, \
    DUMMY, \
    DUMMY, \
    LORA_STD, \
    FSK_STD }

/* values available for the 'modulation' parameters */
#define MOD_DEFAULT     0
#define MOD_LORA        0x10
#define MOD_FSK         0x20
#define MOD_GFSK        0x21

/* values available for the 'bandwidth' parameters */
#define BW_DEFAULT      0
#define BW_500KHZ       0x04
#define BW_250KHZ       0x08
#define BW_125KHZ       0x0C
// TODO: add all the supporter FSK bandwidth

/* values available for the 'datarate' parameters */
#define DR_DEFAULT      0
#define DR_LORA_SF7     0x02
#define DR_LORA_SF8     0x04
#define DR_LORA_SF9     0x08
#define DR_LORA_SF10    0x10
#define DR_LORA_SF11    0x20
#define DR_LORA_SF12    0x40
#define DR_LORA_MULTI   0x7E
// TODO: add FSK datarates

/* values available for the 'coderate' parameters */
#define CR_DEFAULT      0
#define CR_LORA_4_5     0x11
#define CR_LORA_4_6     0x12
#define CR_LORA_4_7     0x13
#define CR_LORA_4_8     0x14

/* values available for the 'status' parameter */
#define PKT_CRC_OK      1
#define PKT_CRC_ERROR   0
#define PKT_NO_CRC      -1

/* values available for the 'tx_mode' parameter */
#define IMMEDIATE       0
#define TIMESTAMPED     1
#define ON_GPS          2
#define ON_EVENT        3
#define GPS_DELAYED     4
#define EVENT_DELAYED   5

/* -------------------------------------------------------------------------- */
/* --- PUBLIC TYPES --------------------------------------------------------- */

/**
@struct lgw_conf_rxrf_s
@brief Configuration structure for a RF chain
*/
struct lgw_conf_rxrf_s {
    bool        enable;     /*!> enable or disable that RF chain */
    uint32_t    freq_hz;    /*!> center frequency of the radio in Hz */
};

/**
@struct lgw_conf_rxif_s
@brief Configuration structure for an IF chain
*/
struct lgw_conf_rxif_s {
    bool        enable;     /*!> enable or disable that IF chain */
    uint8_t     rf_chain;   /*!> to which RF chain is that IF chain associated */
    int32_t     freq_hz;    /*!> center frequ of the IF chain, relative to RF chain frequency */
    uint8_t     bandwidth;  /*!> RX bandwidth, 0 for default */
    uint8_t     datarate;   /*!> RX datarate, 0 for default */
};

/**
@struct lgw_pkt_rx_s
@brief Structure containing the metadata of a packet that was received and a pointer to the payload
*/
struct lgw_pkt_rx_s {
    uint8_t     if_chain;   /*!> by which IF chain was packet received */
    int8_t      status;     /*!> status of the received packet */
    uint8_t     modulation; /*!> modulation used by the packet */
    uint8_t     datarate;   /*!> RX datarate of the packet */
    uint8_t     coderate;   /*!> error-correcting code of the packet */
    uint32_t    count_us;   /*!> internal gateway counter for timestamping, 1 microsecond resolution */
    int8_t      rssi;       /*!> average packet RSSI in dB */
    int8_t      snr;        /*!> average packet SNR, in dB multiplied by 4 (LORA only) */
    int8_t      snr_min;    /*!> minimum packet SNR, in dB multiplied by 4 (LORA only) */
    int8_t      snr_max;    /*!> maximum packet SNR, in dB multiplied by 4 (LORA only) */
    uint16_t    crc;        /*!> CRC that was received in the payload */
    uint16_t    size;       /*!> payload size in bytes */
    uint8_t     *payload;   /*!> pointer to the payload */
};

/**
@struct lgw_pkt_tx_s
@brief Structure containing the configuration of a packet to send and a pointer to the payload
*/
struct lgw_pkt_tx_s {
    uint32_t    freq_hz;    /*!> center frequency of TX */
    uint8_t     tx_mode;    /*!> select on what event/time the TX is triggered */
    uint32_t    count_us;   /*!> timestamp or delay in microseconds for TX trigger */
    uint8_t     rf_chain;   /*!> through which RF chain will the packet be sent */
    int8_t      rf_power;   /*!> TX power, in dBm */
    uint8_t     modulation; /*!> modulation to use for the packet */
    uint8_t     bandwidth;  /*!> modulation bandwidth (LORA only) */
    uint16_t    f_dev;      /*!> frequency deviation (FSK only) */
    uint8_t     datarate;   /*!> TX datarate */
    uint8_t     coderate;   /*!> error-correcting code of the packet */
    uint16_t    preamble;   /*!> set the preamble length, 0 for default */
    bool        no_crc;     /*!> if true, do not send a CRC in the packet */
    bool        no_header;  /*!> if true, enable implicit header mode */
    uint16_t    size;       /*!> payload size in bytes */
    uint8_t     *payload;   /*!> pointer to the payload */
};

/* -------------------------------------------------------------------------- */
/* --- PUBLIC FUNCTIONS PROTOTYPES ------------------------------------------ */

/**
@brief Configure an RF chain (must configure before start)
@param rf_chain number of the RF chain to configure [0, LGW_RF_CHAIN_NB - 1]
@param conf structure containing the configuration parameters
@return LGW_HAL_ERROR id the operation failed, LGW_HAL_SUCCESS else
*/
int lgw_rxrf_setconf(uint8_t rf_chain, struct lgw_conf_rxrf_s conf);

/**
@brief Configure an IF chain + modem (must configure before start)
@param if_chain number of the IF chain to configure [0, LGW_IF_CHAIN_NB - 1]
@param conf structure containing the configuration parameters
@return LGW_HAL_ERROR id the operation failed, LGW_HAL_SUCCESS else
*/
int lgw_rxif_setconf(uint8_t if_chain, struct lgw_conf_rxif_s conf);

/**
@brief Connect to the Lora gateway, reset it and configure it according to previously set parameters
@return LGW_HAL_ERROR id the operation failed, LGW_HAL_SUCCESS else
*/
int lgw_start(void);

/**
@brief Stop the Lora gateway and disconnect it
@return LGW_HAL_ERROR id the operation failed, LGW_HAL_SUCCESS else
*/
int lgw_stop(void);

/**
@brief A non-blocking function that will fetch up to 'max_pkt' packets from the Lora gateway FIFO and data buffer
@param max_pkt maximum number of packet that must be retrieved (equal to the size of the array of struct)
@param pkt_data pointer to an array of struct that will receive the packet metadata and payload pointers
@return LGW_HAL_ERROR id the operation failed, else the number of packets retrieved
*/
int lgw_receive(uint8_t max_pkt, struct lgw_pkt_rx_s *pkt_data);

/**
@brief Schedule a packet to be send immediately or after a delay depending on tx_mode
@param pkt_data structure containing the data and metadata for the packet to send
@return LGW_HAL_ERROR id the operation failed, LGW_HAL_SUCCESS else
*/
int lgw_send(struct lgw_pkt_tx_s pkt_data);

#endif

/* --- EOF ------------------------------------------------------------------ */
