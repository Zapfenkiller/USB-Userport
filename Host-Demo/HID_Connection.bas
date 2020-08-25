Attribute VB_Name = "HID_Connection"
' * The USB-Userport *
' Copyright 2020  René Trapp (rene [dot] trapp (-at-) web [dot] de)
'
' Permission to use, copy, modify, distribute, and sell this
' software and its documentation for any purpose is hereby granted
' without fee, provided that the above copyright notice appear in
' all copies and that both that the copyright notice and this
' permission notice and warranty disclaimer appear in supporting
' documentation, and that the name of the author not be used in
' advertising or publicity pertaining to distribution of the
' software without specific, written prior permission.
'
' The author disclaims all warranties with regard to this
' software, including all implied warranties of merchantability
' and fitness.  In no event shall the author be liable for any
' special, indirect or consequential damages or any damages
' whatsoever resulting from loss of use, data or profits, whether
' in an action of contract, negligence or other tortious action,
' arising out of or in connection with the use or performance of
' this software.


'-----------------------------------------------------------------

' Definitions to connect to the generic HID API.
' https://docs.microsoft.com/en-us/windows-hardware/drivers/hid/
' https://docs.microsoft.com/en-us/windows/win32/devinst/setupapi-h
' (https://github.com/tpn/winsdk-10/blob/master/Include/10.0.10240.0/um/SetupAPI.h)
' http://janaxelson.com/files/usbhidio2.zip

Private Const DIGCF_PRESENT = &H2
Private Const DIGCF_DEVICEINTERFACE = &H10
Private Const FILE_FLAG_OVERLAPPED = &H40000000
Private Const FILE_SHARE_READ = &H1
Private Const FILE_SHARE_WRITE = &H2
Private Const GENERIC_READ = &H80000000
Private Const GENERIC_WRITE = &H40000000
Private Const OPEN_EXISTING = 3

Private Type GUID
    Data1 As Long
    Data2 As Integer
    Data3 As Integer
    Data4(7) As Byte
End Type

Private Type HIDD_ATTRIBUTES
    Size As Long
    VendorID As Integer
    ProductID As Integer
    VersionNumber As Integer
End Type

Private Type SECURITY_ATTRIBUTES
    nLength As Long
    lpSecurityDescriptor As Long
    bInheritHandle As Long
End Type

Private Type SP_DEVICE_INTERFACE_DATA
   cbSize As Long
   InterfaceClassGuid As GUID
   Flags As Long
   Reserved As Long
End Type

Private Type SP_DEVICE_INTERFACE_DETAIL_DATA
    cbSize As Long
    DevicePath As Byte
End Type

Private Type SP_DEVINFO_DATA
    cbSize As Long
    ClassGuid As GUID
    DevInst As Long
    Reserved As Long
End Type


' Get the GUID for all system HIDs.
' Returns the GUID in HidGuid. No function return, but necessary
' calling the API from VBA.
' https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/hidsdi/nf-hidsdi-hidd_gethidguid
Private Declare Function HidD_GetHidGuid _
    Lib "hid.dll" ( _
        ByRef HidGuid As GUID _
    ) _
As Long


' Get a list of all devices for a certain class (classGUID)
' Returns a handle to a device information set.
' https://docs.microsoft.com/en-us/windows/desktop/api/setupapi/nf-setupapi-setupdigetclassdevsw
Private Declare Function SetupDiGetClassDevs _
    Lib "setupapi.dll" _
    Alias "SetupDiGetClassDevsA" ( _
        ByRef ClassGuid As GUID, _
        ByVal Enumerator As String, _
        ByVal hwndParent As Long, _
        ByVal Flags As Long _
    ) _
As Long


