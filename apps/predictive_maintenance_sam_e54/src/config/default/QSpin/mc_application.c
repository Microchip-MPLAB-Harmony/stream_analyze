/*******************************************************************************
 Application source file

  Company:
  Microchip Technology Inc.

  File Name:
  mc_tasks.c

  Summary:
  Application source file

  Description:
  Application source file
 
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
 * Copyright (C) 2022 Microchip Technology Inc. and its subsidiaries.
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
#include "mc_application.h" 

/*******************************************************************************
 * Constants 
 *******************************************************************************/

/*******************************************************************************
 Private data-types 
 *******************************************************************************/
   
/*******************************************************************************
 Private variables 
 *******************************************************************************/
 static button_response_t  mcAppI_StartStopButton_gds;
static button_response_t  mcAppI_DirectionButton_gds;
static uint32_t mcAppI_1msSyncCounter_gdu32;
static uintptr_t dummyForMisra;
static uint8_t runStatus = 0u;

/*******************************************************************************
 Interface variables 
 *******************************************************************************/

/*******************************************************************************
 Private Functions 
 *******************************************************************************/
 
/*! \brief Start stop button scan
 *
 * Details.
 * Start stop button scan
 *
 * @param[in]:
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
__STATIC_INLINE void mcAppI_MotorStartStop(void)
{
    if( 0u == runStatus )
    {
        /** Start motor  */
        mcFocI_FieldOrientedControlEnable( &mcFocI_ModuleData_gds );


        /** Enable voltage source inverter */
        mcHalI_InverterPwmEnable();

        runStatus = 1u;
    }
    else
    {
        /** Start motor  */
        mcFocI_FieldOrientedControlDisable( &mcFocI_ModuleData_gds );
   

        /** Enable voltage source inverter */
        mcHalI_InverterPwmDisable();

        runStatus = 0u;
    }
}

/*! \brief Start stop button scan
 * 
 * Details.
 * Start stop button scan
 * 
 * @param[in]: 
 * @param[in/out]:
 * @param[out]:
 * @return:
 */

__STATIC_INLINE void mcAppI_DirectionReverse(void)
{
    if( 0u == runStatus )
    {
       /** Change state variable to toggle motor spin direction  */
       mcFocI_MotorDirectionChange(&mcFocI_ModuleData_gds);

   
        /** Set fault indicator */
        mcHal_DirectionIndication();
    }
}

/*! \brief 1 ms tasks handler
 *
 * Details.
 * 1 ms tasks handler
 *
 * @param[in]:
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
__STATIC_INLINE void mcAppI_1msTasksHandler( void )
{

    /** Start-stop button scan  */
    mcAppI_StartStopButton_gds.inputVal = mcHalI_StartStopButtonState();
    mcUtils_ButtonResponse(&mcAppI_StartStopButton_gds, &mcAppI_MotorStartStop);
    
    /** Direction button scan  */
    mcAppI_DirectionButton_gds.inputVal = mcHalI_DirectionButtonState();
    mcUtils_ButtonResponse(&mcAppI_DirectionButton_gds, &mcAppI_DirectionReverse);
   
    /** Field Oriented control - Slow Tasks */
    mcFocI_FieldOrientedControlSlow(&mcFocI_ModuleData_gds);

}

/*******************************************************************************
 Interface Functions 
 *******************************************************************************/

/*! \brief Application initialization 
 * 
 * Details.
 * Application initialization 
 * 
 * @param[in]: 
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
void mcAppI_ApplicationInit( void )
{   
    /** ADC end of conversion interrupt generation for FOC control */
    mcHalI_AdcInterruptDisable();
    mcHalI_AdcInterruptClear();

    /** Enable ADC interrupt for field oriented control */
    mcHalI_AdcCallBackRegister(  (ADC_CALLBACK)mcAppI_AdcCalibrationIsr, (uintptr_t)dummyForMisra );
    mcHalI_AdcInterruptEnable( );

    /** Enable ADC module */
    mcHalI_AdcEnable();

    /** Enable interrupt for fault detection */
    mcHalI_PwmCallbackRegister( (TCC_CALLBACK)mcAppI_OverCurrentReactionIsr, (uintptr_t)dummyForMisra );

    /** Enable PWM interrupt to detect fault */
    mcHalI_PwmInterruptEnable( );

    /** Enables PWM channels. */
    mcHalI_PwmTimerStart( );

    /** Disable PWM output */
    mcHalI_InverterPwmDisable();

    /** Set motor parameters */
    mcMotI_MotorParametersInit( &mcMotI_PMSM_gds);
        
    /** Initialize Current calculation */
    mcCurI_CurrentCalculationInit(&mcCurI_ModuleData_gds);

    /** Initialize Voltage measurement  */
    mcVolI_VoltageCalculationInit( &mcVolI_ModuleData_gds );
   

    /** Initialize PMSM motor control */
    mcFocI_FieldOrientedControlInit( &mcFocI_ModuleData_gds);


}

