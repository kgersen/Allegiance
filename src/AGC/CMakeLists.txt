add_library(AGC SHARED)
set_property(TARGET AGC PROPERTY CXX_STANDARD 14)

add_dependencies(AGC Guids)

target_include_directories(AGC PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/Guids
    ../Igc
    ../Zlib
    ../Inc
    ../test/Inc)
    
set(AGCEVENTSFILES 
    ${CMAKE_CURRENT_BINARY_DIR}/AGCEventsCPP.h 
    ${CMAKE_CURRENT_BINARY_DIR}/AGCEventsMC.mc 
    ${CMAKE_CURRENT_BINARY_DIR}/AGCEventsRC2.rc2 
    ${CMAKE_CURRENT_BINARY_DIR}/AGCEventsRCH.h
)

add_custom_command(
  OUTPUT ${AGCEVENTSFILES}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMAND cscript /e:jscript /NoLogo XMLXForm.js AGCEvents.xml AGCEventsCPP.xsl ${CMAKE_CURRENT_BINARY_DIR}/AGCEventsCPP.h
  COMMAND cscript /e:jscript /NoLogo XMLXForm.js AGCEvents.xml AGCEventsMC.xsl  ${CMAKE_CURRENT_BINARY_DIR}/AGCEventsMC.mc
  COMMAND cscript /e:jscript /NoLogo XMLXForm.js AGCEvents.xml AGCEventsRC2.xsl ${CMAKE_CURRENT_BINARY_DIR}/AGCEventsRC2.rc2
  COMMAND cscript /e:jscript /NoLogo XMLXForm.js AGCEvents.xml AGCEventsRCH.xsl ${CMAKE_CURRENT_BINARY_DIR}/AGCEventsRCH.h
  COMMAND mc.exe -r ${CMAKE_CURRENT_BINARY_DIR}/ -h ${CMAKE_CURRENT_BINARY_DIR}/ ${CMAKE_CURRENT_BINARY_DIR}/AGCEventsMC.mc
)

target_sources(AGC PRIVATE  
    ${AGCEVENTSFILES}
)

target_link_libraries(AGC PRIVATE
    Guids
    Zlib
    Igc
    TCLib
    Utility
    Version
    Winmm
)

target_sources(AGC PRIVATE
    AdminSessionHelper.cpp
    AGC.cpp
    AGCAleph.cpp
    AGCAlephs.cpp
    AGCAsteroid.cpp
    AGCAsteroids.cpp
    AGCCommand.cpp
    AGCDBParams.cpp
    agcevent.cpp
    AGCEventData.cpp
    AGCEventDef.cpp
    AGCEventIDRange.cpp
    AGCEventIDRanges.cpp
    AGCEventLogger.cpp
    AGCGame.cpp
    AGCGameParameters.cpp
    AGCGlobal.cpp
    AGCHullType.cpp
    AGCModel.cpp
    AGCModels.cpp
    AGCOrientation.cpp
    AGCProbe.cpp
    AGCProbes.cpp
    AGCSector.cpp
    AGCSectors.cpp
    AGCShip.cpp
    AGCShips.cpp
    AGCStation.cpp
    AGCStations.cpp
    AGCTeam.cpp
    AGCTeams.cpp
    AGCVector.cpp
    AGCVersionInfo.cpp
    dlldatax.c
    pch.cpp
    TCMarshalByValue.cpp
    TCNullStream.cpp
    TCStrings.cpp
    TCUtility.cpp

    #AdminSessionHelper.rgs
    #AGC.def
    #AGC.rc2
    #AGCAleph.rgs
    #AGCAlephs.rgs
    #AGCAsteroid.rgs
    #AGCAsteroids.rgs
    #AGCCommand.rgs
    #AGCDBParams.rgs
    #agcevent.rgs
    #AGCEventIDRange.rgs
    #AGCEventIDRanges.rgs
    #AGCEventLogger.rgs
    #AGCGame.rgs
    #AGCGameParameters.rgs
    #AGCGlobal.rgs
    #AGCHullType.rgs
    #AGCHullTypes.rgs
    #AGCModel.rgs
    #AGCModels.rgs
    #AGCOrientation.rgs
    #AGCProbe.rgs
    #AGCProbes.rgs
    #AGCSector.rgs
    #AGCSectors.rgs
    #AGCShip.rgs
    #AGCShips.rgs
    #AGCStation.rgs
    #AGCStations.rgs
    #AGCTeam.rgs
    #AGCTeams.rgs
    #AGCVector.rgs
    #AGCVersionInfo.rgs
    #EventBrowser.xsl
    #TCMarshalByValue.rgs
    #TCNullStream.rgs
    #TCStrings.rgs
    #TCUtility.rgs
    #XMLXForm.js"

    AGC.rc
)
target_sources(AGC PUBLIC FILE_SET agc_headers TYPE HEADERS FILES
    AdminSessionHelper.h
    AGCAleph.h
    AGCAlephs.h
    AGCAsteroid.h
    AGCAsteroids.h
    AGCChat.h
    AGCCommand.h
    AGCCP.h
    AGCDBParams.h
    AGCDebugHook.h
    agcevent.h
    AGCEventData.h
    AGCEventDef.h
    AGCEventIDRange.h
    AGCEventIDRanges.h
    AGCEventLogger.h
    AGCEventThread.h
    AGCGame.h
    AGCGameParameters.h
    AGCGlobal.h
    AGCHullType.h
    AGCModel.h
    AGCModels.h
    AGCModule.h
    AGCOrientation.h
    AGCProbe.h
    AGCProbes.h
    AGCSector.h
    AGCSectors.h
    AGCShip.h
    AGCShips.h
    AGCSite.h
    AGCStation.h
    AGCStations.h
    AGCTeam.h
    AGCTeams.h
    AGCVector.h
    AGCVersionInfo.h
    AGCWinApp.h
    dlldatax.h
    gameparamdata.h
    IAGCAlephImpl.h
    IAGCAsteroidImpl.h
    IAGCBaseImpl.h
    IAGCBuyableImpl.h
    IAGCCollectionImpl.h
    IAGCCommonImpl.h
    IAGCDamageImpl.h
    IAGCGameImpl.h
    IAGCModelImpl.h
    IAGCProbeImpl.h
    IAGCRangeImpl.h
    IAGCRangesImpl.h
    IAGCScannerImpl.h
    IAGCSectorImpl.h
    IAGCShipImpl.h
    IAGCStationImpl.h
    IAGCTeamImpl.h
    pch.h
    resource.h
    TCMarshalByValue.h
    TCNullStream.h
    TCStrings.h
    TCUtility.h
)

