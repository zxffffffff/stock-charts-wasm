/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include <emscripten.h>

extern "C"
{
    typedef void *StockChartContext;

    StockChartContext init(char *name);
    void cleanup(StockChartContext ctx);

    void draw(StockChartContext ctx);
}
