project(libnetco)

set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
file(GLOB_RECURSE SRC ${PROJECT_SOURCE_DIR}/src/*.cc)

set(LIBRARY_NAME libnetco)

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
find_path(ZOOKEEPER_INCLUDE_DIR zookeeper/zookeeper.h)
find_library(ZOOKEEPER_LIBRARY NAMES zookeeper_mt)
find_package(Threads REQUIRED)
include_directories(${ZOOKEEPER_INCLUDE_DIR})
add_library(${LIBRARY_NAME} SHARED ${SRC})
target_compile_definitions(${LIBRARY_NAME} PRIVATE -DTHREADED)
target_link_libraries(${LIBRARY_NAME} pthread ${Protobuf_LIBRARIES} ${ZOOKEEPER_LIBRARY} Threads::Threads)
file(GLOB EXAMPLES "${PROJECT_SOURCE_DIR}/example/*.cc")
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/rpc_proto)
# set(SRC ${SRC} ${MAIN})
# file(GLOB SRC )

foreach(EXAMPLE_FILE ${EXAMPLES})
    # 获取文件名（不带路径和扩展名）
    get_filename_component(EXAMPLE_NAME ${EXAMPLE_FILE} NAME_WE)
    # 添加可执行文件
    add_executable(${EXAMPLE_NAME} ${EXAMPLE_FILE})
    target_link_libraries(${EXAMPLE_NAME} ${LIBRARY_NAME} pthread ${Protobuf_LIBRARIES} ${ZOOKEEPER_LIBRARY} Threads::Threads)
    target_compile_definitions(${EXAMPLE_NAME} PRIVATE -DTHREADED)
endforeach()