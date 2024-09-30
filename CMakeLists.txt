cmake_minimum_required(VERSION 3.28.3)
project(myProject)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)
add_library(async SHARED  asyncHandler.cpp TaskHandler.cpp TaskOutputer.cpp)
add_executable(bulk main.cpp asyncHandler.cpp TaskHandler.cpp TaskOutputer.cpp)

set_target_properties(async PROPERTIES
                    CXX_STANDARD 20
                    CXX_STANDARD_REQUIRED ON)


set_target_properties(bulk PROPERTIES
                CXX_STANDARD 20
                CXX_STANDARD_REQUIRED ON)
