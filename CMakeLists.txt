cmake_minimum_required(VERSION 3.10)
project(Buffer_Overflow)

# With protection
add_executable(overflow_with_protection main.cpp)

if (MSVC)
    # MSVC stack protector flag
    target_compile_options(overflow_with_protection PRIVATE /GS)
else()
    # GCC/Clang flags for stack protector and no PIE
    target_compile_options(overflow_with_protection PRIVATE -fstack-protector -no-pie -O0 -g)
endif()

# Without protection
add_executable(overflow_no_protection main.cpp)

if (MSVC)
    # Disable MSVC stack protector
    target_compile_options(overflow_no_protection PRIVATE /GS-)
else()
    target_compile_options(overflow_no_protection PRIVATE -fno-stack-protector -no-pie -O0 -g)
endif()
