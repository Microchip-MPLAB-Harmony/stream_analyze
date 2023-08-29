/*******************************************************************************
 SA Studio utilities header file

  Company:
    Microchip Technology Inc.

  File Name:
    SAEngine_Utilities.h

  Summary:
    Header file for SA Studio utilities

  Description:
    This file contains the data structures and function prototypes of SA Studio utilities

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

#ifndef SA_STUDIO_H__
#define SA_STUDIO_H__

/*******************************************************************************
 Header Inclusions
 *******************************************************************************/
 #include <stdint.h>
 #include "definitions.h"
 #include "sa_types.h"
 #include "sa_nanocore_coro.h"

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
 Interface variables
 *******************************************************************************/
 extern  tSAEngine_State_s SAEngine_Global_gds;

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
void SAEngine_Initialize( void );

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
int16_t SAEngine_TaskStatusEvaluate( void );

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
sa_task_state SAEngine_RegisteredTasksRun( void );


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
bool SAEngine_SignalInitialize( uint16_t identifier, volatile float32_t * const source );

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
float SAEngine_SensorSampleGet( uint16_t identifier );

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
uint32_t SAEngine_TrackTimerCountGet (void );

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
size_t SAEngine_SerialBufferWrite(int16_t index, uint8_t * const buffer, size_t len);

/*! \brief Communication port callback fucntion
 *
 * Details.
 * Communication port callback fucntion
 *
 * @param[in]:
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
void SAEngine_UartReadEventHandler(SERCOM_USART_EVENT event, uintptr_t context );

#endif
