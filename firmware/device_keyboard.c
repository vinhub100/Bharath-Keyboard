/*******************************************************************************
Copyright 2016 Microchip Technology Inc. (www.microchip.com)

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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>
#include <string.h>
#include <pic18.h>

#include "system.h"
#include "usb/usb.h"
#include "usb/usb_device_hid.h"

#include "status_led.h"

#if defined(__XC8)
    #define PACKED
#else
    #define PACKED __attribute__((packed))
#endif

// *****************************************************************************
// *****************************************************************************
// Section: File Scope or Global Constants
// *****************************************************************************
// *****************************************************************************

//Class specific descriptor - HID Keyboard
const struct{uint8_t report[HID_RPT01_SIZE];}hid_rpt01={
{   0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
    0x95, 0x05,                    //   REPORT_COUNT (5)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x05, 0x08,                    //   USAGE_PAGE (LEDs)
    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
    0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x03,                    //   REPORT_SIZE (3)
    0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs)
    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0xff,                    //   LOGICAL_MAXIMUM (101)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0xff,                    //   USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0xc0}                          // End Collection
};


// *****************************************************************************
// *****************************************************************************
// Section: File Scope Data Types
// *****************************************************************************
// *****************************************************************************

/* This typedef defines the only INPUT report found in the HID report
 * descriptor and gives an easy way to create the OUTPUT report. */
typedef struct PACKED
{
    /* The union below represents the first byte of the INPUT report.  It is
     * formed by the following HID report items:
     *
     *  0x19, 0xe0, //   USAGE_MINIMUM (Keyboard LeftControl)
     *  0x29, 0xe7, //   USAGE_MAXIMUM (Keyboard Right GUI)
     *  0x15, 0x00, //   LOGICAL_MINIMUM (0)
     *  0x25, 0x01, //   LOGICAL_MAXIMUM (1)
     *  0x75, 0x01, //   REPORT_SIZE (1)
     *  0x95, 0x08, //   REPORT_COUNT (8)
     *  0x81, 0x02, //   INPUT (Data,Var,Abs)
     *
     * The report size is 1 specifying 1 bit per entry.
     * The report count is 8 specifying there are 8 entries.
     * These entries represent the Usage items between Left Control (the usage
     * minimum) and Right GUI (the usage maximum).
     */
    union PACKED
    {
        uint8_t value;
        struct PACKED
        {
            unsigned leftControl    :1;
            unsigned leftShift      :1;
            unsigned leftAlt        :1;
            unsigned leftGUI        :1;
            unsigned rightControl   :1;
            unsigned rightShift     :1;
            unsigned rightAlt       :1;
            unsigned rightGUI       :1;
        } bits;
    } modifiers;

    /* There is one byte of constant data/padding that is specified in the
     * input report:
     *
     *  0x95, 0x01,                    //   REPORT_COUNT (1)
     *  0x75, 0x08,                    //   REPORT_SIZE (8)
     *  0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
     */
    unsigned :8;

    /* The last INPUT item in the INPUT report is an array type.  This array
     * contains an entry for each of the keys that are currently pressed until
     * the array limit, in this case 6 concurent key presses.
     *
     *  0x95, 0x06,                    //   REPORT_COUNT (6)
     *  0x75, 0x08,                    //   REPORT_SIZE (8)
     *  0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
     *  0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
     *  0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
     *  0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
     *  0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
     *
     * Report count is 6 indicating that the array has 6 total entries.
     * Report size is 8 indicating each entry in the array is one byte.
     * The usage minimum indicates the lowest key value (Reserved/no event)
     * The usage maximum indicates the highest key value (Application button)
     * The logical minimum indicates the remapped value for the usage minimum:
     *   No Event has a logical value of 0.
     * The logical maximum indicates the remapped value for the usage maximum:
     *   Application button has a logical value of 101.
     *
     * In this case the logical min/max match the usage min/max so the logical
     * remapping doesn't actually change the values.
     *
     * To send a report with the 'a' key pressed (usage value of 0x04, logical
     * value in this example of 0x04 as well), then the array input would be the
     * following:
     *
     * LSB [0x04][0x00][0x00][0x00][0x00][0x00] MSB
     *
     * If the 'b' button was then pressed with the 'a' button still held down,
     * the report would then look like this:
     *
     * LSB [0x04][0x05][0x00][0x00][0x00][0x00] MSB
     *
     * If the 'a' button was then released with the 'b' button still held down,
     * the resulting array would be the following:
     *
     * LSB [0x05][0x00][0x00][0x00][0x00][0x00] MSB
     *
     * The 'a' key was removed from the array and all other items in the array
     * were shifted down. */
    uint8_t keys[6];
} KEYBOARD_INPUT_REPORT;


