//////////////////////////////////////////////////////////////
// This searches for commands (listed in ChatCommandList) and calls
// OnReceiveCommand if one is found.

var StatusReceiveList = new ActiveXObject("Scripting.Dictionary");
var ElizaEnabled = 0;

function OnReceiveChat(strText, objShip)
{
  var handled = 0;
  strText = strText.toLowerCase();
  for (key in ChatCommandList)
  {
    if (strText.search(ChatCommandList[key]) == 0)
    {
      OnReceiveCommand(strText.substr(0, ChatCommandList[key].length), strText.substr(ChatCommandList[key].length + 1), objShip);
      handled = 1;
    }
  }
  try
  {
    if (handled == 0 && ElizaEnabled == 1 && objShip.Type != "pig")
      objShip.SendChat(enter_text(strText));
  }
  catch (e)
  {
    Trace("\n\n\n********** Eliza Broke!!! ***********\n" + e.description + "\n\n");
  }
  return true;
}

function AddShipToStatusList(objShip)
{
  try
  {
    StatusReceiveList.Add(objShip, "chat");
    return true;
  }
  catch(e)
  {
    return false;
  }
}

function RemoveShipFromStatusList(objShip)
{
  try
  {
    StatusReceiveList.Remove(objShip);
    return true;
  }
  catch(e)
  {
    return false;
  }
}

// StatusMessage sends a chat to wing 9, and all recipients that requested status messages
// with the "status on" command.
function StatusMessage(strText)
{
  // send message to wing 10
  Ship.Team.SendChat(strText, 9);

  // get recipient list
  recipients = (new VBArray(StatusReceiveList.Keys())).toArray();

  // return if there's nobody in the recipient list
  if (recipients.Count == 0)
    return;

  for (key in recipients)
  {
    try
    {
      if (recipients[key].WingID != 10)
        recipients[key].SendChat(strText);
    }
    catch(e)
    {
    }
  }
}
