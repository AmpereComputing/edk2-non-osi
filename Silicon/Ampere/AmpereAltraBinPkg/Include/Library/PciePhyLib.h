/** @file

  Copyright (c) 2020 - 2021, Ampere Computing LLC. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef PCIE_PHY_LIB_H_
#define PCIE_PHY_LIB_H_

#define PHY_TX_PARAM_SIZE     2
#define PHY_RX_PARAM_SIZE     2

//
// PCIe PHY error code
//
typedef enum {
  PHY_SRAM_UPDATE_FAIL = -1,
  PHY_INIT_PASS = 0,
  PHY_ROM_ECC_FAIL,
  PHY_SRAM_ECC_FAIL,
  PHY_CALIB_FAIL,
  PHY_CALIB_TIMEOUT,
  PHY_PLL_FAIL
} PHY_STATUS;

//
// PCIe PHY debug flags
//
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

//
// PCIe PHY setting structure
//
typedef struct {
  UINT8  IsCalBySram;
  UINT32 PllSettings;
  UINT64 TuneTxParam[PHY_RX_PARAM_SIZE];
  UINT64 TuneRxParam[PHY_TX_PARAM_SIZE];
} PHY_SETTING;

//
// PCIe PHY Platform Operations Structure
//
// @Puts: Prints string to serial console
// @PutInt: Prints 32-bit unsigned integer to serial console
// @PutHex: Prints 32-bit unsigned hex to serial console
// @PutHex64: Prints 64-bit unsigned hex to serial console
// @DebugPrint: Prints formated string to serial console
// @MmioRd: Reads 32-bit unsigned integer
// @MmioWr: Writes 32-bit unsigned integer
//
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

//
// PCIe PHY context structure
//
typedef struct {
  UINT64            SdsAddr;          /* PHY base address */
  UINT64            PcieCtrlInfo;     /* PCIe controller related information
                                       * BIT0-1: SoC revision
                                       *      0: Ampere Altra
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

/**
  Function to initial clock and reset setting of the PCIe PHY.

  @param[in]     Ctx       A pointer to the PHY context structure.

  @retval PHY_INIT_PASS    PHY has been initialized.
  @retval Other            Failed to initialize the PHY.
**/
PHY_STATUS
SerdesInitClkrst (
  IN PHY_CONTEXT *Ctx
  );

#endif
