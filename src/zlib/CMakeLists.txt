SET_MODULE("zlib")

INCLUDE_DIRECTORIES(../Inc)

ADD_LIBRARY(Zlib ${Zlib_SRC} ${PLATFORM_SRC})
SET_PROPERTY(TARGET Zlib PROPERTY CXX_STANDARD 17)
target_sources(Zlib
    PRIVATE
    StackTracer.cpp
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
    zmath.cpp
    zstring.cpp
    ztime.cpp
   )

IF (MSVC)
    SET(Zlib_SRC ${Zlib_SRC} VersionInfo_msvc.cpp)
ELSE()
    SET(Zlib_SRC ${Zlib_SRC} VersionInfo_dummy.cpp)
ENDIF()

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
    MESSAGE(FATAL_ERROR "Unhandled platform")
ENDIF()

