/**************************************************************************************************
Filename:       SampleApp.c
Revised:        $Date: 2009-03-18 15:56:27 -0700 (Wed, 18 Mar 2009) $
Revision:       $Revision: 19453 $

Description:    Sample Application (no Profile).


Copyright 2007 Texas Instruments Incorporated. All rights reserved.

IMPORTANT: Your use of this Software is limited to those specific rights
granted under the terms of a software license agreement between the user
who downloaded the software, his/her employer (which must be your employer)
and Texas Instruments Incorporated (the "License").  You may not use this
Software unless you agree to abide by the terms of the License. The License
limits your use, and you acknowledge, that the Software may not be modified,
copied or distributed unless embedded on a Texas Instruments microcontroller
or used solely and exclusively in conjunction with a Texas Instruments radio
frequency transceiver, which is integrated into your product.  Other than for
the foregoing purpose, you may not use, reproduce, copy, prepare derivative
works of, modify, distribute, perform, display or sell this Software and/or
its documentation for any purpose.

YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
PROVIDED 揂S IS?WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

Should you have any questions regarding your right to use this Software,
contact Texas Instruments Incorporated at www.TI.com.
**************************************************************************************************/

/*********************************************************************
This application isn't intended to do anything useful, it is
intended to be a simple example of an application's structure.

This application sends it's messages either as broadcast or
broadcast filtered group messages.  The other (more normal)
message addressing is unicast.  Most of the other sample
applications are written to support the unicast message model.

Key control:
SW1:  Sends a flash command to all devices in Group 1.
SW2:  Adds/Removes (toggles) this device in and out
of Group 1.  This will enable and disable the
reception of the flash command.
*********************************************************************/

/*********************************************************************
* INCLUDES
*/
#include "OSAL.h"
#include "ZGlobals.h"
#include "AF.h"
#include "aps_groups.h"
#include "ZDApp.h"

#include "SampleApp.h"
#include "SampleAppHw.h"

#include "OnBoard.h"

/* HAL */
#include "hal_lcd.h"
#include "hal_led.h"
#include "hal_key.h"
#include "MT_UART.h"
#include "MT_APP.h"
#include "MT.h"
#include "ds18b20.h"


/* user define*/
#include "string.h"
#include "stdlib.h"

/*********************************************************************
* MACROS
*/

/*********************************************************************
* CONSTANTS
*/

/*********************************************************************
* TYPEDEFS
*/

/*********************************************************************
* GLOBAL VARIABLES全局变量定义
*/
//串口
#define MY_DEFINI_UART_PORT 0 
#ifndef RX_MAX_LENGTH
#define RX_MAX_LENGTH 20
#endif
uint8 RX_BUFFER[RX_MAX_LENGTH];
uint8 RX_Length;
void UART_CallBackFunction(uint8 port, uint8 event);//当串口有数据到来时，调用此函数




// This list should be filled with Application specific Cluster IDs.
const cId_t SampleApp_ClusterList[SAMPLEAPP_MAX_CLUSTERS] =
{
  SAMPLEAPP_PERIODIC_CLUSTERID,
  SAMPLEAPP_FLASH_CLUSTERID
};

const SimpleDescriptionFormat_t SampleApp_SimpleDesc =
{
  SAMPLEAPP_ENDPOINT,              //  int Endpoint;
  SAMPLEAPP_PROFID,                //  uint16 AppProfId[2];
  SAMPLEAPP_DEVICEID,              //  uint16 AppDeviceId[2];
  SAMPLEAPP_DEVICE_VERSION,        //  int   AppDevVer:4;
  SAMPLEAPP_FLAGS,                 //  int   AppFlags:4;
  SAMPLEAPP_MAX_CLUSTERS,          //  uint8  AppNumInClusters;
  (cId_t *)SampleApp_ClusterList,  //  uint8 *pAppInClusterList;
  SAMPLEAPP_MAX_CLUSTERS,          //  uint8  AppNumInClusters;
  (cId_t *)SampleApp_ClusterList   //  uint8 *pAppInClusterList;
};

