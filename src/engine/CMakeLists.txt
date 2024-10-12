ADD_DEFINITIONS(-D__MODULE__="Engine" -D_ALLEGIANCE_PROD_)

IF(WIN32)
    ADD_DEFINITIONS(-DDIRECT3D_VERSION=0x0900 -DEnablePerformanceCounters)
ENDIF()

INCLUDE_DIRECTORIES(../Inc ../zlib ../_Utility ../Lib/AllegianceSecurity ../Lib/directx/include)

ADD_LIBRARY(Engine STATIC)
SET_PROPERTY(TARGET Engine PROPERTY CXX_STANDARD 14)

target_sources(Engine PRIVATE
    bounds.cpp
    bspgeo.cpp
    button.cpp
    camera.cpp
    caption.cpp
    Configuration.cpp
    context.cpp
    controls.cpp
    D3DDevice9.cpp
    dderror.cpp
    device3d.cpp
    DeviceModesDX9.cpp
    dinput.cpp
    engine.cpp
    engineapp.cpp
    enginewindow.cpp
    font.cpp
    frameimage.cpp
    geometry.cpp
    HashTable.cpp
    image.cpp
    ImageTransfer.cpp
    imagetransform.cpp
    keyframe.cpp
    LogFile.cpp
    material.cpp
    mdl.cpp
    mdlbin.cpp
    menu.cpp
    modeler.cpp
    namespace.cpp
    pane.cpp
    paneimage.cpp
    pixelformat.cpp
    popup.cpp
    RasterD3D9.cpp
    surface.cpp
    toppane.cpp
    transform.cpp
    transform2.cpp
    value.cpp
    valuetransform.cpp
    VBIBManager.cpp
    VertexGenerator.cpp
    VideoSettingsDX9.cpp
    viewport.cpp
    VRAMManager.cpp
    xfile.cpp
    # include
    bounds.h
    bspgeo.h
    button.h
    camera.h
    caption.h
    Configuration.h
    context.h
    controls.h
    D3DDevice9.h
    ddstruct.h
    DeviceModesDX9.h
    engine.h
    engineapp.h
    enginep.h
    EngineSettings.h
    enginewindow.h
    font.h
    frameimage.h
    geometry.h
    HashTable.h
    image.h
    ImageTransfer.h
    imagetransform.h
    inputengine.h
    keyframe.h
    LogFile.h
    material.h
    mdl.h
    menu.h
    model.h
    namespace.h
    pane.h
    paneimage.h
    pixelformat.h
    popup.h
    surface.h
    toppane.h
    transform.h
    UIVertexDefn.h
    value.h
    valuetransform.h
    VBIBManager.h
    VertexGenerator.h
    VideoSettingsDX9.h
    viewport.h
    VRAMManager.h
    xfile.h
)