//////////////////////////////


typedef struct PACKED
{
    uint8_t data[8];
} E2_REPORT;

typedef struct PACKED
{
    uint8_t data[8];
} E2_OUT_REPORT;


//////////////////////////////
/* This typedef defines the only OUTPUT report found in the HID report
 * descriptor and gives an easy way to parse the OUTPUT report. */
typedef union PACKED
{
    /* The OUTPUT report is comprised of only one byte of data. */
    uint8_t value;
    struct
    {
        /* There are two report items that form the one byte of OUTPUT report
         * data.  The first report item defines 5 LED indicators:
         *
         *  0x95, 0x05,                    //   REPORT_COUNT (5)
         *  0x75, 0x01,                    //   REPORT_SIZE (1)
         *  0x05, 0x08,                    //   USAGE_PAGE (LEDs)
         *  0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
         *  0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
         *  0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
         *
         * The report count indicates there are 5 entries.
         * The report size is 1 indicating each entry is just one bit.
         * These items are located on the LED usage page
         * These items are all of the usages between Num Lock (the usage
         * minimum) and Kana (the usage maximum).
         */
        unsigned numLock        :1;
        unsigned capsLock       :1;
        unsigned scrollLock     :1;
        unsigned compose        :1;
        unsigned kana           :1;

        /* The second OUTPUT report item defines 3 bits of constant data
         * (padding) used to make a complete byte:
         *
         *  0x95, 0x01,                    //   REPORT_COUNT (1)
         *  0x75, 0x03,                    //   REPORT_SIZE (3)
         *  0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs)
         *
         * Report count of 1 indicates that there is one entry
         * Report size of 3 indicates the entry is 3 bits long. */
        unsigned                :3;
    } leds;
} KEYBOARD_OUTPUT_REPORT;


/* This creates a storage type for all of the information required to track the
 * current state of the keyboard. */
typedef struct
{
    USB_HANDLE lastINTransmission;
    USB_HANDLE lastOUTTransmission;
} KEYBOARD;

// *****************************************************************************
// *****************************************************************************
// Section: File Scope or Global Variables
// *****************************************************************************
// *****************************************************************************
static KEYBOARD keyboard;

#if !defined(KEYBOARD_INPUT_REPORT_DATA_BUFFER_ADDRESS_TAG)
    #define KEYBOARD_INPUT_REPORT_DATA_BUFFER_ADDRESS_TAG
#endif
static KEYBOARD_INPUT_REPORT inputReport KEYBOARD_INPUT_REPORT_DATA_BUFFER_ADDRESS_TAG;

#if !defined(E2_REPORT_DATA_BUFFER_ADDRESS_TAG)
    #define E2_REPORT_DATA_BUFFER_ADDRESS_TAG
#endif
static E2_REPORT e2Report E2_REPORT_DATA_BUFFER_ADDRESS_TAG;


#if !defined(E2_OUT_DATA_BUFFER_ADDRESS_TAG)
    #define E2_OUT_DATA_BUFFER_ADDRESS_TAG
#endif
static volatile E2_OUT_REPORT e2outReport E2_OUT_DATA_BUFFER_ADDRESS_TAG;




#if !defined(KEYBOARD_OUTPUT_REPORT_DATA_BUFFER_ADDRESS_TAG)
    #define KEYBOARD_OUTPUT_REPORT_DATA_BUFFER_ADDRESS_TAG
#endif
static volatile KEYBOARD_OUTPUT_REPORT outputReport KEYBOARD_OUTPUT_REPORT_DATA_BUFFER_ADDRESS_TAG;


// *****************************************************************************
// *****************************************************************************
// Section: Private Prototypes
// *****************************************************************************
// *****************************************************************************
static void KeyboardProcessOutputReport(void);


//Exteranl variables declared in other .c files
extern volatile signed int SOFCounter;


//Application variables that need wide scope
KEYBOARD_INPUT_REPORT oldInputReport;
signed int keyboardIdleRate;
signed int LocalSOFCount;
static signed int OldSOFCount;

static USB_HANDLE ep2_handle;
static USB_HANDLE ep2_out_handle;
E2_REPORT old_ep2report;




