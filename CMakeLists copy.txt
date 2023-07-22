cmake_minimum_required(VERSION 3.10)

# Set the project name and the required C++ standard
project(scanner_project LANGUAGES CXX)

# Require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Add executable and link libraries
add_executable(scanner main.cpp scanner.cpp)
target_link_libraries(scanner PRIVATE ${OpenCV_LIBS})

# Include SANE headers
target_include_directories(scanner PRIVATE "/usr/include/sane")

# Link against SANE library
target_link_libraries(scanner PRIVATE sane)

find_package(unofficial-libharu CONFIG REQUIRED)
target_link_libraries(scanner PRIVATE unofficial::libharu::hpdf)
