Attribute VB_Name = "DirectIODemo"
'
' * The USB-Userport *
' Copyright 2021  René Trapp (rene [dot] trapp (-at-) web [dot] de)
' License: "The MIT license" - see license.txt'
'
'-----------------------------------------------------------------

' https://stackoverflow.com/questions/1544526/how-to-pause-for-specific-amount-of-time-excel-vba

Public Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)


' Blinks LED[1] (the TX-LED) 5 times.
' The TX-LED is connected to PD5. It is lit when PD5 gets driven to '0'.
' Controlling just the LED means the address needs to be written just once.
Sub Blink()
    Dim pattern As Byte

    If Is_Connected Then
        ' Enable PD5 as output (normally part of the USB-Userport internal init)
        ' but shown to here as example how different registers get addressed and
        ' how a single bit is flipped.
        ' DDRD  0x2A
        ' PORTD 0x2B
        ' Set Bit 5 in DDRD => make it an output
        Call IO_SetAddr(&H2A)
        pattern = IO_Read()
        pattern = pattern Or &H20
        Call IO_Write(pattern)

        ' Now access the PORTB register
        Call IO_SetAddr(&H2B)

        ' Invert LED status 10 times.
        For i = 1 To 10
            pattern = IO_Read()
            If (pattern And &H20) = 0 Then
                ' LED[0] = Aus
                pattern = pattern Or &H20
            Else
                ' LED[0] = An
                pattern = pattern And (Not &H20)
            End If
            Call IO_Write(pattern)
            Sleep (500)
        Next i
    Else
        Message_Unconnected
    End If

End Sub
