
'
' Type declarations
'
TYPE VTVariable
    Title as STRING
    Value as STRING
END TYPE

TYPE VTVariableList
    Element as ARRAY of VTVariable
    Count as LONG
END TYPE


'
' Variable methods.
'
SUB VTVLInit(VTVList as VTVariableList)
	REDIM (VTVList.Element) (0 TO 63) as VTVariable
    VTVLClear(VTVList)
END SUB


SUB VTVLTerm(VTVList as VTVariableList)
    VTVLClear(VTVList)
END SUB


SUB VTVLClear(VTVList as VTVariableList)
    VTVList.Count = 0
END SUB


FUNCTION VTVLAddValue(VTVList as VTVariableList, Title as STRING, Value as STRING) as LONG

    IF (VTVList.Count < 64) THEN
        VTVList.Element(VTVList.Count).Title  = Title
        VTVList.Element(VTVList.Count).Value = Value
        VTVList.Count = VTVList.Count + 1
        VTVLAddValue = 1
    ELSE
        VTVLAddValue = 0
    END IF
END FUNCTION


FUNCTION VTVLGetStringValue(VTVList as VTVariableList, Title as STRING, Value as STRING) as LONG
    DIM dwIndex as LONG

    VTVLGetStringValue = 0

    FOR dwIndex = 0 to VTVList.Count 
        IF (Title = VTVList.Element(dwIndex).Title) THEN
            Value = VTVList.Element(dwIndex).Value
            VTVLGetStringValue = 1
            EXIT FOR
        END IF
    NEXT

END FUNCTION


FUNCTION VTVLGetLongValue(VTVList as VTVariableList, Title as STRING, Value as LONG) as LONG
    DIM dwIndex as LONG

    VTVLGetLongValue = 0

    FOR dwIndex = 0 to VTVList.Count 
        IF (Title = VTVList.Element(dwIndex).Title) THEN
            Value = VAL(VTVList.Element(dwIndex).Value)
            VTVLGetLongValue = 1
            EXIT FOR
        END IF
    NEXT

END FUNCTION








