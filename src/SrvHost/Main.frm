VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form fmMain 
   Caption         =   "Allegiance Server Host"
   ClientHeight    =   7530
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   9465
   LinkTopic       =   "Form1"
   ScaleHeight     =   7530
   ScaleWidth      =   9465
   StartUpPosition =   3  'Windows Default
   Begin VB.Frame frChat 
      Caption         =   "Chat"
      Height          =   5535
      Left            =   4800
      TabIndex        =   13
      Top             =   1560
      Width           =   3975
      Begin MSComctlLib.ListView listChat 
         Height          =   3735
         Left            =   240
         TabIndex        =   17
         Top             =   840
         Width           =   3495
         _ExtentX        =   6165
         _ExtentY        =   6588
         View            =   3
         LabelWrap       =   -1  'True
         HideSelection   =   -1  'True
         AllowReorder    =   -1  'True
         _Version        =   393217
         ForeColor       =   -2147483640
         BackColor       =   -2147483643
         BorderStyle     =   1
         Appearance      =   1
         NumItems        =   3
         BeginProperty ColumnHeader(1) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
            Text            =   "Source"
            Object.Width           =   1323
         EndProperty
         BeginProperty ColumnHeader(2) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
            SubItemIndex    =   1
            Text            =   "Target"
            Object.Width           =   1323
         EndProperty
         BeginProperty ColumnHeader(3) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
            SubItemIndex    =   2
            Text            =   "Message"
            Object.Width           =   3351
         EndProperty
      End
      Begin VB.TextBox tbChatInput 
         Height          =   375
         Left            =   240
         TabIndex        =   15
         Top             =   4800
         Width           =   3495
      End
      Begin VB.CheckBox ckAllMessages 
         Caption         =   "All Messages"
         Height          =   255
         Left            =   240
         TabIndex        =   14
         Top             =   360
         Value           =   1  'Checked
         Width           =   3375
      End
   End
   Begin VB.Frame frPlayers 
      Caption         =   "Players"
      Height          =   4095
      Left            =   360
      TabIndex        =   12
      Top             =   3000
      Width           =   4095
      Begin MSComctlLib.ListView listPlayers 
         Height          =   3375
         Left            =   240
         TabIndex        =   16
         Top             =   480
         Width           =   3615
         _ExtentX        =   6376
         _ExtentY        =   5953
         View            =   3
         LabelEdit       =   1
         Sorted          =   -1  'True
         MultiSelect     =   -1  'True
         LabelWrap       =   -1  'True
         HideSelection   =   0   'False
         AllowReorder    =   -1  'True
         FullRowSelect   =   -1  'True
         _Version        =   393217
         ForeColor       =   -2147483640
         BackColor       =   -2147483643
         BorderStyle     =   1
         Appearance      =   1
         NumItems        =   3
         BeginProperty ColumnHeader(1) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
            Text            =   "Name"
            Object.Width           =   2646
         EndProperty
         BeginProperty ColumnHeader(2) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
            SubItemIndex    =   1
            Text            =   "Team"
            Object.Width           =   1764
         EndProperty
         BeginProperty ColumnHeader(3) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
            SubItemIndex    =   2
            Text            =   "Sector"
            Object.Width           =   1764
         EndProperty
      End
   End
   Begin VB.Frame frParameters 
      Caption         =   "Parameters"
      Height          =   2535
      Left            =   360
      TabIndex        =   4
      Top             =   240
      Width           =   3975
      Begin VB.VScrollBar vscrollMaxPlayers 
         Height          =   375
         Left            =   1920
         Max             =   999
         TabIndex        =   11
         Top             =   1800
         Value           =   989
         Width           =   135
      End
      Begin VB.TextBox tbMaxPlayers 
         BeginProperty DataFormat 
            Type            =   1
            Format          =   "0"
            HaveTrueFalseNull=   0
            FirstDayOfWeek  =   0
            FirstWeekOfYear =   0
            LCID            =   1033
            SubFormatType   =   1
         EndProperty
         Height          =   375
         Left            =   1560
         TabIndex        =   7
         Text            =   "10"
         ToolTipText     =   "Maximum Number of Players At Once"
         Top             =   1800
         Width           =   375
      End
      Begin VB.TextBox tbPassword 
         Height          =   375
         IMEMode         =   3  'DISABLE
         Left            =   1560
         PasswordChar    =   "*"
         TabIndex        =   6
         Text            =   "Text2"
         ToolTipText     =   "Password To Join"
         Top             =   1080
         Width           =   2055
      End
      Begin VB.TextBox tbName 
         Height          =   375
         Left            =   1560
         TabIndex        =   5
         Text            =   "Game Name"
         ToolTipText     =   "Server's Name"
         Top             =   360
         Width           =   2055
      End
      Begin VB.Label lbMaxPlayers 
         Caption         =   "Max Players"
         Height          =   375
         Left            =   240
         TabIndex        =   10
         Top             =   1800
         Width           =   975
      End
      Begin VB.Label lbPassword 
         Caption         =   "Password"
         Height          =   375
         Left            =   240
         TabIndex        =   9
         Top             =   1080
         Width           =   735
      End
      Begin VB.Label lbName 
         Caption         =   "Name"
         Height          =   375
         Left            =   240
         TabIndex        =   8
         Top             =   360
         Width           =   615
      End
   End
   Begin VB.Frame Frame1 
      Caption         =   "Controls"
      Height          =   1095
      Left            =   4800
      TabIndex        =   0
      Top             =   240
      Width           =   3975
      Begin VB.CommandButton bnStop 
         Caption         =   "Stop"
         Height          =   375
         Left            =   2760
         TabIndex        =   3
         Top             =   360
         Width           =   855
      End
      Begin VB.CommandButton bnPause 
         Caption         =   "Pause"
         Height          =   375
         Left            =   1560
         TabIndex        =   2
         ToolTipText     =   "Pause"
         Top             =   360
         Width           =   735
      End
      Begin VB.CommandButton bnPlay 
         Caption         =   "Play"
         Height          =   375
         Left            =   240
         TabIndex        =   1
         ToolTipText     =   "Play"
         Top             =   360
         Width           =   735
      End
   End
