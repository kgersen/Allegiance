/*-------------------------------------------------------------------------
  LobbyEvents.cpp
  
  List of lobby event messages
  
  Owner: 
  
  Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/
 
#include "pch.h"

const char * g_rgszLobbyEvents[] = 
{
  "Registry String Missing/Invalid: %s; Defaulting to: %s",
  "Registry String Missing/Invalid: %s",
  "Registry Integer Missing/Invalid: %s; Defaulting to: %d",
  "Allegiance Lobby Service Handler not installed",
  "Allegiance Lobby Service stopped",
  "Bad service request",
  "Allegiance Lobby started",
  "Allegiance Lobby Initialization failed (hopefully some other more descriptive event has been fired already.",
  "Uh, where's the HKEY_CLASSES_ROOT/AppID key???",
  "AllLobby.exe is not registered. Use \"AllLobby -RegServer|Service\" to register Lobby",
  "Creating Lobby",
  "Could not resolve \"%s\" specified in the GameInfoServers reg entry to a valid ip address.\n",
  "Shutting down Lobby",
  "Initializing Lobby",
  "Could not host dplay session. Check for correct dplay installation",
  "Server %s was dropped for missing roll call.",
  "Running Lobby",
  "ODBC Error=> %s",
  "assertion failed: %s", 
  "Hacker alert! Invalid CD Key (%s) received from server %s for player %s!",
  "Attempting to remove player %s from mission cookie=%x; player not found.",
  "Corrupt player join message recieved from server %s.",
  "Join game message for player %s from server %s for mission %x, which could not be found.",
  "Corrupt player quit message recieved from server %s.",
  "Server connected: %s(%s)",
  "Server disconnected: %s.",
  "ACK!! Servers session was lost!",
  "Server %s paused, missions made invisible.",
  "Server %s unpaused, missions made visible.",
  "cTokensMax (zauth.cpp) needs to be increased",
  "Hacker alert! Invalid zone ticket received! Don't know name (since ticket is invalid), but ip address is: %s.",
  "IZoneAuthDecrypt::DecryptTicket failed for unpredicted reason: %x",
  "User (ip address %s) attempted to log into secure zone lobby with no login credentials. They probably think they're connecting to the Internet Lobby.",
  "Corrupt find player message recieved from %s(%u).",
  "Lobby received unknown message type %d from %s(%u)",
  "ACK!! Clients session was lost!",
  "Created mission, cookie=%x,\n  on %s/%s (which already had %d players),\n  requested by %s",
  "Mission gone, cookie=%x, on %s (which had %d players)",
  "AutoUpdate System has been disabled due to missing/invalid registry items",
  "AutoUpdate System deactivated: %s",
  "SQL initialization failed. Check SQLConfig value in registry.",
  "SQL Error Record:\nMessage:   %S\nProcedure: %S\nNative:    %d\nLine:      %d\nOleDB:     %S"
};



