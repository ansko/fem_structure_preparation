cmake_minimum_required (VERSION 2.6) 
 
set (PROJECT polygons) 
 
project (${PROJECT}) 

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -std=c++17 -g -lm  -O3")
endif (CMAKE_COMPILER_IS_GNUCXX)


set (ALL_HEADERS
    src/point.hpp
    src/convex_polygon.hpp
    src/polygonal_cylinder.hpp
    src/figures_collection.hpp
    src/utils.hpp
    src/options.hpp
)

add_executable (disks ${ALL_HEADERS} ternary_disks_mc.cpp)
