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

# ------------------------------------------------------------------------------------------------------------------ #
#                                                        Inclusions                                                                         #
# ------------------------------------------------------------------------------------------------------------------ #
import os

# ------------------------------------------------------------------------------------------------------------------ #
#                                                        Global variables                                                                #
# ------------------------------------------------------------------------------------------------------------------ #

# ------------------------------------------------------------------------------------------------------------------ #
#                                     Tasks on attachment of dependencies                                                   #
# ------------------------------------------------------------------------------------------------------------------ #
def onAttachmentConnected(source, target):

    localComponent = source["component"]
    remoteComponent = target["component"]
    remoteID = remoteComponent.getID()
    srcID = source["id"]
    targetID = target["id"]

    if (srcID == "saEngineUartDependency"):
        symBaudRate.setVisible(True)
        symDmaChannelTxEn.setVisible(True)
        symDmaChannelRxEn.setVisible(True)
        symDmaChannelTxEn.setReadOnly(False)
        symDmaChannelRxEn.setReadOnly(False)

        usartPeripheral = Database.getSymbolValue(remoteID, "SERCOM_INSTANCE_NAME")
        global dmaChannelIDTx
        dmaChannelIDTx = "DMA_CH_FOR_" + str(usartPeripheral) + "_Transmit"
        global dmaRequestIDTx
        dmaRequestIDTx = "DMA_CH_NEEDED_FOR_" + str(usartPeripheral) + "_Transmit"

        global dmaChannelIDRx
        dmaChannelIDRx = "DMA_CH_FOR_" + str(usartPeripheral) + "_Receive"
        global dmaRequestIDRx
        dmaRequestIDRx = "DMA_CH_NEEDED_FOR_" + str(usartPeripheral) + "_Receive"

        periph_name = Database.getSymbolValue(remoteID, "USART_PLIB_API_PREFIX")
        symTargetInterface.setValue(periph_name.upper())

        localComponent.getSymbolByID("SA_ENGINE_PERIPH_USED").setValue("UART")
        localComponent.getSymbolByID("SA_ENGINE_PERIPH_USED").clearValue()
        localComponent.getSymbolByID("SA_ENGINE_PERIPH_USED").setValue(periph_name)
        localComponent.getSymbolByID("SA_ENGINE_PERIPH_REGS").setValue(remoteID.upper() + "_REGS")
        Database.setSymbolValue(remoteID, "USART_INTERRUPT_MODE", False)

        Database.sendMessage(remoteID, "UART_RING_BUFFER_MODE", {"isEnabled":True, "isReadOnly":True})
        Database.setSymbolValue(remoteID, global_BAUD_RATE_SYMBOL, localComponent.getSymbolByID("SA_ENGINE_BAUD_RATE").getValue())

        # Enable DMA channels by default
        symDmaChannelTxEn.setValue(True)
        symDmaChannelRxEn.setValue(True)
    elif (srcID == "saEngineTimerDependency"):
        global timerID
        timerID = remoteID

        symTimerUsed.setValue(targetID.upper())


        # Configure timer in 32-bit counter mode
        if "rtc" != timerID:
            symTimerPeriod.setVisible(True)

            Database.setSymbolValue(remoteID, "TC_CTRLA_MODE", 2)

            # Work-around
            Database.setSymbolValue(remoteID, "TC_CTRLA_PRESCALER", 7)
            symTimerPeriod.setValue(21600000)
        else:
            symTimerPeriod.setVisible(False)



        # Update the timer instance used
        localComponent.getSymbolByID("SA_ENGINE_TIMER_USED").setValue(remoteID.upper())

    elif (connectID == "sysTick_dependency"):
        localComponent.getSymbolByID("SYS_TIME_REMOTE_COMPONENT_ID").setValue(remoteID)
        plibUsed = localComponent.getSymbolByID("SYS_TIME_PLIB")
        plibUsed.clearValue()
        plibUsed.setValue(remoteID.upper())
        #Request PLIB to publish it's capabilities
        sysTimeDict = Database.sendMessage(remoteID, "SYS_TIME_PUBLISH_CAPABILITIES", sysTimeDict)

