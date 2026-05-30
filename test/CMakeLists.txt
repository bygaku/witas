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

# Check whether miniaudio is linked
add_executable(test_miniaudio_link
        test_miniaudio_link.cpp
)

# wit / miniaudio - libraries
target_link_libraries(test_miniaudio_link
        PRIVATE
        ${LIB_CODE}
        miniaudio
)

# Register
add_test(NAME miniaudio_link
        COMMAND $<TARGET_FILE:test_miniaudio_link>
)

# miniaudio Audio thread test
add_executable(test_audio_thread_pipe
        test_audio_thread_pipe.cpp
)

# wit - static library
target_link_libraries(test_audio_thread_pipe
        PRIVATE
        ${LIB_CODE}
)

# Register
add_test(NAME test_audio_thread_pipe
        COMMAND $<TARGET_FILE:test_audio_thread_pipe>
)