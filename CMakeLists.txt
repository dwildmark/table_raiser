cmake_minimum_required(VERSION 3.9)
project(table_raiser)

if (NOT CMAKE_TOOLCHAIN_FILE)
    message(FATAL_ERROR "Forgot to add -DCMAKE_TOOLCHAIN_FILE=<toolchain>?")
endif ()

if ((NOT ${CMAKE_C_COMPILER_ID} STREQUAL "GNU") OR
    (NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU") OR
    (${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 7.2.1))
    message(WARNING "This project is targeted for ARM GCC versions 7.2.1 or above. Some features may not be supported\
     by your compiler. You are using:
     ${CMAKE_CXX_COMPILER_ID} C, version ${CMAKE_C_COMPILER_VERSION}
     ${CMAKE_CXX_COMPILER_ID} CXX, version ${CMAKE_CXX_COMPILER_VERSION}")
endif()

# Set the C/C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

# Verbose logging
set(LOG_VERBOSE true)

# Choose target device here
set(DEVICE STM32F303VC)

# Include libopencm3
include(cmake/libopencm3.cmake)

# Include doxygen for dank docs
include(cmake/doxygen.cmake)

# Include additional compilation flags
include(cmake/compile_flags.cmake)

# Collect sources into SOURCE_FILES variable and include your headers directory
file(GLOB_RECURSE SOURCE_FILES "table_raiser/*.c")
include_directories(table_raiser)

# Specify output executable
add_executable(${PROJECT_NAME}.elf ${SOURCE_FILES})

# Output elf file size
add_custom_command(
    TARGET ${PROJECT_NAME}.elf
    POST_BUILD
    COMMAND ${ARM_SIZE} ${PROJECT_NAME}_${CMAKE_BUILD_TYPE}.elf
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

add_custom_command(
    OUTPUT flash_elf
    COMMAND ../flash.sh ${PROJECT_NAME}_${CMAKE_BUILD_TYPE}.elf
)

add_custom_target(flash
                  DEPENDS flash_elf
)

message("----------------------------------------")
