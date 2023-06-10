cmake_minimum_required(VERSION 3.10)

# Set the project name and the required C++ standard
project(scanner_project LANGUAGES CXX)

# Require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Find OpenCV
find_package(OpenCV REQUIRED)

# Find libharu
find_package(unofficial-libharu CONFIG REQUIRED)

find_package(cpprestsdk REQUIRED)

# Include SANE headers
include_directories("/usr/include/sane")

# Add executable and link libraries
add_executable(scanner main.cpp scanner.cpp DocumentScanner.cpp ScannedDocument.cpp ScannedPage.cpp)

# Link libraries
target_link_libraries(scanner PRIVATE ${OpenCV_LIBS})
target_link_libraries(scanner PRIVATE sane)
target_link_libraries(scanner PRIVATE unofficial::libharu::hpdf)
target_link_libraries(scanner PRIVATE cpprestsdk::cpprest)