# ------------------------------------------------------------------------------------------------------------------ #
#                                     Tasks on de-attachment of dependencies                                             #
# ------------------------------------------------------------------------------------------------------------------ #
def onAttachmentDisconnected(source, target):

    localComponent = source["component"]
    remoteComponent = target["component"]
    remoteID = remoteComponent.getID()
    srcID = source["id"]
    targetID = target["id"]

    if (srcID == "saEngineUartDependency"):
        symBaudRate.setVisible(False)
        symTargetInterface.setValue("")

        symDmaChannelTxEn.setValue(False)
        symDmaChannelRxEn.setValue(False)
        symDmaChannelTxEn.setVisible(False)
        symDmaChannelRxEn.setVisible(False)
        symDmaChannelTxEn.setReadOnly(True)
        symDmaChannelRxEn.setReadOnly(True)

        localComponent.getSymbolByID("SA_ENGINE_PERIPH_USED").clearValue()
        Database.sendMessage(remoteID, "UART_RING_BUFFER_MODE", {"isReadOnly":False})

        Database.sendMessage("core", "DMA_CHANNEL_DISABLE", {"dma_channel":dmaRequestIDTx})
        Database.sendMessage("core", "DMA_CHANNEL_DISABLE", {"dma_channel":dmaRequestIDRx})


    elif (srcID == "saEngineTimerDependency"):
        symTimerUsed.setValue("")
        symTimerPeriod.setVisible(False)
        localComponent.getSymbolByID("SA_ENGINE_TIMER_USED").clearValue()


# ------------------------------------------------------------------------------------------------------------------ #
#                                       Handle message                                                                                 #
# ------------------------------------------------------------------------------------------------------------------ #
def handleMessage(messageID, args ):
    if messageID == "SYS_TIME_PLIB_CAPABILITY":
        sysTimePLIBConfig = dict()

        sysTickRate = {"sys_time_tick_ms" : 0.0 }
        sysTickRate["sys_time_tick_ms"] = float(sysTickRateMs.getValue())

        Database.sendMessage("core", "SYS_TIME_PLIB_MODE_PERIOD", {})
        Database.sendMessage("core", "SYS_TIME_TICK_RATE_CHANGED", sysTickRate)

        sysTickRateMs.setVisible(True)
        sysTimePLIBConfig["plib_mode"] = "SYS_TIME_PLIB_MODE_PERIOD"
        sysTimePLIBConfig["sys_time_tick_ms"] = float(sysTickRateMs.getValue())

        return sysTimePLIBConfig



# ------------------------------------------------------------------------------------------------------------------ #
#                                      Callback Functions                                                            #
# ------------------------------------------------------------------------------------------------------------------ #

def updateCommPort(symbol, event):
    Database.setSymbolValue(symbol.getValue(), global_BAUD_RATE_SYMBOL, event["value"])

def updateTimer(symbol, event):
    Database.sendMessage(timerID, "SYS_TIME_TICK_RATE_CHANGED", {"sys_time_tick_ms":symbol.getValue()})

def setVisibility(symbol, event):
    if( event["value"] == True ):
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)

def sysTickOperatingModeCallback(symbol, event):
    dummyDict = {}
    sysTickRate = {"sys_time_tick_ms" : 0.0}
    dummyDict = Database.sendMessage("core", "SYS_TIME_PLIB_MODE_PERIOD", dummyDict)
    sysTickRate["sys_time_tick_ms"] = float(sysTickRateMs.getValue())
    dummyDict = Database.sendMessage("core", "SYS_TIME_TICK_RATE_CHANGED", sysTickRate)

def sysTimeTickRateCallback(symbol, event):
    global sysTimeRemoteComponentId

    dummyDict = {}
    sysTickRate = {"sys_time_tick_ms" : 0.0}

    if (event["id"] == "SYS_TIME_TICK_RATE_MS"):
        sysTickRate["sys_time_tick_ms"] = float(symbol.getValue())
        dummyDict = Database.sendMessage("core", "SYS_TIME_TICK_RATE_CHANGED", sysTickRate)

