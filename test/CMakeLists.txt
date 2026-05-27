# wit/test/CMakeLists.txt

# Pipeline connectivity test
add_executable(wit_lib
    test_connectivity.cpp
)
 
# wit - static library
target_link_libraries(wit_lib
    PRIVATE
    ${LIB_CODE}
)

# Register
add_test(
    NAME    wit_lib
    COMMAND $<TARGET_FILE:wit_lib>
)