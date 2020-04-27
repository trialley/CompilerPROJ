cmake_minimum_required (VERSION 2.8.7)
project (PL0)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(SOURCES_DIRECTORY ${PROJECT_SOURCE_DIR}/src)
set(INCLUDES_DIRECTORY ${PROJECT_SOURCE_DIR}/src)

include_directories(${INCLUDES_DIRECTORY})

#set(PL0_DIRECTORY ${SOURCES_DIRECTORY}/)
#foreach(dir ${PL0_DIRECTORY})
# get directory sources and headers
# set sources
#endforeach()


file(GLOB s_${SOURCES_DIRECTORY} "${SOURCES_DIRECTORY}/*.cpp")
file(GLOB h_${SOURCES_DIRECTORY} "${SOURCES_DIRECTORY}/*.h")
set(pl0_sources ${s_${SOURCES_DIRECTORY}} ${h_${SOURCES_DIRECTORY}})

# 测试部分
add_executable(PL0 ${pl0_sources})