End
Attribute VB_Name = "fmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim WithEvents objSession As ALLEGIANCESERVERLib.AdminSession
Attribute objSession.VB_VarHelpID = -1
Dim objGame As ALLEGIANCESERVERLib.AdminGame

Dim UserChangeScroll As Boolean ' for tbMaxPlayers and vscrollMaxPlayers
Dim ShutdownServerAtExit As Boolean
Dim StopWasPressed As Boolean

Private Sub bnPause_Click()
    MsgBox "Does nothing now"
End Sub

Private Sub bnPlay_Click()
 On Error GoTo HandleError
    
    Set objSession = CreateObject("AllSrv.AdminSession")
    
    If objSession.WhoStartedServer Is objSession Then
        ShutdownServerAtExit = True
    Else
        ShutdownServerAtExit = False
    End If
    
    objSession.ActivateEvents EventID_AdminPage
    objSession.ActivateEvents EventID_GameCreated
    objSession.ActivateEvents EventID_GameDestroyed
    objSession.ActivateEvents EventID_LoginGame
    objSession.ActivateEvents EventID_LogoutGame
    objSession.ActivateEvents EventID_ServerShutdown
    objSession.ActivateEvents EventID_JoinTeam
    objSession.ActivateEvents EventID_LeaveTeam
    objSession.ActivateEvents EventID_ShipChangesSectors
    
    ckAllMessages_Click ' actuvate chat events as needed
    
    ' Get the server object from the session
    Dim objServer As AdminServer
    Set objServer = objSession.Server
        
    If objServer.MissionCount = 1 Then
        ConnectToExistingGame objServer.Games(0)
        Exit Sub
    Else
        If objServer.MissionCount > 1 Then
            ' this shouldn't happen
            MsgBox "More than one game already in progress"
            Exit Sub
        End If
    End If
    
    If Not IsNumeric(tbMaxPlayers.Text) Then
        MsgBox "Max Players is invalid"
        Exit Sub
    End If
    
    CreateGame tbName, tbMaxPlayers.Text, objServer
    
    ' At this point a EventID_GameCreated should happen
  
    Exit Sub
HandleError: ' ignore errors
    MsgBox Err.Description, vbOKOnly, "Error"
End Sub

Private Sub bnStop_Click()
    
    StopWasPressed = True
    
    'Dim Users As AdminUsers
    'Set Users = objGame.Users

    'For Each user In Users
     '   user.Boot
    'Next

    objGame.Kill
End Sub

Private Sub ckAllMessages_Click()
    If ckAllMessages Then
        objSession.ActivateEvents EventID_ChatMessage
    Else
        objSession.DeactivateEvents EventID_ChatMessage
    End If
End Sub

Private Sub Form_Load()
    ChangePlayingState False, ""
    UserChangeScroll = True  ' assume true, used as a parameter to vscrollMaxPlayers_Change()
    ShutdownServerAtExit = False
    StopWasPressed = False
