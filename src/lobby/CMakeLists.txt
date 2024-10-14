add_executable(AllLobby)

add_dependencies(AllLobby Guids Igc Zlib Utility sharemem)

set_property(TARGET AllLobby PROPERTY CXX_STANDARD 14)

target_compile_definitions(AllLobby PRIVATE
    NOMINMAX
    _TEST
    ALLSRV_STANDALONE
    LOBBY
    _ATL_STATIC_REGISTRY
    WIN
    _CRT_SECURE_NO_DEPRECATE
    _WIN32
)

target_link_directories(AllLobby PUBLIC
    ../Lib/DirectPlay
    ../Lib/appWeb
)

target_link_libraries(AllLobby
PUBLIC
    SteamAPI
    AllegianceSecurity
PRIVATE
    appweb
    sharemem
    Zlib
    Igc
    Guids
    Utility
    Version
    winmm
    dxguid
    Netapi32
    Wininet
    Ws2_32
)


target_include_directories(AllLobby PRIVATE
PUBLIC
    SteamAPI
    AGC
    ../Inc/appWeb
    ../Inc
    ../test/Inc
    ../Zone
    ${CMAKE_BINARY_DIR}/src/Guids # fix this 
PRIVATE
    ../zlib
    ../_Utility
    ../Igc
    ../guids
)

# copy dlls if Windows
if (WIN32)
  add_custom_command(
    TARGET AllLobby POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:AllLobby> $<TARGET_FILE_DIR:AllLobby>
    COMMAND_EXPAND_LISTS
  )
endif ()

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/lobbymsg.mc
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMAND mc.exe -r ${CMAKE_CURRENT_BINARY_DIR} -h ${CMAKE_CURRENT_BINARY_DIR}/ lobbymsg.msg
  BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/lobbymsg.h
)

target_sources(AllLobby PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}/lobbymsg.mc
    AutoUpdate.cpp
    broadcast.cpp
    client.cpp
    CSteamValidation.cpp
    Lobby.CPP
    lobbyapp.cpp
    lobbyevents.cpp
    lobqguids.cpp
    mission.cpp
    pch.cpp
    server.cpp
    zgameinfo.cpp

    alllobby.rc

    AutoUpdate.h
    broadcast.h
    client.h
    counters.h
    CSteamValidation.h
    Lobby.H
    lobbyapp.h
    LobbyAppSite.h
    lobbyevents.h
    lobbymodule.h
    lobqueries.h
    messagesall.h
    messageslc.h
    messagesls.h
    mission.h
    pch.h
    resource.h
    server.h
    zgameinfo.h
)