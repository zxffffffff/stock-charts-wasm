/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#include "StockChart.h"
#include <stdio.h>
#include "View/ChartViewCanvas.h"
#include "Model/Plugin/PluginIndicator.h"
#include "ViewModel/Layer/LayerBG.h"
#include "ViewModel/Layer/LayerStock.h"
#include "ViewModel/Layer/LayerIndicator.h"
#include "ViewModel/Layer/LayerCrossLine.h"

using namespace StockCharts;

namespace StockChartsWasm
{
    std::atomic<int> mem_leak_count = 0;
}

struct StockChartModel_p
{
    std::shared_ptr<StockCore> stock;
    std::shared_ptr<ChartModel> model;

    StockChartModel_p() { ++StockChartsWasm::mem_leak_count; }
    ~StockChartModel_p() { --StockChartsWasm::mem_leak_count; }
};

struct StockChartViewModel_p
{
    std::shared_ptr<ChartViewModel> vm;

    StockChartViewModel_p() { ++StockChartsWasm::mem_leak_count; }
    ~StockChartViewModel_p() { --StockChartsWasm::mem_leak_count; }
};

struct StockChartView_p
{
    HTMLCanvasElement *canvas = nullptr;
    std::shared_ptr<ChartViewCanvas> view;

    StockChartView_p() { ++StockChartsWasm::mem_leak_count; }
    ~StockChartView_p() { --StockChartsWasm::mem_leak_count; }
};

EMSCRIPTEN_KEEPALIVE
void global_init()
{
    printf("%s:%d: %s\n", __FILE__, __LINE__, __FUNCTION__);
    StockChartsWasm::mem_leak_count = 0;
}

EMSCRIPTEN_KEEPALIVE
void global_cleanup()
{
    printf("%s:%d: %s\n", __FILE__, __LINE__, __FUNCTION__);
    if (StockChartsWasm::mem_leak_count != 0)
        printf("Error: mem_leak_count:%d\n", (int)StockChartsWasm::mem_leak_count);
}

EMSCRIPTEN_KEEPALIVE
StockChartModel new_model(const char *stock_data)
{
    printf("%s:%d: %s(input=%s)\n", __FILE__, __LINE__, __FUNCTION__, stock_data);
    StockChartModel_p *p_stock = new StockChartModel_p;
    p_stock->stock = std::make_shared<StockCore>();
    const std::vector<std::string> list = Utils::splitStr(stock_data, '\n');
    for (int i = 0; i < list.size(); ++i)
    {
        std::vector<std::string> s_core = Utils::splitStr(list[i], ',');
        const int sz = s_core.size();
        std::vector<Number> core(sz);
        if (i == 6)
        {
            for (int j = 0; j < sz; ++j)
                core[j] = NumberUtils::toTimestamp(s_core[j], "%Y-%m-%d");
        }
        else
        {
            for (int j = 0; j < sz; ++j)
                core[j] = atof(s_core[j].c_str());
        }

        switch (i)
        {
        case 0:
            p_stock->stock->open = NumberCore(std::move(core), std::move(s_core));
            break;
        case 1:
            p_stock->stock->high = NumberCore(std::move(core), std::move(s_core));
            break;
        case 2:
            p_stock->stock->low = NumberCore(std::move(core), std::move(s_core));
            break;
        case 3:
            p_stock->stock->close = NumberCore(std::move(core), std::move(s_core));
            break;
        case 4:
            p_stock->stock->vol = NumberCore(std::move(core), std::move(s_core));
            break;
        case 5:
            p_stock->stock->amount = NumberCore(std::move(core), std::move(s_core));
            break;
        case 6:
            p_stock->stock->timestamp = NumberCore(std::move(core), std::move(s_core));
            break;
        }
    }
    p_stock->model = std::make_shared<ChartModel>(p_stock->stock);
    return (StockChartModel *)p_stock;
}

EMSCRIPTEN_KEEPALIVE
void del_model(StockChartModel model)
{
    printf("%s:%d: %s\n", __FILE__, __LINE__, __FUNCTION__);
    StockChartModel_p *p_model = (StockChartModel_p *)model;
    delete p_model;
}

EMSCRIPTEN_KEEPALIVE
void add_plugin(StockChartModel model, const char *type)
{
    printf("%s:%d: %s(type=%s)\n", __FILE__, __LINE__, __FUNCTION__, type);
    StockChartModel_p *p_model = (StockChartModel_p *)model;
    if (strcmp(type, "PluginIndicator") == 0)
        p_model->model->addPlugin<PluginIndicator>();
}

EMSCRIPTEN_KEEPALIVE
StockChartViewModel new_vm(StockChartModel model)
{
    printf("%s:%d: %s\n", __FILE__, __LINE__, __FUNCTION__);
    StockChartModel_p *p_model = (StockChartModel_p *)model;
    StockChartViewModel_p *p_vm = new StockChartViewModel_p;
    p_vm->vm = std::make_shared<ChartViewModel>(p_model->model);
    return (StockChartViewModel *)p_vm;
}

