# witas/lib/CMakeLists.txt

add_library(${LIB_CODE}
    STATIC
        src/dummy.cpp
)

include_directories(${LIB_CODE}
    PUBLIC
        src/
)