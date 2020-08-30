Attribute VB_Name = "Demo_App"
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
'


Function Message_Unconnected()
    dummy = MsgBox("Please connect to the device first.", , "Unconnected!")
End Function


Sub Connect_to_Device()
    If Find_and_Open_Device(&H4D8, &HEBC7, Cells(2, 3)) Then
        With Cells(3, 3)
            .Value = "Connected"
            .Font.Color = RGB(0, 192, 0)
            .Font.TintAndShade = 0
        End With
        ActiveSheet.Buttons("BTN_Connect").Visible = False
        ActiveSheet.Buttons("BTN_Disconnect").Visible = True
    Else
        myMessage = "The desired USB-Userport with address " & Cells(2, 3) & " is not available."
        dummy = MsgBox(myMessage, , "Sorry!")
    End If
End Sub


Sub Disconnect_from_Device()
    Call Release_Device
    With Cells(3, 3)
        .Value = "Disconnected"
        .Font.Color = RGB(255, 0, 0)
        .Font.TintAndShade = 0
    End With
    ActiveSheet.Buttons("BTN_Connect").Visible = True
    ActiveSheet.Buttons("BTN_Disconnect").Visible = False
End Sub


Sub Run_LED_CTRL_Dialog()
    Dim pattern As Byte
    
    If Is_Connected Then
        pattern = LEDs_Get()
        If (pattern And LED0_CTRL_POS) = 0 Then
            Sheets("LEDs").CheckBoxes("LED_0").Value = xlOff
        Else
            Sheets("LEDs").CheckBoxes("LED_0").Value = xlOn
        End If
        If (pattern And LED1_CTRL_POS) = 0 Then
            Sheets("LEDs").CheckBoxes("LED_1").Value = xlOff
        Else
            Sheets("LEDs").CheckBoxes("LED_1").Value = xlOn
        End If
        Sheets("LEDs").Show
    Else
        Message_Unconnected
    End If
End Sub


Sub LEDs_Done_Click()
' "Done"
End Sub


Sub LEDs_LED1_Click()
    Dim pattern As Byte
    
    pattern = LEDs_Get()
    If (Sheets("LEDs").CheckBoxes("LED_1").Value) = xlOn Then
        ' LED[1] = An
        pattern = pattern Or LED1_CTRL_POS
    Else
        ' LED[1] = Aus
        pattern = pattern And (Not LED1_CTRL_POS)
    End If
    Call LEDs_Set(pattern)
End Sub


Sub LEDs_LED0_Click()
    Dim pattern As Byte
    
    pattern = LEDs_Get()
    If (Sheets("LEDs").CheckBoxes("LED_0").Value) = xlOn Then
        ' LED[0] = An
        pattern = pattern Or LED0_CTRL_POS
    Else
        ' LED[0] = Aus
        pattern = pattern And (Not LED0_CTRL_POS)
    End If
    Call LEDs_Set(pattern)
End Sub


Sub Run_GPIO_CTRL_Dialog()
    If Is_Connected Then
'        Dim GPIO1_PORT As Long
        Dim GPIO1_DDR As Long
        Dim GPIO2_DDR As Long
        Dim GPIO1_PIN As Long
        Dim GPIO2_PIN As Long
        
        GPIO1_DDR = GPIO1_Dirs_Get
        GPIO1_PIN = GPIO1_Ports_Get
        GPIO2_DDR = GPIO2_Dirs_Get
        GPIO2_PIN = GPIO2_Ports_Get

        For i = 0 To 15
            If (GPIO1_DDR And 2 ^ i) = 0 Then
                Sheets("GPIOs").CheckBoxes("GPIO1_" & i & "_DDR").Value = xlOff
            Else
                Sheets("GPIOs").CheckBoxes("GPIO1_" & i & "_DDR").Value = xlOn
            End If
            If (GPIO1_PIN And 2 ^ i) = 0 Then
                Sheets("GPIOs").CheckBoxes("GPIO1_" & i & "_PIN").Value = xlOff
            Else
                Sheets("GPIOs").CheckBoxes("GPIO1_" & i & "_PIN").Value = xlOn
            End If
            If (GPIO2_DDR And 2 ^ i) = 0 Then
                Sheets("GPIOs").CheckBoxes("GPIO2_" & i & "_DDR").Value = xlOff
            Else
                Sheets("GPIOs").CheckBoxes("GPIO2_" & i & "_DDR").Value = xlOn
            End If
            If (GPIO2_PIN And 2 ^ i) = 0 Then
                Sheets("GPIOs").CheckBoxes("GPIO2_" & i & "_PIN").Value = xlOff
            Else
                Sheets("GPIOs").CheckBoxes("GPIO2_" & i & "_PIN").Value = xlOn
            End If
        Next i
        Sheets("GPIOs").Show
    Else
        Message_Unconnected
    End If
End Sub


Sub GPIOs_Done_Click()
' "Done"
End Sub


