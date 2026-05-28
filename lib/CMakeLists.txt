# witas/lib/CMakeLists.txt

add_library(${LIB_CODE}
    STATIC
        src/dummy.cpp
)

target_include_directories(${LIB_CODE} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/audio_basics
        ${CMAKE_CURRENT_SOURCE_DIR}/src/format
)
target_include_directories(${LIB_CODE} INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/audio_basics
        ${CMAKE_CURRENT_SOURCE_DIR}/src/format
)