cmake_minimum_required(VERSION 3.5)
project(RISCV_Simulator)

set(CMAKE_CXX_STANDARD 14)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
            "Choose the type of build, options are: Debug Release
RelWithDebInfo MinSizeRel."
            FORCE)
endif(NOT CMAKE_BUILD_TYPE)

add_library(RISCV_Simulator STATIC src/Memory.cpp src/Register.cpp src/Instruction.cpp src/Buffer.cpp src/Pipeline/Fetch.cpp src/Pipeline/Execute.cpp src/Pipeline/Decode.cpp src/Pipeline/MemoryAccess.cpp src/Pipeline/WriteBack.cpp header/ALU.h header/Buffer.h header/Instruction.h header/exception.h header/Memory.h header/Register.h header/Pipeline.h)
add_executable(code main.cpp)
target_link_libraries(code RISCV_Simulator)