# find_package(Lua 5.4 REQUIRED)
# find_package(Sol2 4.0.0 REQUIRED)

add_executable(Allegiance WIN32
    ../Lang/Usa/allegiance/allegiance.rc
)

add_dependencies(Igc Guids Zlib Utility Engine effect SoundEngine UI Training ClintLib)

target_link_directories(Allegiance PUBLIC
    ../Lib/DirectPlay # keep before directx for dxguid
    ../Lib/directx/Lib/x86
    ../Lib/lua
    ../Lib/OggVorbis
)

set_property(TARGET Allegiance PROPERTY CXX_STANDARD 14)

target_link_libraries(Allegiance
PUBLIC
    SteamAPI
    AllegianceSecurity
PRIVATE
    Utility
    effect
    Engine
    Guids
    ClintLib
    Igc
    SoundEngine
    Training
    Zlib
    UI
    lua53
    # dbghelp
    d3d9
    d3dx9
    dsound
    dinput8
    dxguid
    urlmon
    winmm
    comctl32
    version
    ws2_32
    delayimp
    wininet
    lz32
    netapi32
    # comsupp
    # oledb
    vorbis_nonsse2
    ogg_nonsse2
    file_nonsse2
)


target_include_directories(Allegiance PRIVATE
PUBLIC
    SteamAPI
PRIVATE
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
    WINTREK
    # _USRDLL
    _DLL
    DIRECTINPUT_VERSION=0x0800
    _WINDOWS
    WIN32
    _AFXDLL
    EnablePerformanceCounters
)

target_sources(Allegiance PRIVATE
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

# copy dlls if Windows
if (WIN32)
  add_custom_command(
    TARGET Allegiance POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:Allegiance> $<TARGET_FILE_DIR:Allegiance>
    COMMAND_EXPAND_LISTS
  )
endif ()