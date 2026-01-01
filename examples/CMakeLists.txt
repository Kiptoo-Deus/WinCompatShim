cmake_minimum_required(VERSION 3.15)

add_executable(RoundedWindowDemo WIN32
    rounded_window_demo.cpp
)

target_link_libraries(RoundedWindowDemo PRIVATE
    WinCompatShim
    user32
    gdi32
)
