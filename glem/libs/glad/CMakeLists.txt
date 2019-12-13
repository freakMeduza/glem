cmake_minimum_required(VERSION 3.12)

project(glad LANGUAGES C)

file(GLOB HDRS ${PROJECT_SOURCE_DIR}/include/*.h)
file(GLOB SRCS ${PROJECT_SOURCE_DIR}/src/*.c)

if(UNIX)
    add_definitions("-fPIC")
endif()

add_library(${PROJECT_NAME} STATIC ${HDRS} ${SRCS})