End Sub


Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    If Not objSession Is Nothing Then
    
        Dim nResponse
        nResponse = MsgBox("Shutdown Allegiance Server?", vbYesNo, "Verify")
        
        If Not vbYes = nResponse Then
            Cancel = 1
            Exit Sub
        End If
    
        On Error GoTo HandleError
        ' for now boot them, but the server should do this automatically
        If Not objGame Is Nothing Then
            Dim Users As AdminUsers
            Set Users = objGame.Users

            For Each User In Users
               User.Boot
            Next
        End If
HandleError:

        On Error GoTo SevereError
        
        If Not objSession Is Nothing Then
            objSession.Stop  ' kill the server
        End If
        
SevereError:

    End If
End Sub

Private Sub tbChatInput_KeyDown(KeyCode As Integer, Shift As Integer)
    If KeyCode = vbKeyReturn Then
        
        Counter = 0
        
        For Each Item In listPlayers.ListItems
        
            If (Item.Selected) Then
                Counter = Counter + 1
                
                Dim objUser As AdminUser
                Set objUser = objSession.Server.LookupUser(Item.Tag)
                
                If Not objUser Is Nothing Then
                    objUser.SendMsg tbChatInput
                End If
            End If
        Next
    
        ' if no one is selected then send chat to game
        If Counter = 0 Then objGame.SendMsg tbChatInput
        tbChatInput = ""
    End If
End Sub

Private Sub tbMaxPlayers_Change()

    If tbMaxPlayers.Text = "" Then
       UserChangeScroll = False ' used as a parameter to vscrollMaxPlayers_Change()
       vscrollMaxPlayers.Value = vscrollMaxPlayers.Max
    Else
        On Error GoTo HandleError
        vscrollMaxPlayers.Value = vscrollMaxPlayers.Max - tbMaxPlayers.Text
    End If
    Exit Sub
HandleError:
      tbMaxPlayers.Text = vscrollMaxPlayers.Max - vscrollMaxPlayers.Value
End Sub



Private Sub vscrollMaxPlayers_Change()
    If UserChangeScroll Then
        tbMaxPlayers.Text = vscrollMaxPlayers.Max - vscrollMaxPlayers.Value
    End If
    UserChangeScroll = True
End Sub


'
'
Private Sub ChangePlayingState(bNewState As Boolean, strTitle As String)
    bnPause.Enabled = bNewState
    bnStop.Enabled = bNewState
    listPlayers.Enabled = bNewState
    listChat.Enabled = bNewState
    frPlayers.Enabled = bNewState
    ckAllMessages.Enabled = bNewState
    frChat.Enabled = bNewState
    listChat.Enabled = bNewState
    tbChatInput.Enabled = bNewState
    
    bnPlay.Enabled = Not bNewState
    lbName.Enabled = Not bNewState
    tbName.Enabled = Not bNewState
    lbPassword.Enabled = Not bNewState
    tbPassword.Enabled = Not bNewState
    lbMaxPlayers.Enabled = Not bNewState
    tbMaxPlayers.Enabled = Not bNewState
    vscrollMaxPlayers.Enabled = Not bNewState
    frParameters.Enabled = Not bNewState
    
    If strTitle = "" Then
        fmMain.Caption = "Allegiance Server Host"
    Else
        fmMain.Caption = strTitle + " - Allegiance Server Host"
    End If
    
End Sub

Private Sub ConnectToExistingGame(newGame As AdminGame)

    Set objGame = newGame
    
    userCount = objGame.Users.Count
    ' Fix, make this use "for each user in objGame.Users" once object model is fixed
    For i = 1 To userCount
        AddUserToList objGame.Users(i - 1)
    Next
    
    tbName = newGame.Name
    
    ChangePlayingState True, tbName
End Sub


Private Sub CreateGame(Name As String, MaxPlayers As Integer, Server As AdminServer)

    Dim objCreateParams As AGCGameParameters
    Set objCreateParams = CreateObject("AGC.GameParameters")
    
    '
    ' Let's set a couple params, note that ALL mission
    ' params are currently changable.
    '
    objCreateParams.GameName = Name
    objCreateParams.AllowJoiners = False
    objCreateParams.Lives = 10
    objCreateParams.MaxPlayers = MaxPlayers
    
    objCreateParams.MinPlayers = 1
    
    objCreateParams.Validate
   
    '
    ' Make a game with these params
    '
    Server.Games.Add objCreateParams

End Sub


