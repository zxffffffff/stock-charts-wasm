#############################################################################
## MIT License
## 
## Author	: xiaofeng.zhu
## Support	: zxffffffff@outlook.com, 1337328542@qq.com
## 
#############################################################################

message("[stock-charts-wasm] -------- start")
cmake_minimum_required (VERSION 3.16)

add_definitions(-std=c++17)
set(CMAKE_CXX_STANDARD 17)

# 使用 emsdk 工具链，需要在 project 之前设置
set(CMAKE_TOOLCHAIN_FILE "C:/Users/xiaofeng.zhu/Documents/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake")
message(STATUS "[stock-charts-wasm] CMAKE_TOOLCHAIN_FILE : ${CMAKE_TOOLCHAIN_FILE}")

project ("stock-charts-wasm")

message(STATUS "[stock-charts-wasm] CMAKE_CURRENT_SOURCE_DIR : ${CMAKE_CURRENT_SOURCE_DIR}")

set(ALL_INCLUDE
    "C:/Users/xiaofeng.zhu/Documents/emsdk/upstream/emscripten/cache/sysroot/include" # emsdk
    ${CMAKE_CURRENT_SOURCE_DIR}/stock-charts-cpp/lib-chart
    ${CMAKE_CURRENT_SOURCE_DIR}/wasm-canvas/include
    ${CMAKE_CURRENT_SOURCE_DIR}/lib-chart-wasm
)
include_directories(${ALL_INCLUDE})

add_executable(stockchart
    ${CMAKE_CURRENT_SOURCE_DIR}/wasm-canvas/include/canvas.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/wasm-canvas/include/window.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/lib-chart-wasm/StockChart.cpp
)
target_include_directories(stockchart PUBLIC ${ALL_INCLUDE})

# 生成 wasm 文件，而不是默认为 javascript 文件
set(CMAKE_EXECUTABLE_SUFFIX ".wasm")

# -Os                优化和减少代码大小
# -s WASM=1          0 表示编译为 JavaScript，1 表示编译为 WebAssembly，2 表示编译为两者
# -s SIDE_MODULE=1   默认为 0，1 删除 wasm 库中我们当前不需要的许多其他条目，2 删除定义中没有 EMSCRIPTEN_KEEPALIVE 宏的函数
# -s STANDALONE_WASM 不需要支持 JavaScript 文件即可运行的 WebAssembly 文件
# --no-entry         表示没有 main 函数
set_target_properties(stockchart PROPERTIES COMPILE_FLAGS "-Os -s SIDE_MODULE=1 ")
set_target_properties(stockchart PROPERTIES LINK_FLAGS    "-Os -s WASM=1 -s SIDE_MODULE=1 -s STANDALONE_WASM --no-entry")

message("[stock-charts-wasm] -------- done")
