
'$INCLUDE 'varlist.inc'

'
' Type declarations
'

'
' Variable declarations
'
GLOBAL mVTVList as VTVariableList
GLOBAL mVTEdit as LONG

SUB WintrekInit()
    VTVLInit(mVTVList)
    mVTEdit = 0
END SUB


SUB WintrekTerm()
    VTVLTerm(mVTVList)
END SUB


SUB WintrekRefreshVariableList()
    DIM strVTText as STRING
    DIM strTitle as STRING
    DIM strValue as STRING
    DIM dwIndex as LONG

    '
    ' Make sure we have found the edit control.
    '
    IF (0 = mVTEdit) THEN
        mVTEdit = WFndWndC("Version=", "Edit", FW_HIDDENOK OR FW_NOFOCUS OR FW_ALL OR FW_PREFIX, 10)
    END IF

    IF (0 <> mVTEdit) THEN
        '
        ' Get the text from the control.
        '
        strVTText = GetText(mVTEdit)
        IF ("" <> strVTText) THEN
            '
            ' Update our list.
            '
            VTVLClear(mVTVList)
            DO
                dwIndex = INSTR(strVTText, "=")
                IF (0 <> dwIndex) THEN
                    '
                    ' We found the equal sign.
                    '
                    strTitle = LEFT(strVTText, dwIndex - 1)
                    strVTText = MID(strVTText, dwIndex + 1)
                    dwIndex = INSTR(strVTText, ",")
                    if (0 <> dwIndex) THEN
                        strValue = LEFT(strVTText, dwIndex - 1)
                        strVTText = MID(strVTText, dwIndex + 1)
                    ELSE
                        strValue = strVTText
                        strVTText = ""
                    END IF

                    VTVLAddValue(mVTVList, strTitle, strValue)
                ELSE
                    '
                    ' There's no equal sign.  This is an error.
                    '
                    strVTText = ""
                END IF
            LOOP WHILE (Len(strVTText) > 0)
        END IF
    END IF
END SUB


FUNCTION WintrekGetString(strTitle as STRING, strValue as STRING) as LONG
    WintrekRefreshVariableList()
    WintrekGetString = VTVLGetStringValue(mVTVList, strTitle, strValue)
END FUNCTION


FUNCTION WintrekGetLong(strTitle as STRING, dwValue as LONG) as LONG
    WintrekRefreshVariableList()
    WintrekGetLong = VTVLGetLongValue(mVTVList, strTitle, dwValue)
END FUNCTION


FUNCTION WintrekWaitForString(strTitle as STRING, strValue as STRING, csecWait as LONG) as LONG
    DIM strNewValue as STRING
    WintrekWaitForString = 0

    DO
        WintrekRefreshVariableList()
        IF (1 = WintrekGetString(strTitle, strNewValue)) THEN
            IF (strNewValue = strValue) THEN
                WintrekWaitForString = 1
                EXIT DO
            END IF
        END IF

        IF (cSecWait > 0) THEN
            SLEEP 1
            csecWait = cSecWait - 1
        END IF
    LOOP WHILE(csecWait > 0)
END FUNCTION


FUNCTION WintrekWaitForLong(strTitle as STRING, dwValue as LONG, csecWait as LONG) as LONG
    DIM dwNewValue as LONG
    WintrekWaitForLong = 0

    DO
        WintrekRefreshVariableList()
        IF (1 = WintrekGetLong(strTitle, dwNewValue)) THEN
            IF (dwNewValue = dwValue) THEN
                WintrekWaitForLong = 1
                EXIT DO
            END IF
        END IF

        IF (cSecWait > 0) THEN
            SLEEP 1
            csecWait = cSecWait - 1
        END IF
    LOOP WHILE(csecWait > 0)
END FUNCTION




