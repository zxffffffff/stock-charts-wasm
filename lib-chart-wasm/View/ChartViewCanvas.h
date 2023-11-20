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

    virtual ~ChartViewCanvas() = default;

    virtual void on(DataBinding *sender, const std::string &id) override
    {
        if (m_vm.get() == sender)
        {
            fire(id);
            draw();
        }
    }

public:
    const ChartProps &getProps() const
    {
        return m_vm->getProps();
    }

    const ChartContext &getContext() const
    {
        return m_vm->getContext();
    }

public:
    void draw()
    {
        PainterCanvas painter(canvas->getContext(canvas, "2d"));
        painter.fillRect(m_vm->getContext().rectView, Color("#FFFFFF"));
        m_vm->onPaint(painter);
    }

    void onResize(double width, double height)
    {
        m_vm->onResize(Rect(0, 0, width, height));
    }

    void onMouseMove(double x, double y)
    {
        m_vm->onMouseMove(Point(x, y));
    }

    void onMouseLeave()
    {
        m_vm->onMouseLeave();
    }

    void onDBClick(double x, double y)
    {
        m_vm->onDBClick(Point(x, y));
    }

    void onScrollX(int step)
    {
        m_vm->onScrollX(step);
    }

    void onWheelY(int step)
    {
        m_vm->onWheelY(step);
    }

    double onScrollX_pix(double pix, bool inverted)
    {
        return m_vm->onScrollX_pix(pix, inverted);
    }

    double onWheelY_pix(double pix, bool inverted)
    {
        return m_vm->onWheelY_pix(pix, inverted);
    }

private:
    std::shared_ptr<ChartViewModel> m_vm;
    HTMLCanvasElement *canvas;
};