' Get information about the device interface(s).
' Returns success status (true / false).
' Delivers a SP_DEVICE_INTERFACE_DATA structure.
' https://docs.microsoft.com/en-us/windows/win32/api/setupapi/nf-setupapi-setupdienumdeviceinterfaces
Private Declare Function SetupDiEnumDeviceInterfaces _
    Lib "setupapi.dll" ( _
        ByVal DeviceInfoSet As Long, _
        ByVal DeviceInfoData As Long, _
        ByRef InterfaceClassGuid As GUID, _
        ByVal MemberIndex As Long, _
        ByRef DeviceInterfaceData As SP_DEVICE_INTERFACE_DATA _
    ) _
As Long


' Get detail information about the device interface(s).
' Returns success status (true / false).
' Delivers a SP_DEVICE_INTERFACE_DETAIL_DATA structure
' https://docs.microsoft.com/en-us/windows/win32/api/setupapi/nf-setupapi-setupdigetdeviceinterfacedetaila
Private Declare Function SetupDiGetDeviceInterfaceDetail _
    Lib "setupapi.dll" _
    Alias "SetupDiGetDeviceInterfaceDetailA" ( _
        ByVal DeviceInfoSet As Long, _
        ByRef DeviceInterfaceData As SP_DEVICE_INTERFACE_DATA, _
        ByVal DeviceInterfaceDetailData As Long, _
        ByVal DeviceInterfaceDetailDataSize As Long, _
        ByRef RequiredSize As Long, _
        ByVal DeviceInfoData As Long _
    ) _
As Long


' Copy bytes in memory
' https://docs.microsoft.com/en-us/windows/win32/devnotes/rtlmovememory
Private Declare Function RtlMoveMemory _
    Lib "kernel32" ( _
        Destination As Any, _
        Source As Any, _
        ByVal Count As Long _
    ) _
As Long


' Open a file - connects also to a HID device
' https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea
Private Declare Function CreateFile _
    Lib "kernel32" _
    Alias "CreateFileA" ( _
        ByVal lpFileName As String, _
        ByVal dwDesiredAccess As Long, _
        ByVal dwShareMode As Long, _
        ByRef lpSecurityAttributes As SECURITY_ATTRIBUTES, _
        ByVal dwCreationDisposition As Long, _
        ByVal dwFlagsAndAttributes As Long, _
        ByVal hTemplateFile As Long _
    ) _
As Long


' Close an already opened handle
' https://docs.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-closehandle
Private Declare Function CloseHandle _
    Lib "kernel32" ( _
        ByVal hObject As Long _
    ) _
As Long


' Get the HID device attributes
' https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/hidsdi/nf-hidsdi-hidd_getattributes
Private Declare Function HidD_GetAttributes _
    Lib "hid.dll" _
    (ByVal HidDeviceObject As Long, _
    ByRef Attributes As HIDD_ATTRIBUTES) _
As Long


' Get the serial number string
' https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/hidsdi/nf-hidsdi-hidd_getserialnumberstring
Private Declare Function HidD_GetSerialNumberString _
    Lib "hid.dll" _
    ( _
        ByVal HidDeviceObject As Long, _
        ByRef Buffer As Byte, _
        ByVal BufferLength As Long _
    ) _
As Boolean


' Send an Output Report (host to device data)
' https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/hidsdi/nf-hidsdi-hidd_setoutputreport
Private Declare Function HidD_SetOutputReport _
    Lib "hid.dll" _
    ( _
        ByVal HidDeviceObject As Long, _
        ByRef reportbuffer As Any, _
        ByVal ReportBufferLength As Long _
    ) _
As Boolean


' Get an Input Report (device to host data)
' https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/hidsdi/nf-hidsdi-hidd_getinputreport
Private Declare Function HidD_GetInputReport _
    Lib "hid.dll" _
    ( _
        ByVal HidDeviceObject As Long, _
        ByRef reportbuffer As Any, _
        ByVal ReportBufferLength As Long _
    ) _
As Boolean


'-----------------------------------------------------------------

' Definitions dedicated to the USB-Userport

' LEDs
Public Const LED0_CTRL_POS = 1
Public Const LED1_CTRL_POS = 2

Private Const REPORT_ID_DEVICE_LEDS = 1
Private Type LedReport_t
    rID As Byte
    data As Byte
