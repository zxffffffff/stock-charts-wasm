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

    static std::string parseColor(const Color &color)
    {
        char buf[64];
        sprintf(buf, "rgba(%d,%d,%d,%f)", color.r, color.g, color.b, color.a / 255.0);
        return buf;
    }

    static std::string parsePenStyle(LineType lineStyle)
    {
        switch (lineStyle)
        {
        case StockCharts::LineType::SolidLine:
            return "";
        case StockCharts::LineType::DashLine:
            return "5,5";
        case StockCharts::LineType::DotLine:
            return "2,5";
        case StockCharts::LineType::DashDotLine:
            return "10,5,2,5";
        default:
            return "";
        }
    }

    static std::pair<std::string, std::string> parsePaintDirection(PaintDirection dir)
    {
        switch (dir)
        {
        case StockCharts::PaintDirection::TopLeft:
            return {"top", "left"};
        case StockCharts::PaintDirection::TopCenter:
            return {"top", "center"};
        case StockCharts::PaintDirection::TopRight:
            return {"top", "right"};
        case StockCharts::PaintDirection::CenterLeft:
            return {"middle", "left"};
        case StockCharts::PaintDirection::Center:
            return {"middle", "center"};
        case StockCharts::PaintDirection::CenterRight:
            return {"middle", "right"};
        case StockCharts::PaintDirection::BottomLeft:
            return {"bottom", "left"};
        case StockCharts::PaintDirection::BottomCenter:
            return {"bottom", "center"};
        case StockCharts::PaintDirection::BottomRight:
            return {"bottom", "right"};
        default:
            return {"top", "left"};
        }
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
        if (!rect.valid())
            return;
        ctx->setFont(ctx, (std::to_string(font.fontSize) + "px").data());
        ctx->setFillStyle(ctx, parseColor(font.color).data());
        auto baseline_align = parsePaintDirection(font.dir);
        ctx->setTextBaseline(ctx, baseline_align.first.data());
        ctx->setTextAlign(ctx, baseline_align.second.data());
        if (baseline_align.second == "left")
            ctx->fillText(ctx, (char *)text.c_str(), rect.left(), rect.centerY(), rect.width());
        else if (baseline_align.second == "right")
            ctx->fillText(ctx, (char *)text.c_str(), rect.right(), rect.centerY(), rect.width());
        else
            ctx->fillText(ctx, (char *)text.c_str(), rect.centerX(), rect.centerY(), rect.width());
    }

    virtual void drawRect(const Rect &rect, const Pen &pen) override
    {
        if (!rect.valid())
            return;
        ctx->setLineDash(ctx, parsePenStyle(pen.lineType).data());
        ctx->setLineWidth(ctx, pen.lineWidth);
        ctx->setStrokeStyle(ctx, parseColor(pen.color).data());
        ctx->strokeRect(ctx, rect.left(), rect.top(), rect.width(), rect.height());
    }

    virtual void fillRect(const Rect &rect, const Pen &pen) override
    {
        if (!rect.valid())
            return;
        ctx->setFillStyle(ctx, parseColor(pen.color).data());
        ctx->fillRect(ctx, rect.left(), rect.top(), rect.width(), rect.height());
    }

    virtual void drawLine(const Line &line, const Pen &pen) override
    {
        if (!line.valid())
            return;
        ctx->setLineDash(ctx, parsePenStyle(pen.lineType).data());
        ctx->setLineWidth(ctx, pen.lineWidth);
        ctx->setStrokeStyle(ctx, parseColor(pen.color).data());
        ctx->beginPath(ctx);
        ctx->moveTo(ctx, line.first.x, line.first.y);
        ctx->lineTo(ctx, line.second.x, line.second.y);
        ctx->stroke(ctx);
    }

    virtual void drawPath(const std::vector<Point> &points, const Pen &pen) override
    {
        ctx->setLineDash(ctx, parsePenStyle(pen.lineType).data());
        ctx->setLineWidth(ctx, pen.lineWidth);
        ctx->setStrokeStyle(ctx, parseColor(pen.color).data());
        ctx->beginPath(ctx);
        ctx->moveTo(ctx, points[0].x, points[0].y);
        for (auto &point : points)
        {
            if (!point.valid())
                break;
            ctx->lineTo(ctx, point.x, point.y);
        }
        ctx->stroke(ctx);
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
