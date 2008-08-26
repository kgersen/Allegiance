Set accts = CreateObject("Pigs.AccountDispenser")

' Jump into the debugger
Stop


Sub List(names)

  c = 0
  For Each name In names
    WScript.Echo name
    c = c + 1
  Next
  WScript.Echo c & " names listed"

End Sub
