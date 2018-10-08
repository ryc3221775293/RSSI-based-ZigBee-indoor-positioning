Texas Instruments, Inc.

ZStack-CC2530 Release Notes

-------------------------------------------------------------------------------
-------------------------------------------------------------------------------

ZigBee 2007 Release
Version 2.3.0-1.4.0
January 17, 2010


Notices:
 - Z-Stack supports the ZigBee 2007 Specification, including features such
   as PanID Conflict Resolution, Frequency Agility, and Fragmentation. The
   ZigBee 2007 Specification (www.zigbee.org) defines two ZigBee stack
   profiles, ZigBee and ZigBee-Pro. ZStack-2.3.0 provides support for both
   of these profiles. See the Z-Stack Developer's Guide for details.

 - Z-Stack now supports an IAR project to build "ZigBee Network Processor"
   (ZNP) devices. CC2530-based ZNP devices can be used with any host MCU
   that provides either an SPI or UART port to add ZigBee-Pro communication
   to existing or new designs. See the "CC2530ZNP Interface Specification"
   and "Z-Stack User's Guide for CC2530 ZigBee-PRO Network Processor -
   Sample Applications" documents for details and using the ZNP.

 - The library files have been built/tested with EW8051 version 7.51A/W32
   (7.51.1.3) and may not work with previous versions of the IAR tools. You
   can obtain the 7.51A installer and patches from the IAR website.

 - When programming a target for the first time with this release, make sure
   that you select the "Erase Flash" in the "Debugger->Texas Instruments->
   Download" tab in the project options. When programming completes, it is
   recommended that the "Erase Flash" box gets un-checked so that NV items
   are retained during later re-programming.

 - Please review the document "Upgrading To Z-Stack v2.3." for information
   about moving existing v2.2.x applications to v2.3.x.


Changes:
 - The default size of the neighbor table, MAX_NEIGHBOR_ENTRIES, has
   increased from 8 to 16 to provide greater stability in larger networks that
   are using security. [3144]

 - Improved the "best router" algorithm (for ZigBee-Pro devices) to consider
   link quality over network depth when processing beacons during the join
   or rejoin process. [3137]

 - The compile flag "SE_PROFILE" has been changed to "TC_LINKKEY_JOIN"
   to better reflect the code being controlled. Users do not need to make any
   changes - "SE_PROFILE" is redefined in the ZGlobals.h file.  [3121]

 - The OSAL_Timer API has been extended to add an automatically reloading
   timer. In addition to simplifying the use of periodic timers, this feature also
   can be used to eliminate problems with failing to obtain a timer when heap
   resources are depleted (during heavy traffic situations). Four timers in the
   NWK layer have been converted to use reloading timers to increase device
   robustness under heavy traffic situations. Section 5.3 of the "OSAL API"
   document gives details on use of "osal_start_reload_timer()". [3070]

 - The default IDE setting for "Virtual Registers" has been increased from 8
   to 16. The results in a typical CODE size reduction of 1.5% at the expense
   of 8 bytes of DATA memory. [3068]

 - The OSAL_MSG API has been extended to add capability to search for an
   existing OSAL message in the queue. See section 3.6 of the "OSAL API"
   document for details on use of "osal_msg_find()". [3065]

 - The HAL_ADC API has been extended to add capability to set up the ADC
   voltage reference for use by the HalAdcRead() function. The function was
   previously hardwired to use "HAL_ADC_REF_AVDD", which is still default.
   Reference values to be used with the new HalAdcSetReference() function
   are defined in the hal_adc.h file. [3053]

 - The Monitor-Test (MT) API has been extended to provide support for the
   Inter-Pan method of communication. Refer to sections 3.2.1.5 and 3.2.1.3
   of the "Z-Stack Monitor and Test API" document on details of using the MT
   commands and responses. Refer to section 12 of the "Z-Stack Developer's
   Guide" for a discussion of the Inter-Pan feature. [3052]

 - Support has been added for mutiple pre-configured Trust Center link keys.
   The default number of keys remains set at 1. Refer to section 10.5 of the
   "Z-Stack Developer's Guide" for discussion of multiple link keys. [3024]

 - The Monitor-Test (MT) API has been extended to provide support for the
   configuration of application link keys. Refer to sections 3.12.1.26 through
   3.12.1.28 of the "Z-Stack Monitor and Test API" document for details of
   using MT commands and responses for link key configuration. [2997]

 - The Monitor-Test (MT) API has been extended to provide support for
   sending messages using source routing. Refer to sections 3.2.1.4 and
   3.12.2.27 of the "Z-Stack Monitor and Test API" document for details of
   using MT commands and responses for source routing. [2929]


