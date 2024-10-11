add_executable(WinTrek WIN32)
set_property(TARGET WinTrek PROPERTY CXX_STANDARD 17)

target_link_libraries(WinTrek PRIVATE
#    ../Lang\Usa\allegiance\allegiance.rc2
  Utility
  effect
  Engine
  ../Guids
  Igc
  SoundEngine
  training
  Zlib
)


target_include_directories(WinTrek PRIVATE
    ../Lib/AllegianceSecurity
    ../Lib/Steam
    ../Lib/sol/include
    ../Lib/lua/include
    ../effect
    ../engine
    ../ui
    ../zlib
    ../_Utility
    ../Igc
    ../clintlib
    ../soundengine
    ../AGC
    ../FedSrv
    ../training
    ../lobby
    ../Inc
    ../guids
    ../Zone
    ../AGC
    ../FedSrv
    ../Guids
    ../Inc
    ../Lang/NonIntl/allegiance
    ../Lang/USA/allegiance
    ../Lib/directx/include
)


add_compile_definitions(  
    # STEAM_APP_ID=700480
    NOMINMAX
    WINTREK
    _USRDLL
    DLL
    DIRECTINPUT_VERSION=0x0800
    _WINDOWS
    WIN32
    AFXDLL
    EnablePerformanceCounters
)

target_sources(WinTrek PRIVATE
    CallsignTagInfo.cpp
    cdkeydialog.cpp
    CharInfoScreen.cpp
    chatpane.cpp
    cmdview.cpp
    console.cpp
    consoledata.cpp
    CreateGameDialog.h
    ddstream.cpp
    ddvideo.cpp
    downloader.cpp
    gameoverscreen.cpp
    gamescreen.cpp
    gamesite.cpp
    gamestartingscreen.cpp
    gametypes.cpp
    hangarscreen.cpp
    indicator.cpp
    introscreen.cpp
    inventoryimage.cpp
    leaderboardscreen.cpp
    load.cpp
    loadout.cpp
    logon.cpp
    mappreview.cpp
    mappvmaker.cpp
    netinfo.cpp
    newgamescreen.cpp
    passworddialog.cpp
    pch.cpp
    PostTrainingSlideshow.cpp
    radarimage.cpp
    sectormap.cpp
    selpane.cpp
    Slideshow.cpp
    soundinit.cpp
    splashscreen.cpp
    SquadsScreen.cpp
    SteamClans.cpp
    teampane.cpp
    teamscreen.cpp
    teleportpane.cpp
    trainingscreen.cpp
    TrainingSlideshow.cpp
    trekapp.cpp
    trekctrls.cpp
    trekigc.cpp
    trekinput.cpp
    trekmdl.cpp
    treksound.cpp
    WinTrek.cpp
    ZoneClubScreen.cpp
    zoneeventsscreen.cpp
    ZoneSquad.cpp

    artwork.h
    CallsignTagInfo.h
    cmdview.h
    console.h
    consoledata.h
    ddstream.h
    ddvideo.h
    downloader.h
    gamesite.h
    gametypes.h
    hudimage.h
    indicator.h
    load.h
    loadout.h
    logon.h
    mappreview.h
    mappvmaker.h
    passworddialog.h
    pch.h
    radarimage.h
    screen.h
    sectorvw.h
    Slideshow.h
    SteamClans.h
    trekctrls.h
    TrekDlgs.h
    Treki.h
    trekinput.h
    trekmdl.h
    treksound.h
    WinTrek.H
    WinTrekp.h
    ZoneSquad.h

    ../Lang/Usa/allegiance/resource.h

)