EMSCRIPTEN_KEEPALIVE
void del_vm(StockChartViewModel vm)
{
    printf("%s:%d: %s\n", __FILE__, __LINE__, __FUNCTION__);
    StockChartViewModel_p *p_vm = (StockChartViewModel_p *)vm;
    delete p_vm;
}

EMSCRIPTEN_KEEPALIVE
void add_layer(StockChartViewModel vm, const char *type)
{
    printf("%s:%d: %s(type=%s)\n", __FILE__, __LINE__, __FUNCTION__, type);
    StockChartViewModel_p *p_vm = (StockChartViewModel_p *)vm;
    if (strcmp(type, "LayerBG") == 0)
        p_vm->vm->addLayer<LayerBG>();
    else if (strcmp(type, "LayerStock") == 0)
        p_vm->vm->addLayer<LayerStock>();
    else if (strcmp(type, "LayerIndicator") == 0)
        p_vm->vm->addLayer<LayerIndicator>();
    else if (strcmp(type, "LayerCrossLine") == 0)
        p_vm->vm->addLayer<LayerCrossLine>();
}

EMSCRIPTEN_KEEPALIVE
StockChartView new_view(StockChartViewModel vm, const char *canvas_id)
{
    printf("%s:%d: %s(canvas_id=%s)\n", __FILE__, __LINE__, __FUNCTION__, canvas_id);
    StockChartViewModel_p *p_vm = (StockChartViewModel_p *)vm;
    StockChartView_p *p_view = new StockChartView_p;
    p_view->canvas = createCanvas((char *)canvas_id);
    p_view->view = std::make_shared<ChartViewCanvas>(p_vm->vm, p_view->canvas);
    return (StockChartView *)p_view;
}

EMSCRIPTEN_KEEPALIVE
void del_view(StockChartView view)
{
    printf("%s:%d: %s\n", __FILE__, __LINE__, __FUNCTION__);
    StockChartView_p *p_view = (StockChartView_p *)view;
    freeCanvas(p_view->canvas);
    delete p_view;
}

EMSCRIPTEN_KEEPALIVE
void draw(StockChartView view)
{
    printf("%s:%d: %s\n", __FILE__, __LINE__, __FUNCTION__);
    StockChartView_p *p_view = (StockChartView_p *)view;
    p_view->view->draw();
}

EMSCRIPTEN_KEEPALIVE
void resize(StockChartView view, double width, double height)
{
    printf("%s:%d: %s(width=%.1f height=%.1f)\n", __FILE__, __LINE__, __FUNCTION__, width, height);
    StockChartView_p *p_view = (StockChartView_p *)view;
    p_view->view->resize(width, height);
}

EMSCRIPTEN_KEEPALIVE
void mouseMove(StockChartView view, double x, double y)
{
    printf("%s:%d: %s(width=%.1f height=%.1f)\n", __FILE__, __LINE__, __FUNCTION__, x, y);
    StockChartView_p *p_view = (StockChartView_p *)view;
    p_view->view->mouseMove(x, y);
}

EMSCRIPTEN_KEEPALIVE
void mouseLeave(StockChartView view)
{
    printf("%s:%d: %s\n", __FILE__, __LINE__, __FUNCTION__);
    StockChartView_p *p_view = (StockChartView_p *)view;
    p_view->view->mouseLeave();
}

EMSCRIPTEN_KEEPALIVE
void mouseDown(StockChartView view, double x, double y)
{
    printf("%s:%d: %s(width=%.1f height=%.1f)\n", __FILE__, __LINE__, __FUNCTION__, x, y);
    StockChartView_p *p_view = (StockChartView_p *)view;
    p_view->view->mouseDown(x, y);
}

EMSCRIPTEN_KEEPALIVE
void mouseUp(StockChartView view, double x, double y)
{
    printf("%s:%d: %s(width=%.1f height=%.1f)\n", __FILE__, __LINE__, __FUNCTION__, x, y);
    StockChartView_p *p_view = (StockChartView_p *)view;
    p_view->view->mouseUp(x, y);
}

EMSCRIPTEN_KEEPALIVE
void mouseDoubleClick(StockChartView view, double x, double y)
{
    printf("%s:%d: %s(width=%.1f height=%.1f)\n", __FILE__, __LINE__, __FUNCTION__, x, y);
    StockChartView_p *p_view = (StockChartView_p *)view;
    p_view->view->mouseDoubleClick(x, y);
}

EMSCRIPTEN_KEEPALIVE
void keyPress(StockChartView view, const char *key)
{
    printf("%s:%d: %s(key=%s)\n", __FILE__, __LINE__, __FUNCTION__, key);
    StockChartView_p *p_view = (StockChartView_p *)view;
    p_view->view->keyPress(key);
}
