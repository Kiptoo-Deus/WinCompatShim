cmake_minimum_required(VERSION 3.15)
project(WinCompatShim VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT WIN32)
    message(FATAL_ERROR "WinCompatShim only supports Windows")
endif()

add_library(WinCompatShim SHARED
    src/dllmain.cpp
    src/version.cpp
    src/dwm_shim.cpp
)

target_include_directories(WinCompatShim
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(WinCompatShim PRIVATE WINCOMPATSHIM_EXPORTS)

target_link_libraries(WinCompatShim PRIVATE
    dwmapi
    gdi32
    ntdll
)

set_target_properties(WinCompatShim PROPERTIES
    OUTPUT_NAME "WinCompatShim"
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)

option(BUILD_EXAMPLES "Build example applications" ON)
option(BUILD_TESTS "Build tests" ON)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

install(TARGETS WinCompatShim
    EXPORT WinCompatShimTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(FILES include/WinCompatShim.h DESTINATION include)

install(EXPORT WinCompatShimTargets
    FILE WinCompatShimTargets.cmake
    NAMESPACE WinCompat::
    DESTINATION lib/cmake/WinCompatShim
)
