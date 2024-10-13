ADD_LIBRARY(Guids)
SET_PROPERTY(TARGET Guids PROPERTY CXX_STANDARD 17)

target_sources(Guids PRIVATE
    guids.cpp
    guids.h
)

