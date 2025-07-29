cmake_minimum_required(VERSION 3.20)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)

# specify cross compilers and tools
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# project settings
project(samd_project C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

set(MCPU cortex-m0plus)

# Uncomment for software floating point
# add_compile_options(-mfloat-abi=soft)
add_compile_options(-mcpu=${MCPU} -mthumb)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)

add_compile_definitions(F_CPU=48000000UL)

# uncomment to mitigate c++17 absolute addresses warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-register")

if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "DebugOptimized")
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-Og -g)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    message(STATUS "No optimization, debug info included")
    add_compile_options(-O0 -g)
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}/src/third-party/CMSIS/Core
    ${CMAKE_SOURCE_DIR}/src/third-party/SAMD21J18A/include
    ${CMAKE_SOURCE_DIR}/src/third-party/SAMD21J18A/include/component
    ${CMAKE_SOURCE_DIR}/src/third-party/SAMD21J18A/include/instance
    ${CMAKE_SOURCE_DIR}/src/third-party/SAMD21J18A/include/pio
    ${CMAKE_SOURCE_DIR}/src/components/clock-generator
    ${CMAKE_SOURCE_DIR}/src/components/systick
    ${CMAKE_SOURCE_DIR}/src/config
    ${CMAKE_SOURCE_DIR}/src/components/Utilities
    ${CMAKE_SOURCE_DIR}/src/components/Colors
    ${CMAKE_SOURCE_DIR}/src/components/Peripherals
    ${CMAKE_SOURCE_DIR}/src/components/Managers
    ${CMAKE_SOURCE_DIR}/src/components/Audio
    ${CMAKE_SOURCE_DIR}/src/components/GameStates
)

# add_definitions(#[[TODO insert definitions, ex. -DUSE_HAL_DRIVER -DCORE_CM4 -DDEBUG -DSTM32H745xx]])
file(GLOB_RECURSE SOURCES src/*.cpp src/*.c)

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/samd21j18a_flash.ld)

add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-mcpu=${MCPU} -mthumb -mthumb-interwork)
add_link_options(-T ${LINKER_SCRIPT})

add_executable(firmware.elf ${SOURCES} ${LINKER_SCRIPT})

set(HEX_FILE ${PROJECT_BINARY_DIR}/firmware.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/firmware.bin)

add_custom_command(TARGET firmware.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:firmware.elf> ${HEX_FILE}
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:firmware.elf> ${BIN_FILE}
    COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")