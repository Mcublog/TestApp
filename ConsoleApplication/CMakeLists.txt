cmake_minimum_required (VERSION 3.0)
     
project(App)
 
set(SOURCE_FILES "ConsoleApplication.cpp")

add_executable(App ${SOURCE_FILES}) 
find_package (Threads) 

include_directories("../LibSawAnalyzing")

target_link_libraries(App stdc++fs ${CMAKE_THREAD_LIBS_INIT} LibSawAnalyzing)
