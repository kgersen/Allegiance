ADD_LIBRARY(TCLib)
SET_PROPERTY(TARGET TCLib PROPERTY CXX_STANDARD 14)

target_include_directories(TCLib PRIVATE
    ../../Zlib
    ../Inc
)

target_sources(TCLib PRIVATE
    DynaLib.cpp
    ExcepInfo.cpp
    InvokeArgs.cpp
    pch.cpp
    PropertyPageSite.cpp
    RelPath.cpp
    StrManip.cpp
    TCLib.cpp
    TCThread.cpp
    UtilImpl.cpp
    UtilityThread.cpp
    WorkerThread.cpp
    
    ../Inc/TCLib.h
    AdviseHolder.h
    AutoCriticalSection.h
    AutoHandle.h
    AutoMutex.h
    BinString.h
    CoInit.h
    CommonControls.h
    CompositeValue.h
    DynaLib.h
    ExcepInfo.h
    InvokeArgs.h
    LookupTable.h
    ObjectLock.h
    pch.h
    PropertyPageSite.h
    Range.h
    RangeSet.h
    RefCountedData.h
    RelPath.h
    StrManip.h
    TCCAUUID.h
    TCThread.h
    TraceScope.h
    tstring.h
    UtilImpl.h
    UtilityThread.h
    WinApp.h
    WindowThreadBase.h
    WindowThreadImpl.h
    WorkerThread.h
    WotLib.h
)