ADD_DEFINITIONS(-D__MODULE__="Utility")

INCLUDE_DIRECTORIES(../Inc ../zlib ../guids ../Lib/nlohmann/json/include)

ADD_LIBRARY(Utility STATIC)
SET_PROPERTY(TARGET Utility PROPERTY CXX_STANDARD 17)

target_sources(Utility PRIVATE
    allegdb.cpp
    CollisionEntry.cpp
    CollisionQueue.cpp
    ConfigurationStore.cpp
    CRC.cpp
    Endpoint.cpp
    FileLoader.cpp
    HitTest.cpp
    KDnode.cpp
    KDroot.cpp
    Messages.cpp
    pch.cpp
    Utility.cpp

    ConfigurationStore.h
    CRC.h
    FileLoader.h
    listwrappers.h
    Utility.h
)

