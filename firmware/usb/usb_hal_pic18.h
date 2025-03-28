// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright 2015 Microchip Technology Inc. (www.microchip.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

To request to license the code under the MLA license (www.microchip.com/mla_license),
please contact mla_licensing@microchip.com
*******************************************************************************/
//DOM-IGNORE-END

#ifndef _USB_HAL_PIC18_H
#define _USB_HAL_PIC18_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/*  This section lists the other files that are included in this file.
*/
#if defined(__XC8)
    #include <xc.h>
    #include <stdint.h>
#elif defined(__C18)
    #include <p18cxxx.h>
    #ifndef uint8_t
        #define uint8_t unsigned char
    #endif
    #ifndef uint16_t
        #define uint16_t unsigned int
    #endif
    #ifndef uint32_t
        #define uint32_t unsigned long int
    #endif
#endif

#include <string.h>

#include "usb_conf.h"

#ifdef __cplusplus  // Provide C++ Compatability
    extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Constants
// *****************************************************************************
// *****************************************************************************
#define USB_HAL_VBUSTristate()                  //No dedicated VBUS pin on these devices.

//----- USBEnableEndpoint() input definitions ----------------------------------
#define USB_HANDSHAKE_ENABLED   0x10
#define USB_HANDSHAKE_DISABLED  0x00

#define USB_OUT_ENABLED         0x04
#define USB_OUT_DISABLED        0x00

#define USB_IN_ENABLED          0x02
#define USB_IN_DISABLED         0x00

#define USB_ALLOW_SETUP         0x00
#define USB_DISALLOW_SETUP      0x08

#define USB_STALL_ENDPOINT      0x01

//----- usb_config.h input definitions -----------------------------------------
#define USB_PULLUP_ENABLE 0x10
#define USB_PULLUP_DISABLED 0x00

#define USB_INTERNAL_TRANSCEIVER 0x00
#define USB_EXTERNAL_TRANSCEIVER 0x08

#define USB_FULL_SPEED 0x04
#define USB_LOW_SPEED  0x00

//----- Interrupt Flag definitions --------------------------------------------
#define USBTransactionCompleteIE UIEbits.TRNIE
#define USBTransactionCompleteIF UIRbits.TRNIF
#define USBTransactionCompleteIFReg UIR
#define USBTransactionCompleteIFBitNum 0xF7		//AND mask for clearing TRNIF bit position 4

#define USBResetIE  UIEbits.URSTIE
#define USBResetIF  UIRbits.URSTIF
#define USBResetIFReg UIR
#define USBResetIFBitNum 0xFE					//AND mask for clearing URSTIF bit position 0

#define USBIdleIE UIEbits.IDLEIE
#define USBIdleIF UIRbits.IDLEIF
#define USBIdleIFReg UIR
#define USBIdleIFBitNum 0xEF					//AND mask for clearing IDLEIF bit position 5

#define USBActivityIE UIEbits.ACTVIE
#define USBActivityIF UIRbits.ACTVIF
#define USBActivityIFReg UIR
#define USBActivityIFBitNum 0xFB				//AND mask for clearing ACTVIF bit position 2

#define USBSOFIE UIEbits.SOFIE
#define USBSOFIF UIRbits.SOFIF
#define USBSOFIFReg UIR
#define USBSOFIFBitNum 0xBF						//AND mask for clearing SOFIF bit position 6

#define USBStallIE UIEbits.STALLIE
#define USBStallIF UIRbits.STALLIF
#define USBStallIFReg UIR
#define USBStallIFBitNum 0xDF					//AND mask for clearing STALLIF bit position 5

#define USBErrorIE UIEbits.UERRIE
#define USBErrorIF UIRbits.UERRIF
#define USBErrorIFReg UIR
#define USBErrorIFBitNum 0xFD					//UERRIF bit position 1.  Note: This bit is read only and is cleared by clearing the enabled UEIR flags

