# CMake 最小编译版本
CMAKE_MINIMUM_REQUIRED(VERSION 3.4.1)

# 指定需要使用的头文件
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/main/jniLibs/include)
# 指定外部链接库目录
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/main/jniLibs/armeabi-v7a)

# 添加要打包的资源
FILE(GLOB SRC_LISTS "${PROJECT_SOURCE_DIR}/src/main/cpp/*.cpp" "${PROJECT_SOURCE_DIR}/src/main/cpp/*.c")
add_library(
        yuv-sample
        SHARED
        ${SRC_LISTS}
)

# 将打包的 so 链接到项目中
target_link_libraries(
        # 目标库
        yuv-sample
        # other libs
        yuv
        # Android libs
        jnigraphics
        log
)