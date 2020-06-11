Attribute VB_Name = "Module1"
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
'
' Instructions:
' -------------
' Run `Build_Workbook()` to initialize the Main sheet, dialog
' definition(s) and some buttons to control the IO-lines. Dialog
' definition sheet(s) might be hidden. You can unhide them any
' time for your reference.
'
' Existing sheets are not (re)build. It is recommended to run the
' `Build_Workbook()` macro just in a fresh excell workbook.
'
' Connect your USB-Userport board, select its address (cell C2) and
' click the `Connect` button. `Disconnect` the application when you
' you are done. Start playing around with the onboard LEDs.
' The remainder should be self-explanatory then.
'


Sub Build_Workbook()
    LEDs_Build_Form
    
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
        Selection.name = "BTN_GPIO1ctrl"
        Selection.OnAction = "Run_GPIO1_CTRL_Dialog"
        Selection.Caption = "Run GPIO1 Control"
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
        Selection.name = "BTN_GPIO2ctrl"
        Selection.OnAction = "Run_GPIO2_CTRL_Dialog"
        Selection.Caption = "Run GPIO2 Control"
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


Function sheet_exists(ByVal sName As String) As Boolean
    For i = 1 To Application.ActiveWorkbook.Sheets.Count
        If Application.ActiveWorkbook.Sheets(i).name = sName Then
            sheet_exists = True
            Exit Function
        End If
    Next i
    sheet_exists = False
End Function

