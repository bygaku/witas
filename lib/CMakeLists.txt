# witas/lib/CMakeLists.txt

# Fetch third-party data first
add_subdirectory(external)

set(LIB_SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/audio_engine.cpp
)

set(LIB_INCLUDE_HEADER
        ${CMAKE_CURRENT_SOURCE_DIR}/src/audio_basics
        ${CMAKE_CURRENT_SOURCE_DIR}/src/format
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core
)

add_library(${LIB_CODE}
    STATIC
        src/dummy.cpp
        ${LIB_SOURCE}
)

target_include_directories(${LIB_CODE} PRIVATE
        ${LIB_INCLUDE_HEADER}
)
target_include_directories(${LIB_CODE} INTERFACE
        ${LIB_INCLUDE_HEADER}
)

# This target don't open to the public to users
target_link_libraries(${LIB_CODE}
        PRIVATE
        miniaudio
)
