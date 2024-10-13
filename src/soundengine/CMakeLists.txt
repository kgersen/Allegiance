ADD_DEFINITIONS(-D__MODULE__="SoundEngine")
INCLUDE_DIRECTORIES(../Inc ../zlib ../Lib/directx/include)

ADD_LIBRARY(SoundEngine)
SET_PROPERTY(TARGET SoundEngine PROPERTY CXX_STANDARD 14)

target_sources(SoundEngine PRIVATE
    ds3dbuffer.cpp
    ds3dengine.cpp
    ds3dutil.cpp
    ds3dvirtualbuffer.cpp
    dummysound.cpp
    soundbase.cpp
    soundtemplates.cpp
    soundutil.cpp
    #
    ds3dbuffer.h
    ds3dengine.h
    ds3dutil.h
    ds3dvirtualbuffer.h
    soundbase.h
    soundengine.h
    soundtemplates.h
    soundutil.h
)