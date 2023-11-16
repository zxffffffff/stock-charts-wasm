# stock-charts-wasm
 基于 C++ WASM 实现股票图表和技术指标（模仿富途），提供 Web HTML 实现。

# WebAssembly (WASM)
- Wasm 是一种基于堆栈的虚拟机（二进制指令格式）用于嵌入到 Web
> https://webassembly.org/

# Emscripten (需要手动安装，步骤较为繁琐)
- Emscripten 是基于 LLVM/Clang 的编辑器，将 C/C++ 代码编译为 WASM
> https://emscripten.org/

## 依赖 stock-charts-cpp 项目
```bash
git subtree add --prefix stock-charts-cpp https://github.com/zxffffffff/stock-charts-cpp.git main --squash
git subtree pull --prefix stock-charts-cpp https://github.com/zxffffffff/stock-charts-cpp.git main --squash
```

### 扩展 lib-chart-wasm 项目
```bash
cmake .
```

## 依赖 wasm-canvas 项目
```bash
git subtree add --prefix wasm-canvas https://github.com/zxffffffff/wasm-canvas.git master --squash
git subtree pull --prefix wasm-canvas https://github.com/zxffffffff/wasm-canvas.git master --squash
```

### 使用 index.html 测试
```bash
npx http-server ./ 
```
