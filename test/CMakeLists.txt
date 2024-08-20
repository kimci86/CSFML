include(FetchContent)

set(CATCH_CONFIG_FAST_COMPILE ON CACHE BOOL "")
set(CATCH_CONFIG_NO_EXPERIMENTAL_STATIC_ANALYSIS_SUPPORT ON CACHE BOOL "")
FetchContent_Declare(Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.6.0
    GIT_SHALLOW ON)
FetchContent_MakeAvailable(Catch2)
include(Catch)

# Build Catch2 in C++17 mode to enable C++17 features
target_compile_features(Catch2 PRIVATE cxx_std_17)

# Ensure that Catch2 sources and headers are not analyzed by any tools
set_target_properties(Catch2 PROPERTIES COMPILE_OPTIONS "" EXPORT_COMPILE_COMMANDS OFF)
set_target_properties(Catch2WithMain PROPERTIES EXPORT_COMPILE_COMMANDS OFF)
get_target_property(CATCH2_INCLUDE_DIRS Catch2 INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(Catch2 SYSTEM INTERFACE ${CATCH2_INCLUDE_DIRS})

add_executable(test-csfml-system
    System/Buffer.test.cpp
    System/Clock.test.cpp
    System/Sleep.test.cpp
    System/Time.test.cpp
    System/Vector2.test.cpp
    System/Vector3.test.cpp
)
target_link_libraries(test-csfml-system PRIVATE csfml-system Catch2::Catch2WithMain)
set_target_warnings(test-csfml-system)
catch_discover_tests(test-csfml-system)

add_executable(test-csfml-window
    Window/Joystick.test.cpp
)
target_link_libraries(test-csfml-window PRIVATE csfml-window Catch2::Catch2WithMain)
set_target_warnings(test-csfml-window)
catch_discover_tests(test-csfml-window)

# Copy DLLs into the same directory
if(SFML_OS_WINDOWS AND NOT CSFML_LINK_SFML_STATICALLY)
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/copy-dlls.stamp
        COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_RUNTIME_DLLS:test-csfml-system>
            $<TARGET_RUNTIME_DLLS:test-csfml-window>
            ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/copy-dlls.stamp
        DEPENDS
            $<TARGET_RUNTIME_DLLS:test-csfml-system>
            $<TARGET_RUNTIME_DLLS:test-csfml-window>
        COMMENT "Copying DLLs for CSFML tests"
        VERBATIM
        COMMAND_EXPAND_LISTS
    )
    add_custom_target(copy-dlls-for-tests DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/copy-dlls.stamp)

    add_dependencies(test-csfml-system copy-dlls-for-tests)
    add_dependencies(test-csfml-window copy-dlls-for-tests)
endif()