def onSysTimeUseSystickChange(symbol, event):
    localComponent = symbol.getComponent()
    if event["id"] == "SYS_TIME_USE_SYSTICK":
        if event["value"] == True:
            #Request PLIB to publish it's capabilities
            Database.sendMessage("core", "SYS_TIME_PUBLISH_CAPABILITIES", {"ID": symComponentID.getValue()})

            # Disable dependency
            localComponent.setDependencyEnabled("saEngineInterruptDependency", False)

            # sysTickOperatingMode.setVisible(True)
            sysTickRateMs.setVisible(True)
        else:
            #Let sys_tick PLIB know that SYS Time no longer uses sys_tick PLIB
            Database.sendMessage("core", "SYS_TIME_PUBLISH_CAPABILITIES", {"ID":"None"})

            # Enable dependency
            localComponent.setDependencyEnabled("saEngineInterruptDependency", True)

            # sysTickOperatingMode.setVisible(False)
            sysTickRateMs.setVisible(False)

def saEngineConfigEnable( symbol, event):
    localComponent = symbol.getComponent()
    if(event["value"] == False):
        symTaskHandler.setVisible(False)
        symTimerHook.setVisible(False)
        symCommInterface.setVisible(False)

        # Disable dependency
        localComponent.setDependencyEnabled("saEngineUartDependency", False)
        localComponent.setDependencyEnabled("saEngineTimerDependency", False)

    else:
        symTaskHandler.setVisible(True)
        symTimerHook.setVisible(True)
        symCommInterface.setVisible(True)

        # Enable dependency
        localComponent.setDependencyEnabled("saEngineUartDependency", True)
        localComponent.setDependencyEnabled("saEngineTimerDependency", True)