// *****************************************************************************
// *****************************************************************************
// Section: Macros or Functions
// *****************************************************************************
// *****************************************************************************
void APP_KeyboardInit(void)
{
    //initialize the variable holding the handle for the last
    // transmission
    keyboard.lastINTransmission = 0;
    ep2_handle = 0;
    

    //Set the default idle rate to 500ms (until the host sends a SET_IDLE request to change it to a new value)
    keyboardIdleRate = 500;

    //Copy the (possibly) interrupt context SOFCounter value into a local variable.
    //Using a while() loop to do this since the SOFCounter isn't necessarily atomically
    //updated and therefore we need to read it a minimum of twice to ensure we captured the correct value.
    while(OldSOFCount != SOFCounter)
    {
        OldSOFCount = SOFCounter;
    }

    //enable the HID endpoint
    USBEnableEndpoint(HID_EP, USB_IN_ENABLED|USB_OUT_ENABLED|USB_HANDSHAKE_ENABLED|USB_DISALLOW_SETUP);
    USBEnableEndpoint(2, USB_IN_ENABLED|USB_OUT_ENABLED|USB_HANDSHAKE_ENABLED|USB_DISALLOW_SETUP);

    //Arm OUT endpoint so we can receive caps lock, num lock, etc. info from host
    keyboard.lastOUTTransmission = HIDRxPacket(HID_EP,(uint8_t*)&outputReport, sizeof(outputReport) );
    ep2_out_handle = HIDRxPacket(2,(uint8_t*)&e2outReport, sizeof(e2outReport));
}

#define CK 1
#define MK 2
#define E2 3

#define LANG 1
#define QC 2
#define N1 3
#define N2 4
#define N3 5
#define N4 6
#define N5 7

typedef struct keyStruct {
    uint8_t keyCode;
    uint8_t modifier;
    uint8_t type;
} KeyStruct;


KeyStruct CodeChart[17][8] = {
{{0x3e,0x00,CK},{0x3f,0x00,CK},{0x40,0x00,CK},{0x41,0x00,CK},{0x42,0x00,CK},{0x43,0x00,CK},{0x44,0x00,CK},{0x45,0x00,CK}},
{{0x21,0x00,CK},{0x1e,0x00,CK},{0x1f,0x00,CK},{0x20,0x00,CK},{0x22,0x00,CK},{0x23,0x00,CK},{0x24,0x00,CK},{0x25,0x00,CK}},
{{0x18,0x40,CK},{0x15,0x40,CK},{0x17,0x40,CK},{0x1c,0x40,CK},{0x0c,0x40,CK},{0x14,0x40,CK},{0x1a,0x40,CK},{0x08,0x40,CK}},
{{0x35,0x00,CK},{0x39,0x00,CK},{0x2b,0x00,CK},{0x29,0x00,CK},{0x3a,0x00,CK},{0x3b,0x00,CK},{0x3c,0x00,CK},{0x3d,0x00,CK}},
{{0x26,0x00,CK},{0x27,0x00,CK},{0x2d,0x00,CK},{0x2e,0x00,CK},{N5,0x00,E2},{N4,0x00,E2},{N3,0x00,E2},{LANG,0x00,E2}},
{{0x00,0x00,0x00},{QC,0x00,E2},{0x49,0x00,CK},{0x4a,0x00,CK},{0x4b,0x00,CK},{0x4e,0x00,CK},{0x4d,0x00,CK},{0x4c,0x00,CK}},
{{0x16,0x40,CK},{0x07,0x40,CK},{0x31,0x00,CK},{N2,0x00,E2},{N1,0x00,E2},{0x12,0x40,CK},{0x13,0x40,CK},{0x04,0x40,CK}},
{{0x14,0x00,CK},{0x15,0x00,CK},{0x17,0x00,CK},{0x09,0x40,CK},{0x0a,0x40,CK},{0x0b,0x40,CK},{0x1a,0x00,CK},{0x08,0x00,CK}},
{{0x16,0x00,CK},{0x1b,0x40,CK},{0x0e,0x40,CK},{0x1d,0x40,CK},{0x04,0x00,CK},{0x07,0x00,CK},{0x09,0x00,CK},{0x0f,0x40,CK}},
{{0x2a,0x00,CK},{0x30,0x00,CK},{0x0c,0x00,CK},{0x18,0x00,CK},{0x1c,0x00,CK},{0x12,0x00,CK},{0x2f,0x00,CK},{0x13,0x00,CK}},
{{0x0e,0x00,CK},{0x0b,0x00,CK},{0x34,0x00,CK},{0x33,0x00,CK},{0x0f,0x00,CK},{0x28,0x00,CK},{0x0d,0x00,CK},{0x0a,0x00,CK}},
{{0x19,0x00,CK},{0x1b,0x00,CK},{0x05,0x40,CK},{0x06,0x00,CK},{0x19,0x40,CK},{0x1d,0x00,CK},{0x06,0x40,CK},{0x11,0x40,CK}},
{{0xe6,0x40,MK},{0xe3,0x08,MK},{0xe1,0x02,MK},{0xe2,0x04,MK},{0x0d,0x40,CK},{0xe7,0x80,MK},{0x2c,0x00,CK},{0xe0,0x01,MK}},
{{0x38,0x00,CK},{0x37,0x00,CK},{0x36,0x00,CK},{0x10,0x00,CK},{0x11,0x00,CK},{0x05,0x00,CK},{0x00,0x00,0x00},{0xe5,0x20,MK}},
{{0x5a,0x00,CK},{0x5d,0x00,CK},{0x60,0x00,CK},{0x54,0x00,CK},{0x53,0x00,CK},{0x5f,0x00,CK},{0x5c,0x00,CK},{0x59,0x00,CK}},
{{0x56,0x00,CK},{0x57,0x00,CK},{0x58,0x00,CK},{0x63,0x00,CK},{0x5b,0x00,CK},{0x5e,0x00,CK},{0x61,0x00,CK},{0x55,0x00,CK}},
{{0x52,0x00,CK},{0x50,0x00,CK},{0x51,0x00,CK},{0x4f,0x00,CK},{0xe4,0x10,MK},{0x62,0x00,CK},{0x65,0x00,CK},{0x00,0x00,0x00}}
};


