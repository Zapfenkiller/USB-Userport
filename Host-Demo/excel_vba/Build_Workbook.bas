Attribute VB_Name = "Build_Workbook"
'
' * The USB-Userport *
' Copyright 2020  René Trapp (rene [dot] trapp (-at-) web [dot] de)
' License: "The MIT license" - see license.txt'
'
'-----------------------------------------------------------------
'
'
' Instructions:
' -------------
' Run `Build_Workbook()` to initialize the Main sheet, dialog
' definition(s) and some buttons to control the IO-lines. Dialog
' definition sheet(s) might be hidden. You can unhide them any
' time for your reference.
'
' Existing sheets are not (re)build. It is recommended to run the
' `Build_Workbook()` macro just in a fresh Excel workbook.
'
' Connect your USB-Userport board, select its address (cell C2) and
' click the `Connect` button. `Disconnect` the application when you
' you are done. Start playing around with the onboard LEDs.
' The remainder should be self-explanatory then.
'


Sub Build_Workbook()
    LEDs_Build_Form
    GPIOs_Build_Form
    Servo_Build_Form
    Main_Build_Sheet
End Sub


Sub Main_Build_Sheet()
    If Not sheet_exists("Main") Then
        Sheets.Add.name = "Main"

        Cells.Select
        With Selection
            .VerticalAlignment = xlTop
            .WrapText = False
            .Orientation = 0
            .AddIndent = False
            .IndentLevel = 0
            .ShrinkToFit = False
            .ReadingOrder = xlContext
            .MergeCells = False
        End With

        Cells(2, 2).Value = "USB-Userport Address:"
        Cells(2, 3).Value = 1
        Cells(3, 2).Value = "Status:"
        With Cells(3, 3)
            .Value = "Disconnected"
            .Font.Color = RGB(255, 0, 0)
            .Font.TintAndShade = 0
        End With
        Columns("B:B").EntireColumn.AutoFit
        With Columns("C:C")
            .EntireColumn.AutoFit
            .HorizontalAlignment = xlRight
        End With

        ActiveSheet.Buttons.Add(55, 53, 100, 22).Select
        Selection.name = "BTN_Disconnect"
        Selection.OnAction = "Disconnect_from_Device"
        Selection.Caption = "Disconnect"
        With Selection.Font
            .name = "Calibri"
            .FontStyle = "Regular"
            .Size = 11
            .Strikethrough = False
            .Superscript = False
            .Subscript = False
            .OutlineFont = False
            .Shadow = False
            .Underline = xlUnderlineStyleNone
            .ColorIndex = 1
        End With
        Selection.Visible = False

        ActiveSheet.Buttons.Add(55, 53, 100, 22).Select
        Selection.name = "BTN_Connect"
        Selection.OnAction = "Connect_to_Device"
        Selection.Caption = "Connect"
        With Selection.Font
            .name = "Calibri"
            .FontStyle = "Regular"
            .Size = 11
            .Strikethrough = False
            .Superscript = False
            .Subscript = False
            .OutlineFont = False
            .Shadow = False
            .Underline = xlUnderlineStyleNone
            .ColorIndex = 1
        End With

        ActiveSheet.Buttons.Add(55, 83, 100, 22).Select
        Selection.name = "BTN_LEDctrl"
        Selection.OnAction = "Run_LED_CTRL_Dialog"
        Selection.Caption = "Run LED Control"
        With Selection.Font
            .name = "Calibri"
            .FontStyle = "Regular"
            .Size = 11
            .Strikethrough = False
            .Superscript = False
            .Subscript = False
            .OutlineFont = False
            .Shadow = False
            .Underline = xlUnderlineStyleNone
            .ColorIndex = 1
        End With

        ActiveSheet.Buttons.Add(55, 113, 100, 22).Select
        Selection.name = "BTN_GPIOctrl"
        Selection.OnAction = "Run_GPIO_CTRL_Dialog"
        Selection.Caption = "Run GPIO Control"
        With Selection.Font
            .name = "Calibri"
            .FontStyle = "Regular"
            .Size = 11
            .Strikethrough = False
            .Superscript = False
            .Subscript = False
            .OutlineFont = False
            .Shadow = False
            .Underline = xlUnderlineStyleNone
            .ColorIndex = 1
        End With

        ActiveSheet.Buttons.Add(55, 143, 100, 22).Select
        Selection.name = "BTN_Servoctrl"
        Selection.OnAction = "Run_Servo_CTRL_Dialog"
        Selection.Caption = "Run Servo Control"
        With Selection.Font
            .name = "Calibri"
            .FontStyle = "Regular"
            .Size = 11
            .Strikethrough = False
            .Superscript = False
            .Subscript = False
            .OutlineFont = False
            .Shadow = False
            .Underline = xlUnderlineStyleNone
            .ColorIndex = 1
        End With

    End If
    Sheets("Main").Select
    Cells(2, 3).Select
