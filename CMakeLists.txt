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

set(CMAKE_DEBUG_POSTFIX d)

# Set architecture flags for 32-bit
if(MSVC)
    # For Visual Studio
    set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} /arch:IA32")
    add_compile_options(/wd4996)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else()
    # For GCC/Clang
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
endif()

# AllegianceSecurity
add_library(AllegianceSecurity STATIC IMPORTED GLOBAL)
set_property(
  TARGET AllegianceSecurity APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG
  )

set_target_properties(AllegianceSecurity PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${CMAKE_SOURCE_DIR}/src/Lib/AllegianceSecurity/AllegianceSecurityd.lib"
  )
set_property(
    TARGET AllegianceSecurity APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE
)
set_target_properties(AllegianceSecurity PROPERTIES
    IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
    IMPORTED_LOCATION_RELEASE "../Lib/AllegianceSecurity/AllegianceSecurity.lib"
)
set_target_properties(AllegianceSecurity PROPERTIES INTERFACE_INCLUDE_DIRECTORIES  "${CMAKE_SOURCE_DIR}/src/Lib/AllegianceSecurity")

ADD_SUBDIRECTORY(src)

# Steam
set(STEAM_PATH "${CMAKE_SOURCE_DIR}/src/Lib/steam")
add_library(SteamAPI SHARED IMPORTED GLOBAL)
set_target_properties(SteamAPI PROPERTIES IMPORTED_LOCATION ${STEAM_PATH}/steam_api.dll)
set_target_properties(SteamAPI PROPERTIES IMPORTED_IMPLIB ${STEAM_PATH}/steam_api.lib)
set_target_properties(SteamAPI PROPERTIES INTERFACE_INCLUDE_DIRECTORIES  ${STEAM_PATH})

