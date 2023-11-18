/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#include "StockChart.h"
#include "View/ChartViewCanvas.h"
#include "Model/Plugin/PluginIndicator.h"
#include "ViewModel/Layer/LayerBG.h"
#include "ViewModel/Layer/LayerStock.h"
#include "ViewModel/Layer/LayerIndicator.h"
#include "ViewModel/Layer/LayerCrossLine.h"
#include "ViewModel/Layer/LayerTitle.h"
#include <stdio.h>

using namespace StockCharts;

namespace StockChartsWasm
{
    std::atomic<int> mem_leak_count = 0;
}

struct StockChartCore_p
{
    std::shared_ptr<StockCore> stock;

    StockChartCore_p() { ++StockChartsWasm::mem_leak_count; }
    ~StockChartCore_p() { --StockChartsWasm::mem_leak_count; }
};

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

    char c_drag_flag = '0';
    double x_drag_last = 0;
    double y_drag_last = 0;

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
StockChartCore new_stock(const char *stock)
{
    printf("%s:%d: %s(%s)\n", __FILE__, __LINE__, __FUNCTION__, stock);
    StockChartCore_p *p_stock = new StockChartCore_p;
    p_stock->stock = std::make_shared<StockCore>();
    const std::vector<std::string> list = Utils::splitStr(stock, '\n');
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
    return p_stock;
}

EMSCRIPTEN_KEEPALIVE
void reverse_stock(StockChartCore stock)
{
    printf("%s:%d: %s\n", __FILE__, __LINE__, __FUNCTION__);
    StockChartCore_p *p_stock = (StockChartCore_p *)stock;
    p_stock->stock->reverse();
}

EMSCRIPTEN_KEEPALIVE
void del_stock(StockChartCore stock)
{
    printf("%s:%d: %s\n", __FILE__, __LINE__, __FUNCTION__);
    StockChartCore_p *p_stock = (StockChartCore_p *)stock;
    delete p_stock;
}

