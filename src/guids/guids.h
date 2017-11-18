/*-------------------------------------------------------------------------
  Guids.h
  
  All the guids used by any component of the product are defined here
  
  Owner: 
  
  Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/
#include "objbase.h"
//#include "zoneauth.h"
//#include "zoneauthdecrypt.h"
//#include "zoneauthencrypt.h"

// FEDSRV_GUID. The guid of the dplay session application of the server
// {81662310-FCB4-11d0-A88A-006097B58FBF}
DEFINE_GUID(FEDSRV_GUID, 0x81662310, 0xfcb4, 0x11d0, 0xa8, 0x8a, 0x0, 0x60, 0x97, 0xb5, 0x8f, 0xbf);

// FEDSRV_GUID. The guid of the dplay session application of the standalone server that clients connect to
// {E4934C93-D76F-4eb7-A5BF-CC3AFB0BD108}
DEFINE_GUID(FEDSRV_STANDALONE_PRIVATE_GUID, 0xE4934C93, 0xD76F, 0x4eb7, 0xa5, 0xbf, 0xcc, 0x3a, 0xfb, 0x0b, 0xd1, 0x08);

// FEDLOBBYSERVERS_GUID. The guid of the dplay session application of the CLUB lobby that the game servers connect to
// {8FB61F5B-00B9-4640-9877-5F695B8130C5}
DEFINE_GUID(FEDLOBBYSERVERS_GUID, 0x8fb61f5b, 0xb9, 0x4640, 0x98, 0x77, 0x5f, 0x69, 0x5b, 0x81, 0x30, 0xc5);

// FEDFREELOBBYSERVERS_GUID. The guid of the dplay session application of the FREE lobby that the game servers connect to
// {797BA096-71DE-4bcb-A26A-183AF1BDF74D}
DEFINE_GUID(FEDFREELOBBYSERVERS_GUID, 0x797ba096, 0x71de, 0x4bcb, 0xa2, 0x6a, 0x18, 0x3a, 0xf1, 0xbd, 0xf7, 0x4d);

// FEDLOBBYCLIENTS_GUID. The guid of the dplay session application of the CLUB lobby that the clients connect to
// {F3E93155-7C62-48be-8CB5-B22700B939A4}
DEFINE_GUID(FEDLOBBYCLIENTS_GUID, 0xf3e93155, 0x7c62, 0x48be, 0x8c, 0xb5, 0xb2, 0x27, 0x0, 0xb9, 0x39, 0xa4);

// FEDFREELOBBYCLIENTS_GUID. The guid of the dplay session application of the FREE lobby that the clients connect to
// {F5BFFE91-B1C9-4eed-A53E-0AAD8FB526A2}
DEFINE_GUID(FEDFREELOBBYCLIENTS_GUID, 0xf5bffe91, 0xb1c9, 0x4eed, 0xa5, 0x3e, 0xa, 0xad, 0x8f, 0xb5, 0x26, 0xa2);

// FEDCLUB_GUID. The guid of the dplay session application of the lobby that the clients connect to
// {357C7F39-ECB6-4038-A27E-DB272A336315}
DEFINE_GUID(FEDCLUB_GUID, 0x357c7f39, 0xecb6, 0x4038, 0xa2, 0x7e, 0xdb, 0x27, 0x2a, 0x33, 0x63, 0x15);

// FS_DPLAY_LOBBY_GUID. currently defunct. Used with dplay lobby, which we don't support anymore
// **NOT** the guid for the lobby app!!
// {8AF97637-62F4-11d2-8801-0000F803A4CD}
DEFINE_GUID(FS_DPLAY_LOBBY_GUID, 0x8af97637, 0x62f4, 0x11d2, 0x88, 0x1, 0x0, 0x0, 0xf8, 0x3, 0xa4, 0xcd);

// WINTREK_GUID. Defunct
DEFINE_GUID(WINTREK_GUID,0x279AFA8B,0x4981,0x11CE,0xA5,0x21,0x00,0x20,0xAF,0x0B,0xE5,0x60);

// DPMIC_GUID. Defunct
DEFINE_GUID(DPMIC_GUID, 0xc6046760, 0xf024, 0x11cf, 0x89, 0xa8, 0x0, 0xa0, 0xc9, 0x5, 0x41, 0x29);

// MSRG_CLSID_DPLAY. The monolithic dplay we use until all our fixes get incorporated back into standard released dplay.
// {DA9CABC6-C724-4265-A61D-6E78EB2042B4}
DEFINE_GUID(MSRG_CLSID_DPLAY, 
0xda9cabc6, 0xc724, 0x4265, 0xa6, 0x1d, 0x6e, 0x78, 0xeb, 0x20, 0x42, 0xb4);


