project("PygaMasher")
cmake_minimum_required(VERSION 3.13)
set (CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Wall)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

include(FindPkgConfig)
find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-3.0)

add_executable(PygaMasher main.cpp mainwindow.cpp imagewriter.cpp imagereader.cpp  commandrunner.cpp)
target_link_libraries(PygaMasher stdc++fs pthread)

link_directories(${GTKMM_LIBRARY_DIRS})

include_directories(${GTKMM_INCLUDE_DIRS})

target_link_libraries(PygaMasher ${GTKMM_LIBRARIES})
