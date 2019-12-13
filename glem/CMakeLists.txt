cmake_minimum_required(VERSION 3.12)

project(glem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    configure_file(CMakeListsGLFW3win32.txt.in glfw3-download/CMakeLists.txt)

    execute_process(
        COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/glfw3-download )

    if(result)
        message(FATAL_ERROR "CMake step for glfw3 failed: ${result}")
    endif()

    execute_process(
        COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/glfw3-download )

    if(result)
        message(FATAL_ERROR "Build step for glfw failed: ${result}")
    endif()

    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

    add_subdirectory(
        ${CMAKE_CURRENT_BINARY_DIR}/glfw3-src
        ${CMAKE_CURRENT_BINARY_DIR}/glfw3-build
        EXCLUDE_FROM_ALL)

    configure_file(CMakeListsGLMwin32.txt.in glm-download/CMakeLists.txt)

    execute_process(
        COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/glm-download )

    if(result)
        message(FATAL_ERROR "CMake step for glm failed: ${result}")
    endif()

    execute_process(
        COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/glm-download )

    if(result)
        message(FATAL_ERROR "Build step for glm failed: ${result}")
    endif()

    add_subdirectory(
        ${CMAKE_CURRENT_BINARY_DIR}/glm-src
        ${CMAKE_CURRENT_BINARY_DIR}/glm-build
        EXCLUDE_FROM_ALL)
elseif(UNIX)
    find_package(glfw3 REQUIRED)
    find_package(glm   REQUIRED)
endif()

find_package(OpenGL REQUIRED)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/..
    ${PROJECT_SOURCE_DIR}/libs/glad/include)

file(GLOB_RECURSE HDRS ${PROJECT_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE SRCS ${PROJECT_SOURCE_DIR}/*.cpp)

if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

    list(APPEND LIBS opengl32 glfw glm glad)
elseif(UNIX)
    list(APPEND LIBS ${OPENGL_opengl_LIBRARY} glfw glm glad ${CMAKE_DL_LIBS})
endif()

add_library(${PROJECT_NAME} SHARED ${HDRS} ${SRCS})

target_link_libraries(${PROJECT_NAME} ${LIBS})

add_subdirectory(libs)