# ------------------------------------------------------------------------------------------------------------------ #
#                                      Instantiate component                                                                         #
# ------------------------------------------------------------------------------------------------------------------ #
def instantiateComponent(component):
    Log.writeInfoMessage("Running X2C Model")
    configName = Variables.get("__CONFIGURATION_NAME")

    # License comment first sentence
    symLicenseComment = component.createCommentSymbol("SA_ENGINE_LICENSE_COMMENT_1", None)
    symLicenseComment.setLabel("This firmware generated from this component is for evaluation purpose")
    symLicenseComment.setVisible(True)

    # License comment first sentence
    symLicenseComment = component.createCommentSymbol("SA_ENGINE_LICENSE_COMMENT_2", None)
    symLicenseComment.setLabel("and is subject to Stream Analyze's Terms and Conditions")
    symLicenseComment.setVisible(True)

    # License comment first sentence
    symLicenseComment = component.createCommentSymbol("SA_ENGINE_LICENSE_COMMENT_3", None)
    symLicenseComment.setLabel("Read 'Stream_Analyze_Terms_of_Use.pdf' carefully.")
    symLicenseComment.setVisible(True)

    global symLicenseReadAcknowkledge
    symLicenseReadAcknowkledge = component.createBooleanSymbol("SA_ENGINE_LICENSE_READ_ACK", None)
    symLicenseReadAcknowkledge.setLabel("I have read the Stream Analyze's Terms of use")
    symLicenseReadAcknowkledge.setVisible(True)
    symLicenseReadAcknowkledge.setDependencies(saEngineConfigEnable, ["SA_ENGINE_LICENSE_READ_ACK"])


    # Set Heap size
    Database.sendMessage("core", "HEAP_SIZE", {"heap_size": 160000 })
    
    # Set stack size

    # Configuration Parameters
    global symComponentID
    symComponentID = component.createStringSymbol("SA_ENGINE_COMPONENT_ID", None)
    symComponentID.setLabel("Component ID")
    symComponentID.setDefaultValue(component.getID())
    symComponentID.setVisible(False)

    # Task Handling
    global symTaskHandler
    symTaskHandler = component.createMenuSymbol( None, None )
    symTaskHandler.setLabel("Model Timeout Source")
    symTaskHandler.setDescription("Model Timeout Source")

    # System timer for task handling
    global sysTimeSymbol
    sysTimeSymbol = component.createBooleanSymbol("SYS_TIME_USE_SYSTICK", symTaskHandler)
    sysTimeSymbol.setLabel("Use SysTick Timer")
    sysTimeSymbol.setDescription("Use SysTick Timer")
    sysTimeSymbol.setDefaultValue(False)
    sysTimeSymbol.setVisible(True)
    sysTimeSymbol.setReadOnly(True)
    sysTimeSymbol.setDependencies(onSysTimeUseSystickChange, ["SYS_TIME_USE_SYSTICK"])

    global sysTickRateMs
    sysTickRateMs = component.createFloatSymbol("SYS_TIME_TICK_RATE_MS", sysTimeSymbol)
    sysTickRateMs.setLabel("Tick Rate (ms)")
    sysTickRateMs.setMax(5000)          # 5 seconds
    sysTickRateMs.setMin(0.1)           # 100 usec
    sysTickRateMs.setDefaultValue(1)
    sysTickRateMs.setVisible(True)
    sysTickRateMs.setDependencies(sysTimeTickRateCallback, ["SYS_TIME_TICK_RATE_MS"])

    # Time Keeping
    global symTimerHook
    symTimerHook = component.createMenuSymbol("SA_ENGINE_TIMER_HOOK", None)
    symTimerHook.setLabel("Timer Source")
    symTimerHook.setDescription("Timer Source")

    global symTimerUsed
    symTimerUsed = component.createStringSymbol("SA_ENGINE_TIMER_USED", symTimerHook)
    symTimerUsed.setLabel("Peripheral used")
    symTimerUsed.setValue("")
    symTimerUsed.setVisible(True)

    global symTimerPeriod
    symTimerPeriod = component.createFloatSymbol("SA_ENGINE_TIMER_PERIOD", symTimerHook )
    symTimerPeriod.setLabel("Period in milliseconds")
    symTimerPeriod.setDescription("Period in milliseconds")
    symTimerPeriod.setVisible(False)
    symTimerPeriod.setDefaultValue(1)
    symTimerPeriod.setDependencies(updateTimer, ["SA_ENGINE_TIMER_PERIOD"])

    # Configuration Parameters
    global symCommInterface
    symCommInterface = component.createMenuSymbol("SA_ENGINE_COMM_INT", None)
    symCommInterface.setLabel("Communication Interface")

    symBufferSize = component.createIntegerSymbol("SA_ENGINE_BUFFER_SIZE", symCommInterface)
    symBufferSize.setLabel("Buffer Size")
    symBufferSize.setDescription("Buffer Size")
    symBufferSize.setMin(0)
    symBufferSize.setMax(10000)
    symBufferSize.setDefaultValue(800)

    global symTargetInterface
    symTargetInterface = component.createStringSymbol("SA_ENGINE_TARGET_INT", symCommInterface)
    symTargetInterface.setLabel("Target")
    symTargetInterface.setDefaultValue("")

    global symBaudRate
    symBaudRate = component.createIntegerSymbol("SA_ENGINE_BAUD_RATE", symTargetInterface)
    symBaudRate.setLabel("Baud Rate")
    symBaudRate.setDefaultValue(115200)
    symBaudRate.setVisible(False)
    symBaudRate.setDependencies(updateCommPort, ["SA_ENGINE_BAUD_RATE"])

    # DMA Channel assignment for transmit
    global symDmaChannelTxEn
    symDmaChannelTxEn = component.createBooleanSymbol("SA_ENGINE_DMA_CHANNEL_TX_EN", symTargetInterface)
    symDmaChannelTxEn.setLabel("Use DMA Channel for Transmit")
    symDmaChannelTxEn.setReadOnly(True)
    symDmaChannelTxEn.setVisible(False)

    global symDmaChannelTx
    symDmaChannelTx = component.createIntegerSymbol("SA_ENGINE_DMA_CHANNEL_TX", symDmaChannelTxEn)
    symDmaChannelTx.setLabel("Transmit Channel")
    symDmaChannelTx.setReadOnly(True)
    symDmaChannelTx.setDependencies(updateTransmitDmaChannel, ["SA_ENGINE_DMA_CHANNEL_TX_EN"])
    symDmaChannelTx.setVisible(symDmaChannelTxEn.getValue())

    # DMA Channel assignment for receive
    global symDmaChannelRxEn
    symDmaChannelRxEn = component.createBooleanSymbol("SA_ENGINE_DMA_CHANNEL_RX_EN", symTargetInterface)
    symDmaChannelRxEn.setLabel("Use DMA Channel for Receive")
    symDmaChannelRxEn.setReadOnly(True)
    symDmaChannelRxEn.setVisible(False)

    global symDmaChannelRx
    symDmaChannelRx = component.createIntegerSymbol("SA_ENGINE_DMA_CHANNEL_RX", symDmaChannelRxEn)
    symDmaChannelRx.setLabel("Receive Channel")
    symDmaChannelRx.setReadOnly(True)
    symDmaChannelRx.setDependencies(updateReceiveDmaChannel, ["SA_ENGINE_DMA_CHANNEL_RX_EN"])
    symDmaChannelRx.setVisible(symDmaChannelRxEn.getValue())

    symSerialPortUsed = component.createStringSymbol("SA_ENGINE_PERIPH_USED", symTargetInterface)
    symSerialPortUsed.setValue(symTargetInterface.getValue())
    symSerialPortUsed.setVisible(False)

    symSerialPortRegs = component.createStringSymbol("SA_ENGINE_PERIPH_REGS", symTargetInterface)
    symSerialPortRegs.setValue("Not selected")
    symSerialPortRegs.setVisible(False)

    symHostInterface =  component.createComboSymbol("SA_ENGINE_HOST_PORT", symCommInterface, ["USB Serial Device"])
    symHostInterface.setLabel("Host")
    symHostInterface.setDescription("Host")

    symHostPortNumber =  component.createIntegerSymbol("SA_ENGINE_HOST_PORT_NUMBER", symHostInterface )
    symHostPortNumber.setLabel("Port Number")
    symHostPortNumber.setDescription("Port Number")

    # Include directory
    configName = Variables.get("__CONFIGURATION_NAME")
    includeDirectory = component.createSettingSymbol(None, None)
    includeDirectory.setCategory("C32")
    includeDirectory.setKey("extra-include-directories")
    includeDirectory.setValue("../src/config/" + configName + "/sa.engine/inc")
    includeDirectory.setAppend(True, ";")

    # SA Studio connection scripts
    for root, dirs, files in os.walk( Module.getPath()+"config/templates/scripts"):
        for filename in files:
            saScriptFiles = component.createFileSymbol(str(filename), None)
            saScriptFiles.setSourcePath("config/templates/scripts/" + filename)
            if (filename.endswith(".ftl")):
                filename = filename[:-4]
            saScriptFiles.setOutputName(filename)
            saScriptFiles.setDestPath("../../")
            saScriptFiles.setProjectPath("")
            saScriptFiles.setType("IMPORTANT")
            saScriptFiles.setMarkup(True)

    # Code Generation
    for root, dirs, files in os.walk( Module.getPath()+"config/templates/src"):
        for filename in files:
            if (".c" in filename):
                saEngineSourceFile = component.createFileSymbol(str(filename), None)
                saEngineSourceFile.setSourcePath("config/templates/src/" + filename)
                if (filename.endswith(".ftl")):
                    filename = filename[:-4]
                saEngineSourceFile.setOutputName(filename)
                saEngineSourceFile.setDestPath("/sa.engine/src/")
                saEngineSourceFile.setProjectPath("config/" + configName + "/sa.engine/src/")
                saEngineSourceFile.setType("SOURCE")
                saEngineSourceFile.setMarkup(True)

    for root, dirs, files in os.walk( Module.getPath()+"config/templates/inc"):
        for filename in files:
            if (".h" in filename):
                saEngineHeaderFile = component.createFileSymbol(str(filename), None)
                saEngineHeaderFile.setSourcePath("config/templates/inc/" + filename)
                if (filename.endswith(".ftl")):
                    filename = filename[:-4]
                saEngineHeaderFile.setOutputName(filename)
                saEngineHeaderFile.setDestPath("/sa.engine/inc/")
                saEngineHeaderFile.setProjectPath("config/" + configName+ "/sa.engine/inc/")
                saEngineHeaderFile.setType("HEADER")
                saEngineHeaderFile.setMarkup(True)

    if( ("SAMD5" in Variables.get("__PROCESSOR")) or ("SAME5" in Variables.get("__PROCESSOR"))):
        global global_BAUD_RATE_SYMBOL
        global_BAUD_RATE_SYMBOL = "BAUD_RATE"

        saEngineLibraryFile = component.createLibrarySymbol("SA_ENGINE_LIB_SAME54_A", None)
        saEngineLibraryFile.setSourcePath("config/templates/lib/libCortexM4-ATSAM_SAEngine.a")
        saEngineLibraryFile.setOutputName("libCortexM4-ATSAM_SAEngine.a")
        saEngineLibraryFile.setDestPath("/sa.engine/lib/")

    # Generate Initialization File
    initializeFileSource = component.createFileSymbol("SA_ENGINE_INITIALIZATION_C", None)
    initializeFileSource.setType("STRING")
    initializeFileSource.setOutputName("core.LIST_SYSTEM_INIT_C_INITIALIZE_MIDDLEWARE")
    initializeFileSource.setSourcePath("config/templates/system/initialization.c.ftl")
    initializeFileSource.setMarkup(True)

    definationFileSource = component.createFileSymbol("SA_ENGINE_SYS_DEF_HEADER", None)
    definationFileSource.setType("STRING")
    definationFileSource.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    definationFileSource.setSourcePath("config/templates/system/definitions.h.ftl")
    definationFileSource.setMarkup(True)

    # Pre processor symbols
    saEngineLinux = component.createSettingSymbol("SA_ENGINE_DEF_LINUX", None)
    saEngineLinux.setCategory("C32")
    saEngineLinux.setKey("preprocessor-macros")
    saEngineLinux.setValue("LINUX")
    saEngineLinux.setAppend(True, ";")

    saEngineMicrocore = component.createSettingSymbol("SA_ENGINE_DEF_MICROCORE", None)
    saEngineMicrocore.setCategory("C32")
    saEngineMicrocore.setKey("preprocessor-macros")
    saEngineMicrocore.setValue("MICROCORE")
    saEngineMicrocore.setAppend(True, ";")

    saEngineDebug = component.createSettingSymbol("SA_ENGINE_DEF_DEBUG", None)
    saEngineDebug.setCategory("C32")
    saEngineDebug.setKey("preprocessor-macros")
    saEngineDebug.setValue("DEBUG")
    saEngineDebug.setAppend(True, ";")

    saEngineArmM4 = component.createSettingSymbol("SA_ENGINE_DEF_ARMM4", None)
    saEngineArmM4.setCategory("C32")
    saEngineArmM4.setKey("preprocessor-macros")
    saEngineArmM4.setValue("ARMM4")
    saEngineArmM4.setAppend(True, ";")

    saEngineSame54 = component.createSettingSymbol("SA_ENGINE_DEF_SAME54", None)
    saEngineSame54.setCategory("C32")
    saEngineSame54.setKey("preprocessor-macros")
    saEngineSame54.setValue("SAME54")
    saEngineSame54.setAppend(True, ";")

    saEngineStatic = component.createSettingSymbol("SA_ENGINE_DEF_STATIC", None)
    saEngineStatic.setCategory("C32")
    saEngineStatic.setKey("preprocessor-macros")
    saEngineStatic.setValue("STATIC")
    saEngineStatic.setAppend(True, ";")

    saEngineUnix = component.createSettingSymbol("SA_ENGINE_DEF_UNIX", None)
    saEngineUnix.setCategory("C32")
    saEngineUnix.setKey("preprocessor-macros")
    saEngineUnix.setValue("UNIX")
    saEngineUnix.setAppend(True, ";")

    saEngineMicrokernel = component.createSettingSymbol("SA_ENGINE_DEF_MICROKERNEL", None)
    saEngineMicrokernel.setCategory("C32")
    saEngineMicrokernel.setKey("preprocessor-macros")
    saEngineMicrokernel.setValue("MICROKERNEL")
    saEngineMicrokernel.setAppend(True, ";")

    saEngineMicrokernelNoFS = component.createSettingSymbol("SA_ENGINE_DEF_MICROCORE_NO_FS", None)
    saEngineMicrokernelNoFS.setCategory("C32")
    saEngineMicrokernelNoFS.setKey("preprocessor-macros")
    saEngineMicrokernelNoFS.setValue("MICROCORE_NO_FS")
    saEngineMicrokernelNoFS.setAppend(True, ";")

    saEngineSplitImage = component.createSettingSymbol("SA_ENGINE_DEF_SPLIT_IMAGE", None)
    saEngineSplitImage.setCategory("C32")
    saEngineSplitImage.setKey("preprocessor-macros")
    saEngineSplitImage.setValue("SPLIT_IMAGE")
    saEngineSplitImage.setAppend(True, ";")

    saEngineSimd = component.createSettingSymbol("SA_ENGINE_DEF_SIMD_FP", None)
    saEngineSimd.setCategory("C32")
    saEngineSimd.setKey("preprocessor-macros")
    saEngineSimd.setValue("SIMD_FP")
    saEngineSimd.setAppend(True, ";")

