# stock-charts-wasm

 基于 C++ WASM 实现股票图表和技术指标（模仿富途），提供 Web HTML 实现。

## 作者说明
- 精力有限，偶尔维护，有需要可以联系我答疑解惑（zxffffffff@outlook.com, 1337328542@qq.com）。
- `star >= 100` 可以考虑更新绘图、叠加、复权等功能。
- C++（Qt）原始版本：https://github.com/zxffffffff/stock-charts-cpp.git

## 参数控制，指标管理（模仿富途牛牛）
![image](https://github.com/zxffffffff/stock-charts-wasm/blob/main/doc/stock-chart.png)

## C++ 架构图
![image](https://github.com/zxffffffff/stock-charts-cpp/blob/main/doc/architecture.png)

# WebAssembly (WASM)

Wasm 是一种基于堆栈的虚拟机（二进制指令格式）用于嵌入到 Web

> https://webassembly.org/

# Emscripten (需要手动安装，步骤较为繁琐)

Emscripten 是基于 LLVM/Clang 的编辑器，将 C/C++ 代码编译为 WASM

> https://emscripten.org/

```Bash
cd # 软件安装目录，mac建议“/Users/xxx”，win建议“C://Program Files”
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
git pull

# win
./emsdk.bat install latest  # 下载安装tools
./emsdk.bat activate latest # 激活当前sdk
./emsdk_env.bat             # 添加环境变量
./emcmdprompt.bat           # 打开terminal新窗口（建议）

# mac
./emsdk install latest  
./emsdk activate latest 
source ./emsdk_env.sh       # 添加环境变量（每次打开terminal新窗口都要调用！）

# 检查
emcc --check
```

# 工程目录

- wasm-canvas (git subtree)
- stock-charts-cpp (git subtree)
- lib-chart-wasm
- CMakeLists.txt
- index.js

## wasm-canvas 说明

wasm-canvas is a **C99-compliant** layer of abstraction for interacting with the **HTML Canvas API**. 

```bash
# 已添加到工程目录
git subtree add --prefix wasm-canvas https://github.com/zxffffffff/wasm-canvas.git master --squash
git subtree pull --prefix wasm-canvas https://github.com/zxffffffff/wasm-canvas.git master --squash
```

## stock-charts-cpp 和 lib-chart-wasm 说明

stock-charts-cpp 基于 pure C++ 实现股票图表和技术指标，支持任何 UI 框架。

lib-chart-wasm 在其基础上增加 wasm canvas 支持，使用 `./CMakeLists.txt` 编译。

```bash
# 已添加到工程目录
git subtree add --prefix stock-charts-cpp https://github.com/zxffffffff/stock-charts-cpp.git main --squash
git subtree pull --prefix stock-charts-cpp https://github.com/zxffffffff/stock-charts-cpp.git main --squash

# 打开 CMakeLists.txt 修改 emsdk 绝对路径，建议使用 IDE
mkdir build
cd build
cmake ..
make
```

## 使用 index.html 测试

使用任意 http server 启动 index.html，例如：

```bash
npm install --global http-server
npx http-server ./ 
```