End Sub


Sub LEDs_Build_Form()
    If Not sheet_exists("LEDs") Then
        DialogSheets.Add.name = "LEDs"
        ActiveSheet.Shapes("Dialog 1").name = "LEDs"
        ActiveSheet.Shapes("LEDs").Select
        Selection.Characters.Text = "LED Control"
        Selection.ShapeRange.ScaleHeight 0.45, msoFalse, msoScaleFromTopLeft
        Selection.ShapeRange.ScaleWidth 0.5, msoFalse, msoScaleFromTopLeft

        Sheets("LEDs").CheckBoxes.Add(80, 43, 40, 8).Select
        Selection.name = "LED_1"
        Selection.Characters.Text = "LED[1]"
        Selection.OnAction = "LEDs_LED1_Click"

        Sheets("LEDs").CheckBoxes.Add(80, 65, 40, 8).Select
        Selection.name = "LED_0"
        Selection.Characters.Text = "LED[0]"
        Selection.OnAction = "LEDs_LED0_Click"

        ' Button 3 = Cancel - no need for it
        Sheets("LEDs").Shapes("Button 3").Delete
        ' Button 2 = OK - gets renamed and repositioned
        Sheets("LEDs").Shapes("Button 2").Select
        Selection.name = "Btn_Done"
        Selection.Characters.Text = "Done"
        Selection.ShapeRange.IncrementLeft -140
        Selection.ShapeRange.IncrementTop 12.8
        Selection.OnAction = "LEDs_Done_Click"

        Sheets("LEDs").Visible = False
    End If
End Sub