Bug Fixes:
 - Fixed a problem with the Smart Energy key establishment process
   where it could fail due to timing issues with APS retries. [3160]

 - Fixed a problem where a DMA transmit interrupt for serial I/O could
   be missed, causing serial transmission to become disabled. [3155]

 - Fixed a problem that inadvertently included some Link Status code in
   End-Device builds. This could prevent an End-Device from entering the
   "deep sleep" mode for lowest power consumption. [3154]

 - Fixed a "buffer memory leak" in which devices that forward network
   address conflict messages eventually run out of TX buffer capacity and
   lock up after handling 18 address conflicts. [3129]

 - Fixed a problem where the afDataReqMTU() function would return an
   incorrect size (14 bytes too small) when security was disabled when
   building a device from a standard library-based project. [3112]

 - Updated the characterized values in the "HalAdcVddLimit[]" table to
   reflect the internal 1.15 reference voltage of CC2530. The prevoius
   values could indicate "low battery" situation before necessary. [3103]

 - Improved delivery of unicast response messages in a larger network
   that has broadcast "storms" - route responses could get lost when a
   route request occured. A delay was added to re-broadcasts of route
   requests and limited queuing of incoming broadcast messages. [3099]

 - Fixed a problem in zclGeneral_KeyEstablishment_GetRandom() where
   the 40-byte random number was limited to 2^16 values obtained from
   the ZStack 16-bit pseudo-random number generator. [3096]

 - Improved performance of device joining/rejoining when there is more
   than one network with the same extended PanID in the neighborhood.
   Between joining cycles, devices now clear their neighbor table and the
   network descriptor list. [3094]

 - Fixed a problem where a device would use old routing table entries
   after rejoing the network to a different parent. Devices now remove
   appropriate routing entries when a Device Announce is handled. [3091]

 - Fixed the CTS/RTS pin definitions in the _hal_uart files. This will allow
   serial I/O that depends on flow control to sork properly. [3087]

 - Fixed a "buffer memory leak" in which devices with security enabled
   would eventually run out of TX buffer capacity and lock up. [3086]

 - Fixed a problem with sending a message using fragmentation that
   occured when the 64-bit etended destination address was provided
   instead of the network short address. [3083]

 - Improved handling of source routed messages - when a device is the
   last one in the relay list, it will now send the message directly to the
   destination address, regardless of whether the destination device is
   in the neighbor table. [3081]

 - Fixed a memory leak in the Trust Center where memory allocated for
   processing of transport key commands was not deallocated. [3077]

 - Fixed a problem that produced phantom "shift" key input when joystick
   input occured. In addition, the "shift" key did not work properly. [3066]

 - Added feedback to the application layer after ZDO processing of a leave
   request. The application layer is notified by ZDO_STATE_CHANGE. [3064]

 - Fixed a problem (when using NV_RESTORE) with ZDO leave processing
   that would restore the original network state if power was cycled to the
   device. Devices now get a new network short address if power is cycled
   after processing a ZDO leave command. [3063]

 - Corrected the enumerated values defined in the "ZMacTransmitPower_t"
   table defined in the ZMAC.h file. [3056]

 - Fixed a routing loop problem that could occur when Many-To-One routing
   was used. When the link to the next hop was down, the device sent a MTO
   route error to a random neighbor device, which would be forwarded to the
   concentrator. A routing loop would be generated if the random neighbor
   device was the device that forwarded the packet. [3051]

 - Fixed an error in the "zclCCPublishPrice_t" structure definition - the member
   "PriceTier" was located in the wrong position, causing erroneous data to be
   displayed by a device issuing a Get Current Price command. [3044]

 - Fixed a problem in the SE Sample App which could allow the simple meter
   device to send APS packets with a bogus application link key. The message
   handler for key establishment indication was not properly checking status
   of ZCL_KEY_ESTABLISH_IND. [3042]

 - Added the time cluster (ZCL_CLUSTER_ID_GEN_TIME) to the cluster list for
   each of the SE Sample App device configurations. [3041]

 - Fixed a problem that could occur under heavy traffic conditions - a memory
   allocation failure during confirm processing would cause a "buffer memory
   leak" and leave a completed message in the NWK buffer. The device would
   eventually lose its ability to send messages, including Link Status. [3028]

 - Fixed a problem where a router would allow a device to join or rejoin and
   save information in the associated device table even if there was no empty
   space in the Address Manager. With security enabled, this was causing the
   router to request an Update Device (key) with an extended address of all
   zeroes. In addition, the router and end-device capacity bits in the beacon
   payload is now updated when the Address Manager table is full. [3017]

 - Fixed a problem in security key handling after a PRO end-device rejoined
   a network at a different router than the original parent. When a second
   end-device joined the network, the parent of the first end-device would
   request security keys for both end-devices, instead of just the 2nd. [3016]

 - Improved performance of messages using APS ACK in a busy network
   with security enabled - retried APS ACK messages could be dropped due
   to stale frame counter values. Messages are now stored un-encrypted in
   the NWK buffer - encryption/decryption occurs during transfer to/from
   the NWK and MAC layers. [3009]

 - Fixed a deviation from the ZigBee spec (2.4.3.1.7.1) in some sample apps
   would broadcast Match Descriptor request to destination address 0xFFFF.
   The address 0xFFFD (macRxOnWhenIdle = TRUE) is now used. Note that
   this disallows ZStack sample apps to find end devices via SW4. [3008]

 - Added error response, DEVICE_NOT_FOUND, for IEEE Address request
   and NWK Address request that are recieved as unicast messages and no
   device can be found. [2991]

 - Fixed a problem, when security was enabled, that prevented processing
   of rejoin requests and responses when a neighbor table entry for source
   devices did not exist or could not be created. Packets with radius=1 now
   get processed even if a neighbor table entry can't be referenced. [2887]

 - Improved response to recieved multicast message with ACK requested.
   Even though broadcast/group messages should not request an ACK, the
   erroneous request is now filtered before processing the message. [2874]

 - Fixed two problems with End-Devices configured with "RX on when idle"
   (CAPINFO_RCVR_ON_IDLE): they never received messages addressed to
   0xFFFD (NWK_BROADCAST_SHORTADDR_DEVRXON) and intermittantly
   got ones sent to 0xFFFF (NWK_BROADCAST_SHORTADDR). [2868]


Known Issues:
 - OAD supports two modes, internal and external. Internal mode does not
   use an external flash memory, so the OAD image must fit within unused
   flash memory on the CC2530. In other words, two images must fit in the
   CC2530 memory. External mode uses and external flash memory, which
   is 128K on SmartRF05EB Rev 1.3 boards, and 256K on 1.7 boards. 

 - Z-Stack now provides support for the Texas Instruments CC2590-CC2591
   PA/LNA range extender. The CC2590-CC2591 increases the RF link budget
   via PA for increased transmit output power and LNA for improved receiver
   sensitivity. See the "HAL Driver API" document for details. Please note that
   the CC2530 + CC2590 "Combo" board is not yet available for evaluation of
   the PA/LNA.

 - To disable security at build time, use the "SECURE=0" compile option. Do
   not attempt to disable security by setting the SECURITY_LEVEL to zero.

 - SerialApp is not reliable when used for high-speed file transfer.

 - The ZDO Complex Descriptor is not supported.

-------------------------------------------------------------------------------
-------------------------------------------------------------------------------

For technical support please contact:

Texas Instruments, Inc.
Low Power RF
lpwsupport@ti.com