#define _XTAL_FREQ 8000000

KeyStruct SendList[8];
uint8_t SendListCount = 0;

KeyStruct  CharKeyList[8],ModKeyList[8],EP2KeyList[8];
uint8_t CharListCount = 0,ModListCount = 0,EP2ListCount = 0;

//uint8_t PrevSendListCount;

void SortSendList(void){
    
    if(SendListCount == 0){return;}
    
    for(uint8_t i = 0; i < SendListCount; i++){
        switch(SendList[i].type)
        {
            case CK :
                CharKeyList[CharListCount] = SendList[i];
                CharListCount++;
                break;
            case MK :
                ModKeyList[ModListCount] = SendList[i];
                ModListCount++;
                break;
            case E2 :
                if(EP2ListCount < 4){
                    EP2KeyList[EP2ListCount] = SendList[i];
                    EP2ListCount++;
                }
                break;
        }
    }
    
    for(uint8_t j = 0;j < 8;j++){ SendList[j] = (KeyStruct){.keyCode = 0,.modifier = 0,.type = 0};}
    
    SendListCount = 0;
    
}

void AddToSendList(KeyStruct keyCode){
    
    if(SendListCount < 8 ){
        SendList[SendListCount] = keyCode;
        SendListCount++;
    }
}

void ProcessLine(uint8_t lineNo){
    
    uint8_t readBuf, tempBuf;
    uint8_t filt = 0b00000001;
    

    
    readBuf = 0;
    __delay_ms(5);
    
    for(uint8_t x = 0; x<5; x++){
        tempBuf = PORTA;
        readBuf = readBuf | tempBuf;
    }
    
    
    for(uint8_t i = 0; i < 8; i++){
        if((readBuf & filt) != 0){
            AddToSendList(CodeChart[lineNo][i]);
        }
        
        filt = filt << 1;
    }
}


void ProcessKeyboard(void){
    
    uint8_t line = 0;
    uint8_t latWriter = 0b00000001;
    
    for(uint8_t n = 0; n < 8; n++){
        
        LATD = latWriter;
        latWriter = latWriter << 1;
        ProcessLine(line);
        line++;
    }
    
    LATD = 0b00000000;
    latWriter = 0b00000001;
    
    for(uint8_t n = 0; n < 6; n++){
        
        LATB = latWriter;
        latWriter = latWriter << 1;
        ProcessLine(line);
        line++;
        
    }
    
    LATB = 0b00000000;
    latWriter = 0b00000001;
    
    for(uint8_t n = 0; n < 3; n++){
        
        LATC = latWriter;
        latWriter = latWriter << 1;
//        __delay_ms(50);
        ProcessLine(line);
        line++;
        
    }
    
    LATC = 0b00000000;
    
    SortSendList();
    
    // Reset SendList
}


