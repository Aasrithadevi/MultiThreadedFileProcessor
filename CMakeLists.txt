cmake_minimum_required(VERSION 3.10)

# Set the project name
project(MultiThreadedFileProcessor)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the source files
set(SOURCES
    Bill.cpp
    UserManager.cpp
    main.cpp
)

# Include the header files
include_directories(${CMAKE_SOURCE_DIR})

# Add the executable
add_executable(FileProcessorApp ${SOURCES})

# Link pthread library (necessary for threading on Unix-based systems)
if(UNIX)
    target_link_libraries(FileProcessorApp pthread)
endif()
