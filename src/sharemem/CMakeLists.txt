ADD_LIBRARY(sharemem)
SET_PROPERTY(TARGET sharemem PROPERTY CXX_STANDARD 17)

target_include_directories(sharemem PRIVATE
    ../Inc
)
target_sources(sharemem PRIVATE
    pch.cpp
    sharemem.cpp
    pch.h
)

