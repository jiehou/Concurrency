cmake_minimum_required (VERSION 2.6)
project (learn_concurrency)

#dirctory headers
#include_directories("include")
include_directories(".")

#Files
file(GLOB SOURCES "./*/*.cpp")

#compile flags
set(CMAKE_CXX_FLAGS "-Wall -std=c++11 -g -pedantic")
set(CMAKE_CXX_FLAGS -pthread)

foreach( sourcefile ${SOURCES} )
	get_filename_component( demo_name ${sourcefile} NAME_WE )
	add_executable( ${demo_name} ${sourcefile} )	
endforeach( sourcefile ${SOURCES} )
