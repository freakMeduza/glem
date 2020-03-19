cmake_minimum_required(VERSION 3.12)

project(glem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/libs/stb
    ${PROJECT_SOURCE_DIR}/libs/glad/include
    ${PROJECT_SOURCE_DIR}/libs/glfw/include)

file(GLOB HDRS ${PROJECT_SOURCE_DIR}/*.hpp)
file(GLOB SRCS ${PROJECT_SOURCE_DIR}/*.cpp)

if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

    list(APPEND LIBS opengl32 glfw glm glad)
elseif(UNIX)
    list(APPEND LIBS ${OPENGL_opengl_LIBRARY} glfw glm glad ${CMAKE_DL_LIBS})
endif()

add_library(${PROJECT_NAME} SHARED ${HDRS} ${SRCS})

target_link_libraries(${PROJECT_NAME} ${LIBS})

add_subdirectory(libs)
