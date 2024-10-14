SET_MODULE(Effect)

ADD_LIBRARY(effect)
SET_PROPERTY(TARGET effect PROPERTY CXX_STANDARD 14)

add_dependencies(effect Zlib Engine)

add_compile_definitions(Effect PRIVATE FixPermedia=1)

target_include_directories(effect PRIVATE ../Inc ../zlib ../engine ../soundengine ../_Utility ../Lib/directx/include)

target_sources(effect PRIVATE
    alephgeo.cpp
    combopane.cpp
    conegeo.cpp
    debrisgeo.cpp
    efapp.cpp
    efgeo.cpp
    efimage.cpp
    efpane.cpp
    efpopup.cpp
    #efsound.cpp
    efwindow.cpp
    explosion.cpp
    framedata.cpp
    gamestate.cpp
    help.cpp
    listpane.cpp
    mmlpopup.cpp
    navpane.cpp
    pagepane.cpp
    #panes.cpp
    particlegeo.cpp
    starimage.cpp
    thinggeo.cpp
    # include
    efsound.h
    combopane.h
    debrisgeo.h
    efapp.h
    efart.h
    effect.h
    efgeo.h
    efimage.h
    efpane.h
    efpopup.h
    efwindow.h
    explosion.h
    framedata.h
    gamestate.h
    help.h
    listpane.h
    particlegeo.h
    starimage.h
    thinggeo.h
)

