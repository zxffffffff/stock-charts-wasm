/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "Core/DataBinding.h"
#include "ViewModel/ChartViewModel.h"
#include "Graphics/PainterCanvas.h"
#include "../Graphics/PainterCanvas.h"

using namespace StockCharts;

class ChartViewCanvas : public DataBinding
{
public:
    ChartViewCanvas(HTMLCanvasElement *canvas)
        : canvas(canvas)
    {
    }

    void init(std::shared_ptr<ChartViewModel> vm)
    {
        m_vm = vm;
        listen(m_vm.get());
    }

    void draw()
    {
        PainterCanvas painter(canvas->getContext(canvas, "2d"));
        m_vm->onPaint(painter);
    }

    virtual void on(DataBinding *sender, const std::string &id) override
    {
        if (m_vm.get() == sender)
        {
            fire(id);
            // TODO update();
        }
    }

private:
    HTMLCanvasElement *canvas;
    std::shared_ptr<ChartViewModel> m_vm;
};
