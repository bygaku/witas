# wit/test/test_audio_engine/CMakeLists.txt

add_executable(test_audio_engine
        test_audio_engine.cpp
)

# wit - static library
target_link_libraries(test_audio_engine
        PRIVATE
        ${LIB_CODE}
)

find_package(Threads
        REQUIRED
)

target_link_libraries(test_audio_engine
        PRIVATE
        Threads::Threads
)

# Register
add_test(NAME test_audio_engine
        COMMAND $<TARGET_FILE:test_audio_engine>
)

add_executable(test_audio_engine_float
        test_audio_engine_float.cpp
)

# wit - static library
target_link_libraries(test_audio_engine_float
        PRIVATE
        ${LIB_CODE}
)

find_package(Threads
        REQUIRED
)

target_link_libraries(test_audio_engine_float
        PRIVATE
        Threads::Threads
)

# Register
add_test(NAME test_audio_engine_float
        COMMAND $<TARGET_FILE:test_audio_engine_float>
)