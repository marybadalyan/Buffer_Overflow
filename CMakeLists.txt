cmake_minimum_required(VERSION 3.10)
project(Buffer_Overflow)

# With protection
add_executable(overflow_with_protection main.cpp)

if (MSVC)
    # MSVC stack protector flag
    target_compile_options(overflow_with_protection PRIVATE /GS )
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


set(ANALYSIS_DIR "${CMAKE_BINARY_DIR}/analysis")
file(MAKE_DIRECTORY ${ANALYSIS_DIR})

if(MSVC)
    add_custom_command(
        TARGET overflow_with_protection POST_BUILD
        # This command runs after the build for the given configuration
        COMMAND cmd /c "dumpbin /DISASM \"$<TARGET_OBJECTS:overflow_with_protection>\" > \"${ANALYSIS_DIR}/assembly.txt\""
        COMMENT "Dumping assembly for $<CONFIG> build to analysis/assembly.txt"
    )
else() # For GCC/Clang
     add_custom_command(
        TARGET overflow_with_protection POST_BUILD
        COMMAND objdump -d -M intel "$<TARGET_FILE:overflow_with_protection>" > "${ANALYSIS_DIR}/assembly.txt"
        COMMENT "Dumping assembly for $<CONFIG> build to analysis/assembly.txt"
    )
endif()


if(MSVC)
    add_custom_command(
        TARGET overflow_no_protection POST_BUILD
        # This command runs after the build for the given configuration
        COMMAND cmd /c "dumpbin /DISASM \"$<TARGET_OBJECTS:overflow_no_protection>\" > \"${ANALYSIS_DIR}/assembly.txt\""
        COMMENT "Dumping assembly for $<CONFIG> build to analysis/assembly.txt"
    )
else() # For GCC/Clang
     add_custom_command(
        TARGET overflow_no_protection POST_BUILD
        COMMAND objdump -d -M intel "$<TARGET_FILE:overflow_no_protection>" > "${ANALYSIS_DIR}/assembly.txt"
        COMMENT "Dumping assembly for $<CONFIG> build to analysis/assembly.txt"
    )
endif()