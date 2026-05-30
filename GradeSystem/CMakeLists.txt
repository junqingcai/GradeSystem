cmake_minimum_required(VERSION 3.10)

project(GradeSystem C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(GradeSystem
        main.c
        student.c
        file.c
        analysis.c)

# Source files are UTF-8. On Windows terminals, GBK output is usually safest.
if (MSVC)
    target_compile_options(GradeSystem PRIVATE
            /source-charset:utf-8
            /execution-charset:.936)
elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    if (WIN32)
        target_compile_options(GradeSystem PRIVATE
                -finput-charset=UTF-8
                -fexec-charset=GBK)
    else ()
        target_compile_options(GradeSystem PRIVATE
                -finput-charset=UTF-8
                -fexec-charset=UTF-8)
    endif ()
endif ()

# Copy data.txt to CLion's build/run directory so the program can read it.
configure_file(${CMAKE_SOURCE_DIR}/data.txt
        ${CMAKE_BINARY_DIR}/data.txt COPYONLY)
