cmake_minimum_required (VERSION 3.0)
 
set(CMAKE_CXX_STANDARD 17) 

project(LibSawAnalyzing)
 
set(SOURCE_FILES "LibSawAnalyzing.cpp")
set(HEADER_FILES "LibSawAnalyzing.h")
 
add_library(LibSawAnalyzing STATIC ${HEADER_FILES} ${SOURCE_FILES})