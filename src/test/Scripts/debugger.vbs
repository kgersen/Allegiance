' Connect to the Pig Server
Set app = CreateObject("Pigs.Session")

' Get the collection of pigs
Set pigs = app.Pigs

' Get the first pig in the collection
Dim pig, ship
If pigs.Count Then
    Set pig = pigs(0)
    Set ship = pig.ship
Else
    Set pig = Nothing
    Set ship = Nothing
End If

Set pig = itPigs.item()

' Jump straight into the debugger
Stop

 