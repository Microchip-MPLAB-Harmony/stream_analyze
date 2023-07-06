/*******************************************************************************
 SA Engine utilities source file

  Company:
    Microchip Technology Inc.

  File Name:
    sa_Utilities.c

  Summary:
    Source file for SA Engine utilities

  Description:
    This file contains the data structures and function prototypes of SA Engine utilities
    
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2023 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END

/*******************************************************************************
 Header Inclusions
 *******************************************************************************/
#include "../inc/sa_utilities.h"

/*******************************************************************************
 Pre-processor macros
 *******************************************************************************/
 #define NUMBER_OF_SIGNALS  2u
 #define BUFFER_SIZE 800u

/*******************************************************************************
 Data-types
 *******************************************************************************/

 typedef struct
 {
     int16_t sensorType;
     volatile float * source;
 }tSAEngine_Signal_s;

  typedef struct
 {
     bool initDone;
     tSAEngine_Signal_s signals[NUMBER_OF_SIGNALS] ;
 }tSAEngine_State_s;

 /*******************************************************************************
 Global Variables
 *******************************************************************************/
 char rxBuffer[BUFFER_SIZE];
 tSAEngine_State_s SAEngine_Global_gds;

/*******************************************************************************
 Public Functions
 *******************************************************************************/
/*! \brief Initialize/ Configure SA Engine resources
 *
 * Details.
 * Initialize/ Configure SA Engine resources
 *
 * @param[in]:
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
void SAEngine_Initialize( void )
{
    /** The main init function for SA Engine  */
    SAEngine_NanocoreInit( NULL );

    /** Setting up the stack needs for the subsystem */
    SAEngine_NanocoreInitTask(0,0);

    /** Communication port for communication dev board and the relay process */
    SAEngine_NanocoreInitCOM();

    /*All things related to clock*/
    RTC_Timer32CounterSet(0);
    RTC_Timer32Start();

    /** SysTick needed for communication and long jump */
    SYSTICK_TimerCallbackSet(&SAEngine_TimeoutHandlerTick, (uintptr_t) NULL);
    SYSTICK_TimerStart();

    /** Register a callback for read events */
    SERCOM2_USART_ReadCallbackRegister(SAEngine_UartReadEventHandler, (uintptr_t) NULL);

    /** Set USART read threshold */
    SERCOM2_USART_ReadThresholdSet(1);

     /* Enable RX event notifications */
    SERCOM2_USART_ReadNotificationEnable(true, false);

    SAEngine_Global_gds.initDone = true;
}


/*! \brief Run sa.engine evaluation loop
 *
 * Details.
 * Run sa.engine evaluation loop
 *
 * @param[in]:
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
int16_t SAEngine_TaskStatusEvaluate( void )
{
    /** Run sa.engine nanocore evaluation loop */
    return SAEngine_NanocoreInitEvalLoop();
}

/*! \brief Run sa.engine tasks
 *
 * Details.
 * Run sa.engine tasks
 *
 * @param[in]:
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
sa_task_state SAEngine_RegisteredTasksRun( void )
{
    /** Run sa.engine nanocore */
    return SAEngine_NanocoreRunTasks();
}


/*! \brief API to initialize sensor signal source
 *
 * Details.
 * API to initialize sensor signal source
 *
 * @param[in]:
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
bool SAEngine_SignalInitialize( int16_t identifier, volatile float32_t * const source )
{
    bool initStatus = false;
    if( identifier < NUMBER_OF_SIGNALS )
    {
        SAEngine_Global_gds.signals[identifier].source = source;
        initStatus = true;
    }
    return initStatus;
}

/*! \brief API to read sensor signals
 *
 * Details.
 * API to read sensor signals
 *
 * @param[in]:
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
float SAEngine_SensorSampleGet( int16_t identifier )
{
    float result = 0.0f;

    if( identifier < NUMBER_OF_SIGNALS )
    {
        result =  *SAEngine_Global_gds.signals[identifier].source;
    }
    else
    {
       result = 0.0f;
    }

    return result;
}

/*! \brief Return timer counter
 *
 * Details.
 * Return timer counter
 *
 * @param[in]:
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
uint32_t SAEngine_TrackTimerCountGet (void )
{
    return RTC_Timer32CounterGet();

}

/*! \brief Write data to communication buffer
 *
 * Details.
 * Write data to communication buffer
 *
 * @param[in]:
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
int16_t SAEngine_SerialBufferWrite(int16_t index, uint8_t * const buffer, size_t len)
{
    bool flag = false;
    if(index == 0)
    {
        while (!flag)
        {
            flag = DMAC_ChannelTransfer(DMAC_CHANNEL_0, buffer, (const void *)&(SERCOM2_REGS->USART_INT.SERCOM_DATA), len );
        }
        return len;
    }
    return len;
}

/*! \brief Communication port callback function
 *
 * Details.
 * Communication port callback function
 *
 * @param[in]:
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
void SAEngine_UartReadEventHandler(SERCOM_USART_EVENT event, uintptr_t context )
{
   if (event == SERCOM_USART_EVENT_READ_THRESHOLD_REACHED)
    {
         /** Receiver should atleast have the threshold number of bytes in the ring buffer */
         uint32_t nBytesAvailable = SERCOM2_USART_ReadCountGet();

         /** Read USART ring buffer into rxBuffer */
         SERCOM2_USART_Read((uint8_t*)&rxBuffer[0], nBytesAvailable);

         /** Read serial buffer */
         SAEngine_SerialBufferRead(&rxBuffer[0], nBytesAvailable);
    }
}