/*! \brief Over current reaction ISR
 *
 * Details.
 * Fault reaction ISR
 *
 * @param[in]:
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
void mcAppI_OverCurrentReactionIsr( uint32_t status,  uintptr_t context )
{
    /** Initialize PMSM motor control */
    mcFocI_FieldOrientedControlInit( &mcFocI_ModuleData_gds);

    /** Reset software modules */
    mcAppI_ApplicationReset();

    /** Enable voltage source inverter */
    mcHalI_InverterPwmDisable();

    /** Set fault indicator */
    mcHal_FaultIndicationSet();

}

/*! \brief Motor control application calibration
 * 
 * Details.
 *  Motor Control application calibration 
 * 
 * @param[in]: 
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
void mcAppI_AdcCalibrationIsr(ADC_STATUS status, uintptr_t context)
{    
    tmcTypes_StdReturn_e returnStatus;

    /** ADC end of conversion interrupt generation for FOC control */
    mcHalI_AdcInterruptDisable();
    mcHalI_AdcInterruptClear();

    /** Read phase currents  */
    mcHalI_PhaseACurrentGet();
    mcHalI_PhaseBCurrentGet();

    /** Phase current offset measurement  */
    returnStatus = mcCurI_CurrentOffsetCalculation(&mcCurI_ModuleData_gds );

    /** Current sense amplifiers offset calculation */
    if( StdReturn_Complete == returnStatus )
    {
        mcHalI_AdcCallBackRegister( (ADC_CALLBACK)mcAppI_AdcFinishedIsr, (uintptr_t)dummyForMisra );
    }
    else
    {
        /** For MISRA Compliance */
    }

    /** Calibration and monitoring update */
//    X2CScope_Update();

     /** ADC end of conversion interrupt generation for FOC control */
    mcHalI_AdcInterruptClear();
    mcHalI_AdcInterruptEnable();
}

/*! \brief Interrupt tasks execution 
 * 
 * Details.
 *  Interrupt tasks execution 
 * 
 * @param[in]: 
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
void mcAppI_AdcFinishedIsr(ADC_STATUS status, uintptr_t context )
{
    /** ADC interrupt disable  */
    mcHalI_AdcInterruptDisable();
    mcHalI_AdcInterruptClear();

    /** Read phase currents  */
    mcHalI_PhaseACurrentGet();
    mcHalI_PhaseBCurrentGet();
    
    /** Set Potentiometer and bus voltage channels */
    mcHalI_PotentiometerChannelSelect();
    mcHalI_DcLinkVoltageChannelSelect();
    
    /** Start software conversion */
    mcHalI_AdcSoftwareConversionStart();

    /** Current calculation */
    mcCurI_CurrentCalculation(&mcCurI_ModuleData_gds);

    /** Initialize PMSM motor control */
    mcFocI_FieldOrientedControlFast( &mcFocI_ModuleData_gds);

    /** Set duty */
    mcHalI_InverterPwmSet(mcPwmI_Duty_gau16);

    /** Bus voltage calculation */
    mcVolI_VoltageCalculation( &mcVolI_ModuleData_gds );

    /** Read DC bus voltage */
    mcHalI_DcLinkVoltageGet();

    /** Read potentiometer input */
    mcHalI_PotentiometerInputGet();

    /** Set phase A and phase B current channels */ 
    mcHalI_PhaseACurrentChannelSelect();
    mcHalI_PhaseBCurrentChannelSelect();
    
    /** Re-enable hardware trigger for ADC channels */ 
    mcHalI_AdcHardwareTriggerRenable();

    /** Calibration and monitoring update */
   // X2CScope_Update();

    /** Increment interrupt counter */
    mcAppI_1msSyncCounter_gdu32++;

    /** ADC interrupt clear  */
    mcHalI_AdcInterruptClear();
    mcHalI_AdcInterruptEnable();
}

/*! \brief Non-ISR tasks
 *
 * Details.
 * Non-ISR tasks
 *
 * @param[in]:
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
void mcAppI_NonISRTasks( void )
{
    float32_t loopCount = 0.001f * (float32_t)(20000);
    if( mcAppI_1msSyncCounter_gdu32 >= (uint32_t)loopCount )
    {
        mcAppI_1msSyncCounter_gdu32 = 0u;
        mcAppI_1msTasksHandler();
    }
}

/*! \brief Application reset 
 * 
 * Details.
 * Application reset 
 * 
 * @param[in]: 
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
void mcAppI_ApplicationReset( void )
{
    /** Voltage measurement  */
    mcVolI_VoltageCalculationReset( &mcVolI_ModuleData_gds );

    /** PMSM motor control */
    mcFocI_FieldOrientedControlReset( &mcFocI_ModuleData_gds);
}
