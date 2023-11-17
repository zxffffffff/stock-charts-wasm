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
    ChartViewCanvas(std::shared_ptr<ChartViewModel> vm, HTMLCanvasElement *canvas)
        : m_vm(vm), canvas(canvas)
    {
        listen(m_vm.get());
    }

    virtual void on(DataBinding *sender, const std::string &id) override
    {
        if (m_vm.get() == sender)
        {
            fire(id);
            // TODO update();
        }
    }

    void draw()
    {
        PainterCanvas painter(canvas->getContext(canvas, "2d"));
        painter.fillRect(m_vm->getContext().rectChart, Color("#FFFFFF"));
        m_vm->onPaint(painter);
    }

    void resize(double width, double height)
    {
        m_vm->onResize(Rect(0, 0, width, height));
    }

    void mouseMove(double x, double y)
    {
        m_vm->onMouseMove(Point(x, y));
    }

    void mouseLeave()
    {
        m_vm->onMouseLeave();
    }

    void mouseDown(double x, double y)
    {
    }

    void mouseUp(double x, double y)
    {
    }

    void mouseDoubleClick(double x, double y)
    {
        m_vm->onDBClick(Point(x, y));
    }

    void keyPress(const std::string &key)
    {
        if (key == "w" || key == "W" || key == "+" || key == "=")
            m_vm->onWheelY(1);
        if (key == "s" || key == "S" || key == "-" || key == "_")
            m_vm->onWheelY(-1);
        if (key == "a" || key == "A")
            m_vm->onScrollX(-1);
        if (key == "d" || key == "D")
            m_vm->onScrollX(1);
    }

private:
    std::shared_ptr<ChartViewModel> m_vm;
    HTMLCanvasElement *canvas;
};
