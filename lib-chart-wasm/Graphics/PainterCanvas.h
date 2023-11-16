/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "PainterCanvas.h"
#include "Graphics/Painter.h"
#include "window.h"
#include "canvas.h"

using namespace StockCharts;

class PainterCanvas : public Painter
{
public:
    PainterCanvas(CanvasRenderingContext2D *ctx)
        : ctx(ctx)
    {
    }

    virtual void save() override
    {
        ctx->save(ctx);
    }
    virtual void restore() override
    {
        ctx->restore(ctx);
    }

    virtual void drawString(const Rect &rect, const std::string &text, const Font &font) override
    {
        // TODO font
        ctx->fillText(ctx, (char *)text.c_str(), rect.left(), rect.top(), rect.width());
    }

    virtual void drawRect(const Rect &rect, const Pen &pen) override
    {
        // TODO pen
        ctx->strokeRect(ctx, rect.left(), rect.top(), rect.width(), rect.height());
    }

    virtual void fillRect(const Rect &rect, const Pen &pen) override
    {
        // TODO pen
        ctx->fillRect(ctx, rect.left(), rect.top(), rect.width(), rect.height());
    }

    virtual void drawLine(const Line &line, const Pen &pen) override
    {
        // TODO pen
        ctx->moveTo(ctx, line.first.x, line.first.y);
        ctx->lineTo(ctx, line.second.x, line.second.y);
    }

    virtual void drawPath(const std::vector<Point> &points, const Pen &pen) override
    {
        // TODO pen
        ctx->moveTo(ctx, points[0].x, points[0].y);
        for (auto &point : points)
        {
            if (!point.valid())
                break;
            ctx->lineTo(ctx, point.x, point.y);
        }
    }

    virtual void drawStick(const std::vector<Stick> &sticks, const Color &rise, const Color &fall) override
    {
        for (auto &stick : sticks)
        {
            if (!stick.valid())
                break;
            const Color &color = stick.flag >= 0 ? rise : fall;
            if (stick.width() == 1)
            {
                drawLine(stick.line, color);
                continue;
            }
            fillRect(stick.rect, color);
            drawLine(stick.line, color);
        }
    }

    virtual void drawStickHollow(const std::vector<Stick> &sticks, const Color &rise, const Color &fall) override
    {
        for (auto &stick : sticks)
        {
            if (!stick.valid())
                break;
            const Color &color = stick.flag >= 0 ? rise : fall;
            if (stick.width() == 1)
            {
                drawLine(stick.line, color);
                continue;
            }
            if (stick.flag >= 0)
            {
                drawRect(stick.rect, color);
                drawLine(Line(stick.centerX(), stick.top() - 1, stick.centerX(), stick.line.first.y), color);
                drawLine(Line(stick.centerX(), stick.bottom() + 1, stick.centerX(), stick.line.second.y), color);
            }
            else
            {
                fillRect(stick.rect, color);
                drawLine(stick.line, color);
            }
        }
    }

    virtual void drawBAR(const std::vector<Stick> &sticks, const Color &rise, const Color &fall) override
    {
        for (auto &stick : sticks)
        {
            if (!stick.valid())
                break;
            const Color &color = stick.flag >= 0 ? rise : fall;
            if (stick.width() == 1)
            {
                drawLine(stick.line, color);
                continue;
            }
            int openY;
            int closeY;
            if (stick.flag >= 0)
            {
                closeY = stick.top();
                openY = stick.bottom();
            }
            else
            {
                openY = stick.top();
                closeY = stick.bottom();
            }
            drawLine(stick.line, color);
            drawLine(Line(stick.left(), openY, stick.centerX(), openY), color);
            drawLine(Line(stick.right(), closeY, stick.centerX(), closeY), color);
        }
    }

private:
    CanvasRenderingContext2D *ctx;
};
