add_executable(AllSrv WIN32
   # FedSrv.rc
)

set_property(TARGET AllSrv PROPERTY CXX_STANDARD 14)

# <AdditionalIncludeDirectories>..\src\Lib\nlohmann\json\include;..\src\lib\AllegianceSecurity;..\src\lib\Steam;.\$(IntDir)..\Guids;.\;..\src\guids;..\src\zlib;..\src\_Utility;..\src\Igc;..\src\Inc;..\src\AGC;..\src\test\Inc;..\src\lobby;..\src\Zone;.\$(IntDir)..\Igc;.\$(IntDir)..\FedSrv;.\$(IntDir)..\AGC;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>
# <AdditionalDependencies> ;dbghelp.lib;$(OutDir)..\sharemem\sharemem.lib;urlmon.lib;..\src\Lib\DirectPlay\dxguid.lib;comctl32.lib;delayimp.lib;wininet.lib;lz32.lib;netapi32.lib;comsupp.lib
target_link_directories(AllSrv PUBLIC
    ../Lib/appweb
    ../Lib/DirectPlay # keep before directx for dxguid
)

target_link_libraries(AllSrv
PUBLIC
    SteamAPI
    AllegianceSecurity
PRIVATE
    appweb
    sharemem
    Zlib
    Igc
    TCLib
    Guids
    Utility
    Version
    winmm
    dxguid
    ws2_32
    oledb
    Rpcrt4
    Netapi32
    Wininet
)


target_include_directories(AllSrv PRIVATE
PUBLIC
    SteamAPI
    AGC
    ../Inc/appWeb
    ../Inc
    ../test/Inc
    ../Zone
    ../lobby
    ../AGC
    ${CMAKE_CURRENT_BINARY_DIR}/../AGC # fix this 
PRIVATE
    ../zlib
    ../_Utility
    ../Igc
    ../guids
)


add_compile_definitions(  
    # STEAM_APP_ID=700480
    NOMINMAX
    WINTREK
    # _USRDLL
    _DLL
    _WINDOWS
    WIN32
    _AFXDLL
    EnablePerformanceCounters
    _DISABLE_CONSTEXPR_MUTEX_CONSTRUCTOR
    ALLSRV_STANDALONE
    FEDSRV
    AGC_HOST
    #SRVLOG
    _CONSOLE
    WIN
)

target_sources(AllSrv PRIVATE
    AdminEventLoggerHook.cpp
    AdminGame.cpp
    AdminGames.cpp
    AdminServer.cpp
    AdminSession.cpp
    AdminSessionClass.cpp
    AdminSessionEventSink.cpp
    AdminShip.cpp
    AdminUser.cpp
    AdminUsers.cpp
    allsrvmodule.cpp
    BitArray.cpp
    CSteamAchievements.cpp
    CSteamUserGroupStatus.cpp
    CSteamValidation.cpp
    FedSrv.CPP
    fscluster.cpp
    fslobby.cpp
    fsmission.cpp
    fsship.cpp
    fsside.cpp
    pch.cpp
    sqlhelp.cpp
    srvqguids.cpp
  
    # AdminGame.rgs
    # AdminGames.rgs
    # AdminServer.rgs
    # AdminSession.rgs
    # AdminShip.rgs
    # AdminUser.rgs
    # AdminUsers.rgs
    # allsrv.rgs
    # AllSrv32.rgs
    # DPMono.rgs
    # fedsrv.ico
    # fedsrv.rc2
    # regini.txt
  
    AdminEventLoggerHook.h
    AdminGame.h
    AdminGames.h
    AdminServer.h
    AdminSession.h
    AdminSessionClass.h
    AdminSessionEventSink.h
    AdminSessionSecure.h
    AdminSessionSecureHost.h
    AdminShip.h
    AdminUser.h
    AdminUsers.h
    AdminUtil.h
    allsrvmodule.h
    BitArray.h
    counters.h
    CSteamAchievements.h
    CSteamUserGroupStatus.h
    CSteamValidation.h
    dbgutil.h
    FedSrv.H
    FedSrvApp.h
    fedsrvsql.h
    fscluster.h
    fsmission.h
    fsship.h
    fsside.h
    pch.h
    resource.h
    srvqueries.h
)

# copy dlls if Windows
if (WIN32)
  add_custom_command(
    TARGET AllSrv POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:AllSrv> $<TARGET_FILE_DIR:AllSrv>
    COMMAND_EXPAND_LISTS
  )
endif ()

    # <Midl>
    #   <AdditionalIncludeDirectories>.\AGC;..\src\Inc;.\$(Outdir)..\Guids;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>
    #   <TypeLibraryName>$(TargetDir)/%(FileName).tlb</TypeLibraryName>
    #   <OutputDirectory>$(TargetDir)\</OutputDirectory>
    #   <HeaderFileName>%(FileName).h</HeaderFileName>
    #   <DllDataFileName>%(Filename)_dlldata.c</DllDataFileName>
    # </Midl>


set(AllSrvModule_H
    ${CMAKE_CURRENT_BINARY_DIR}/AllSrvModuleIDL.h
)
set(AllSrvModule_IDL
    ${CMAKE_CURRENT_LIST_DIR}/AllSrvModuleIDL.idl
)
add_custom_command(
    OUTPUT ${AllSrvModule_H}
    COMMAND midl /I ../Inc /I ${CMAKE_CURRENT_BINARY_DIR} /I ../AGC /I  ${CMAKE_CURRENT_BINARY_DIR}/../AGC /h ${AllSrvModule_H} /out ${CMAKE_CURRENT_BINARY_DIR} ${AllSrvModule_IDL}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ${AllSrvModule_IDL}
    MAIN_DEPENDENCY ${AllSrvModule_IDL}
    VERBATIM
)
    
target_sources(AllSrv PRIVATE ${AllSrvModule_H})
target_include_directories(AllSrv PRIVATE ${CMAKE_CURRENT_BINARY_DIR})