void APP_KeyboardTasks(void)
{
    signed int TimeDeltaMilliseconds;
    unsigned char i;
    bool needToSendNewReportPacket;

    /* If the USB device isn't configured yet, we can't really do anything
     * else since we don't have a host to talk to.  So jump back to the
     * top of the while loop. */
    if( USBGetDeviceState() < CONFIGURED_STATE )
    {
        return;
    }

    /* If we are currently suspended, then we need to see if we need to
     * issue a remote wakeup.  In either case, we shouldn't process any
     * keyboard commands since we aren't currently communicating to the host
     * thus just continue back to the start of the while loop. */
    if( USBIsDeviceSuspended()== true )
    {
        //Check if we should assert a remote wakeup request to the USB host,
        //when the user presses the pushbutton.
//        if(BUTTON_IsPressed(BUTTON_USB_DEVICE_REMOTE_WAKEUP) == 0)
//        {
            //Add code here to issue a resume signal.
//        }

        return;
    }
    
    //Copy the (possibly) interrupt context SOFCounter value into a local variable.
    //Using a while() loop to do this since the SOFCounter isn't necessarily atomically
    //updated and we need to read it a minimum of twice to ensure we captured the correct value.
    
    
    
    while(LocalSOFCount != SOFCounter)
    {
        LocalSOFCount = SOFCounter;
    }

  
    TimeDeltaMilliseconds = LocalSOFCount - OldSOFCount;
    if(TimeDeltaMilliseconds < 0)
    {
        TimeDeltaMilliseconds = (32767 - OldSOFCount) + LocalSOFCount;
    }
   
    if(TimeDeltaMilliseconds > 5000)
    {
        OldSOFCount = LocalSOFCount - 5000;
    }
    
    

    ProcessKeyboard();
    

    /* Check if the IN endpoint is busy, and if it isn't check if we want to send
     * keystroke data to the host. */
    
    // PREPARE FOR EP1 
    
    if(HIDTxHandleBusy(keyboard.lastINTransmission) == false)
    {
        /* Clear the INPUT report buffer.  Set to all zeros. */
        memset(&inputReport, 0, sizeof(inputReport));
        

        //  COPY FROM EP1_List TO inputReport
        
        if(ModListCount > 0){
            for(uint8_t j = 0;j < ModListCount;j++){
                
                inputReport.modifiers.value = ModKeyList[j].modifier | inputReport.modifiers.value;
                 
                
            }
        }
        
        
        if(CharListCount > 0){
            for(uint8_t j = 0; j < CharListCount; j++){
                if(j < 6){
                    inputReport.modifiers.value = CharKeyList[j].modifier | inputReport.modifiers.value;
                    inputReport.keys[j] = CharKeyList[j].keyCode;
                }
            }
        }
        
    

        //Check to see if the new packet contents are somehow different from the most
        //recently sent packet contents.
        needToSendNewReportPacket = false;
        for(i = 0; i < sizeof(inputReport); i++)
        {
            if(*((uint8_t*)&oldInputReport + i) != *((uint8_t*)&inputReport + i))
            {
                needToSendNewReportPacket = true;
                break;
            }
        }

       
        
        
        if(keyboardIdleRate != 0)
        {
            if(TimeDeltaMilliseconds >= keyboardIdleRate)
            {
                needToSendNewReportPacket = true;
            }
        }

        
        
        //Now send the new input report packet, if it is appropriate to do so (ex: new data is
        //present or the idle rate limit was met).
        if(needToSendNewReportPacket == true)
        {
            //Save the old input report packet contents.  We do this so we can detect changes in report packet content
            //useful for determining when something has changed and needs to get re-sent to the host when using
            //infinite idle rate setting.
            oldInputReport = inputReport;

            /* Send the 8 byte packet over USB to the host. */
            keyboard.lastINTransmission = HIDTxPacket(HID_EP, (uint8_t*)&inputReport, sizeof(inputReport));
            OldSOFCount = LocalSOFCount;    //Save the current time, so we know when to send the next packet (which depends in part on the idle rate setting)
        }
  
    }//if(HIDTxHandleBusy(keyboard.lastINTransmission) == false)
    
    for(uint8_t j = 0;j < 8;j++){ ModKeyList[j] = (KeyStruct){.keyCode = 0,.modifier = 0,.type = 0}; }
    for(uint8_t j = 0;j < 8;j++){ CharKeyList[j] = (KeyStruct){.keyCode = 0,.modifier = 0,.type = 0}; }
    ModListCount = 0;
    CharListCount = 0;
    
    // PREPARE FOR EP2
    
    if(HIDTxHandleBusy(ep2_handle) == false){
        

//            memset(&e2Report, 0, sizeof(e2Report));
            for(uint8_t j = 0; j < 4; j++){e2Report.data[j] = 0;}
            
            
            if(EP2ListCount > 0){
            
                for(uint8_t j = 0; j < EP2ListCount; j++){
                    e2Report.data[j] = EP2KeyList[j].keyCode;
                }
            }
            
            uint8_t shd_send = 0;
            
            
            for(uint8_t j = 0; j < sizeof(e2Report.data);j++){
//                if(*((uint8_t*)&old_ep2report + i) != *((uint8_t*)&e2Report + i)){
                if(old_ep2report.data[j] != e2Report.data[j]){
                   shd_send = 1;
                   break;
                }
            }
            
                        
            if(shd_send == 1){
               
                ep2_handle = HIDTxPacket(2,(uint8_t*)&e2Report,sizeof(e2Report));
                
                old_ep2report = e2Report;
                
            }
         
    }
    
    for(uint8_t j = 0;j < sizeof(EP2KeyList);j++){ EP2KeyList[j] = (KeyStruct){.keyCode = 0,.modifier = 0,.type = 0}; }
    EP2ListCount = 0;

    

    /* Check if any data was sent from the PC to the keyboard device.  Report
     * descriptor allows host to send 1 byte of data.  Bits 0-4 are LED states,
     * bits 5-7 are unused pad bits.  The host can potentially send this OUT
     * report data through the HID OUT endpoint (EP1 OUT), or, alternatively,
     * the host may try to send LED state information by sending a SET_REPORT
     * control transfer on EP0.  See the USBHIDCBSetReportHandler() function. */
    
    if(HIDRxHandleBusy(keyboard.lastOUTTransmission) == false)
    {
        KeyboardProcessOutputReport();

        keyboard.lastOUTTransmission = HIDRxPacket(HID_EP,(uint8_t*)&outputReport,sizeof(outputReport));
    }
   
    if(HIDRxHandleBusy(ep2_out_handle) == false){
        ep2_out_handle = HIDRxPacket(2,(uint8_t*)&e2outReport, sizeof(e2outReport));
    }
    
    return;		
}

