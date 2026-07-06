cmake_minimum_required(VERSION 3.28)

project(ProjectUntitled LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_DIR "C:/SFML/lib/cmake/SFML")

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System Audio)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/*.cpp
)

add_executable(ProjectUntitled ${SOURCES})

target_include_directories(ProjectUntitled PRIVATE src)

target_link_libraries(ProjectUntitled PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
)