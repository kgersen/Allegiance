INCLUDE_DIRECTORIES(../Inc ../zlib ../_Utility ../soundengine ../engine ../Lib/directx/include ../Lib/sol/include ../Lib/lua/include)

if (MSVC)
  add_compile_options(/bigobj)
else ()
  add_compile_options(-Wa,-mbig-obj)
endif ()

ADD_LIBRARY(UI STATIC)
SET_PROPERTY(TARGET UI PROPERTY CXX_STANDARD 14)

target_sources(UI PRIVATE
    UiEngine.cpp
    UiState.cpp
    # include
    Exposer.h
    items.hpp
    ns_color.hpp
    ns_event.hpp
    ns_file.hpp
    ns_font.hpp
    ns_image.hpp
    ns_list.hpp
    ns_math.hpp
    ns_point.hpp
    ns_screen.hpp
    ns_string.hpp
    ui.h
    UiEngine.h
    UiState.h
    ui_types.h
)