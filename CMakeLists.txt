cmake_minimum_required(VERSION 3.10)
project(IndyCar_RaceSimulator VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCE_FILES "src/*.cpp")

include_directories(include)

find_package(Curses REQUIRED)

include_directories(${CURSES_INCLUDE_DIR})

include_directories(/usr/local/include)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})  

target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES} /usr/local/lib/libyaml-cpp.dylib)