//----- Event call back defintions --------------------------------------------
#if defined(USB_DISABLE_SOF_HANDLER)
    #define USB_SOF_INTERRUPT 0x00
#else
    #define USB_SOF_INTERRUPT 0x40
#endif

#if defined(USB_DISABLE_ERROR_HANDLER)
    #define USB_ERROR_INTERRUPT 0x02
#else
    #define USB_ERROR_INTERRUPT 0x02
#endif

//----- USB module control bits -----------------------------------------------
#define USBPingPongBufferReset UCONbits.PPBRST
#define USBSE0Event UCONbits.SE0
#define USBSuspendControl UCONbits.SUSPND
#define USBPacketDisable UCONbits.PKTDIS
#define USBResumeControl UCONbits.RESUME

//----- BDnSTAT bit definitions -----------------------------------------------
#define _BSTALL     0x04        //Buffer Stall enable
#define _DTSEN      0x08        //Data Toggle Synch enable
#define _INCDIS     0x10        //Address increment disable
#define _KEN        0x20        //SIE keeps buff descriptors enable
#define _DAT0       0x00        //DATA0 packet expected next
#define _DAT1       0x40        //DATA1 packet expected next
#define _DTSMASK    0x40        //DTS Mask
#define _USIE       0x80        //SIE owns buffer
#define _UCPU       0x00        //CPU owns buffer
#define _STAT_MASK  0xFF

#define USTAT_EP0_PP_MASK   ~0x02
#define USTAT_EP_MASK       0x7E
#define USTAT_EP0_OUT       0x00
#define USTAT_EP0_OUT_EVEN  0x00
#define USTAT_EP0_OUT_ODD   0x02
#define USTAT_EP0_IN        0x04
#define USTAT_EP0_IN_EVEN   0x04
#define USTAT_EP0_IN_ODD    0x06

#define ENDPOINT_MASK 0b01111000

//----- U1EP bit definitions --------------------------------------------------
#define UEP_STALL 0x0001
// Cfg Control pipe for this ep
/* Endpoint configuration options for USBEnableEndpoint() function */
#define EP_CTRL     0x06            // Cfg Control pipe for this ep
#define EP_OUT      0x0C            // Cfg OUT only pipe for this ep
#define EP_IN       0x0A            // Cfg IN only pipe for this ep
#define EP_OUT_IN   0x0E            // Cfg both OUT & IN pipes for this ep

//----- Remap the PIC18 register name space------------------------------------
#define U1ADDR UADDR
#define U1IE UIE
#define U1IR UIR
#define U1EIR UEIR
#define U1EIE UEIE
#define U1CON UCON
#define U1EP0 UEP0
#define U1CONbits UCONbits
#define U1EP1 UEP1
#define U1CNFG1 UCFG
#define U1STAT USTAT
#define U1EP0bits UEP0bits

//----- Defintions for BDT address --------------------------------------------
#if defined(__16F1454) || defined(__16F1455) || defined(__16F1459) || defined(__16LF1454) || defined(__16LF1455) || defined(__16LF1459)
    #define USB_BDT_ADDRESS 0x020
#elif defined(__18F14K50) || defined(__18F13K50) || defined(__18LF14K50) || defined(__18LF13K50)
    #define USB_BDT_ADDRESS 0x200     //See Linker Script, BDT in bank 2 on these devices - usb2:0x200-0x2FF(256-byte)
#elif defined(__18F24K50) || defined(__18F25K50) || defined(__18F45K50) || defined(__18LF24K50) || defined(__18LF25K50) || defined(__18LF45K50)
    #define USB_BDT_ADDRESS 0x400     //See Linker Script, BDT in bank 4
#elif defined(__18F47J53) || defined(__18F46J53) || defined(__18F27J53) || defined(__18F26J53) || defined(__18LF47J53) || defined(__18LF46J53) || defined(__18LF27J53) || defined(__18LF26J53)
    #define USB_BDT_ADDRESS 0xD00		//BDT in Bank 13 on these devices
