/** @file

  Copyright (c) 2020, Ampere Computing LLC. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef PCIE_PHY_LIB_H_
#define PCIE_PHY_LIB_H_

#define PHY_TX_PARAM_SIZE           2
#define PHY_RX_PARAM_SIZE           2

#define BUG_67112                   1
#define UPDATE_SRAM                 0

#define MULTWRITE_ENABLE            0
#define MULTI_WR_EN                 0
#define PHY_N_ADDR_OFFSET           0x40000
#define SNPS_PHY0_BASE_ADDR         0x100000
#define SNPS_PHY1_BASE_ADDR         (SNPS_PHY0_BASE_ADDR + PHY_N_ADDR_OFFSET)
#define SNPS_PHY2_BASE_ADDR         (SNPS_PHY1_BASE_ADDR + PHY_N_ADDR_OFFSET)
#define SNPS_PHY3_BASE_ADDR         (SNPS_PHY2_BASE_ADDR + PHY_N_ADDR_OFFSET)

#define BROADCAST_PMA(Src)          ((Src | (0x5 << 12)) * 4)
#define BROADCAST_RAW_PCS(Src)      ((Src | (0x6 << 12)) * 4)
#define BROADCAST_RAW_PCS_AON(Src)  ((Src | (0x7 << 12)) * 4)

#define PHY_CALIB_TO_VALUE          1000
#define SRAM_INIT_TO_VALUE          1000
#define MRDY_DELAY                  10

#define SRAM_BYPASS_0               0
#define SRAM_BYPASS_1               1
#define SRAM_BYPASS_2               2
#define SRAM_BYPASS_3               3

#define SRAM_BOOTLOAD_BYPASS_0      0x1
#define SRAM_BOOTLOAD_BYPASS_1      0x2
#define SRAM_BOOTLOAD_BYPASS_2      0x4
#define SRAM_BOOTLOAD_BYPASS_3      0x8

#define SRIS_MODE_EN                0
#define CLK_REF_SEL                 0
#define CLK_SSC_SEL                 1

#define CRP_SEL                     1
#define MAX_PHY                     3

#define LANE_PER_PHY_NUM_MAX        4

#define HOST_SECURE_ACCESS(Addr)    (UINT64)(Addr | 0x40000000000000)
#define STARTING_SRAM_ADDRESS       0x130000

/*
 * PCIe PHY error code
 */
typedef enum {
  PHY_SRAM_UPDATE_FAIL = -1,
  PHY_INIT_PASS = 0,
  PHY_ROM_ECC_FAIL,
  PHY_SRAM_ECC_FAIL,
  PHY_CALIB_FAIL,
  PHY_CALIB_TIMEOUT,
  PHY_PLL_FAIL
} PHY_STATUS;

typedef enum {
  PHY_DBG_ERROR = 0x0001,
  PHY_DBG_INFO = 0x0002,
  PHY_DBG_WARN = 0x0004,
  PHY_DBG_VERBOSE = 0x0008,
  GEN1 = 0,
  GEN2 = 1,
  GEN3 = 2,
  GEN4 = 3,
  CCIX = 4
} PHY_DBG_FLAGS;

typedef struct {
  UINT8  IsCalBySram;
  UINT32 PllSettings;
  UINT64 TuneTxParam[PHY_RX_PARAM_SIZE];
  UINT64 TuneRxParam[PHY_TX_PARAM_SIZE];
} PHY_SETTING;

/**
 * struct serdes_plat_resource - Serdes Platform Operations
 * @Puts: Prints string to serial console
 * @PutInt: Prints 32-bit unsigned integer to serial console
 * @PutHex: Prints 32-bit unsigned hex to serial console
 * @PutHex64: Prints 64-bit unsigned hex to serial console
 * @DebugPrint: Prints formated string to serial console
 * @MmioRd: Reads 32-bit unsigned integer
 * @MmioWr: Writes 32-bit unsigned integer
 */
typedef struct {
  VOID  (*Puts)(CONST CHAR8 *Msg);
  VOID  (*PutInt)(UINT32 Val);
  VOID  (*PutHex)(UINT32 Val);
  VOID  (*PutHex64)(UINT64 Val);
  INT32 (*DebugPrint)(CONST CHAR8 *Fmt, ...);
  VOID  (*MmioRd)(UINT64 Addr, UINT32 *Val);
  VOID  (*MmioWr)(UINT64 Addr, UINT32 Val);
  VOID  (*UsDelay)(UINT32 Val);
} PHY_PLAT_RESOURCE;

typedef struct {
  UINT64            SdsAddr;          /* PHY base address */
  UINT64            PcieCtrlInfo;     /* PCIe controller related information
                                       * BIT0-1: SoC revision
                                       *      0: Ampere Altra, 1: Ampere Altra Max, 2: Siryn
                                       * BIT2  : SocketID (0: Socket0, 1: Socket1)
                                       * BIT3  : Reserved
                                       * BIT4-6: Root Complex context (RCA0/1/2/3 or RCB0/4/5/6)
                                       * BIT7  : Reserved
                                       * BIT8-9: PHY Numbers within RCA/RCB [0 to 3 each controls 4 lane]
                                       *      0: x16, 1: x8 , 2:x4, 3: 0x2
                                       * BIT10-11 : Gen
                                       *      0: Gen1, 1: Gen2, 2: Gen3, 3: Gen4 + ESM
                                       * BIT13-15 : Setting configuration selection
                                       */
  PHY_SETTING       PhySetting;       /* PHY input setting */
  PHY_PLAT_RESOURCE *PhyPlatResource; /* Debug & misc function pointers */
  PHY_DBG_FLAGS     Debug;
} PHY_CONTEXT;

/*
 * Input:
 *  Ctx    - Serdes context pointer
 *
 * Return:
 *  PHY_STATUS - Return status
 */
PHY_STATUS
SerdesSramUpdate (
  PHY_CONTEXT *Ctx
  );

/*
 * Input:
 *  Ctx    - Serdes context pointer
 *
 * Return:
 *  PHY_STATUS - Return status
 */
PHY_STATUS
SerdesInitClkrst (
  PHY_CONTEXT *Ctx
  );

/*
 * Input:
 *  Ctx    - Serdes context pointer
 *
 * Return:
 *  PHY_STATUS - Return status
 */
PHY_STATUS
SerdesInitCalib (
  PHY_CONTEXT *Ctx
  );

#endif
