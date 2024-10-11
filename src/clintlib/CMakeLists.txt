add_library(ClintLib)
set_property(TARGET ClintLib PROPERTY CXX_STANDARD 14)
target_include_directories(ClintLib PRIVATE
    ../guids
    ../zlib
    ../_Utility
    ../Igc
    ../Zone
    ../Inc
    ../lobby
    ../Lib/AllegianceSecurity
    ../Lib/Steam
)

target_sources(ClintLib PRIVATE
    appmsg.cpp
    AutoDownload.cpp
    badwords.cpp
    clintlib.cpp
    pch.cpp

    AutoDownload.h
    badwords.h
    clintlib.h
    pch.h
)

