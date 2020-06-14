Attribute VB_Name = "Module5"
' * The USB-Userport *
' Copyright 2020  Ren� Trapp (rene [dot] trapp (-at-) web [dot] de)
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
            Sheets("LEDs").CheckBoxes("LED_0").Value = pattern And LED0_CTRL_POS
        Else
            Sheets("LEDs").CheckBoxes("LED_0").Value = 1
        End If
        If (pattern And LED1_CTRL_POS) = 0 Then
            Sheets("LEDs").CheckBoxes("LED_1").Value = 0
        Else
            Sheets("LEDs").CheckBoxes("LED_1").Value = 1
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
    If (Sheets("LEDs").CheckBoxes("LED_1").Value) = 1 Then
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
    If (Sheets("LEDs").CheckBoxes("LED_0").Value) = 1 Then
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
        Dim GPIO1_PORT As Long
        Dim GPIO1_DDR As Long
        Dim GPIO2_PORT As Long
        Dim GPIO2_DDR As Long
        
        GPIO1_PORT = GPIO1_Ports_Get
        GPIO1_DDR = GPIO1_Dirs_Get
        GPIO2_PORT = GPIO2_Ports_Get
        GPIO2_DDR = GPIO2_Dirs_Get

        For i = 0 To 15
            If (GPIO1_PORT And 2 ^ i) = 0 Then
                Sheets("GPIOs").CheckBoxes("GPIO1_" & i & "_PORT").Value = 0
            Else
                Sheets("GPIOs").CheckBoxes("GPIO1_" & i & "_PORT").Value = 1
            End If
            If (GPIO1_DDR And 2 ^ i) = 0 Then
                Sheets("GPIOs").CheckBoxes("GPIO1_" & i & "_DDR").Value = 0
            Else
                Sheets("GPIOs").CheckBoxes("GPIO1_" & i & "_DDR").Value = 1
            End If
            If (GPIO2_PORT And 2 ^ i) = 0 Then
                Sheets("GPIOs").CheckBoxes("GPIO2_" & i & "_PORT").Value = 0
            Else
                Sheets("GPIOs").CheckBoxes("GPIO2_" & i & "_PORT").Value = 1
            End If
            If (GPIO2_DDR And 2 ^ i) = 0 Then
                Sheets("GPIOs").CheckBoxes("GPIO2_" & i & "_DDR").Value = 0
            Else
                Sheets("GPIOs").CheckBoxes("GPIO2_" & i & "_DDR").Value = 1
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

    GPIO1_PORT = 0
    GPIO1_DDR = 0
    GPIO2_PORT = 0
    GPIO2_DDR = 0

    For i = 0 To 15
        If (Sheets("GPIOs").CheckBoxes("GPIO1_" & i & "_PORT").Value = 1) Then
            GPIO1_PORT = GPIO1_PORT Or 2 ^ i
        End If
        If (Sheets("GPIOs").CheckBoxes("GPIO1_" & i & "_DDR").Value = 1) Then
            GPIO1_DDR = GPIO1_DDR Or 2 ^ i
        End If
        If (Sheets("GPIOs").CheckBoxes("GPIO2_" & i & "_PORT").Value = 1) Then
            GPIO2_PORT = GPIO2_PORT Or 2 ^ i
        End If
        If (Sheets("GPIOs").CheckBoxes("GPIO2_" & i & "_DDR").Value = 1) Then
            GPIO2_DDR = GPIO2_DDR Or 2 ^ i
        End If
    Next i

    Call GPIO1_Ports_Set(GPIO1_PORT, 65535)
    Call GPIO1_Dirs_Set(GPIO1_DDR, 65535)
    Call GPIO2_Ports_Set(GPIO2_PORT, 65535)
    Call GPIO2_Dirs_Set(GPIO2_DDR, 65535)

    GPIO1_PIN = GPIO1_Ports_Get
    GPIO2_PIN = GPIO2_Ports_Get

    For i = 0 To 15
        If (GPIO1_PIN And 2 ^ i) = 0 Then
            Sheets("GPIOs").CheckBoxes("GPIO1_" & i & "_PIN").Value = 0
        Else
            Sheets("GPIOs").CheckBoxes("GPIO1_" & i & "_PIN").Value = 1
        End If
        If (GPIO2_PIN And 2 ^ i) = 0 Then
            Sheets("GPIOs").CheckBoxes("GPIO2_" & i & "_PIN").Value = 0
        Else
            Sheets("GPIOs").CheckBoxes("GPIO2_" & i & "_PIN").Value = 1
        End If
    Next i
End Sub