Sub GPIOs_Build_Form()
    If Not sheet_exists("GPIOs") Then
        DialogSheets.Add.name = "GPIOs"
        ActiveSheet.Shapes("Dialog 1").name = "GPIOs"
        ActiveSheet.Shapes("GPIOs").Select
        Selection.Characters.Text = " GPIO Control "
        Selection.ShapeRange.ScaleHeight 3.06, msoFalse, msoScaleFromTopLeft
        Selection.ShapeRange.ScaleWidth 2.01, msoFalse, msoScaleFromTopLeft

        ActiveSheet.GroupBoxes.Add(75.6, 50, 155, 389).name = "Box GPIO1" 'Select
        ActiveSheet.Shapes("Box GPIO1").Select
        Selection.Characters.Text = " GPIO1 "

        ActiveSheet.GroupBoxes.Add(255.6, 50, 155, 389).name = "Box GPIO2" 'Select
        ActiveSheet.Shapes("Box GPIO2").Select
        Selection.Characters.Text = " GPIO2 "

        ActiveSheet.GroupBoxes.Add(435.6, 50, 155, 389).name = "Box ADC" 'Select
        ActiveSheet.Shapes("Box ADC").Select
        Selection.Characters.Text = " ADC "

        y = 85
        y_inc = 22
        
        ' Button 3 = Cancel - no need for it
        Sheets("GPIOs").Shapes("Button 3").Delete
        ' Button 2 = OK - gets renamed and repositioned
        Sheets("GPIOs").Shapes("Button 2").Select
        With Selection
            .name = "Btn_Done"
            .Characters.Text = "Done"
            .ShapeRange.IncrementLeft 240
            .ShapeRange.IncrementTop 15 * y_inc + y + 2
            .OnAction = "GPIOs_Done_Click"
        End With

        Sheets("GPIOs").Buttons.Add(104, 16 * y_inc + y + 10, 50, 16).Select
        With Selection
            .name = "Btn_Update"
            .Characters.Text = "Update"
            .OnAction = "GPIOs_Update_Click"
        End With

        For i = 0 To 15
            Sheets("GPIOs").CheckBoxes.Add(80, y + i * y_inc, 50, 8).Select
            Selection.name = "GPIO1_" & 15 - i & "_PORT"
            Selection.Characters.Text = "Set[" & 15 - i & "]"

            Sheets("GPIOs").CheckBoxes.Add(130, y + i * y_inc, 50, 8).Select
            Selection.name = "GPIO1_" & 15 - i & "_DDR"
            Selection.Characters.Text = "Dir[" & 15 - i & "]"

            Sheets("GPIOs").CheckBoxes.Add(180, y + i * y_inc, 50, 8).Select
            Selection.name = "GPIO1_" & 15 - i & "_PIN"
            Selection.Characters.Text = "Get[" & 15 - i & "]"
            Selection.Enabled = False

            Sheets("GPIOs").CheckBoxes.Add(260, y + i * y_inc, 50, 8).Select
            Selection.name = "GPIO2_" & 15 - i & "_PORT"
            Selection.Characters.Text = "Set[" & 15 - i & "]"

            Sheets("GPIOs").CheckBoxes.Add(310, y + i * y_inc, 50, 8).Select
            Selection.name = "GPIO2_" & 15 - i & "_DDR"
            Selection.Characters.Text = "Dir[" & 15 - i & "]"

            Sheets("GPIOs").CheckBoxes.Add(360, y + i * y_inc, 50, 8).Select
            Selection.name = "GPIO2_" & 15 - i & "_PIN"
            Selection.Characters.Text = "Get[" & 15 - i & "]"
            Selection.Enabled = False

        Next i

        Sheets("GPIOs").Shapes("GPIO1_0_PORT").Visible = False
        Sheets("GPIOs").CheckBoxes("GPIO1_0_PORT").Value = xlOff
        Sheets("GPIOs").Shapes("GPIO1_0_DDR").Visible = False
        Sheets("GPIOs").CheckBoxes("GPIO1_0_DDR").Value = xlOff
        Sheets("GPIOs").Shapes("GPIO1_0_PIN").Visible = False
        
        Sheets("GPIOs").Shapes("GPIO1_13_PORT").Visible = False
        Sheets("GPIOs").CheckBoxes("GPIO1_13_PORT").Value = xlOff
        Sheets("GPIOs").Shapes("GPIO1_13_DDR").Visible = False
        Sheets("GPIOs").CheckBoxes("GPIO1_13_DDR").Value = xlOff
        Sheets("GPIOs").Shapes("GPIO1_13_PIN").Visible = False

        Sheets("GPIOs").Shapes("GPIO2_0_PORT").Visible = False
        Sheets("GPIOs").CheckBoxes("GPIO2_0_PORT").Value = xlOff
        Sheets("GPIOs").Shapes("GPIO2_0_DDR").Visible = False
        Sheets("GPIOs").CheckBoxes("GPIO2_0_DDR").Value = xlOff
        Sheets("GPIOs").Shapes("GPIO2_0_PIN").Visible = False

        Sheets("GPIOs").Shapes("GPIO2_1_PORT").Visible = False
        Sheets("GPIOs").CheckBoxes("GPIO2_1_PORT").Value = xlOff
        Sheets("GPIOs").Shapes("GPIO2_1_DDR").Visible = False
        Sheets("GPIOs").CheckBoxes("GPIO2_1_DDR").Value = xlOff
        Sheets("GPIOs").Shapes("GPIO2_1_PIN").Visible = False

        Sheets("GPIOs").Shapes("GPIO2_2_PORT").Visible = False
        Sheets("GPIOs").CheckBoxes("GPIO2_2_PORT").Value = xlOff
        Sheets("GPIOs").Shapes("GPIO2_2_DDR").Visible = False
        Sheets("GPIOs").CheckBoxes("GPIO2_2_DDR").Value = xlOff
        Sheets("GPIOs").Shapes("GPIO2_2_PIN").Visible = False

        Sheets("GPIOs").Shapes("GPIO2_3_PORT").Visible = False
        Sheets("GPIOs").CheckBoxes("GPIO2_3_PORT").Value = xlOff
        Sheets("GPIOs").Shapes("GPIO2_3_DDR").Visible = False
        Sheets("GPIOs").CheckBoxes("GPIO2_3_DDR").Value = xlOff
        Sheets("GPIOs").Shapes("GPIO2_3_PIN").Visible = False

        Sheets("GPIOs").Shapes("GPIO2_8_PORT").Visible = False
        Sheets("GPIOs").CheckBoxes("GPIO2_8_PORT").Value = xlOff
        Sheets("GPIOs").Shapes("GPIO2_8_DDR").Visible = False
        Sheets("GPIOs").CheckBoxes("GPIO2_8_DDR").Value = xlOff
        Sheets("GPIOs").Shapes("GPIO2_8_PIN").Visible = False

        Sheets("GPIOs").Shapes("GPIO2_9_PORT").Visible = False
        Sheets("GPIOs").CheckBoxes("GPIO2_9_PORT").Value = xlOff
        Sheets("GPIOs").Shapes("GPIO2_9_DDR").Visible = False
        Sheets("GPIOs").CheckBoxes("GPIO2_9_DDR").Value = xlOff
        Sheets("GPIOs").Shapes("GPIO2_9_PIN").Visible = False

        Sheets("GPIOs").Shapes("GPIO2_10_PORT").Visible = False
        Sheets("GPIOs").CheckBoxes("GPIO2_10_PORT").Value = xlOff
        Sheets("GPIOs").Shapes("GPIO2_10_DDR").Visible = False
        Sheets("GPIOs").CheckBoxes("GPIO2_10_DDR").Value = xlOff
        Sheets("GPIOs").Shapes("GPIO2_10_PIN").Visible = False

        Sheets("GPIOs").Shapes("GPIO2_11_PORT").Visible = False
        Sheets("GPIOs").CheckBoxes("GPIO2_11_PORT").Value = xlOff
        Sheets("GPIOs").Shapes("GPIO2_11_DDR").Visible = False
        Sheets("GPIOs").CheckBoxes("GPIO2_11_DDR").Value = xlOff
        Sheets("GPIOs").Shapes("GPIO2_11_PIN").Visible = False

        Sheets("GPIOs").Shapes("GPIO2_12_PORT").Visible = False
        Sheets("GPIOs").CheckBoxes("GPIO2_12_PORT").Value = xlOff
        Sheets("GPIOs").Shapes("GPIO2_12_DDR").Visible = False
        Sheets("GPIOs").CheckBoxes("GPIO2_12_DDR").Value = xlOff
        Sheets("GPIOs").Shapes("GPIO2_12_PIN").Visible = False

        Sheets("GPIOs").Shapes("GPIO2_13_PORT").Visible = False
        Sheets("GPIOs").CheckBoxes("GPIO2_13_PORT").Value = xlOff
        Sheets("GPIOs").Shapes("GPIO2_13_DDR").Visible = False
        Sheets("GPIOs").CheckBoxes("GPIO2_13_DDR").Value = xlOff
        Sheets("GPIOs").Shapes("GPIO2_13_PIN").Visible = False

        Sheets("GPIOs").Shapes("GPIO2_14_PORT").Visible = False
        Sheets("GPIOs").CheckBoxes("GPIO2_14_PORT").Value = xlOff
        Sheets("GPIOs").Shapes("GPIO2_14_DDR").Visible = False
        Sheets("GPIOs").CheckBoxes("GPIO2_14_DDR").Value = xlOff
        Sheets("GPIOs").Shapes("GPIO2_14_PIN").Visible = False

        Sheets("GPIOs").Shapes("GPIO2_15_PORT").Visible = False
        Sheets("GPIOs").CheckBoxes("GPIO2_15_PORT").Value = xlOff
        Sheets("GPIOs").Shapes("GPIO2_15_DDR").Visible = False
        Sheets("GPIOs").CheckBoxes("GPIO2_15_DDR").Value = xlOff
        Sheets("GPIOs").Shapes("GPIO2_15_PIN").Visible = False

        For i = 4 To 7
            Sheets("GPIOs").CheckBoxes.Add(440, y + (i + 4) * y_inc, 50, 8).Select
            Selection.name = "ADC_" & 11 - i & "_DIDR"
            Selection.Characters.Text = "DIDR[" & 11 - i & "]"

            Sheets("GPIOs").CheckBoxes.Add(490, y + (i + 4) * y_inc, 50, 8).Select
            Selection.name = "ADC_" & 11 - i & "_SCAN"
            Selection.Characters.Text = "Enable"

            Sheets("GPIOs").EditBoxes.Add(540, y + (i + 4) * y_inc, 40, 18).Select
            With Selection
                .name = "ADC_" & 11 - i & "_VAL"
                .Characters.Text = "0"
                .InputType = xlInteger
                .MultiLine = False
                .DisplayVerticalScrollBar = False
                .PasswordEdit = False
                .Enabled = False
            End With

        Next i
        ActiveSheet.OptionButtons.Add(465, y - y_inc, 50, 8).Select
        With Selection
            .name = "Adc_Aref_Sel_Avcc"
            .Characters.Text = "AVcc"
            .Value = xlOn
            .LinkedCell = ""
            .Accelerator = ""
        End With
        ActiveSheet.OptionButtons.Add(515, y - y_inc, 50, 8).Select
        With Selection
            .name = "Adc_Aref_Sel_2V56"
            .Characters.Text = "2.56 V"
            .Value = xlOff
            .LinkedCell = ""
            .Accelerator = ""
        End With

        Sheets("GPIOs").Visible = False
    End If
