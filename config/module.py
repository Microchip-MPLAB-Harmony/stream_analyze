# coding: utf-8
"""*****************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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
*****************************************************************************"""

#---------------------------------------------------------------------------------------------------------------------------#
#                                                   Load Module                                                                                   #
#---------------------------------------------------------------------------------------------------------------------------#
def loadModule():
    print("Load Module: Harmony SA Studio")

    if( ("SAMD5" in Variables.get("__PROCESSOR")) or ("SAME5" in Variables.get("__PROCESSOR"))):

        # Symbol Name               Component        Component ID        Display Name    Display Path     File Path
        component = Module.CreateComponent("sa_engine", "SA Engine", "/AI-ML",  "config/scripts/main.py")
        component.setDisplayType("AI-ML Component")

        #                                        Symbol Name          Dependency ID     Dependency    Generic Required
        component.addDependency("saEngineUartDependency", "UART",  False, True )

        #                                        Symbol Name          Dependency ID     Dependency    Generic Required
        component.addDependency("saEngineTimerDependency", "TMR",  False, True )

        #                                         Symbol Name          Dependency ID     Dependency    Generic Required
        component.addDependency("saEngineInterruptDependency", "TMR",  False, True )
