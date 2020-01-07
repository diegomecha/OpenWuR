/*
 * Copyright (c) 2015, Weptech elektronik GmbH Germany
 * http://www.weptech.de
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 */

#include "cc1200-rf-cfg.h"
#include "cc1200-const.h"

/*
 * This is a setup for the following configuration:
 *
 * cc1200 at 2.73 kbps, ook, 200000 Hz Channel Spacing (915 MHz).
 */

/* Base frequency in kHz */
#define RF_CFG_CHAN_CENTER_F0           915000
/* Channel spacing in Hz */
#define RF_CFG_CHAN_SPACING             400000
/* The minimum channel */
#define RF_CFG_MIN_CHANNEL              0
/* The maximum channel */
#define RF_CFG_MAX_CHANNEL              33
/* The maximum output power in dBm */
#define RF_CFG_MAX_TXPOWER              CC1200_CONST_TX_POWER_MAX
/* The carrier sense level used for CCA in dBm */
#define RF_CFG_CCA_THRESHOLD            (-91)
/* The RSSI offset in dBm */
#define RF_CFG_RSSI_OFFSET              (-81)
/*---------------------------------------------------------------------------*/
static const char rf_cfg_descriptor[] = "915MHz ook 2.73 kbps";
/*---------------------------------------------------------------------------*/
/*
 * Register settings exported from SmartRF Studio using the standard template
 * "trxEB RF Settings Performance Line".
 */

// Modulation format = ook
// Whitening = false
// Symbol rate = 2.73
// Deviation = 3.986359 no hay en ook
// Carrier frequency = 915000000
// Manchester enable = false
// Bit rate = 2.73
// RX filter BW = 104.17 KHz

/*static const registerSetting_t preferredSettings[]=
{
  {CC1200_IOCFG2,            0x06},
  {CC1200_SYNC3,             0x6F},
  {CC1200_SYNC2,             0x4E},
  {CC1200_SYNC1,             0x90},
  {CC1200_SYNC0,             0x4E},
  {CC1200_SYNC_CFG1,         0xEB},
  {CC1200_SYNC_CFG0,         0x23},
  {CC1200_DEVIATION_M,       0x47},
  {CC1200_MODCFG_DEV_E,      0x1B},
  {CC1200_DCFILT_CFG,        0x56},
  {CC1200_PREAMBLE_CFG0,     0xBA},
  {CC1200_IQIC,              0xC8},
  {CC1200_CHAN_BW,           0x84},
  {CC1200_MDMCFG1,           0x42},
  {CC1200_MDMCFG0,           0x05},
  {CC1200_SYMBOL_RATE2,      0x88},
  {CC1200_SYMBOL_RATE1,      0x93},
  {CC1200_SYMBOL_RATE0,      0x75},
  //{CC1200_SYMBOL_RATE2,      0x51},
  //{CC1200_SYMBOL_RATE1,      0xE4},
  //{CC1200_SYMBOL_RATE0,      0x2E},
  {CC1200_AGC_REF,           0x27},
  {CC1200_AGC_CS_THR,        0x01},
  {CC1200_AGC_CFG1,          0x11},
  {CC1200_AGC_CFG0,          0x90},
  {CC1200_FIFO_CFG,          0x00},
  {CC1200_FS_CFG,            0x12},
  {CC1200_PKT_CFG2,          0x24},
  {CC1200_PKT_CFG0,          0x20},
  {CC1200_ASK_CFG,           0x3F},
  {CC1200_PKT_LEN,           0xFF},
  {CC1200_IF_MIX_CFG,        0x18},
  {CC1200_TOC_CFG,           0x03},
  {CC1200_MDMCFG2,           0x02},
  {CC1200_FREQ2,             0x5B},
  {CC1200_FREQ1,             0x80},
  {CC1200_IF_ADC1,           0xEE},
  {CC1200_IF_ADC0,           0x10},
  {CC1200_FS_DIG1,           0x04},
  {CC1200_FS_DIG0,           0x55},
  {CC1200_FS_CAL1,           0x40},
  {CC1200_FS_CAL0,           0x0E},
  {CC1200_FS_DIVTWO,         0x03},
  {CC1200_FS_DSM0,           0x33},
  {CC1200_FS_DVC0,           0x17},
  {CC1200_FS_PFD,            0x00},
  {CC1200_FS_PRE,            0x6E},
  {CC1200_FS_REG_DIV_CML,    0x1C},
  {CC1200_FS_SPARE,          0xAC},
  {CC1200_FS_VCO0,           0xB5},
  {CC1200_IFAMP,             0x05},
  {CC1200_XOSC5,             0x0E},
  {CC1200_XOSC1,             0x03},
};*/

