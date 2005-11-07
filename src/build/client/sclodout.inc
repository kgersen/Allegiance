
'
' Variable declarations.
'
GLOBAL gareaLoadoutBuy as Area
GLOBAL gareaLoadoutBuyHeader as Area
GLOBAL ghlistareaLoadoutBuyTab1 as HorizontalListArea
GLOBAL ghlistareaLoadoutBuyTab2 as HorizontalListArea
GLOBAL gareaLoadoutBuyList as Area
GLOBAL gvlistareaLoadoutBuyList as VerticalListArea
GLOBAL gbuttonLoadoutBuy as ButtonArea
GLOBAL gbuttonLoadoutInvest as ButtonArea
GLOBAL gbuttonLoadoutToTeamRoom as ButtonArea
GLOBAL gbuttonLoadoutLaunch as ButtonArea


'
' Initialize the coordinates of all the buttons.
'
gareaLoadoutBuy.X1 = 15
gareaLoadoutBuy.Y1 = 62
gareaLoadoutBuy.X2 = 249
gareaLoadoutBuy.Y2 = 433


gareaLoadoutBuyHeader.X1 = gareaLoadoutBuy.X1
gareaLoadoutBuyHeader.Y1 = gareaLoadoutBuy.Y1
gareaLoadoutBuyHeader.X2 = gareaLoadoutBuy.X2
gareaLoadoutBuyHeader.Y2 = gareaLoadoutBuyHeader.Y1 + 12


ghlistareaLoadoutBuyTab1.X1 = gareaLoadoutBuy.X1
ghlistareaLoadoutBuyTab1.Y1 = gareaLoadoutBuyHeader.Y2 + 1
ghlistareaLoadoutBuyTab1.X2 = gareaLoadoutBuy.X2
ghlistareaLoadoutBuyTab1.Y2 = ghlistareaLoadoutBuyTab1.Y1 + 24
ghlistareaLoadoutBuyTab1.IX = (ghlistareaLoadoutBuyTab1.X2 - ghlistareaLoadoutBuyTab1.X1) / 2
ghlistareaLoadoutBuyTab1.CY = (ghlistareaLoadoutBuyTab1.Y1 + ghlistareaLoadoutBuyTab1.Y2) / 2
ghlistareaLoadoutBuyTab1.CX0 = ghlistareaLoadoutBuyTab1.X1 + ghlistareaLoadoutBuyTab1.IX / 2


ghlistareaLoadoutBuyTab2.X1 = gareaLoadoutBuy.X1
ghlistareaLoadoutBuyTab2.Y1 = ghlistAreaLoadoutBuyTab1.Y2 + 1
ghlistareaLoadoutBuyTab2.X2 = gareaLoadoutBuy.X2
ghlistareaLoadoutBuyTab2.Y2 = ghlistAreaLoadoutBuyTab2.Y1 + 44
ghlistareaLoadoutBuyTab2.IX = (ghlistareaLoadoutBuyTab2.X2 - ghlistareaLoadoutBuyTab2.X1) / 4
ghlistareaLoadoutBuyTab2.CY = (ghlistareaLoadoutBuyTab2.Y1 + ghlistareaLoadoutBuyTab2.Y2) / 2
ghlistareaLoadoutBuyTab2.CX0 = ghlistareaLoadoutBuyTab2.X1 + ghlistareaLoadoutBuyTab2.IX / 2


gareaLoadoutBuyList.X1 = gareaLoadoutBuy.X1
gareaLoadoutBuyList.Y1 = ghlistareaLoadoutBuyTab2.Y2 + 1
gareaLoadoutBuyList.X2 = gareaLoadoutBuy.X2
gareaLoadoutBuyList.Y2 = gareaLoadoutBuyList.Y1 + (6 * 43)


'
' FUTURE - put in the scroll bar
'
gvlistareaLoadoutBuyList.X1 = gareaLoadoutBuyList.X1
gvlistareaLoadoutBuyList.Y1 = gareaLoadoutBuyList.Y1
gvlistareaLoadoutBuyList.X2 = gareaLoadoutBuyList.X2
gvlistareaLoadoutBuyList.Y2 = gareaLoadoutBuyList.Y2
gvlistareaLoadoutBuyList.IY = (gareaLoadoutBuyList.Y2 - gareaLoadoutBuyList.Y1) / 6
gvlistareaLoadoutBuyList.CX = (gareaLoadoutBuyList.X1 + gareaLoadoutBuyList.X2) / 2
gvlistareaLoadoutBuyList.CY0 = gvlistareaLoadoutBuyList.Y1 + gvlistareaLoadoutBuyList.IY / 2


gbuttonLoadoutBuy.X1 = gareaLoadoutBuy.X1 + 136
gbuttonLoadoutBuy.Y1 = gareaLoadoutBuy.Y1 + 315
gbuttonLoadoutBuy.X2 = gbuttonLoadoutBuy.X1 + 72
gbuttonLoadoutBuy.Y2 = gbuttonLoadoutBuy.Y1 + 25
gbuttonLoadoutBuy.CX = (gbuttonLoadoutBuy.X1 + gbuttonLoadoutBuy.X2) / 2
gbuttonLoadoutBuy.CY = (gbuttonLoadoutBuy.Y1 + gbuttonLoadoutBuy.Y2) / 2