#elif defined(__18F97J94) || defined(__18F87J94) || defined(__18F67J94) || defined(__18F96J94) || defined(__18F86J94) || defined(__18F66J94) || defined(__18F96J99) || defined(__18F95J94) || defined(__18F86J99) || defined(__18F85J94) || defined(__18F66J99) || defined(__18F65J94)
    #define USB_BDT_ADDRESS 0x100		//BDT in Bank 1 on these devices
#else
    #define USB_BDT_ADDRESS 0x400     //All other PIC18 devices place the BDT in usb4:0x400-0x4FF(256-byte)
#endif

#if (USB_PING_PONG_MODE == USB_PING_PONG__NO_PING_PONG)
    #define BDT_NUM_ENTRIES      ((USB_MAX_EP_NUMBER + 1) * 2)
#elif (USB_PING_PONG_MODE == USB_PING_PONG__EP0_OUT_ONLY)
    #define BDT_NUM_ENTRIES      (((USB_MAX_EP_NUMBER + 1) * 2)+1)
#elif (USB_PING_PONG_MODE == USB_PING_PONG__FULL_PING_PONG)
    #define BDT_NUM_ENTRIES      ((USB_MAX_EP_NUMBER + 1) * 4)
#elif (USB_PING_PONG_MODE == USB_PING_PONG__ALL_BUT_EP0)
    #define BDT_NUM_ENTRIES      (((USB_MAX_EP_NUMBER + 1) * 4)-2)
#else
    #error "No ping pong mode defined."
#endif

#if defined(__XC8)
    #define CTRL_TRF_SETUP_ADDRESS (USB_BDT_ADDRESS+(BDT_NUM_ENTRIES*4))
    #define CTRL_TRF_DATA_ADDRESS (CTRL_TRF_SETUP_ADDRESS + USB_EP0_BUFF_SIZE)

    #if(__XC8_VERSION < 2000)
        #define BDT_BASE_ADDR_TAG   @USB_BDT_ADDRESS
        #define CTRL_TRF_SETUP_ADDR_TAG @CTRL_TRF_SETUP_ADDRESS
        #define CTRL_TRF_DATA_ADDR_TAG @CTRL_TRF_DATA_ADDRESS
    #else
        #define BDT_BASE_ADDR_TAG   __at(USB_BDT_ADDRESS)
        #define CTRL_TRF_SETUP_ADDR_TAG __at(CTRL_TRF_SETUP_ADDRESS)
        #define CTRL_TRF_DATA_ADDR_TAG __at(CTRL_TRF_DATA_ADDRESS)
    #endif

    #if defined(USB_USE_MSD)
        //MSD application specific USB endpoint buffer placement macros (so they
        //get linked to a USB module accessible portion of RAM)
        #define MSD_CBW_ADDRESS (CTRL_TRF_DATA_ADDRESS + USB_EP0_BUFF_SIZE)
        #define MSD_CSW_ADDRESS (MSD_CBW_ADDRESS + MSD_OUT_EP_SIZE)
        #if(__XC8_VERSION < 2000)
            #define MSD_CBW_ADDR_TAG    @MSD_CBW_ADDRESS
            #define MSD_CSW_ADDR_TAG    @MSD_CSW_ADDRESS
        #else
            #define MSD_CBW_ADDR_TAG    __at(MSD_CBW_ADDRESS)
            #define MSD_CSW_ADDR_TAG    __at(MSD_CSW_ADDRESS)
        #endif
    #endif
#else
    #define BDT_BASE_ADDR_TAG   __attribute__ ((aligned (512)))
    #define CTRL_TRF_SETUP_ADDR_TAG
    #define CTRL_TRF_DATA_ADDR_TAG
    #define MSD_CBW_ADDR_TAG
    #define MSD_CSW_ADDR_TAG
#endif