#-------------------------------------------------------------------------------------------------------------------------------------#
#                                                              Finalize component                                                     #
#-------------------------------------------------------------------------------------------------------------------------------------#
def finalizeComponent(component):
    # Enable SysTick timer use
    sysTimeSymbol.setValue(True)

    # Enable DMA use
    # symDmaChannelTxEn.setValue(True)
    # symDmaChannelRxEn.setValue(True)

    # Set acknowledgement done as false
    symLicenseReadAcknowkledge.setValue(False)

def updateTransmitDmaChannel(symbol, event):
    # Clear the DMA symbol. Done for backward compatibility.
    Database.clearSymbolValue("core", dmaRequestIDTx)

    dummyDict = {}

    if event["value"] == False:
        dummyDict = Database.sendMessage("core", "DMA_CHANNEL_DISABLE", {"dma_channel":dmaRequestIDTx})
        # usartTXDMAChannelComment.setVisible(False)
        symbol.setVisible(False)
    else:
        symbol.setVisible(True)
        dummyDict = Database.sendMessage("core", "DMA_CHANNEL_ENABLE", {"dma_channel":dmaRequestIDTx})

    # Get the allocated channel and assign it
    channel = Database.getSymbolValue("core", dmaChannelIDTx)
    symbol.setValue(channel)

def updateReceiveDmaChannel(symbol, event):
    # Clear the DMA symbol. Done for backward compatibility.
    Database.clearSymbolValue("core", dmaRequestIDRx)

    dummyDict = {}

    if event["value"] == False:
        dummyDict = Database.sendMessage("core", "DMA_CHANNEL_DISABLE", {"dma_channel":dmaRequestIDRx})
        # usartTXDMAChannelComment.setVisible(False)
        symbol.setVisible(False)
    else:
        symbol.setVisible(True)
        dummyDict = Database.sendMessage("core", "DMA_CHANNEL_ENABLE", {"dma_channel":dmaRequestIDRx})

    # Get the allocated channel and assign it
    channel = Database.getSymbolValue("core", dmaChannelIDRx)
    symbol.setValue(channel)