End Type

' GPIOs
Private Const REPORT_ID_GPIO1_PORT = 2
Private Const REPORT_ID_GPIO1_DIR = 3
Private Const REPORT_ID_GPIO2_PORT = 4
Private Const REPORT_ID_GPIO2_DIR = 5

Private Type GpioSetReport_t
    rID As Byte
    statesL As Byte
    statesU As Byte
    maskL As Byte
    maskU As Byte
End Type

Private Type GpioGetReport_t
    rID As Byte
    statesL As Byte
    statesU As Byte
End Type

' ADC
Public Const AREF_2p56 = 2
Public Const AREF_AVCC = 0
Public Const ADC_EN = 128

Private Const REPORT_ID_ADC_SET = 16
Private Const REPORT_ID_ADC4_GET = 20
Private Const REPORT_ID_ADC5_GET = 21
Private Const REPORT_ID_ADC6_GET = 22
Private Const REPORT_ID_ADC7_GET = 23

Private Type AdcSetReport_t
    rID As Byte
    Didr As Byte
    Aref As Byte
End Type

Private Type AdcGetReport_t
    rID As Byte
    valL As Byte
    valH As Byte
End Type

' Servos
Private Const REPORT_ID_SERVO = 32

Private Type ServoReport_t
    rID As Byte
    Servo1 As Byte
    Servo2 As Byte
End Type

Public Type positions_t
    Servo(2) As Byte
End Type


'-----------------------------------------------------------------

' Control logic

Private GoldenHandle As Long
Private HandleIsValid As Boolean


' Try to connect to a given HID device.
' Returns success status ('true' if found).
' https://docs.microsoft.com/en-us/windows-hardware/drivers/hid/finding-and-opening-a-hid-collection
' http://janaxelson.com/files/usbhidio2.zip
Public Function Find_and_Open_Device(vid, pid, snr) As Boolean
    Dim Handle As Long
    Dim HidGuid As GUID
    Dim DeviceInfoSet As Long
    Dim DeviceInterfaceData As SP_DEVICE_INTERFACE_DATA
    Dim DeviceInfoData As SP_DEVINFO_DATA
    Dim more As Boolean
    Dim i As Long
    Dim RequiredSize As Long
    Dim DeviceInterfaceDetailData As SP_DEVICE_INTERFACE_DETAIL_DATA
    Dim DeviceInterfaceDetailDataExtended() As Byte
    Dim DevicePath As String
    Dim lpSecurityAttributes As SECURITY_ATTRIBUTES
    Dim Attributes As HIDD_ATTRIBUTES
    Dim SerialNum As String
    Dim SerialNumRaw(260) As Byte


    GoldenHandle = -1
    HandleIsValid = False
    Call HidD_GetHidGuid(HidGuid)
    DeviceInfoSet = SetupDiGetClassDevs( _
                        HidGuid, _
                        vbNullString, _
                        0, _
                        (DIGCF_PRESENT Or DIGCF_DEVICEINTERFACE) _
                    )
    more = True
    i = 0
    Do
        DeviceInterfaceData.cbSize = LenB(DeviceInterfaceData)
        more = SetupDiEnumDeviceInterfaces( _
                    DeviceInfoSet, _
                    0, _
                    HidGuid, _
                    i, _
                    DeviceInterfaceData _
               )
        If more Then
            DeviceInfoData.cbSize = Len(DeviceInfoData)
            Call SetupDiGetDeviceInterfaceDetail( _
                    DeviceInfoSet, _
                    DeviceInterfaceData, _
                    0, _
                    0, _
                    RequiredSize, _
                    0 _
                 )
            ReDim DeviceInterfaceDetailDataExtended(RequiredSize)
            DeviceInterfaceDetailData.cbSize = Len(DeviceInterfaceDetailData)
            Call RtlMoveMemory( _
                    DeviceInterfaceDetailDataExtended(0), _
                    DeviceInterfaceDetailData, _
                    4 _
                 )
            Call SetupDiGetDeviceInterfaceDetail( _
                    DeviceInfoSet, _
                    DeviceInterfaceData, _
                    VarPtr(DeviceInterfaceDetailDataExtended(0)), _
                    RequiredSize, _
                    RequiredSize, _
                    0 _
                 )
            DevicePath = CStr(DeviceInterfaceDetailDataExtended())
            DevicePath = StrConv(DevicePath, vbUnicode)
            DevicePath = Right$(DevicePath, Len(DevicePath) - 4)
            lpSecurityAttributes.lpSecurityDescriptor = 0
            lpSecurityAttributes.bInheritHandle = True
            lpSecurityAttributes.nLength = Len(lpSecurityAttributes)
            Handle = CreateFile( _
                        DevicePath, _
                        GENERIC_READ Or GENERIC_WRITE, _
                        (FILE_SHARE_READ Or FILE_SHARE_WRITE), _
                        lpSecurityAttributes, _
                        OPEN_EXISTING, _
                        0&, _
                        0 _
                     )
            If Handle > -1 Then
                Attributes.Size = LenB(Attributes)
                If HidD_GetAttributes( _
                      Handle, _
                      Attributes _
                   ) _
                Then
                    If (Attributes.VendorID = vid) _
                    And (Attributes.ProductID = pid) Then
                        If (HidD_GetSerialNumberString(Handle, SerialNumRaw(0), 126)) Then
                            SerialNum = SerialNumRaw
                            If (CInt(SerialNum) = CInt(snr)) Then
                                GoldenHandle = Handle
                                HandleIsValid = True
                                more = False
                             End If
                       End If
                    Else
                        Call CloseHandle(Handle)
                    End If
                End If
            End If
        End If
        i = i + 1
    Loop While (more)
    Find_and_Open_Device = HandleIsValid