EMSCRIPTEN_KEEPALIVE
StockChartModel new_model(StockChartCore stock)
{
    printf("%s:%d: %s\n", __FILE__, __LINE__, __FUNCTION__);
    StockChartCore_p *p_stock = (StockChartCore_p *)stock;
    StockChartModel_p *p_model = new StockChartModel_p;
    p_model->stock = p_stock->stock;
    p_model->model = std::make_shared<ChartModel>(p_stock->stock);
    return p_model;
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
void add_indicator(StockChartModel model, const char *name, const char *expression, const char *params)
{
    printf("%s:%d: %s(name=%s, expression=%s, params=%s)\n", __FILE__, __LINE__, __FUNCTION__, name, expression, params);
    StockChartModel_p *p_model = (StockChartModel_p *)model;
    IndexFormula formular;
    formular.name = name;
    formular.expression = expression;
    const std::vector<std::string> list = Utils::splitStr(params, '\n');
    for (int i = 0; i < list.size(); ++i)
    {
        std::vector<std::string> param = Utils::splitStr(list[i], ',');
        if (param.size() == 2)
        {
            formular.params[param[0]] = atoi(param[1].c_str());
        }
    }
    if (auto pluginIndicator = p_model->model->getPlugin<PluginIndicator>())
        pluginIndicator->addIndicator(formular);
}

EMSCRIPTEN_KEEPALIVE
StockChartViewModel new_vm(StockChartModel model)
{
    printf("%s:%d: %s\n", __FILE__, __LINE__, __FUNCTION__);
    StockChartModel_p *p_model = (StockChartModel_p *)model;
    StockChartViewModel_p *p_vm = new StockChartViewModel_p;
    p_vm->vm = std::make_shared<ChartViewModel>(p_model->model);
    return p_vm;
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
    else if (strcmp(type, "LayerTitle") == 0)
        p_vm->vm->addLayer<LayerTitle>(ChartTitleItemFlagStock | ChartTitleItemFlagIndicator);
    else if (strcmp(type, "LayerTitle_Stock") == 0)
        p_vm->vm->addLayer<LayerTitle>(ChartTitleItemFlagStock);
    else if (strcmp(type, "LayerTitle_Indicator") == 0)
        p_vm->vm->addLayer<LayerTitle>(ChartTitleItemFlagIndicator);
}

EMSCRIPTEN_KEEPALIVE
void set_sync_other(StockChartViewModel vm, StockChartViewModel other)
{
    printf("%s:%d: %s\n", __FILE__, __LINE__, __FUNCTION__);
    StockChartViewModel_p *p_vm = (StockChartViewModel_p *)vm;
    StockChartViewModel_p *p_other = (StockChartViewModel_p *)other;
    p_vm->vm->setSyncOther(p_other->vm.get());
}

EMSCRIPTEN_KEEPALIVE
StockChartView new_view(StockChartViewModel vm, const char *canvas_id)
{
    printf("%s:%d: %s(canvas_id=%s)\n", __FILE__, __LINE__, __FUNCTION__, canvas_id);
    StockChartViewModel_p *p_vm = (StockChartViewModel_p *)vm;
    StockChartView_p *p_view = new StockChartView_p;
    p_view->canvas = createCanvas((char *)canvas_id);
    p_view->view = std::make_shared<ChartViewCanvas>(p_vm->vm, p_view->canvas);
    return p_view;
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
    p_view->view->onResize(width, height);
}

EMSCRIPTEN_KEEPALIVE
void keyPress(StockChartView view, const char *key)
{
    printf("%s:%d: %s(key=%s)\n", __FILE__, __LINE__, __FUNCTION__, key);
    StockChartView_p *p_view = (StockChartView_p *)view;
    const char c = key[0];
    switch (c)
    {
    case 'w':
    case 'W':
    case '+':
    case '=':
        p_view->view->onWheelY(1);
        break;
    case 's':
    case 'S':
    case '-':
    case '_':
        p_view->view->onWheelY(-1);
        break;
    case 'a':
    case 'A':
        p_view->view->onScrollX(-1);
        break;
    case 'd':
    case 'D':
        p_view->view->onScrollX(1);
        break;
    }
}

EMSCRIPTEN_KEEPALIVE
void mouseMove(StockChartView view, double x, double y)
{
    // printf("%s:%d: %s(x=%.1f y=%.1f)\n", __FILE__, __LINE__, __FUNCTION__, x, y);
    StockChartView_p *p_view = (StockChartView_p *)view;
    p_view->view->onMouseMove(x, y);
    if (p_view->c_drag_flag == 'x')
    {
        double x_offset = x - p_view->x_drag_last;
        double x_remainder = p_view->view->onScrollX_pix(x_offset);
        p_view->x_drag_last = x - x_remainder;
    }
    else if (p_view->c_drag_flag == 'y')
    {
        double y_offset = y - p_view->y_drag_last;
        double y_remainder = p_view->view->onWheelY_pix(y_offset);
        p_view->y_drag_last = y - y_remainder;
    }
}

EMSCRIPTEN_KEEPALIVE
void mouseLeave(StockChartView view)
{
    printf("%s:%d: %s\n", __FILE__, __LINE__, __FUNCTION__);
    StockChartView_p *p_view = (StockChartView_p *)view;
    p_view->view->onMouseLeave();
}

EMSCRIPTEN_KEEPALIVE
void mouseDown(StockChartView view, double x, double y)
{
    printf("%s:%d: %s(x=%.1f y=%.1f)\n", __FILE__, __LINE__, __FUNCTION__, x, y);
    StockChartView_p *p_view = (StockChartView_p *)view;
    const ChartContext &chartCtx = p_view->view->getContext();
    p_view->c_drag_flag = (chartCtx.rectChart.contains(Point(x, y)) || chartCtx.rectXAxis.contains(Point(x, y))) ? 'x' : 'y';
    p_view->x_drag_last = x;
    p_view->y_drag_last = y;
}

EMSCRIPTEN_KEEPALIVE
void mouseUp(StockChartView view, double x, double y)
{
    printf("%s:%d: %s(x=%.1f y=%.1f)\n", __FILE__, __LINE__, __FUNCTION__, x, y);
    StockChartView_p *p_view = (StockChartView_p *)view;
    p_view->c_drag_flag = '0';
}

EMSCRIPTEN_KEEPALIVE
void mouseDoubleClick(StockChartView view, double x, double y)
{
    printf("%s:%d: %s(x=%.1f y=%.1f)\n", __FILE__, __LINE__, __FUNCTION__, x, y);
    StockChartView_p *p_view = (StockChartView_p *)view;
    p_view->view->onDBClick(x, y);
}
