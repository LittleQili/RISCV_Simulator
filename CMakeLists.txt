cmake_minimum_required(VERSION 3.13)
project(RISCV_Simulator)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_FLAGS "-O3")

#add_library(RISCV_Simulator src/Memory.cpp src/Register.cpp src/Instruction.cpp src/Buffer.cpp src/Parallel.cpp src/Pipeline/Fetch.cpp src/Pipeline/Execute.cpp src/Pipeline/Decode.cpp header/ALU.h header/Buffer.h header/Instruction.h header/exception.h header/Memory.h header/Register.h header/Parallel.h)
add_executable(RISCV_Simulator main.cpp src/Memory.cpp src/Register.cpp src/Instruction.cpp src/Buffer.cpp src/Parallel.cpp src/Pipeline/Fetch.cpp src/Pipeline/Execute.cpp src/Pipeline/Decode.cpp header/ALU.h header/Buffer.h header/Instruction.h header/exception.h header/Memory.h header/Register.h header/Parallel.h)
#target_link_libraries(code RISCV_Simulator)