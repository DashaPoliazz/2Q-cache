cmake_minimum_required(VERSION 3.10)
project("2Q")

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define paths for header files
include_directories(include)

# Add the main executable
add_executable(MyExecutable src/main.cpp)
target_include_directories(MyExecutable PUBLIC include)

# Add Google Test and link libraries
add_subdirectory(googletest) # Add this line to include googletest in your project

# Add tests
file(GLOB TEST_SOURCES "tests/*.cpp") # Include all test files from the tests directory
add_executable(MyTests ${TEST_SOURCES})

# Link Google Test with the test executable, using locally built libraries
target_link_libraries(MyTests gtest gtest_main pthread)
