
cmake_minimum_required (VERSION 3.13)

set (CMAKE_CXX_FLAGS "-g -std=c++0x")

set(CMAKE_PREFIX_PATH "../../build")

link_directories(../../build)

find_package (vsomeip3 3.1.37 REQUIRED)
find_package( Boost 1.55 COMPONENTS system thread log REQUIRED )

include_directories (
    ${Boost_INCLUDE_DIR}
    ${VSOMEIP_INCLUDE_DIRS}
)

add_executable(client client.cpp)
target_link_libraries(client vsomeip3 vsomeip3-cfg vsomeip3-sd ${Boost_LIBRARIES})
