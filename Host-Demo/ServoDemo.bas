Attribute VB_Name = "ServoDemo"
' https://stackoverflow.com/questions/1544526/how-to-pause-for-specific-amount-of-time-excel-vba

Public Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)


' Enables the 2 servo outputs, sets servo 1 to 1/4 of its way,
' servo 2 stays at its default
Sub Servo1()
    Call Servo_Set(63, 255)      ' new default position
    Call Servo_Set(255, 253)     ' start the pulse generator
End Sub

' Advances servo 1 from one end to another
Sub Servo2()
    Dim i As Byte
    For i = 0 To 250
        Call Servo_Set(i, 255)
        Sleep (5)
    Next i
    For i = 0 To 250
        Call Servo_Set(250 - i, 255)
        Sleep (5)
    Next i
End Sub

' Advances servo 2 from one end to another
Sub Servo3()
    Dim i As Byte
    For i = 0 To 250
        Call Servo_Set(255, i)
        Sleep (5)
    Next i
    For i = 0 To 250
        Call Servo_Set(255, 250 - i)
        Sleep (5)
    Next i
End Sub

Sub test()
    Call Servo_Set(125, 125)
End Sub
