cmake_minimum_required(VERSION 3.12)

project(sandbox LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    link_directories(${CMAKE_BINARY_DIR}/glem)
endif()

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/../glem
)

file(GLOB HDRS ${PROJECT_SOURCE_DIR}/*.hpp)
file(GLOB SRCS ${PROJECT_SOURCE_DIR}/*.cpp)

add_executable(${PROJECT_NAME} "main.cpp" ${HDRS} ${SRCS})

target_link_libraries(${PROJECT_NAME} glem)

if(WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:glem>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/$<TARGET_FILE_NAME:glem>
    )
endif()
