cmake_minimum_required(VERSION 3.29 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

project(cpp_project-terminal_based_graphing_calculator LANGUAGES CXX)

set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

find_package(Curses REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include /usr/include ${CURSES_INCLUDE_DIR})

add_executable(calculator 
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/TUI.cpp
    ${PROJECT_SOURCE_DIR}/src/analysis_parameters.cpp
    ${PROJECT_SOURCE_DIR}/src/tokenizer.cpp
    ${PROJECT_SOURCE_DIR}/src/parser.cpp
    ${PROJECT_SOURCE_DIR}/src/ast.cpp
)

target_link_libraries(calculator ${CURSES_LIBRARIES})

# Install the calculator executable to ${CMAKE_INSTALL_PREFIX}/bin
install(TARGETS calculator DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

# Install the headers to ${CMAKE_INSTALL_PREFIX}/include
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

# Install the demo script to ${CMAKE_INSTALL_PREFIX}/bin
install(PROGRAMS ${PROJECT_SOURCE_DIR}/demo DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