// This is the Endpoint/Interface description.  It is defined here, but
// filled-in in SampleApp_Init().  Another way to go would be to fill
// in the structure here and make it a "const" (in code space).  The
// way it's defined in this sample app it is define in RAM.
endPointDesc_t SampleApp_epDesc;

/*********************************************************************
* EXTERNAL VARIABLES
*/

/*********************************************************************
* EXTERNAL FUNCTIONS
*/

/*********************************************************************
* LOCAL VARIABLES
*/
uint8 SampleApp_TaskID;   // Task ID for internal task/event processing
// This variable will be received when
// SampleApp_Init() is called.
devStates_t SampleApp_NwkState;

uint8 SampleApp_TransID;  // This is the unique message ID (counter)

afAddrType_t SampleApp_Periodic_DstAddr;
afAddrType_t SampleApp_Flash_DstAddr;
afAddrType_t SampleApp_p2p_DstAddr;//点播地址

aps_Group_t SampleApp_Group;

uint8 SampleAppPeriodicCounter = 0;
uint8 SampleAppFlashCounter = 0;
//锚节点与协调器之间的数据传输使用
uint8 rssi_val[10];
uint8 rssi_index = 0; 
uint8 rssi_s[4] = {'\0'};

/*********************************************************************
* LOCAL FUNCTIONS
*/
void SampleApp_HandleKeys( uint8 shift, uint8 keys );
void SampleApp_MessageMSGCB( afIncomingMSGPacket_t *pckt );
void SampleApp_SendPeriodicMessage( void );
void SampleApp_SendFlashMessage( uint16 flashTime );
void SampleApp_P2P_SendMessage(void);
void SamplaAPP_ProcessBlindData( afIncomingMSGPacket_t *pkt);//参考节点处理接收到的锚节点的数据

/*********************************************************************
* NETWORK LAYER CALLBACKS
*/

/*********************************************************************
* PUBLIC FUNCTIONS
*/