//----- Deprecated definitions - will be removed at some point of time----------
//--------- Deprecated in v2.2
#define _LS         0x00            // Use Low-Speed USB Mode
#define _FS         0x04            // Use Full-Speed USB Mode
#define _TRINT      0x00            // Use internal transceiver
#define _TREXT      0x08            // Use external transceiver
#define _PUEN       0x10            // Use internal pull-up resistor
#define _OEMON      0x40            // Use SIE output indicator

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

// Buffer Descriptor Status Register layout.
typedef union _BD_STAT
{
    uint8_t Val;
    struct{
        //If the CPU owns the buffer then these are the values
        unsigned BC8:1;         //bit 8 of the byte count
        unsigned BC9:1;         //bit 9 of the byte count
        unsigned BSTALL:1;      //Buffer Stall Enable
        unsigned DTSEN:1;       //Data Toggle Synch Enable
        unsigned INCDIS:1;      //Address Increment Disable
        unsigned KEN:1;         //BD Keep Enable
        unsigned DTS:1;         //Data Toggle Synch Value
        unsigned UOWN:1;        //USB Ownership
    };
    struct{
        //if the USB module owns the buffer then these are
        // the values
        unsigned :2;
        unsigned PID0:1;        //Packet Identifier
        unsigned PID1:1;
        unsigned PID2:1;
        unsigned PID3:1;
        unsigned :1;
    };
    struct{
        unsigned :2;
        unsigned PID:4;         //Packet Identifier
        unsigned :2;
    };
} BD_STAT;                      //Buffer Descriptor Status Register

// BDT Entry Layout
typedef union __BDT
{
    struct
    {
        BD_STAT STAT;
        uint8_t CNT;
        uint8_t ADRL;                      //Buffer Address Low
        uint8_t ADRH;                      //Buffer Address High
    };
    struct
    {
        unsigned :8;
        unsigned :8;
        uint16_t     ADR;                      //Buffer Address
    };
    uint32_t Val;
    uint8_t v[4];
} BDT_ENTRY;

// USTAT Register Layout
typedef union __USTAT
{
    struct
    {
        unsigned char filler1:1;
        unsigned char ping_pong:1;
        unsigned char direction:1;
        unsigned char endpoint_number:4;
    };
    uint8_t Val;
} USTAT_FIELDS;

//Macros for fetching parameters from a USTAT_FIELDS variable.
#define USBHALGetLastEndpoint(stat)     stat.endpoint_number
#define USBHALGetLastDirection(stat)    stat.direction
#define USBHALGetLastPingPong(stat)     stat.ping_pong


typedef union _POINTER
{
    struct
    {
        uint8_t bLow;
        uint8_t bHigh;
        //byte bUpper;
    };
    uint16_t _word;                         // bLow & bHigh

    //pFunc _pFunc;                       // Usage: ptr.pFunc(); Init: ptr.pFunc = &<Function>;

    uint8_t* bRam;                         // Ram byte pointer: 2 bytes pointer pointing
                                        // to 1 byte of data
    uint16_t* wRam;                         // Ram word poitner: 2 bytes poitner pointing
                                        // to 2 bytes of data

    const uint8_t* bRom;                     // Size depends on compiler setting
    const uint16_t* wRom;
    //rom near byte* nbRom;               // Near = 2 bytes pointer
    //rom near word* nwRom;
    //rom far byte* fbRom;                // Far = 3 bytes pointer
    //rom far word* fwRom;
} POINTER;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************

#define ConvertToPhysicalAddress(a) ((uint16_t)(a))
#define ConvertToVirtualAddress(a)  ((void *)(a))


