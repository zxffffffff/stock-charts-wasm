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
    typedef void *StockChartModel;
    typedef void *StockChartViewModel;
    typedef void *StockChartView;

    void global_init();
    void global_cleanup();

    StockChartModel new_model(const char *stock_data);
    void del_model(StockChartModel model);
    void add_plugin(StockChartModel model, const char *type);

    StockChartViewModel new_vm(StockChartModel model);
    void del_vm(StockChartViewModel vm);
    void add_layer(StockChartViewModel vm, const char *type);

    StockChartView new_view(StockChartViewModel vm, const char *canvas_id);
    void del_view(StockChartView view);
    void draw(StockChartView view);
    void resize(StockChartView view, double width, double height);
    void mouseMove(StockChartView view, double x, double y);
    void mouseLeave(StockChartView view);
    void mouseDown(StockChartView view, double x, double y);
    void mouseUp(StockChartView view, double x, double y);
    void mouseDoubleClick(StockChartView view, double x, double y);
    void keyPress(StockChartView view, const char *key);
}