/*********************************************************************
* @fn      SampleApp_Init
*
* @brief   Initialization function for the Generic App Task.
*          This is called during initialization and should contain
*          any application specific initialization (ie. hardware
*          initialization/setup, table initialization, power up
*          notificaiton ... ).
*
* @param   task_id - the ID assigned by OSAL.  This ID should be
*                    used to send messages and set timers.
*
* @return  none
*/
void SampleApp_Init( uint8 task_id )
{
  SampleApp_TaskID = task_id;
  SampleApp_NwkState = DEV_INIT;
  SampleApp_TransID = 0;
  halUARTCfg_t uartConfig;//初始化串口使用
  
  // Device hardware initialization can be added here or in main() (Zmain.c).
  // If the hardware is application specific - add it here.
  // If the hardware is other parts of the device add it in main().
  
#if defined ( BUILD_ALL_DEVICES )
  // The "Demo" target is setup to have BUILD_ALL_DEVICES and HOLD_AUTO_START
  // We are looking at a jumper (defined in SampleAppHw.c) to be jumpered
  // together - if they are - we will start up a coordinator. Otherwise,
  // the device will start as a router.
  if ( readCoordinatorJumper() )
    zgDeviceLogicalType = ZG_DEVICETYPE_COORDINATOR;
  else
    zgDeviceLogicalType = ZG_DEVICETYPE_ROUTER;
#endif // BUILD_ALL_DEVICES
  
#if defined ( HOLD_AUTO_START )
  // HOLD_AUTO_START is a compile option that will surpress ZDApp
  //  from starting the device and wait for the application to
  //  start the device.
  ZDOInitDevice(0);
#endif
  
  // Setup for the periodic message's destination address
  // Broadcast to everyone，
  //在定位实验中，盲节点应该使用广播形式，让参考节点能够均能接收到数据
  /*
  SampleApp_Periodic_DstAddr.addrMode = (afAddrMode_t)AddrBroadcast;
  SampleApp_Periodic_DstAddr.endPoint = SAMPLEAPP_ENDPOINT;
  SampleApp_Periodic_DstAddr.addr.shortAddr = 0xFFFF;
  
  
  //组播形式的初期设想应该是每层作为一个组使用。本次测试实验，忽略
  // Setup for the flash command's destination address - Group 1
  /*
  SampleApp_Flash_DstAddr.addrMode = (afAddrMode_t)afAddrGroup;
  SampleApp_Flash_DstAddr.endPoint = SAMPLEAPP_ENDPOINT;
  SampleApp_Flash_DstAddr.addr.shortAddr = SAMPLEAPP_FLASH_GROUP;
  */
  
  // 点播设置。定位时，参考节点与协调器之间的通讯是应该是点播，从而让协调器能够分类
  
  SampleApp_p2p_DstAddr.addrMode = (afAddrMode_t)Addr16Bit;
  SampleApp_p2p_DstAddr.endPoint = SAMPLEAPP_ENDPOINT;
  SampleApp_p2p_DstAddr.addr.shortAddr = 0x0000;//发送信息给协调器
  
  
  // Fill out the endpoint description.
  SampleApp_epDesc.endPoint = SAMPLEAPP_ENDPOINT;
  SampleApp_epDesc.task_id = &SampleApp_TaskID;
  SampleApp_epDesc.simpleDesc
    = (SimpleDescriptionFormat_t *)&SampleApp_SimpleDesc;
  SampleApp_epDesc.latencyReq = noLatencyReqs;
  
  // Register the endpoint description with the AF
  afRegister( &SampleApp_epDesc );
  
  // Register for all key events - This app will handle all key events
  RegisterForKeys( SampleApp_TaskID );
  
  // By default, all devices start out in Group 1
  //默认情况下，所有设备都是组1，可以对此进行设置，从而时不同的设备进入不同的组
  SampleApp_Group.ID = 0x0001;
  osal_memcpy( SampleApp_Group.name, "Group 1", 7  );
  aps_AddGroup( SAMPLEAPP_ENDPOINT, &SampleApp_Group );
  
  
  //串口初始化
  uartConfig.configured       = TRUE;
  uartConfig.baudRate         = HAL_UART_BR_9600;
  uartConfig.flowControl      = FALSE;
  uartConfig.flowControlThreshold = MT_UART_THRESHOLD;//默认使用流控制
  uartConfig.rx.maxBufSize     = 200;
  uartConfig.tx.maxBufSize     =200;
  uartConfig.idleTimeout       = MT_UART_IDLE_TIMEOUT;
  uartConfig.intEnable          =TRUE;
  uartConfig.callBackFunc       =UART_CallBackFunction;
  HalUARTOpen(MY_DEFINI_UART_PORT,&uartConfig);//使用自定义的串口初始化变量来开启串口通讯
  
  
  
  
  
  
  
#if defined ( LCD_SUPPORTED )
  HalLcdWriteString( "SampleApp", HAL_LCD_LINE_1 );
#endif
}

