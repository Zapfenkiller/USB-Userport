Attribute VB_Name = "BlinkDemo"
'
' * The USB-Userport *
' Copyright 2020  René Trapp (rene [dot] trapp (-at-) web [dot] de)
' License: "The MIT license" - see license.txt'
'
'-----------------------------------------------------------------

' https://stackoverflow.com/questions/1544526/how-to-pause-for-specific-amount-of-time-excel-vba

Public Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)


' Blinks LED[0] (the RX-LED) 5 times.
Sub Blink()
    Dim pattern As Byte

    If Is_Connected Then
        ' Invert LED status 10 times.
        For i = 1 To 10
            pattern = LEDs_Get()
            If (pattern And LED0_CTRL_POS) = 0 Then
                ' LED[0] = An
                pattern = pattern Or LED0_CTRL_POS
            Else
                ' LED[0] = Aus
                pattern = pattern And (Not LED0_CTRL_POS)
            End If
            Call LEDs_Set(pattern)
            Sleep (500)
        Next i
    Else
        Message_Unconnected
    End If

End Sub