//------------------------------------------------------------------------------
//This section is for the PIC18F45K50 Family microcontrollers
//------------------------------------------------------------------------------
#if defined(__18F45K50) || defined(__18F25K50) || defined(__18F24K50) || defined(__18LF45K50) || defined(__18LF25K50) || defined(__18LF24K50)
    #define USBClearUSBInterrupt() PIR3bits.USBIF = 0;
    #if defined(USB_INTERRUPT)
        #define USBMaskInterrupts() {PIE3bits.USBIE = 0;}
        #define USBUnmaskInterrupts() {PIE3bits.USBIE = 1;}
    #else
        #define USBMaskInterrupts()
        #define USBUnmaskInterrupts()
    #endif

    #define USBInterruptFlag PIR3bits.USBIF

    //STALLIE, IDLEIE, TRNIE, and URSTIE are all enabled by default and are required
    #if defined(USB_INTERRUPT)
        #define USBEnableInterrupts() {RCONbits.IPEN = 1;IPR3bits.USBIP = 1;PIE3bits.USBIE = 1;INTCONbits.GIEH = 1;}
    #else
        #define USBEnableInterrupts()
    #endif

    #define USBDisableInterrupts() {PIE3bits.USBIE = 0;}

    #define SetConfigurationOptions()   {\
                                            U1CNFG1 = USB_PULLUP_OPTION | USB_TRANSCEIVER_OPTION | USB_SPEED_OPTION | USB_PING_PONG_MODE;\
                                            U1EIE = 0x9F;\
                                            UIE = 0x39 | USB_SOF_INTERRUPT | USB_ERROR_INTERRUPT;\
                                        }

//------------------------------------------------------------------------------
//This section is for the PIC16F145x Family Microcontrollers
//------------------------------------------------------------------------------
#elif defined(__16F1454) || defined(__16F1455) || defined(__16F1459) || defined(__16LF1454) || defined(__16LF1455) || defined(__16LF1459)
    #define USBClearUSBInterrupt() PIR2bits.USBIF = 0;
    #if defined(USB_INTERRUPT)
        #define USBMaskInterrupts() {PIE2bits.USBIE = 0;}
        #define USBUnmaskInterrupts() {PIE2bits.USBIE = 1;}
    #else
        #define USBMaskInterrupts()
        #define USBUnmaskInterrupts()
    #endif

    #define USBInterruptFlag PIR2bits.USBIF

    //STALLIE, IDLEIE, TRNIE, and URSTIE are all enabled by default and are required
    #if defined(USB_INTERRUPT)
        #define USBEnableInterrupts() {PIE2bits.USBIE = 1;INTCONbits.PEIE = 1;INTCONbits.GIE = 1;}
    #else
        #define USBEnableInterrupts()
    #endif

    #define USBDisableInterrupts() {PIE2bits.USBIE = 0;}

    #define SetConfigurationOptions()   {\
                                            U1CNFG1 = USB_PULLUP_OPTION | USB_TRANSCEIVER_OPTION | USB_SPEED_OPTION | USB_PING_PONG_MODE;\
                                            U1EIE = 0x9F;\
                                            UIE = 0x39 | USB_SOF_INTERRUPT | USB_ERROR_INTERRUPT;\
                                        }
#else
//------------------------------------------------------------------------------
//This section is for all other PIC18 USB microcontrollers
//------------------------------------------------------------------------------
    #define USBClearUSBInterrupt() {PIR2bits.USBIF = 0;}
    #if defined(USB_INTERRUPT)
        #define USBMaskInterrupts() {PIE2bits.USBIE = 0;}
        #define USBUnmaskInterrupts() {PIE2bits.USBIE = 1;}
    #else
        #define USBMaskInterrupts()
        #define USBUnmaskInterrupts()
    #endif

    #define USBInterruptFlag PIR2bits.USBIF

    //STALLIE, IDLEIE, TRNIE, and URSTIE are all enabled by default and are required
    #if defined(USB_INTERRUPT)
        #define USBEnableInterrupts() {RCONbits.IPEN = 1;IPR2bits.USBIP = 1;PIE2bits.USBIE = 1;INTCONbits.GIEH = 1;}
    #else
        #define USBEnableInterrupts()
    #endif

    #define USBDisableInterrupts() {PIE2bits.USBIE = 0;}

    #define SetConfigurationOptions()   {\
                                            U1CNFG1 = USB_PULLUP_OPTION | USB_TRANSCEIVER_OPTION | USB_SPEED_OPTION | USB_PING_PONG_MODE;\
                                            U1EIE = 0x9F;\
                                            UIE = 0x39 | USB_SOF_INTERRUPT | USB_ERROR_INTERRUPT;\
                                        }
