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

#ifndef FIXED_MEMORY_ADDRESS_H
#define FIXED_MEMORY_ADDRESS_H

#define FIXED_ADDRESS_MEMORY

#if(__XC8_VERSION < 2000)
    #define KEYBOARD_INPUT_REPORT_DATA_BUFFER_ADDRESS_TAG @0x500
    #define KEYBOARD_OUTPUT_REPORT_DATA_BUFFER_ADDRESS_TAG @0x508
    #define E2_REPORT_DATA_BUFFER_ADDRESS_TAG @0x510
    #define E2_OUT_DATA_BUFFER_ADDRESS_TAG @0x518
#else
    #define KEYBOARD_INPUT_REPORT_DATA_BUFFER_ADDRESS_TAG      __at(0x500)
    #define KEYBOARD_OUTPUT_REPORT_DATA_BUFFER_ADDRESS_TAG     __at(0x508)
    #define E2_REPORT_DATA_BUFFER_ADDRESS_TAG     __at(0x510)
    #define E2_OUT_DATA_BUFFER_ADDRESS_TAG     __at(0x518)
#endif

#endif //FIXED_MEMORY_ADDRESS
