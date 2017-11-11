ADD_DEFINITIONS(-D__MODULE__="zlib")

INCLUDE_DIRECTORIES(../Inc ../Lib/steam)

SET(Zlib_SRC
    StackTracer.cpp
    VersionInfo.cpp
    Win32app.cpp
    color.cpp
    event.cpp
    input.cpp
    matrix.cpp
    matrix2.cpp
    orientation.cpp
    quaternion.cpp
    tlist.cpp
    token.cpp
    vector.cpp
    window.cpp
    zstring.cpp
    ztime.cpp
   )

IF(UNIX)
    SET(PLATFORM_SRC
        base_unix.cpp
        FTPSession_unix.cpp
        )
ELSEIF(WIN32)
    SET(PLATFORM_SRC
        base_win.cpp
        FTPSession_win.cpp
        )
ELSE()
    MESSAGE(FATAL_ERROR "Unhandled platfor")
ENDIF()

ADD_LIBRARY(Zlib ${Zlib_SRC} ${PLATFORM_SRC})
SET_PROPERTY(TARGET Zlib PROPERTY CXX_STANDARD 14)