/*********************************************************************
* @fn      SampleApp_ProcessEvent
*
* @brief   Generic Application Task event processor.  This function
*          is called to process all events for the task.  Events
*          include timers, messages and any other user defined events.
*
* @param   task_id  - The OSAL assigned task ID.
* @param   events - events to process.  This is a bit map and can
*                   contain more than one event.
*
* @return  none
*/
uint16 SampleApp_ProcessEvent( uint8 task_id, uint16 events )
{
  afIncomingMSGPacket_t *MSGpkt;
  (void)task_id;  // Intentionally unreferenced parameter
  
  if ( events & SYS_EVENT_MSG )
  {
    MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( SampleApp_TaskID );
    while ( MSGpkt )
    {
      switch ( MSGpkt->hdr.event )
      {
        // Received when a key is pressed
      case KEY_CHANGE:
        SampleApp_HandleKeys( ((keyChange_t *)MSGpkt)->state, ((keyChange_t *)MSGpkt)->keys );
        break;
        
        // Received when a messages is received (OTA) for this endpoint
      case AF_INCOMING_MSG_CMD:
        SampleApp_MessageMSGCB( MSGpkt );
        break;
        
        // Received whenever the device changes state in the network
      case ZDO_STATE_CHANGE:
        SampleApp_NwkState = (devStates_t)(MSGpkt->hdr.status);
        if (// (SampleApp_NwkState == DEV_ZB_COORD)|| //协调器只负责接收数据，不发送数据
            (SampleApp_NwkState == DEV_ROUTER)
              || (SampleApp_NwkState == DEV_END_DEVICE) )
        {
          // Start sending the periodic message in a regular interval.
          osal_start_timerEx( SampleApp_TaskID,
                             SAMPLEAPP_SEND_PERIODIC_MSG_EVT,
                             SAMPLEAPP_SEND_PERIODIC_MSG_TIMEOUT );
        }
        else
        {
          // Device is no longer in the network
        }
        break;
        
      default:
        break;
      }
      
      // Release the memory
      osal_msg_deallocate( (uint8 *)MSGpkt );
      
      // Next - if one is available
      MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( SampleApp_TaskID );
    }
    
    // return unprocessed events
    return (events ^ SYS_EVENT_MSG);
  }
  
  
  
  // Discard unknown events
  return 0;
}

/*********************************************************************
* Event Generation Functions
*/
/*********************************************************************
* @fn      SampleApp_HandleKeys
*
* @brief   Handles all key events for this device.
*
* @param   shift - true if in shift/alt.
* @param   keys - bit field for key events. Valid entries:
*                 HAL_KEY_SW_2
*                 HAL_KEY_SW_1
*
* @return  none
*/
void SampleApp_HandleKeys( uint8 shift, uint8 keys )
{
  (void)shift;  // Intentionally unreferenced parameter
  
  if ( keys & HAL_KEY_SW_1 )
  {
    /* This key sends the Flash Command is sent to Group 1.
    * This device will not receive the Flash Command from this
    * device (even if it belongs to group 1).
    */
    SampleApp_SendFlashMessage( SAMPLEAPP_FLASH_DURATION );
  }
  
  if ( keys & HAL_KEY_SW_2 )
  {
    /* The Flashr Command is sent to Group 1.
    * This key toggles this device in and out of group 1.
    * If this device doesn't belong to group 1, this application
    * will not receive the Flash command sent to group 1.
    */
    aps_Group_t *grp;
    grp = aps_FindGroup( SAMPLEAPP_ENDPOINT, SAMPLEAPP_FLASH_GROUP );
    if ( grp )
    {
      // Remove from the group
      aps_RemoveGroup( SAMPLEAPP_ENDPOINT, SAMPLEAPP_FLASH_GROUP );
    }
    else
    {
      // Add to the flash group
      aps_AddGroup( SAMPLEAPP_ENDPOINT, &SampleApp_Group );
    }
  }
}

/*********************************************************************
* LOCAL FUNCTIONS
*/