Private Sub AddUserToList(ByRef objUser As AdminUser)
    Dim nextIndex As Integer

    nextIndex = listPlayers.ListItems.Count + 1
    
    id = objUser.UniqueID
    
    Key = "ID" + CStr(id)
    
    ' Add a new user to the list collection.  As you notice,
    ' the key can't be a number or a number in string format, but must have
    ' letters in it.  This took me 30 minutes to figure out
    listPlayers.ListItems.Add , Key, objUser.Name
    
    listPlayers.ListItems(Key).Tag = id
    
    If Not objUser.Ship.Team Is Nothing Then
        listPlayers.ListItems(Key).ListSubItems.Add 1, , objUser.Ship.Team.Name
    Else
        listPlayers.ListItems(Key).ListSubItems.Add 1, , " "
    End If
    
    
    If Not objUser.Ship.Sector Is Nothing Then
        listPlayers.ListItems(Key).ListSubItems.Add 2, , objUser.Ship.Sector.Name
    Else
        listPlayers.ListItems(Key).ListSubItems.Add 2, , " "
    End If
    
    listPlayers.ListItems(Key).Selected = False
    
End Sub



Private Sub UpdateSector(ShipID As Long)

    Dim objUser As AdminUser
    Dim objShip As AdminShip
    
    Set objShip = objGame.LookupShip(ShipID)
    
    If objShip Is Nothing Then Exit Sub
    
    Set objUser = objShip.User
    
    If objUser Is Nothing Then Exit Sub
    
    Dim objSector As AGCSector
    
    Set objSector = objShip.Sector
    
    If objSector Is Nothing Then Exit Sub

    ' make sure we found him
    listPlayers.ListItems("ID" + CStr(objUser.UserID)).ListSubItems(2) = objSector.Name
End Sub

Private Sub UpdateTeam(UserID As Long, Name As String)
On Error GoTo HandleError
    listPlayers.ListItems("ID" + CStr(UserID)).ListSubItems(1) = Name
HandleError: ' do nothing if no longer in list
End Sub


Private Sub AddChatToList(objEvent As AGCEvent)
    Dim nextIndex As Integer
    
    nextIndex = listChat.ListItems.Count + 1
    
    listChat.ListItems.Add nextIndex, , objEvent("SOURCE NAME")
    
    listChat.ListItems(nextIndex).ListSubItems.Add 1, , objEvent("TARGET")
    
    listChat.ListItems(nextIndex).ListSubItems.Add 2, , objEvent("MESSAGE")
    
End Sub


Private Sub RemoveUserFromList(id As Long)

    For i = 1 To listPlayers.ListItems.Count
        If listPlayers.ListItems(i).Tag = id Then
            listPlayers.ListItems.Remove i
            Exit Sub
        End If
    Next
End Sub




Public Sub objSession_OnEvent(ByVal objEvent As AGCEvent)

On Error GoTo HandleError
          
    Dim objUser As AdminUser
    Dim objTeam As AGCTeam
    
    
    Select Case objEvent.id
    
        Case EventID_ChatMessage
            
            AddChatToList objEvent
        
        Case EventID_AdminPage
            
            AddChatToList objEvent
            
        Case EventID_ShipChangesSectors
            
            UpdateSector objEvent("SHIP")
        
        Case EventID_LeaveTeam
        
            UpdateTeam objEvent("USER"), " "
            
        Case EventID_JoinTeam
        
            Set objTeam = objGame.LookupTeam(objEvent("TEAM"))
            
            If Not objTeam Is Nothing Then
                UpdateTeam objEvent("USER"), objTeam.Name
            End If
        
        Case EventID_GameCreated
        
            listPlayers.ListItems.Clear
            Set objGame = objSession.Server.Games(0)
            ChangePlayingState True, tbName
        
        Case EventID_ServerShutdown
        
            Set objGame = Nothing
            Set objSession = Nothing
            listPlayers.ListItems.Clear
            ChangePlayingState False, ""
        
        Case EventID_GameDestroyed
                    
            If StopWasPressed Then
                Set objGame = Nothing
                ChangePlayingState False, ""
                StopWasPressed = False
            Else
                If Not objSession Is Nothing Then
                    ' A user left the game which caused it to be deleted,
                    ' so now we need to remake it
                    CreateGame tbName, tbMaxPlayers.Text, objSession
                End If
            End If
            
        Case EventID_LoginGame
            
            Set objUser = objSession.LookupUser(objEvent("USER"))
            
            If Not objUser Is Nothing Then
                AddUserToList objUser
            End If
        
        Case EventID_LogoutGame
        
            RemoveUserFromList objEvent("USER")
        
        Case EventID_LogoutServer
        
            RemoveUserFromList objEvent("USER")
            
    End Select

HandleError: ' ingore errors: they are usually related to events coming in on the
             ' game's COM objects being or already destroyed.  Since we don't verify that objGame
             ' is valid before we use it, sometimes errors can occur.  In such
             ' cases we want the program to skip the event which this HandleError
             ' accomplishes.
End Sub



