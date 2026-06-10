cmake_minimum_required(VERSION 3.16)
project(secure_bare_metal_bootloader LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

add_library(bootloader_core
  src/Bootloader.cpp
)
target_include_directories(bootloader_core PUBLIC include)
target_compile_options(bootloader_core PRIVATE -Wall -Wextra -Wpedantic)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE bootloader_core)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

add_executable(bootloader_tests tests/BootloaderTests.cpp)
target_link_libraries(bootloader_tests PRIVATE bootloader_core)
target_compile_options(bootloader_tests PRIVATE -Wall -Wextra -Wpedantic)
add_test(NAME bootloader_tests COMMAND bootloader_tests)
