CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

# set(ZIG_TARGET x86-windows-gnu)
# include(${CMAKE_CURRENT_LIST_DIR}/cmake/zig-toolchain.cmake)

PROJECT(Allegiance)
OPTION(USE32bit "Build for 32-bit architecture" OFF)
IF(USE32bit)
    add_compile_definitions(USE32bit=true)
ENDIF()
add_compile_definitions(NOMINMAX)
OPTION(USE_STEAM "Use steam")

MACRO(SET_MODULE MODULE_NAME)
    add_compile_definitions(__MODULE__="${MODULE_NAME}")
    include_directories(../Inc)
    IF (USE_STEAM)
        include_directories(../Lib/steam)
    ELSE()
        add_compile_definitions(NO_STEAM)
    ENDIF()
ENDMACRO()

# set(CMAKE_DEBUG_POSTFIX d)

OPTION(USE_RAYLIB "Use Raylib for rendering" ON)

IF(USE_RAYLIB)
    include(FetchContent)
    FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/5.0.tar.gz
    )
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(USE_WAYLAND ON CACHE BOOL "" FORCE)
    # GLFW flags
    set(GLFW_BUILD_WAYLAND ON CACHE BOOL "" FORCE)
    set(GLFW_BUILD_X11 OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(raylib)
    add_compile_definitions(USE_RAYLIB)
ENDIF()

# Set architecture flags
if(MSVC)
    # For Visual Studio
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:IA32")
    add_compile_options(/wd4996)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else()
    # For GCC/Clang
    if(USE32bit)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
    endif()
endif()

# AllegianceSecurity
IF(WIN32)
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
        IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/src/Lib/AllegianceSecurity/AllegianceSecurity.lib"
    )
    set_target_properties(AllegianceSecurity PROPERTIES INTERFACE_INCLUDE_DIRECTORIES  "${CMAKE_SOURCE_DIR}/src/Lib/AllegianceSecurity")
ELSE()
    add_library(AllegianceSecurity INTERFACE)
ENDIF()

ADD_SUBDIRECTORY(src)

# Steam
IF(WIN32)
    set(STEAM_PATH "${CMAKE_SOURCE_DIR}/src/Lib/steam")
    add_library(SteamAPI SHARED IMPORTED GLOBAL)
    set_target_properties(SteamAPI PROPERTIES IMPORTED_LOCATION ${STEAM_PATH}/steam_api.dll)
    set_target_properties(SteamAPI PROPERTIES IMPORTED_IMPLIB ${STEAM_PATH}/steam_api.lib)
    set_target_properties(SteamAPI PROPERTIES INTERFACE_INCLUDE_DIRECTORIES  ${STEAM_PATH})
ELSE()
    add_library(SteamAPI INTERFACE)
ENDIF()

