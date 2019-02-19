project("PygaMasher")
cmake_minimum_required(VERSION 3.12)
set (CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Wall)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

include(FindPkgConfig)
find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-3.0)

# Add fec
add_subdirectory(external/fec)

add_executable(PygaMasher 
	commandrunner.cpp
	main.cpp
	mainwindow.cpp
	imagewriter.cpp
	imagereader.cpp 
	rsdecoder.cpp
	rsencoder.cpp
	)


target_link_libraries(PygaMasher stdc++fs pthread fec)

link_directories(${GTKMM_LIBRARY_DIRS})

target_include_directories(PygaMasher PRIVATE ${GTKMM_INCLUDE_DIRS} external)

target_link_libraries(PygaMasher ${GTKMM_LIBRARIES})
