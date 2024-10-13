SET_MODULE("zlib")

INCLUDE_DIRECTORIES(../Inc)

ADD_LIBRARY(Zlib STATIC)
SET_PROPERTY(TARGET Zlib PROPERTY CXX_STANDARD 17)
target_sources(Zlib PRIVATE
    # cpp
    color.cpp
    event.cpp
    input.cpp
    Logger.cpp
    matrix.cpp
    matrix2.cpp
    orientation.cpp
    quaternion.cpp
    StackTracer.cpp
    tlist.cpp
    token.cpp
    vector.cpp
    Win32app.cpp
    window.cpp
    zmath.cpp
    zstring.cpp
    ztime.cpp

    # include
    alloc.h
    base.h
    cast.h
    color.h
    event.h
    fixdelete.h
    FTPSession.h
    genericlist.h
    input.h
    Logger.h
    main.h
    mask.h
    matrix.h
    olddxdefns.h
    orientation.h
    point.h
    quaternion.h
    rect.h
    StackTracer.h
    tcompare.h
    tlist.h
    TlsValue.h
    tmap.h
    token.h
    tref.h
    tstack.h
    tvector.h
    vector.h
    VersionInfo.h
    vertex.h
    Win32app.h
    window.h
    winstyles.h
    zadapt.h
    zassert.h
    zlib.h
    zmath.h
    zreg.h
    zstring.h
    ztime.h
)

IF (MSVC)
    target_sources(Zlib PRIVATE VersionInfo_msvc.cpp)
ELSE()
    target_sources(Zlib PRIVATE VersionInfo_dummy.cpp)
ENDIF()

IF(UNIX)
    target_sources(Zlib PRIVATE
        base_unix.cpp
        FTPSession_unix.cpp
    )
ELSEIF(WIN32)
    target_sources(Zlib PRIVATE
        base_win.cpp
        FTPSession_win.cpp
        )
ELSE()
    MESSAGE(FATAL_ERROR "Unhandled platform")
ENDIF()

