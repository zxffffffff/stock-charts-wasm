/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#include "StockChart.h"
#include "View/ChartViewCanvas.h"

struct StockChartContext_p
{
    HTMLCanvasElement *canvas;
    ChartViewCanvas *view;
};

EMSCRIPTEN_KEEPALIVE StockChartContext init(char *name)
{
    HTMLCanvasElement *canvas = createCanvas(name);
    StockChartContext_p *_p = new StockChartContext_p{canvas, new ChartViewCanvas(canvas)};
    return _p;
}

EMSCRIPTEN_KEEPALIVE void cleanup(StockChartContext ctx)
{
    StockChartContext_p *_p = (StockChartContext_p *)ctx;
    delete _p->view;
    freeCanvas(_p->canvas);
    delete _p;
}

EMSCRIPTEN_KEEPALIVE void draw(StockChartContext ctx)
{
    StockChartContext_p *_p = (StockChartContext_p *)ctx;
    _p->view->draw();
}