End Sub


Sub Servo_Build_Form()
    If Not sheet_exists("Servos") Then
        DialogSheets.Add.name = "Servos"
        ActiveSheet.Shapes("Dialog 1").name = "Servos"
        ActiveSheet.Shapes("Servos").Select
        Selection.Characters.Text = "Servo Control"
        Selection.ShapeRange.ScaleHeight 0.8, msoFalse, msoScaleFromTopLeft
        Selection.ShapeRange.ScaleWidth 0.54, msoFalse, msoScaleFromTopLeft

        Sheets("Servos").Labels.Add(80, 43, 40, 20).Select
        Selection.Characters.Text = "Servo 1:"
        Sheets("Servos").EditBoxes.Add(120, 43, 30, 20).Select
        With Selection
            .name = "Servo_1_Value"
            .Characters.Text = "---"
            .InputType = xlInteger
            .MultiLine = False
            .DisplayVerticalScrollBar = False
            .PasswordEdit = False
            .Enabled = False
        End With
        Sheets("Servos").Spinners.Add(150, 43, 10, 20).Select
        With Selection
            .name = "Servo_1_updown"
            .Value = 0
            .Min = 0
            .Max = 250
            .SmallChange = 1
            .LinkedCell = ""
        End With
        Selection.OnAction = "Servo_1_Spin_Click"
        Sheets("Servos").Buttons.Add(175, 43, 25, 20).Select
        With Selection
            .name = "Servo_1_use"
            .Characters.Text = "Use"
            .OnAction = "Servo1_use_Click"
        End With

        Sheets("Servos").Labels.Add(80, 77, 40, 20).Select
        Selection.Characters.Text = "Servo 2:"
        Sheets("Servos").EditBoxes.Add(120, 77, 30, 20).Select
        With Selection
            .name = "Servo_2_Value"
            .Characters.Text = "---"
            .InputType = xlInteger
            .MultiLine = False
            .DisplayVerticalScrollBar = False
            .PasswordEdit = False
            .Enabled = False
        End With
        Sheets("Servos").Spinners.Add(150, 77, 10, 20).Select
        With Selection
            .name = "Servo_2_updown"
            .Value = 0
            .Min = 0
            .Max = 250
            .SmallChange = 1
            .LinkedCell = ""
        End With
        Selection.OnAction = "Servo_2_Spin_Click"
        Sheets("Servos").Buttons.Add(175, 77, 25, 20).Select
        With Selection
            .name = "Servo_2_use"
            .Characters.Text = "Use"
            .OnAction = "Servo2_use_Click"
        End With

        ' Button 3 = Cancel - no need for it
        Sheets("Servos").Shapes("Button 3").Delete
        ' Button 2 = OK - gets renamed and repositioned
        Sheets("Servos").Shapes("Button 2").Select
        Selection.name = "Btn_Done"
        Selection.Characters.Text = "Done"
        Selection.ShapeRange.IncrementLeft -130
        Selection.ShapeRange.IncrementTop 70
        Selection.OnAction = "Servos_Done_Click"

        Sheets("Servos").Buttons.Add(80, 111, 50, 16).Select
        With Selection
            .name = "Btn_Update"
            .Characters.Text = "Update"
            .OnAction = "Servos_Update_Click"
        End With

        Sheets("Servos").Visible = False
    End If
End Sub


Function sheet_exists(ByVal sName As String) As Boolean
    For i = 1 To Application.ActiveWorkbook.Sheets.Count
        If Application.ActiveWorkbook.Sheets(i).name = sName Then
            sheet_exists = True
            Exit Function
        End If
    Next i
    sheet_exists = False
End Function