static void KeyboardProcessOutputReport(void)
{
    if(outputReport.leds.capsLock)
    {
        LED_On(LED_USB_DEVICE_HID_KEYBOARD_CAPS_LOCK);
    }
    else
    {
        LED_Off(LED_USB_DEVICE_HID_KEYBOARD_CAPS_LOCK);
    }
    
    if(outputReport.leds.numLock)
    {
        LED_On(LED_USB_DEVICE_HID_KEYBOARD_NUM_LOCK);
    }
    else
    {
        LED_Off(LED_USB_DEVICE_HID_KEYBOARD_NUM_LOCK);
    }
    
    if(outputReport.leds.scrollLock)
    {
        LED_On(LED_USB_DEVICE_HID_KEYBOARD_SCROLL_LOCK);
    }
    else
    {
        LED_Off(LED_USB_DEVICE_HID_KEYBOARD_SCROLL_LOCK);
    }
}

static void USBHIDCBSetReportComplete(void)
{
    /* 1 byte of LED state data should now be in the CtrlTrfData buffer.  Copy
     * it to the OUTPUT report buffer for processing */
    outputReport.value = CtrlTrfData[0];

    /* Process the OUTPUT report. */
    KeyboardProcessOutputReport();
}

void USBHIDCBSetReportHandler(void)
{
    /* Prepare to receive the keyboard LED state data through a SET_REPORT
     * control transfer on endpoint 0.  The host should only send 1 byte,
     * since this is all that the report descriptor allows it to send. */
    USBEP0Receive((uint8_t*)&CtrlTrfData, USB_EP0_BUFF_SIZE, USBHIDCBSetReportComplete);
}


//Callback function called by the USB stack, whenever the host sends a new SET_IDLE
//command.
void USBHIDCBSetIdleRateHandler(uint8_t reportID, uint8_t newIdleRate)
{
    //Make sure the report ID matches the keyboard input report id number.
    //If however the firmware doesn't implement/use report ID numbers,
    //then it should be == 0.
    if(reportID == 0)
    {
        keyboardIdleRate = newIdleRate;
    }
}


/*******************************************************************************
 End of File
*/
