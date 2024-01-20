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
# set(CMAKE_TOOLCHAIN_FILE "C:/Users/xiaofeng.zhu/Documents/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake")
set(CMAKE_TOOLCHAIN_FILE "/Volumes/Colorful_4T/ZXF/PATH/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake")
message(STATUS "[stock-charts-wasm] CMAKE_TOOLCHAIN_FILE : ${CMAKE_TOOLCHAIN_FILE}")

project ("stock-charts-wasm")

message(STATUS "[stock-charts-wasm] CMAKE_CURRENT_SOURCE_DIR : ${CMAKE_CURRENT_SOURCE_DIR}")

set(ALL_INCLUDE
    # "C:/Users/xiaofeng.zhu/Documents/emsdk/upstream/emscripten/cache/sysroot/include" # emsdk
    "/Volumes/Colorful_4T/ZXF/PATH/emsdk/upstream/emscripten/cache/sysroot/include" # emsdk
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

# 参考：https://github.com/emscripten-core/emscripten/blob/main/src/settings.js
# -Os                                   优化和减少代码大小（-O0 默认不优化，-O1，-O2 用于生产，-Os 减少体积，-Oz，-O3 增加体积）非常影响编译体积！
# -g0                                   保留llvm调试信息（-g0 用于生产，-g1，-g2，-g3，-g4 全部调试信息）非常影响编译体积！
# --no-entry                            表示没有 main 函数
# -s MODULARIZE=1                       模块化，影响 Module（默认 0 全局范围），注意会影响加载方式
# -s WASM_ASYNC_COMPILATION=0           是否异步加载wasm，主线程需要用异步，worker中可以同步加载
# -s WASM=1                             0 表示编译为 asm.js，1 表示编译为 wasm，2 表示编译为两者
# -s SIDE_MODULE=1                      默认为 0，1 删除 wasm 库中我们当前不需要的许多其他条目，2 删除定义中没有 EMSCRIPTEN_KEEPALIVE 宏的函数
# -s STANDALONE_WASM                    尽可能使用标准 API 生成一个可以独立运行的 wasm 文件，无需依赖 JavaScript
# -s WASM_BIGINT                        使用 STANDALONE_WASM 需要开启支持 int64
# -s EXPORTED_RUNTIME_METHODS           导出 C-API 到 JS-Module
# -s DEFAULT_LIBRARY_FUNCS_TO_INCLUDE   历史原因，类似于 EXPORTED_RUNTIME_METHODS
# -s NO_EXIT_RUNTIME=1                  执行完main后是否释放模块，有main默认true，无main默认false
set_target_properties(stockchart PROPERTIES COMPILE_FLAGS "-Os -g0")
set_target_properties(stockchart PROPERTIES LINK_FLAGS    "-Os -g0 --no-entry -s STANDALONE_WASM -s WASM_BIGINT -s EXPORTED_RUNTIME_METHODS=ccall,cwrap,stringToNewUTF8 -s DEFAULT_LIBRARY_FUNCS_TO_INCLUDE=$stringToNewUTF8")

message("[stock-charts-wasm] -------- done")