static const registerSetting_t preferredSettings[]=
{
  {CC1200_IOCFG2,            0x06},
  {CC1200_SYNC_CFG1,         0xAB},
  {CC1200_MODCFG_DEV_E,      0x1B},
  //{CC1200_MODCFG_DEV_E,      0x1A},
  {CC1200_PREAMBLE_CFG0,     0x8A},
  {CC1200_IQIC,              0xC8},
  {CC1200_CHAN_BW,           0x10},
  {CC1200_MDMCFG1,           0x42},
  {CC1200_MDMCFG0,           0x05},
  {CC1200_SYMBOL_RATE2,      0x8D},
  {CC1200_SYMBOL_RATE1,      0x7D},
  {CC1200_SYMBOL_RATE0,      0xBF},
  {CC1200_AGC_REF,           0x27},
  {CC1200_AGC_CS_THR,        0x01},
  {CC1200_AGC_CFG1,          0x11},
  {CC1200_AGC_CFG0,          0x94},
  {CC1200_FIFO_CFG,          0x00},
  {CC1200_FS_CFG,            0x12},
  {CC1200_PKT_CFG2,          0x00},
  {CC1200_PKT_CFG0,          0x20},
  {CC1200_PA_CFG0,           0x54},
  {CC1200_ASK_CFG,           0x3F},
  //{CC1200_ASK_CFG,           0xBF},
  {CC1200_PKT_LEN,           0xFF},
  {CC1200_IF_MIX_CFG,        0x1C},
  {CC1200_TOC_CFG,           0x03},
  {CC1200_MDMCFG2,           0x02},
  {CC1200_FREQ2,             0x5B},
  {CC1200_FREQ1,             0x80},
  {CC1200_IF_ADC1,           0xEE},
  {CC1200_IF_ADC0,           0x10},
  {CC1200_FS_DIG1,           0x04},
  {CC1200_FS_DIG0,           0x55},
  {CC1200_FS_CAL1,           0x40},
  {CC1200_FS_CAL0,           0x0E},
  {CC1200_FS_DIVTWO,         0x03},
  {CC1200_FS_DSM0,           0x33},
  {CC1200_FS_DVC0,           0x17},
  {CC1200_FS_PFD,            0x00},
  {CC1200_FS_PRE,            0x6E},
  {CC1200_FS_REG_DIV_CML,    0x1C},
  {CC1200_FS_SPARE,          0xAC},
  {CC1200_FS_VCO0,           0xB5},
  {CC1200_IFAMP,             0x09},
  {CC1200_XOSC5,             0x0E},
  {CC1200_XOSC1,             0x03},
};

/*---------------------------------------------------------------------------*/
/* Global linkage: symbol name must be different in each exported file! */
const cc1200_rf_cfg_t cc1200_915_ook_2_73kbps = {
  .cfg_descriptor = rf_cfg_descriptor,
  .register_settings = preferredSettings,
  .size_of_register_settings = sizeof(preferredSettings),
  .tx_pkt_lifetime = (RTIMER_SECOND),
  .tx_rx_turnaround = (RTIMER_SECOND),
  .chan_center_freq0 = RF_CFG_CHAN_CENTER_F0,
  .chan_spacing = RF_CFG_CHAN_SPACING,
  .min_channel = RF_CFG_MIN_CHANNEL,
  .max_channel = RF_CFG_MAX_CHANNEL,
  .max_txpower = RF_CFG_MAX_TXPOWER,
  .cca_threshold = RF_CFG_CCA_THRESHOLD,
  .rssi_offset = RF_CFG_RSSI_OFFSET,
};
/*---------------------------------------------------------------------------*/
