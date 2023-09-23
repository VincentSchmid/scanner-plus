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

# Find GTest
find_package(GTest REQUIRED)

# Include SANE headers
include_directories("/usr/include/sane")
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add executable and link libraries
add_executable(scanner main.cpp src/scanner.cpp src/DocumentScanner.cpp src/ScannedDocument.cpp src/ScannedPage.cpp src/imageProcessing.cpp)

# Link libraries
target_link_libraries(scanner PRIVATE ${OpenCV_LIBS})
target_link_libraries(scanner PRIVATE sane)
target_link_libraries(scanner PRIVATE unofficial::libharu::hpdf)
target_link_libraries(scanner PRIVATE cpprestsdk::cpprest)

add_executable(scanner_tests test/tests.cpp)
target_link_libraries(scanner_tests GTest::GTest GTest::Main ${OpenCV_LIBS} sane unofficial::libharu::hpdf cpprestsdk::cpprest)