gbuttonLoadoutInvest.X1 = gbuttonLoadoutBuy.X1
gbuttonLoadoutInvest.Y1 = gbuttonLoadoutBuy.Y1
gbuttonLoadoutInvest.X2 = gbuttonLoadoutBuy.X2
gbuttonLoadoutInvest.Y2 = gbuttonLoadoutBuy.Y2
gbuttonLoadoutInvest.CX = (gbuttonLoadoutInvest.X1 + gbuttonLoadoutInvest.X2) / 2
gbuttonLoadoutInvest.CY = (gbuttonLoadoutInvest.Y1 + gbuttonLoadoutInvest.Y2) / 2


gbuttonLoadoutToTeamRoom.X1 = 17
gbuttonLoadoutToTeamRoom.Y1 = 559
gbuttonLoadoutToTeamRoom.X2 = gbuttonLoadoutToTeamRoom.X1 + 151
gbuttonLoadoutToTeamRoom.Y2 = gbuttonLoadoutToTeamRoom.Y1 + 33
gbuttonLoadoutToTeamRoom.CX = (gbuttonLoadoutToTeamRoom.X1 + gbuttonLoadoutToTeamRoom.X2) / 2
gbuttonLoadoutToTeamRoom.CY = (gbuttonLoadoutToTeamRoom.Y1 + gbuttonLoadoutToTeamRoom.Y2) / 2


gbuttonLoadoutLaunch.X1 = 656
gbuttonLoadoutLaunch.Y2 = 559
gbuttonLoadoutLaunch.X2 = gbuttonLoadoutLaunch.X1 + 138
gbuttonLoadoutLaunch.Y2 = gbuttonLoadoutLaunch.Y1 + 41
gbuttonLoadoutLaunch.CX = (gbuttonLoadoutLaunch.X1 + gbuttonLoadoutLaunch.X2) / 2
gbuttonLoadoutLaunch.CY = (gbuttonLoadoutLaunch.Y1 + gbuttonLoadoutLaunch.Y2) / 2


SUB LoadoutGenericShip()
    '
    ' Buy a hull.
    '
    WMoveToWnd(0, ghlistareaLoadoutBuyTab1.CX0, ghlistareaLoadoutBuyTab1.CY)
    PLAY "{CLICK CURPOS, CURPOS}"
    SLEEP(1)
    WMoveToWnd(0, ghlistareaLoadoutBuyTab2.CX0, ghlistareaLoadoutBuyTab2.CY)
    PLAY "{CLICK CURPOS, CURPOS}"
    SLEEP(1)
    WMoveToWnd(0, gvlistareaLoadoutBuyList.CX, gvlistareaLoadoutBuyList.CY0)
    PLAY "{CLICK CURPOS, CURPOS}"
    SLEEP(1)
    WMoveToWnd(0, gbuttonLoadoutBuy.CX, gbuttonLoadoutBuy.CY)
    PLAY "{CLICK CURPOS, CURPOS}"
    SLEEP(1)

    '
    ' Buy a few guns.
    '
    WMoveToWnd(0, ghlistareaLoadoutBuyTab2.CX0 + 2 * ghlistareaLoadoutBuyTab2.IX, ghlistareaLoadoutBuyTab2.CY)
    PLAY "{CLICK CURPOS, CURPOS}"
    SLEEP(1)
    WMoveToWnd(0, gvlistareaLoadoutBuyList.CX, gvlistareaLoadoutBuyList.CY0)
    PLAY "{CLICK CURPOS, CURPOS}"
    SLEEP(1)
    WMoveToWnd(0, gbuttonLoadoutBuy.CX, gbuttonLoadoutBuy.CY)
    PLAY "{CLICK CURPOS, CURPOS}"
    SLEEP(1)

    WMoveToWnd(0, gvlistareaLoadoutBuyList.CX, gvlistareaLoadoutBuyList.CY0 + 1 * gvlistareaLoadoutBuyList.IY)
    PLAY "{CLICK CURPOS, CURPOS}"
    SLEEP(1)
    WMoveToWnd(0, gbuttonLoadoutBuy.CX, gbuttonLoadoutBuy.CY)
    PLAY "{CLICK CURPOS, CURPOS}"
    SLEEP(1)

    '
    ' Buy a shield.
    '
    WMoveToWnd(0, ghlistareaLoadoutBuyTab2.CX0 + 1 * ghlistareaLoadoutBuyTab2.IX, ghlistareaLoadoutBuyTab2.CY)
    PLAY "{CLICK CURPOS, CURPOS}"
    SLEEP(1)
    WMoveToWnd(0, gvlistareaLoadoutBuyList.CX, gvlistareaLoadoutBuyList.CY0)
    PLAY "{CLICK CURPOS, CURPOS}"
    SLEEP(1)
    WMoveToWnd(0, gbuttonLoadoutBuy.CX, gbuttonLoadoutBuy.CY)
    PLAY "{CLICK CURPOS, CURPOS}"
    SLEEP(1)

END SUB