/*********************************************************************
* @fn      SampleApp_MessageMSGCB
*
* @brief   Data message processor callback.  This function processes
*          any incoming data - probably from other devices.  So, based
*          on cluster ID, perform the intended action.
*
* @param   none
*
* @return  none
*/
void SampleApp_MessageMSGCB( afIncomingMSGPacket_t *pkt )
{
  uint16 flashTime;
  
  switch ( pkt->clusterId )
  {
    
    //确定只有协调器进行编译
  case SAMPLEAPP_ROUT_ANODE_CLUSTERID://协调器从锚节点接收数据
    HalUARTWrite(0,"A",1);
    HalUARTWrite(0,pkt->cmd.Data,pkt->cmd.DataLength); //输出接收到的数据 
    HalUARTWrite(0,"\n", 1); // 回车换行 
    break;
  case SAMPLEAPP_ROUT_BNODE_CLUSTERID://协调器从锚节点接收数据
    HalUARTWrite(0,"B",1);
    HalUARTWrite(0,pkt->cmd.Data,pkt->cmd.DataLength); //输出接收到的数据 
    HalUARTWrite(0,"\n", 1); // 回车换行 
    break;
  case SAMPLEAPP_ROUT_CNODE_CLUSTERID://协调器从锚节点接收数据
    HalUARTWrite(0,"C",1);
    HalUARTWrite(0,pkt->cmd.Data,pkt->cmd.DataLength); //输出接收到的数据 
    HalUARTWrite(0,"\n", 1); // 回车换行 
    break;
  case SAMPLEAPP_FLASH_CLUSTERID:
    flashTime = BUILD_UINT16(pkt->cmd.Data[1], pkt->cmd.Data[2] );
    HalLedBlink( HAL_LED_4, 4, 50, (flashTime / 4) );
    break;
    
  }
}

/*********************************************************************
* @fn      SampleApp_SendPeriodicMessage
*
* @brief   Send the periodic message.
*
* @param   none
*
* @return  none
*/
void SampleApp_SendPeriodicMessage( void )//盲节点发送数据
{
  byte state[]="Blind node";
  if ( AF_DataRequest( &SampleApp_Periodic_DstAddr, &SampleApp_epDesc,
                      SAMPLEAPP_BLIND_CLUSTERID,
                      sizeof("Blind node"),
                      state,
                      &SampleApp_TransID,
                      AF_DISCV_ROUTE,
                      AF_DEFAULT_RADIUS ) == afStatus_SUCCESS )
  {
  }
  else
  {
    // Error occurred in request to send.
  }
}

/*********************************************************************
* @fn      SampleApp_SendFlashMessage
*
* @brief   Send the flash message to group 1.
*
* @param   flashTime - in milliseconds
*
* @return  none
*/
void SampleApp_SendFlashMessage( uint16 flashTime )
{
  uint8 buffer[3];
  buffer[0] = (uint8)(SampleAppFlashCounter++);
  buffer[1] = LO_UINT16( flashTime );
  buffer[2] = HI_UINT16( flashTime );
  
  if ( AF_DataRequest( &SampleApp_Flash_DstAddr, &SampleApp_epDesc,
                      SAMPLEAPP_FLASH_CLUSTERID,
                      3,
                      buffer,
                      &SampleApp_TransID,
                      AF_DISCV_ROUTE,
                      AF_DEFAULT_RADIUS ) == afStatus_SUCCESS )
  {
  }
  else
  {
    // Error occurred in request to send.
  }
}

/*********************************************************************
*********************************************************************/
static void UART_CallBackFunction(uint8 port, uint8 event)
{
  uint8 RX_Flag = 0;
  RX_Length = 0;//接收字符串长度
  RX_Flag = RX_Length = Hal_UART_RxBufLen(MY_DEFINI_UART_PORT);// 接收到的字符串长度
  
  if(RX_Flag != 0)// 有数据存在
  {
    //读取串口数据
    HalUARTRead(MY_DEFINI_UART_PORT,RX_BUFFER,RX_Length);
    {
      //将数据发回给电脑，使用hal_uart.h的接口函数
      HalUARTWrite(MY_DEFINI_UART_PORT,RX_BUFFER,RX_Length);
    }
  }
  
}



/*
本函数是参考节点处理盲节点的数据处理函数，在本函数中应该得到锚节点与盲节点之间的距离，
然后锚节点在发送数据的时候，将这个距离值也发送出去
*/

