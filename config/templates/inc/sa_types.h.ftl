/*******************************************************************************
 SA Engine Functions header file

  Company:
    Microchip Technology Inc.

  File Name:
    SAEngine_Functions.h

  Summary:
    Header file for SA Studio utilities

  Description:
    This file contains the data structures and function prototypes of SA Engine functions

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

#ifndef SA_TYPES_H__
#define SA_TYPES_H__

/*******************************************************************************
 Header Inclusions
 *******************************************************************************/
#include <stdint.h>
#include "definitions.h"

 /*******************************************************************************
 Data-types
 *******************************************************************************/

/* MISRA C-2012 1.1 deviated below. Deviation record ID - H3_MISRAC_2012_R_1_1_DR_1 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma coverity compliance block deviate:2 "MISRA C-2012 Rule 1.1" "H3_MISRAC_2012_R_1_1_DR_1"

typedef float float32_t;

#pragma coverity compliance end_block "MISRA C-2012 Rule 1.1"
#pragma GCC diagnostic pop
/* MISRAC 2012 deviation block end */

#endif