#endif //end of #if defined(__18F45K50) || defined(__18F25K50)...
//------------------------------------------------------------------------------


/****************************************************************
    Function:
        void USBPowerModule(void)

    Description:
        This macro is used to power up the USB module if required<br>
        PIC18: defines as nothing<br>
        PIC24: defines as U1PWRCbits.USBPWR = 1;<br>

    Parameters:
        None

    Return Values:
        None

    Remarks:
        None

  ****************************************************************/
#define USBPowerModule()

/****************************************************************
    Function:
        void USBModuleDisable(void)

    Description:
        This macro is used to disable the USB module

    Parameters:
        None

    Return Values:
        None

    Remarks:
        None

  ****************************************************************/
#define USBModuleDisable() {\
    UCON = 0;\
    UIE = 0;\
    USBDeviceState = DETACHED_STATE;\
}

/****************************************************************
    Function:
        USBSetBDTAddress(addr)

    Description:
        This macro is used to power up the USB module if required

    Parameters:
        None

    Return Values:
        None

    Remarks:
        None

  ****************************************************************/
#define USBSetBDTAddress(addr)

/********************************************************************
 * Function (macro): void USBClearInterruptFlag(register, uint8_t if_and_flag_mask)
 *
 * PreCondition:    None
 *
 * Input:
 *   register - the register mnemonic for the register holding the interrupt
 				flag to be cleared
 *   uint8_t if_and_flag_mask - an AND mask for the interrupt flag that will be
 				cleared
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Clears the specified USB interrupt flag.
 *
 * Note:
 *******************************************************************/
#define USBClearInterruptFlag(reg_name, if_and_flag_mask)	(reg_name &= if_and_flag_mask)

/********************************************************************
    Function:
        void USBClearInterruptRegister(uint16_t reg)

    Summary:
        Clears the specified interrupt register

    PreCondition:
        None

    Parameters:
        uint16_t reg - the register name that needs to be cleared

    Return Values:
        None

    Remarks:
        None

 *******************************************************************/
#define USBClearInterruptRegister(reg) {reg = 0;}

/********************************************************************
    Function:
        void DisableNonZeroEndpoints(UINT8 last_ep_num)

    Summary:
        Clears the control registers for the specified non-zero endpoints

    PreCondition:
        None

    Parameters:
        UINT8 last_ep_num - the last endpoint number to clear.  This
        number should include all endpoints used in any configuration.

    Return Values:
        None

    Remarks:
        None

 *******************************************************************/
#define DisableNonZeroEndpoints(last_ep_num) memset((void*)&U1EP1,0x00,(last_ep_num));

/*****************************************************************************/
/****** Compiler checks ******************************************************/
/*****************************************************************************/

//Definitions for the BDT
#ifndef USB_PING_PONG_MODE
    #error "No ping pong mode defined."
#endif

/*****************************************************************************/
/****** Extern variable definitions ******************************************/
/*****************************************************************************/

#if !defined(USBDEVICE_C)
    extern USB_VOLATILE uint8_t USBActiveConfiguration;
    extern USB_VOLATILE IN_PIPE inPipes[1];
    extern USB_VOLATILE OUT_PIPE outPipes[1];
#endif

extern volatile BDT_ENTRY* pBDTEntryOut[USB_MAX_EP_NUMBER+1];
extern volatile BDT_ENTRY* pBDTEntryIn[USB_MAX_EP_NUMBER+1];

#ifdef __cplusplus  // Provide C++ Compatibility
    }
#endif

#endif //#ifndef _USB_HAL_PIC18_H


