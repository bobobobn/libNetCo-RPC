project(libnetco)

set(CMAKE_CXX_STANDARD 11)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
file(GLOB_RECURSE SRC ${PROJECT_SOURCE_DIR}/src/*.cc)

set(LIBRARY_NAME libnetco)

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
add_library(${LIBRARY_NAME} SHARED ${SRC})
target_link_libraries(${LIBRARY_NAME} pthread ${Protobuf_LIBRARIES})

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
    target_link_libraries(${EXAMPLE_NAME} ${LIBRARY_NAME} pthread ${Protobuf_LIBRARIES})
endforeach()