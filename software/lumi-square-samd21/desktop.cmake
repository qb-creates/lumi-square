cmake_minimum_required(VERSION 3.0.0)
project(lumi-square-desktop VERSION 0.1.0 LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

# Add the include directories to the project
include_directories(
    ${CMAKE_SOURCE_DIR}/src/components/Colors
    ${CMAKE_SOURCE_DIR}/src/components/Utilities
    ${CMAKE_SOURCE_DIR}/src/components/Peripherals
    ${CMAKE_SOURCE_DIR}/src/components/Managers
    ${CMAKE_SOURCE_DIR}/src/components/GameStates
    ${CMAKE_SOURCE_DIR}/src/components/Audio
    ${CMAKE_SOURCE_DIR}/src/third-party/imgui
    ${CMAKE_SOURCE_DIR}/src/third-party/imgui/GLFW
)

# Collect all .cpp files from your components
file(GLOB_RECURSE SOURCES 
    src/components/*.cpp
    src/third-party/imgui/*.cpp
    src/main.cpp
)

# Remove hardware-specific files for desktop build
list(FILTER SOURCES EXCLUDE REGEX "samd21j18utility\\.cpp$")
list(FILTER SOURCES EXCLUDE REGEX "third-party/CMSIS/.*")
list(FILTER SOURCES EXCLUDE REGEX "third-party/SAMD21J18A/.*")

# Add compile definitions
add_compile_definitions(DESKTOP_SIMULATION)
add_compile_options(-O0 -g)

add_executable(lumi-square-desktop ${SOURCES})

# Link against GLFW library
# You'll need to adjust this path to where you have GLFW
target_link_libraries(lumi-square-desktop "${CMAKE_SOURCE_DIR}/src/lib/libglfw3.a")

# Additional libraries required for GLFW on Windows
if(WIN32)
    target_link_libraries(lumi-square-desktop opengl32 gdi32)
endif()

# ImGui sources are now included in the main SOURCES glob above