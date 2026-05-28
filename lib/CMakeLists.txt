# witas/lib/CMakeLists.txt
set(LIB_SOURCE
)

set(LIB_INCLUDE_HEADER
        ${CMAKE_CURRENT_SOURCE_DIR}/src/audio_basics
        ${CMAKE_CURRENT_SOURCE_DIR}/src/format
)

add_library(${LIB_CODE}
    STATIC
        src/dummy.cpp
        ${LIB_SOURCE}
)

target_include_directories(${LIB_CODE} PRIVATE
        ${LIB_INCLLUDE_HEADER}
)
target_include_directories(${LIB_CODE} INTERFACE
        ${LIB_INCLLUDE_HEADER}
)