Sub GPIOs_Update_Click()
    Dim GPIO1_PORT As Long
    Dim GPIO1_DDR As Long
    Dim GPIO2_PORT As Long
    Dim GPIO2_DDR As Long
    Dim GPIO1_PIN As Long
    Dim GPIO2_PIN As Long
    Dim Analog(4) As Long
    Dim Didr As Byte
    Dim AdcEn As Byte
    Dim AdcAref As Byte

    GPIO1_PORT = 0
    GPIO1_DDR = 0
    GPIO2_PORT = 0
    GPIO2_DDR = 0

    For i = 0 To 15
        If (Sheets("GPIOs").CheckBoxes("GPIO1_" & i & "_PORT").Value = xlOn) Then
            GPIO1_PORT = GPIO1_PORT Or 2 ^ i
        End If
        If (Sheets("GPIOs").CheckBoxes("GPIO1_" & i & "_DDR").Value = xlOn) Then
            GPIO1_DDR = GPIO1_DDR Or 2 ^ i
        End If
        If (Sheets("GPIOs").CheckBoxes("GPIO2_" & i & "_PORT").Value = xlOn) Then
            GPIO2_PORT = GPIO2_PORT Or 2 ^ i
        End If
        If (Sheets("GPIOs").CheckBoxes("GPIO2_" & i & "_DDR").Value = xlOn) Then
            GPIO2_DDR = GPIO2_DDR Or 2 ^ i
        End If
    Next i

    Call GPIO1_Ports_Set(GPIO1_PORT, 65535)
    Call GPIO1_Dirs_Set(GPIO1_DDR, 65535)
    
    Didr = 0
    For i = 4 To 7
        If (Sheets("GPIOs").CheckBoxes("ADC_" & i & "_DIDR").Value = xlOn) Then
            Didr = Didr Or (2 ^ i)
        End If
        If (Sheets("GPIOs").CheckBoxes("ADC_" & i & "_SCAN").Value = xlOn) Then
            AdcEn = ADC_EN
        End If
    Next i
    If (Sheets("GPIOs").OptionButtons("Adc_Aref_Sel_2V56").Value = xlOn) Then
        AdcAref = AREF_2p56
    Else
        AdcAref = AREF_AVCC
    End If
    Call ADC_Set(AdcAref, Didr, AdcEn)

    Call GPIO2_Ports_Set(GPIO2_PORT, 65535)
    Call GPIO2_Dirs_Set(GPIO2_DDR, 65535)

    GPIO1_PIN = GPIO1_Ports_Get
    GPIO2_PIN = GPIO2_Ports_Get

    For i = 0 To 15
        If (GPIO1_PIN And 2 ^ i) = 0 Then
            Sheets("GPIOs").CheckBoxes("GPIO1_" & i & "_PIN").Value = xlOff
        Else
            Sheets("GPIOs").CheckBoxes("GPIO1_" & i & "_PIN").Value = xlOn
        End If
        If (GPIO2_PIN And 2 ^ i) = 0 Then
            Sheets("GPIOs").CheckBoxes("GPIO2_" & i & "_PIN").Value = xlOff
        Else
            Sheets("GPIOs").CheckBoxes("GPIO2_" & i & "_PIN").Value = xlOn
        End If
    Next i

    For i = 4 To 7
        If (Sheets("GPIOs").CheckBoxes("ADC_" & i & "_SCAN").Value = xlOn) Then
            Sheets("GPIOs").EditBoxes("ADC_" & i & "_VAL").Characters.Text = ADC_Get(i)
        End If
    Next i
End Sub


Sub Run_Servo_CTRL_Dialog()
    Dim pwm As positions_t

    If Is_Connected Then
        pwm = Servo_Get()
        Sheets("Servos").EditBoxes("Servo_1_Value").Characters.Text = pwm.Servo(1)
        Sheets("Servos").Spinners("Servo_1_updown").Value = pwm.Servo(1)
        Sheets("Servos").EditBoxes("Servo_2_Value").Characters.Text = pwm.Servo(2)
        Sheets("Servos").Spinners("Servo_2_updown").Value = pwm.Servo(2)
        Sheets("Servos").Show
    Else
        Message_Unconnected
    End If
End Sub


Sub Servos_Done_Click()
' "Done"
End Sub


Sub Servo_1_Spin_Click()
    Sheets("Servos").EditBoxes("Servo_1_Value").Characters.Text = Sheets("Servos").Spinners("Servo_1_updown").Value
End Sub


Sub Servo_2_Spin_Click()
    Sheets("Servos").EditBoxes("Servo_2_Value").Characters.Text = Sheets("Servos").Spinners("Servo_2_updown").Value
End Sub


Sub Servo1_use_Click()
    Call Servo_Set(253, 255)
End Sub


Sub Servo2_use_Click()
    Call Servo_Set(255, 253)
End Sub


Sub Servos_Update_Click()
    Call Servo_Set(Sheets("Servos").Spinners("Servo_1_updown").Value, Sheets("Servos").Spinners("Servo_2_updown").Value)
End Sub