void SamplaAPP_ProcessBlindData( afIncomingMSGPacket_t *pkt)
{
  uint8 i,max_i,min_i; 
  uint16 val = 0;
  
  rssi_val[rssi_index++] = ((~pkt->rssi)+1); //得到rssi的值，并保存      
  if(rssi_index == 10)//当接收到10次RSSI后进行平均值计算
  {
    //去除最大最小值，剩下的求取平均值;
    max_i = min_i = 0;          
    for(i = 1; i<10; i++)
    {
      if(rssi_val[i]>rssi_val[max_i])
        max_i = i;              
      if(rssi_val[i]<rssi_val[min_i])
        min_i = i;
    }          
    for(i = 0; i<10; i++)
    {
      if(i!=max_i && i!=min_i)
        val+=rssi_val[i];//去除最大最小值之后的8次RSSI值的总和
    }          
    val >>= 3;//右移为除法，右移三位表示除以8（val为8次的RSSI值的和）
    
    rssi_s[0] ='A';
    rssi_s[1] = val/100+0x30;//ASCII编码 0x30对应的是数字 0。这里加上这个数就是要显示数字字符
    rssi_s[2] = val%100/10+0x30;
    rssi_s[3] = val%10+0x30;//以上三句分别代表得到：米，分米，厘米      
    
    SampleApp_P2P_SendMessage();//调用此函数进行信息发送
    
    HalUARTWrite(0, rssi_s, 3); //输出接收到的数据
    //        HalUARTWrite(0, "\r\n", 1); // 回车换行 */
    
    rssi_index = 0;//每收到十次RSSI值计算一次需要的RSSI均值
    
    rssi_s[0] = 0;
    rssi_s[1] = 0;
    rssi_s[2] = 0;
    rssi_s[3] = 0;
  }
  
  //  HalUARTWrite(0,"Receive BlindNode Data ",sizeof("Receive BlindNode Data")); //输出接收到的数据  
  //  HalUARTWrite(0,"\r\n", 1); // 回车换行 
}


/*********************************************************************
* @fn      SampleApp_P2P_SendMessage
*
* @brief   point to point.
*
* @param   none
*
* @return  none
在每个路由器或终端节点发送数据时，通过标定AF_DataRequest中的簇ID，就可以简单的
将设备进行分类，即通过设置不同的ID来标定不同的设备，然后在协调器端，通过对接收到
的消息的簇ID，来鉴别出是哪个设备发送来的数据
*/
void SampleApp_P2P_SendMessage( void )
//后期参考节点在发送数据的时候，必须发送的是参考节点与盲节点之间的距离值
{
  
  /*AF在发送数据的时候是非常有针对性的，其第一个参数是发送目的地的地址之类的参数，在SampleApp_Init初始化的时候就定义了。
  然后就是设备描述符（系统默认就好），在SampleApp_Init也定义了。第三个参数本例使用自定义的p2p名字，作用是让接收函数（协调器）
  明确知道此类消息是那种类型（广播、组播、点播还是自定义的p2p）。
  所以一定要明确，在数据发送的时候，已经进行了很多的规定，以免造成接收端任务的麻烦  
  */
  
  
  if ( AF_DataRequest( &SampleApp_p2p_DstAddr, //发送目的地址+端点地址+传送模式
                      &SampleApp_epDesc,//源（答复或确认）终端的描述，即设备描述符
                      SAMPLEAPP_ROUT_BNODE_CLUSTERID,//已经定义（系统或自定义）的簇的ID，用来表示发送的数据标识，让接收函数可以用来分类
                      4,//发送数据的长度
                      rssi_s,//发送数据缓冲区，即数据值
                      &SampleApp_TransID,//任务ID，系统会分配？
                      AF_DISCV_ROUTE,//有效位掩码的发送选项
                      AF_DEFAULT_RADIUS ) == afStatus_SUCCESS )//传送跳数，通常使用默认值
  {
  }
  else
  {
    // Error occurred in request to send.
  }
}

//