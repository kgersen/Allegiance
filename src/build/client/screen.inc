
'
' Type declarations.
'
TYPE Area
    X1 as LONG
    Y1 as LONG
    X2 as LONG
    Y2 as LONG
END TYPE


TYPE ButtonArea
    X1 as LONG
    Y1 as LONG
    X2 as LONG
    Y2 as LONG
    CX as LONG
    CY as LONG
END TYPE


TYPE HorizontalListArea
    X1 as LONG
    Y1 as LONG
    X2 as LONG
    Y2 as LONG
    IX as LONG
    CY as LONG
    CX0 as LONG
END TYPE


TYPE VerticalListArea
    X1 as LONG
    Y1 as LONG
    X2 as LONG
    Y2 as LONG
    IY as LONG
    CX as LONG
    CY0 as LONG
END TYPE

'
' Constants
'
Const c_dsInvalid       = -2
Const c_dsCombat        = -1
Const c_dsStartup       = 0
Const c_dsViewMissions  = 1
Const c_dsSideStrategy  = 2
Const c_dsLoadout       = 3
Const c_dsCreateMission = 4
Const c_dsVoting        = 5
Const c_dsGameOver      = 6


'$INCLUDE 'sctitle.inc'
'$INCLUDE 'scmiss.inc'
'$INCLUDE 'scteamrm.inc'
'$INCLUDE 'sclodout.inc'
'$INCLUDE 'scendgam.inc'


