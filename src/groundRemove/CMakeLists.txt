# cmake_minimum_required(VERSION 2.8.3)
include_directories(include)
include_directories(qt)

add_library(cloud SHARED src/cloud.cpp)
add_library(bin SHARED src/bin.cpp)
add_library(segment SHARED src/segment.cpp)
# add_library(utils SHARED src/utils.cpp) # has been defined in qt sub_derictories



set(GROUND_REMOVE_SET
    bin
    segment
    # component_clustering
    utils
    # cloud
    )
find_package(Threads REQUIRED)
add_library(groundRemove SHARED src/groundRemove.cpp)
target_link_libraries(groundRemove ${GROUND_REMOVE_SET} cloud ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

add_library(component_clustering src/component_clustering.cpp)
target_link_libraries(component_clustering ${OpenCV_LIBS} cloud ${Boost_LIBRARIES})

add_executable(Text src/test.cpp)