End Function    ' Find_and_Open_Device(...)


' Release device connection, closes handle(s).
' Returns success status ('true' disconnected)
Public Function Release_Device() As Boolean
    GoldenHandle = -1
    HandleIsValid = False
    Release_Device = CloseHandle(GoldenHandle)
End Function    ' Release_Device()


' Returns connectivity status ('true' if connected).
Public Function Is_Connected() As Boolean
    Is_Connected = HandleIsValid
End Function    ' Is_Connected()


' Send report to control device LEDs.
Public Function LEDs_Set(pattern As Byte)
    Dim report As LedReport_t
    
    report.rID = REPORT_ID_DEVICE_LEDS
    report.data = pattern
    Call HidD_SetOutputReport(GoldenHandle, report, Len(report))
End Function    ' LEDs_Set(...)


' Read report to get LED status.
' Returns LED pattern.
Public Function LEDs_Get() As Byte
    Dim report As LedReport_t
    
    report.rID = REPORT_ID_DEVICE_LEDS
    Call HidD_GetInputReport(GoldenHandle, report, Len(report))
    LEDs_Get = report.data
End Function


' Send report to control GPIO1 port lines.
Public Function GPIO1_Ports_Set(states As Long, mask As Long)
    Dim report As GpioSetReport_t

    report.rID = REPORT_ID_GPIO1_PORT
    report.statesL = states And 255
    report.statesU = states \ 256
    report.maskL = mask And 255
    report.maskU = mask \ 256
    Call HidD_SetOutputReport(GoldenHandle, report, Len(report))
End Function


Public Function GPIO1_Ports_Get() As Long
    Dim report As GpioGetReport_t
    Dim result As Long

    report.rID = REPORT_ID_GPIO1_PORT
    Call HidD_GetInputReport(GoldenHandle, report, Len(report))
    result = report.statesU
    result = 256 * result
    result = result + report.statesL
    GPIO1_Ports_Get = result
End Function


