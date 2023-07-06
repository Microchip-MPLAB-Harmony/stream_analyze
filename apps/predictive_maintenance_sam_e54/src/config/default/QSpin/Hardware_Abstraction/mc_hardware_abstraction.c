/*******************************************************************************
  Hardware abstraction source file

  Company:
    Microchip Technology Inc.

  File Name:
    mc_hardware_abstraction.c

  Summary:
    This file contains all the functions related to hardware abstraction

  Description:
    This file contains implementation of the hardware abstraction
 
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2021 Microchip Technology Inc. and its subsidiaries.
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
Headers inclusions
*******************************************************************************/
#include "mc_hardware_abstraction.h"

/*******************************************************************************
 * Constants
*******************************************************************************/

/*******************************************************************************
 * Interface variables
*******************************************************************************/
/** Analog interfaces */
uint16_t mcHalI_IaAdcInput_gdu16;
uint16_t mcHalI_IbAdcInput_gdu16;
uint16_t mcHalI_UbusAdcInput_gdu16;
uint16_t mcHalI_Potentiometer_gdu16;
int16_t mcPwmI_Duty_gau16[3u];

/*******************************************************************************
 * Interface Functions
*******************************************************************************/

/*! \brief Enable PWM inverter 
 * 
 * Details.
 * Enable PWM inverter
 * 
 * @param[in]: 
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
void mcHalI_InverterPwmEnable( void )
{
    /*Disable PWM override*/
    while ((TCC0_REGS->TCC_SYNCBUSY & (TCC_SYNCBUSY_PATT_Msk)) == TCC_SYNCBUSY_PATT_Msk)
    {
        __NOP();
    }
    TCC0_REGS->TCC_PATT = 0x0000;
}

/*! \brief Disable PWM inverter 
 * 
 * Details.
 * Disable PWM inverter
 * 
 * @param[in]: 
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
void mcHalI_InverterPwmDisable( void )
{
/** Disable PWM override*/
   /*Override all PWM outputs to low*/
    while ((TCC0_REGS->TCC_SYNCBUSY & (TCC_SYNCBUSY_PATT_Msk)) == TCC_SYNCBUSY_PATT_Msk)
    {
       __NOP();
    }
    TCC0_REGS->TCC_PATT = 0x00FF;
}


/*! \brief Set direction indicator
 * 
 * Details.
 * Set direction indicator
 * 
 * @param[in]: 
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
void mcHal_DirectionIndication( void )
{
    LED_02_Toggle();
}

/*! \brief Set fault indicator
 * 
 * Details.
 * Set fault indicator
 * 
 * @param[in]: 
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
void mcHal_FaultIndicationSet( void )
{
   LED_01_Set();
}

/*! \brief ADC Enable
 * 
 * Details.
 * ADC Enable
 * 
 * @param[in]: 
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
void mcHalI_AdcEnable( void )
{
    ADC0_Enable();
}

/*! \brief PWM Enable
 *
 * Details.
 * PWM Enable
 *
 * @param[in]:
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
void mcHalI_PwmInterruptEnable( void )
{
    NVIC_EnableIRQ(TCC0_OTHER_IRQn);  
}

/*! \brief PWM timer Start
 * 
 * Details.
 * PWM timer Start
 * 
 * @param[in]: 
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
void mcHalI_PwmTimerStart( void )
{
    TCC0_PWMStart( );  
}

/*! \brief ADC callback function
 * 
 * Details.
 * ADC callback function
 * 
 * @param[in]: 
 * @param[in/out]:
 * @param[out]:
 * @return:
 */

void mcHalI_AdcCallBackRegister( ADC_CALLBACK callback, uintptr_t context )
{
    ADC0_CallbackRegister( callback, context);
}

/*! \brief PWM fault callback function
 * 
 * Details.
 * PWM fault callback function
 * 
 * @param[in]: 
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
void mcHalI_PwmCallbackRegister( TCC_CALLBACK callback, uintptr_t context )
{
    TCC0_PWMCallbackRegister( callback, (uintptr_t)context);
}



bool mcHalI_StartStopButtonState( void )
{
     return (bool)BUTTON_01_Get();
}

bool mcHalI_DirectionButtonState( void )
{
     return (bool)BUTTON_02_Get();
}

