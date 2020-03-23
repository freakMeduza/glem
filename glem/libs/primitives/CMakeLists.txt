cmake_minimum_required(VERSION 3.12)

project(primitives LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}
                    ${PROJECT_SOURCE_DIR}/../glm
)

file(GLOB HDRS ${PROJECT_SOURCE_DIR}/include/*.hpp)
file(GLOB SRCS ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_library(${PROJECT_NAME} STATIC ${HDRS} ${SRCS})

target_link_libraries(${PROJECT_NAME} glm)