' Send report to control GPIO1 port directions.
Public Function GPIO1_Dirs_Set(states As Long, mask As Long)
    Dim report As GpioSetReport_t

    report.rID = REPORT_ID_GPIO1_DIR
    report.statesL = states And 255
    report.statesU = states \ 256
    report.maskL = mask And 255
    report.maskU = mask \ 256
    Call HidD_SetOutputReport(GoldenHandle, report, Len(report))
End Function


Public Function GPIO1_Dirs_Get() As Long
    Dim report As GpioGetReport_t
    Dim result As Long

    report.rID = REPORT_ID_GPIO1_DIR
    Call HidD_GetInputReport(GoldenHandle, report, Len(report))
    result = report.statesU
    result = 256 * result
    result = result + report.statesL
    GPIO1_Dirs_Get = result
End Function


' Send report to control GPIO2 port lines.
Public Function GPIO2_Ports_Set(states As Long, mask As Long)
    Dim report As GpioSetReport_t

    report.rID = REPORT_ID_GPIO2_PORT
    report.statesL = states And 255
    report.statesU = states \ 256
    report.maskL = mask And 255
    report.maskU = mask \ 256
    Call HidD_SetOutputReport(GoldenHandle, report, Len(report))
End Function


Public Function GPIO2_Ports_Get() As Long
    Dim report As GpioGetReport_t
    Dim result As Long

    report.rID = REPORT_ID_GPIO2_PORT
    Call HidD_GetInputReport(GoldenHandle, report, Len(report))
    result = report.statesU
    result = 256 * result
    result = result + report.statesL
    GPIO2_Ports_Get = result
End Function


' Send report to control GPIO2 port directions.
Public Function GPIO2_Dirs_Set(states As Long, mask As Long)
    Dim report As GpioSetReport_t

    report.rID = REPORT_ID_GPIO2_DIR
    report.statesL = states And 255
    report.statesU = states \ 256
    report.maskL = mask And 255
    report.maskU = mask \ 256
    Call HidD_SetOutputReport(GoldenHandle, report, Len(report))
End Function


Public Function GPIO2_Dirs_Get() As Long
    Dim report As GpioGetReport_t
    Dim result As Long

    report.rID = REPORT_ID_GPIO2_DIR
    Call HidD_GetInputReport(GoldenHandle, report, Len(report))
    result = report.statesU
    result = 256 * result
    result = result + report.statesL
    GPIO2_Dirs_Get = result
End Function


'Handle ADC.
Public Function ADC_Set(Aref As Byte, Didr As Byte, AdcEn As Byte)
    Dim report As AdcSetReport_t

    report.rID = REPORT_ID_ADC_SET
    report.Aref = Aref Or AdcEn Or 1
    report.Didr = Didr
    Call HidD_SetOutputReport(GoldenHandle, report, Len(report))
End Function


Public Function ADC_Get(Chan) As Long
    Dim report As AdcGetReport_t
    Dim result As Long
    
    If (Chan >= 4) And (Chan <= 7) Then
        report.rID = _
            Switch( _
                Chan = 4, REPORT_ID_ADC4_GET, _
                Chan = 5, REPORT_ID_ADC5_GET, _
                Chan = 6, REPORT_ID_ADC6_GET, _
                Chan = 7, REPORT_ID_ADC7_GET)
        Call HidD_GetInputReport(GoldenHandle, report, Len(report))
        result = report.valH
        result = 256 * result
        result = result + report.valL
    Else
        result = 0
    End If
    ADC_Get = result
End Function


' Servo PWM control
Public Function Servo_Set(pos1 As Byte, pos2 As Byte)
    Dim report As ServoReport_t

    report.rID = REPORT_ID_SERVO
    report.Servo1 = pos1
    report.Servo2 = pos2
    Call HidD_SetOutputReport(GoldenHandle, report, Len(report))
End Function


Public Function Servo_Get() As positions_t
    Dim report As ServoReport_t
    
    report.rID = REPORT_ID_SERVO
    Call HidD_GetInputReport(GoldenHandle, report, Len(report))
    Servo_Get.data(1) = report.Servo1
    Servo_Get.data(2) = report.Servo2
End Function
