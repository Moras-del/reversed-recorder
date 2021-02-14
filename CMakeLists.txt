cmake_minimum_required(VERSION 3.17)
project(reversed_recorder)

set(CMAKE_CXX_STANDARD 14)
file(GLOB SOURCE_FILES src/*)
add_executable(reversed_recorder ${SOURCE_FILES})
target_link_libraries(reversed_recorder pthread sndfile portaudio)
