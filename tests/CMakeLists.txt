cmake_minimum_required(VERSION 3.15)

add_executable(WinCompatTests
    test_version.cpp
    test_dwm.cpp
    main.cpp
)

target_link_libraries(WinCompatTests PRIVATE
    WinCompatShim
)

add_test(NAME VersionTests COMMAND WinCompatTests version)
add_test(NAME DwmTests COMMAND WinCompatTests dwm)
