CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

PROJECT(Allegiance)
ADD_DEFINITIONS(-DNOMINMAX -DUSE32bit=true)
OPTION(USE_STEAM "Use steam" OFF)

MACRO(SET_MODULE MODULE_NAME)
    ADD_DEFINITIONS(-D__MODULE__="${MODULE_NAME}")
    INCLUDE_DIRECTORIES(../Inc)
    IF (USE_STEAM)
        INCLUDE_DIRECTORIES(../Lib/steam)
    ELSE()
        ADD_DEFINITIONS(-DNO_STEAM)
    ENDIF()
ENDMACRO()

# Set architecture flags for 32-bit
if(MSVC)
    # For Visual Studio
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:IA32")
    add_compile_options(/wd4996)
else()
    # For GCC/Clang
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
endif()

ADD_SUBDIRECTORY(src)

