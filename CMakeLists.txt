cmake_minimum_required(VERSION 3.10)
project(TEST01)

# 查找 OpenCV 包
find_package(OpenCV REQUIRED)

# 设置包含目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/inc  # 包含 s.hpp
    ${OpenCV_INCLUDE_DIRS}
)

# 显式列出 src 目录下的源文件
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/a.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/test.cpp
)

# 添加可执行文件
add_executable(test01 ${SOURCES})

# 链接 OpenCV 库
target_link_libraries(test01 ${OpenCV_LIBS})