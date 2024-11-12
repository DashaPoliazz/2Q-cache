# Set the minimum required CMake version
cmake_minimum_required(VERSION 3.10)

# Define the project name
project("2Q")

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add include directory
include_directories(include)

# Include source files
file(GLOB SOURCES "src/*.cpp")

# Add the main executable
add_executable(${PROJECT_NAME} src/main.cpp ${SOURCES})

# Enable testing
enable_testing()

# Find Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Add test executable for A1-in queue
add_executable(A1QueueTests tests/A1QueueTests.cpp src/A1Queue.cpp)
target_link_libraries(A1QueueTests ${GTEST_LIBRARIES} pthread)
add_test(NAME A1QueueTests COMMAND A1QueueTests)

# Add test executable for Am queue
add_executable(AmQueueTests tests/AmQueueTests.cpp src/AmQueue.cpp)
target_link_libraries(AmQueueTests ${GTEST_LIBRARIES} pthread)
add_test(NAME AmQueueTests COMMAND AmQueueTests)
