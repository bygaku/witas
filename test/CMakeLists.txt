# wit/test/CMakeLists.txt

# Pipeline connectivity test
add_executable(pipe_test
        test_connectivity.cpp
)
 
# wit - static library
target_link_libraries(pipe_test
        PRIVATE
        ${LIB_CODE}
)

# Register
add_test(NAME pipe_test
        COMMAND $<TARGET_FILE:pipe_test>
)

# AudioData structure test
add_executable(test_audio_data
        test_audio_data.cpp
)

# wit - static library
target_link_libraries(test_audio_data
        PRIVATE
        ${LIB_CODE}
)

# Register
add_test(NAME test_audio_data
        COMMAND $<TARGET_FILE:test_audio_data>
)
