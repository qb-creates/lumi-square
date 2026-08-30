cmake_minimum_required(VERSION 3.0.0)
project(lumi-square-desktop VERSION 0.1.0 LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

# Makes it so that errors appear in different colors when building.
add_compile_options(-fdiagnostics-color=always)

# Add the include directories to the project
include_directories(
    ${CMAKE_SOURCE_DIR}/src/components/Colors
    ${CMAKE_SOURCE_DIR}/src/components/Utilities
    ${CMAKE_SOURCE_DIR}/src/components/Utilities/Desktop
    ${CMAKE_SOURCE_DIR}/src/components/Peripherals
    ${CMAKE_SOURCE_DIR}/src/components/Managers
    ${CMAKE_SOURCE_DIR}/src/components/GameStates
    ${CMAKE_SOURCE_DIR}/src/components/Audio
    ${CMAKE_SOURCE_DIR}/src/third-party/imgui
    ${CMAKE_SOURCE_DIR}/src/third-party/imgui/GLFW
    ${CMAKE_SOURCE_DIR}/src/third-party/miniaudio
)

file(GLOB COMMON_SOURCES
    src/components/Audio/*.cpp
    src/components/Colors/*.cpp
    src/components/GameStates/*.cpp
    src/components/GameStates/GameOver/*.cpp
    src/components/GameStates/LightDash/*.cpp
    src/components/GameStates/MainMenu/*.cpp
    src/components/GameStates/MemoryMatching/*.cpp
    src/components/GameStates/PowerOn/*.cpp
    src/components/GameStates/Simon/*.cpp
    src/components/Managers/*.cpp
    src/components/Peripherals/*.cpp
    src/components/Utilities/*.cpp
)

file(GLOB_RECURSE HARDWARE_SOURCES
    src/third-party/miniaudio/*.cpp
    src/third-party/miniaudio/*.c
    src/third-party/imgui/*.cpp
    src/third-party/imgui/*.c
    src/components/Utilities/Desktop/*.cpp
)

# Add compile definitions
add_compile_definitions(DESKTOP_SIMULATION)
add_compile_options(-O0 -g)

add_executable(lumi-square-desktop src/main.cpp ${COMMON_SOURCES} ${HARDWARE_SOURCES})

# Link against GLFW library
# You'll need to adjust this path to where you have GLFW
target_link_libraries(lumi-square-desktop "${CMAKE_SOURCE_DIR}/src/lib/libglfw3.a")

# Additional libraries required for GLFW on Windows
if(WIN32)
    target_link_libraries(lumi-square-desktop opengl32 gdi32)
endif()

# ImGui sources are now included in the main SOURCES glob above