/**
 * Facilitates interaction with HTML5 Canvas elements in a similar
 * manner to JavaScript via the DOM, but from C to be compiled with Emscripten.
 * @file canvas.c
 * @author Alex Tyner
 */

#include "canvas.h"

static CanvasRenderingContext2D *createContext(HTMLCanvasElement *canvas, char *contextType);

/* Begin: HTMLCanvasElement static methods */
static int canvas_getWidth(HTMLCanvasElement *_this)
{
    return EM_ASM_INT({
        return document.getElementById(UTF8ToString($0)).width;
    },
                      _this->_private.id);
}
static int canvas_getHeight(HTMLCanvasElement *_this)
{
    return EM_ASM_INT({
        return document.getElementById(UTF8ToString($0)).height;
    },
                      _this->_private.id);
}
static void canvas_setWidth(HTMLCanvasElement *_this, int width)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).width = $1;
    },
           _this->_private.id, width);
}
static void canvas_setHeight(HTMLCanvasElement *_this, int height)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).height = $1;
    },
           _this->_private.id, height);
}
static CanvasRenderingContext2D *canvas_getContext(HTMLCanvasElement *_this, char *contextType)
{
    if (!_this->_private.ctx)
        _this->_private.ctx = createContext(_this, contextType);
    return _this->_private.ctx;
}
/* End: HTMLCanvasElement static methods */

HTMLCanvasElement *createCanvas(char *id)
{
    EM_ASM(
        {
            if (!document.getElementById(UTF8ToString($0)))
                document.body.appendChild(document.createElement("canvas")).setAttribute("id", UTF8ToString($0));
        },
        id);
    HTMLCanvasElement *c = (HTMLCanvasElement *)malloc(sizeof(HTMLCanvasElement));
    /* Begin: set pseudo-private fields */
    c->_private.id = (char *)malloc(strlen(id) + 1);
    strcpy(c->_private.id, id);
    c->_private.ctx = NULL; // we'll lazy-load the context when it's asked for
    /* End: set pseudo-private fields */
    c->getWidth = canvas_getWidth;
    c->getHeight = canvas_getHeight;
    c->setHeight = canvas_setHeight;
    c->setWidth = canvas_setWidth;
    c->getContext = canvas_getContext;
    return c;
}

/* Begin: CanvasRenderingContext2D static methods */
static void context2d_clearRect(CanvasRenderingContext2D *_this, double x, double y, double width, double height)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).getContext('2d').clearRect($1, $2, $3, $4);
    },
           _this->_private.canvas->_private.id, x, y, width, height);
}
static void context2d_fillRect(CanvasRenderingContext2D *_this, double x, double y, double width, double height)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).getContext('2d').fillRect($1, $2, $3, $4);
    },
           _this->_private.canvas->_private.id, x, y, width, height);
}
static void context2d_strokeRect(CanvasRenderingContext2D *_this, double x, double y, double width, double height)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).getContext('2d').strokeRect($1, $2, $3, $4);
    },
           _this->_private.canvas->_private.id, x, y, width, height);
}
static void context2d_fillText(CanvasRenderingContext2D *_this, char *text, double x, double y, double maxWidth)
{
    if (maxWidth < 0.0)
    {
        EM_ASM({
            document.getElementById(UTF8ToString($0)).getContext('2d').fillText(UTF8ToString($1), $2, $3);
        },
               _this->_private.canvas->_private.id, text, x, y);
    }
    else
    {
        EM_ASM({
            document.getElementById(UTF8ToString($0)).getContext('2d').fillText(UTF8ToString($1), $2, $3, $4);
        },
               _this->_private.canvas->_private.id, text, x, y, maxWidth);
    }
}
static void context2d_strokeText(CanvasRenderingContext2D *_this, char *text, double x, double y, double maxWidth)
{
    if (maxWidth < 0.0)
    {
        EM_ASM({
            document.getElementById(UTF8ToString($0)).getContext('2d').strokeText(UTF8ToString($1), $2, $3);
        },
               _this->_private.canvas->_private.id, text, x, y);
    }
    else
    {
        EM_ASM({
            document.getElementById(UTF8ToString($0)).getContext('2d').strokeText(UTF8ToString($1), $2, $3, $4);
        },
               _this->_private.canvas->_private.id, text, x, y, maxWidth);
    }
}
static void context2d_setLineWidth(CanvasRenderingContext2D *_this, double value)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).getContext('2d').lineWidth = ($1);
    },
           _this->_private.canvas->_private.id, value);
}
static double context2d_getLineWidth(CanvasRenderingContext2D *_this)
{
    return EM_ASM_DOUBLE({
        return document.getElementById(UTF8ToString($0)).getContext('2d').lineWidth;
    },
                         _this->_private.canvas->_private.id);
}
static void context2d_setLineCap(CanvasRenderingContext2D *_this, char *type)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).getContext('2d').lineCap = UTF8ToString($1);
    },
           _this->_private.canvas->_private.id, type);
}
static char *context2d_getLineCap(CanvasRenderingContext2D *_this)
{
    if (_this->_private.lineCap)
        free(_this->_private.lineCap);
    _this->_private.lineCap = (char *)EM_ASM_INT({
        var string = document.getElementById(UTF8ToString($0)).getContext('2d').lineCap;
        var strlen = lengthBytesUTF8(string) + 1;
        var strptr = _malloc(strlen);
        stringToUTF8(string, strptr, strlen);
        return strptr;
    },
                                                 _this->_private.canvas->_private.id);
    return _this->_private.lineCap;
}
static void context2d_setLineDash(CanvasRenderingContext2D *_this, char *pattern)
{
    EM_ASM({
        var string = UTF8ToString($1);
        var numbers = string.split(',').map(i => Number(i));
        document.getElementById(UTF8ToString($0)).getContext('2d').setLineDash(numbers);
    },
           _this->_private.canvas->_private.id, pattern);
}
static char *context2d_getLineDash(CanvasRenderingContext2D *_this)
{
    if (_this->_private.lineDash)
        free(_this->_private.lineDash);
    _this->_private.lineDash = (char *)EM_ASM_INT({
        var numbers = document.getElementById(UTF8ToString($0)).getContext('2d').getLineDash();
        var string = numbers.map(i => i.toString()).join(',');
        var strlen = lengthBytesUTF8(string) + 1;
        var strptr = _malloc(strlen);
        stringToUTF8(string, strptr, strlen);
        return strptr;
    },
                                                 _this->_private.canvas->_private.id);
    return _this->_private.lineDash;
}
static void context2d_setLineJoin(CanvasRenderingContext2D *_this, char *type)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).getContext('2d').lineJoin = UTF8ToString($1);
    },
           _this->_private.canvas->_private.id, type);
}
static char *context2d_getLineJoin(CanvasRenderingContext2D *_this)
{
    if (_this->_private.lineJoin)
        free(_this->_private.lineJoin);
    _this->_private.lineJoin = (char *)EM_ASM_INT({
        var string = document.getElementById(UTF8ToString($0)).getContext('2d').lineJoin;
        var strlen = lengthBytesUTF8(string) + 1;
        var strptr = _malloc(strlen);
        stringToUTF8(string, strptr, strlen);
        return strptr;
    },
                                                  _this->_private.canvas->_private.id);
    return _this->_private.lineJoin;
}
static char *context2d_getFont(CanvasRenderingContext2D *_this)
{
    if (_this->_private.font)
        free(_this->_private.font); // _this field could be reused, but we won't just in case it changes from the JS side
    _this->_private.font = (char *)EM_ASM_INT({
        var string = document.getElementById(UTF8ToString($0)).getContext('2d').font;
        var strlen = lengthBytesUTF8(string) + 1;
        var strptr = _malloc(strlen);
        stringToUTF8(string, strptr, strlen);
        return strptr;
    },
                                              _this->_private.canvas->_private.id);
    return _this->_private.font;
}
static void context2d_setFont(CanvasRenderingContext2D *_this, char *value)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).getContext('2d').font = UTF8ToString($1);
    },
           _this->_private.canvas->_private.id, value);
}
static char *context2d_getTextAlign(CanvasRenderingContext2D *_this)
{
    if (_this->_private.textAlign)
        free(_this->_private.textAlign);
    _this->_private.textAlign = (char *)EM_ASM_INT({
        var string = document.getElementById(UTF8ToString($0)).getContext('2d').textAlign;
        var strlen = lengthBytesUTF8(string) + 1;
        var strptr = _malloc(strlen);
        stringToUTF8(string, strptr, strlen);
        return strptr;
    },
                                                   _this->_private.canvas->_private.id);
    return _this->_private.textAlign;
}
static void context2d_setTextAlign(CanvasRenderingContext2D *_this, char *value)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).getContext('2d').textAlign = UTF8ToString($1);
    },
           _this->_private.canvas->_private.id, value);
}
static char *context2d_getTextBaseline(CanvasRenderingContext2D *_this)
{
    if (_this->_private.textBaseline)
        free(_this->_private.textBaseline);
    _this->_private.textBaseline = (char *)EM_ASM_INT({
        var string = document.getElementById(UTF8ToString($0)).getContext('2d').textBaseline;
        var strlen = lengthBytesUTF8(string) + 1;
        var strptr = _malloc(strlen);
        stringToUTF8(string, strptr, strlen);
        return strptr;
    },
                                                   _this->_private.canvas->_private.id);
    return _this->_private.textBaseline;
}
static void context2d_setTextBaseline(CanvasRenderingContext2D *_this, char *value)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).getContext('2d').textBaseline = UTF8ToString($1);
    },
           _this->_private.canvas->_private.id, value);
}
static char *context2d_getFillStyle(CanvasRenderingContext2D *_this)
{
    if (_this->_private.fillStyle)
        free(_this->_private.fillStyle);
    _this->_private.fillStyle = (char *)EM_ASM_INT({
        var string = document.getElementById(UTF8ToString($0)).getContext('2d').fillStyle;
        var strlen = lengthBytesUTF8(string) + 1;
        var strptr = _malloc(strlen);
        stringToUTF8(string, strptr, strlen);
        return strptr;
    },
                                                   _this->_private.canvas->_private.id);
    return _this->_private.fillStyle;
}
static void context2d_setFillStyle(CanvasRenderingContext2D *_this, char *value)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).getContext('2d').fillStyle = UTF8ToString($1);
    },
           _this->_private.canvas->_private.id, value);
}
static char *context2d_getStrokeStyle(CanvasRenderingContext2D *_this)
{
    if (_this->_private.strokeStyle)
        free(_this->_private.strokeStyle);
    _this->_private.strokeStyle = (char *)EM_ASM_INT({
        var string = document.getElementById(UTF8ToString($0)).getContext('2d').strokeStyle;
        var strlen = lengthBytesUTF8(string) + 1;
        var strptr = _malloc(strlen);
        stringToUTF8(string, strptr, strlen);
        return strptr;
    },
                                                     _this->_private.canvas->_private.id);
    return _this->_private.strokeStyle;
}
static void context2d_setStrokeStyle(CanvasRenderingContext2D *_this, char *value)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).getContext('2d').strokeStyle = UTF8ToString($1);
    },
           _this->_private.canvas->_private.id, value);
}
static void context2d_beginPath(CanvasRenderingContext2D *_this)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).getContext('2d').beginPath();
    },
           _this->_private.canvas->_private.id);
}
static void context2d_closePath(CanvasRenderingContext2D *_this)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).getContext('2d').closePath();
    },
           _this->_private.canvas->_private.id);
}
static void context2d_moveTo(CanvasRenderingContext2D *_this, double x, double y)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).getContext('2d').moveTo($1, $2);
    },
           _this->_private.canvas->_private.id, x, y);
}
static void context2d_lineTo(CanvasRenderingContext2D *_this, double x, double y)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).getContext('2d').lineTo($1, $2);
    },
           _this->_private.canvas->_private.id, x, y);
}
static void context2d_bezierCurveTo(CanvasRenderingContext2D *_this, double cp1x, double cp1y, double cp2x, double cp2y, double x, double y)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).getContext('2d').bezierCurveTo($1, $2, $3, $4, $5, $6);
    },
           _this->_private.canvas->_private.id, cp1x, cp1y, cp2x, cp2y, x, y);
}
static void context2d_quadraticCurveTo(CanvasRenderingContext2D *_this, double cpx, double cpy, double x, double y)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).getContext('2d').quadraticCurveTo($1, $2, $3, $4);
    },
           _this->_private.canvas->_private.id, cpx, cpy, x, y);
}
static void context2d_arc(CanvasRenderingContext2D *_this, double x, double y, double radius, double startAngle, double endAngle)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).getContext('2d').arc($1, $2, $3, $4, $5);
    },
           _this->_private.canvas->_private.id, x, y, radius, startAngle, endAngle);
}
static void context2d_arcTo(CanvasRenderingContext2D *_this, double x1, double y1, double x2, double y2, double radius)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).getContext('2d').arcTo($1, $2, $3, $4, $5);
    },
           _this->_private.canvas->_private.id, x1, y1, x2, y2, radius);
}
static void context2d_ellipse(CanvasRenderingContext2D *_this, double x, double y, double radiusX, double radiusY, double rotation, double startAngle, double endAngle)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).getContext('2d').ellipse($1, $2, $3, $4, $5, $6, $7);
    },
           _this->_private.canvas->_private.id, x, y, radiusX, radiusY, rotation, startAngle, endAngle);
}
static void context2d_rect(CanvasRenderingContext2D *_this, double x, double y, double width, double height)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).getContext('2d').rect($1, $2, $3, $4);
    },
           _this->_private.canvas->_private.id, x, y, width, height);
}
static void context2d_fill(CanvasRenderingContext2D *_this)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).getContext('2d').fill();
    },
           _this->_private.canvas->_private.id);
}
static void context2d_stroke(CanvasRenderingContext2D *_this)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).getContext('2d').stroke();
    },
           _this->_private.canvas->_private.id);
}
static void context2d_clip(CanvasRenderingContext2D *_this)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).getContext('2d').clip();
    },
           _this->_private.canvas->_private.id);
}
static int context2d_isPointInPath(CanvasRenderingContext2D *_this, double x, double y)
{
    return EM_ASM_INT({
        return document.getElementById(UTF8ToString($0)).getContext('2d').isPointInPath($1, $2);
    },
                      _this->_private.canvas->_private.id, x, y);
}
static int context2d_isPointInStroke(CanvasRenderingContext2D *_this, double x, double y)
{
    return EM_ASM_INT({
        return document.getElementById(UTF8ToString($0)).getContext('2d').isPointInStroke($1, $2);
    },
                      _this->_private.canvas->_private.id, x, y);
}
static void context2d_rotate(CanvasRenderingContext2D *_this, double angle)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).getContext('2d').rotate($1);
    },
           _this->_private.canvas->_private.id, angle);
}
static void context2d_scale(CanvasRenderingContext2D *_this, double x, double y)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).getContext('2d').scale($1, $2);
    },
           _this->_private.canvas->_private.id, x, y);
}
static void context2d_translate(CanvasRenderingContext2D *_this, double x, double y)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).getContext('2d').translate($1, $2);
    },
           _this->_private.canvas->_private.id, x, y);
}
static void context2d_transform(CanvasRenderingContext2D *_this, double a, double b, double c, double d, double e, double f)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).getContext('2d').transform($1, $2, $3, $4, $5, $6);
    },
           _this->_private.canvas->_private.id, a, b, c, d, e, f);
}
static void context2d_setTransform(CanvasRenderingContext2D *_this, double a, double b, double c, double d, double e, double f)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).getContext('2d').setTransform($1, $2, $3, $4, $5, $6);
    },
           _this->_private.canvas->_private.id, a, b, c, d, e, f);
}
static void context2d_resetTransform(CanvasRenderingContext2D *_this)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).getContext('2d').resetTransform();
    },
           _this->_private.canvas->_private.id);
}
static void context2d_setGlobalAlpha(CanvasRenderingContext2D *_this, double value)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).getContext('2d').globalAlpha = $1;
    },
           _this->_private.canvas->_private.id, value);
}
static double context2d_getGlobalAlpha(CanvasRenderingContext2D *_this)
{
    return EM_ASM_DOUBLE({
        return document.getElementById(UTF8ToString($0)).getContext('2d').globalAlpha;
    },
                         _this->_private.canvas->_private.id);
}
static void context2d_setGlobalCompositeOperation(CanvasRenderingContext2D *_this, char *value)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).getContext('2d').globalCompositeOperation = $1;
    },
           _this->_private.canvas->_private.id, value);
}
static char *context2d_getGlobalCompositeOperation(CanvasRenderingContext2D *_this)
{
    if (_this->_private.globalCompositeOperation)
        free(_this->_private.globalCompositeOperation);
    _this->_private.globalCompositeOperation = (char *)EM_ASM_INT({
        var string = document.getElementById(UTF8ToString($0)).getContext('2d').globalCompositeOperation;
        var strlen = lengthBytesUTF8(string) + 1;
        var strptr = _malloc(strlen);
        stringToUTF8(string, strptr, strlen);
        return strptr;
    },
                                                                  _this->_private.canvas->_private.id);
    return _this->_private.globalCompositeOperation;
}
static void context2d_save(CanvasRenderingContext2D *_this)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).getContext('2d').save();
    },
           _this->_private.canvas->_private.id);
}
static void context2d_restore(CanvasRenderingContext2D *_this)
{
    EM_ASM({
        document.getElementById(UTF8ToString($0)).getContext('2d').restore();
    },
           _this->_private.canvas->_private.id);
}
static HTMLCanvasElement *context2d_getCanvas(CanvasRenderingContext2D *_this)
{
    return _this->_private.canvas;
}
/* End: CanvasRenderingContext2D static methods */

static CanvasRenderingContext2D *createContext(HTMLCanvasElement *canvas, char *contextType)
{
    if (strcmp(contextType, "2d") != 0)
        return NULL;
    CanvasRenderingContext2D *ctx = (CanvasRenderingContext2D *)malloc(sizeof(CanvasRenderingContext2D));
    /* Begin: set pseudo-private fields */
    ctx->_private.canvas = canvas;
    strcpy(ctx->_private.contextType, contextType); // string field is a static length, no need to allocate
    ctx->_private.font = NULL;
    ctx->_private.textAlign = NULL;
    ctx->_private.textBaseline = NULL;
    ctx->_private.fillStyle = NULL;
    ctx->_private.strokeStyle = NULL;
    ctx->_private.lineCap = NULL;
    ctx->_private.lineDash = NULL;
    ctx->_private.lineJoin = NULL;
    ctx->_private.globalCompositeOperation = NULL;
    /* End: set pseudo-private fields */
    ctx->clearRect = context2d_clearRect;
    ctx->fillRect = context2d_fillRect;
    ctx->strokeRect = context2d_strokeRect;
    ctx->fillText = context2d_fillText;
    ctx->strokeText = context2d_strokeText;
    ctx->setLineWidth = context2d_setLineWidth;
    ctx->getLineWidth = context2d_getLineWidth;
    ctx->setLineCap = context2d_setLineCap;
    ctx->getLineCap = context2d_getLineCap;
    ctx->setLineDash = context2d_setLineDash;
    ctx->getLineDash = context2d_getLineDash;
    ctx->setLineJoin = context2d_setLineJoin;
    ctx->getLineJoin = context2d_getLineJoin;
    ctx->setFont = context2d_setFont;
    ctx->getFont = context2d_getFont;
    ctx->setTextAlign = context2d_setTextAlign;
    ctx->getTextAlign = context2d_getTextAlign;
    ctx->setTextBaseline = context2d_setTextBaseline;
    ctx->getTextBaseline = context2d_getTextBaseline;
    ctx->setFillStyle = context2d_setFillStyle;
    ctx->getFillStyle = context2d_getFillStyle;
    ctx->setStrokeStyle = context2d_setStrokeStyle;
    ctx->getStrokeStyle = context2d_getStrokeStyle;
    ctx->beginPath = context2d_beginPath;
    ctx->closePath = context2d_closePath;
    ctx->moveTo = context2d_moveTo;
    ctx->lineTo = context2d_lineTo;
    ctx->bezierCurveTo = context2d_bezierCurveTo;
    ctx->quadraticCurveTo = context2d_quadraticCurveTo;
    ctx->arc = context2d_arc;
    ctx->arcTo = context2d_arcTo;
    ctx->ellipse = context2d_ellipse;
    ctx->rect = context2d_rect;
    ctx->fill = context2d_fill;
    ctx->stroke = context2d_stroke;
    ctx->clip = context2d_clip;
    ctx->isPointInPath = context2d_isPointInPath;
    ctx->isPointInStroke = context2d_isPointInStroke;
    ctx->rotate = context2d_rotate;
    ctx->scale = context2d_scale;
    ctx->translate = context2d_translate;
    ctx->transform = context2d_transform;
    ctx->setTransform = context2d_setTransform;
    ctx->resetTransform = context2d_resetTransform;
    ctx->setGlobalAlpha = context2d_setGlobalAlpha;
    ctx->getGlobalAlpha = context2d_getGlobalAlpha;
    ctx->setGlobalCompositeOperation = context2d_setGlobalCompositeOperation;
    ctx->getGlobalCompositeOperation = context2d_getGlobalCompositeOperation;
    ctx->save = context2d_save;
    ctx->restore = context2d_restore;
    ctx->getCanvas = context2d_getCanvas;
    return ctx;
}

void freeCanvas(HTMLCanvasElement *canvas)
{
    if (canvas)
    {
        free(canvas->_private.id);
        if (canvas->_private.ctx)
        {
            if (canvas->_private.ctx->_private.font)
                free(canvas->_private.ctx->_private.font);
            if (canvas->_private.ctx->_private.textAlign)
                free(canvas->_private.ctx->_private.textAlign);
            if (canvas->_private.ctx->_private.textBaseline)
                free(canvas->_private.ctx->_private.textBaseline);
            if (canvas->_private.ctx->_private.fillStyle)
                free(canvas->_private.ctx->_private.fillStyle);
            if (canvas->_private.ctx->_private.strokeStyle)
                free(canvas->_private.ctx->_private.strokeStyle);
            if (canvas->_private.ctx->_private.lineCap)
                free(canvas->_private.ctx->_private.lineCap);
            if (canvas->_private.ctx->_private.lineDash)
                free(canvas->_private.ctx->_private.lineDash);
            if (canvas->_private.ctx->_private.lineJoin)
                free(canvas->_private.ctx->_private.lineJoin);
            if (canvas->_private.ctx->_private.globalCompositeOperation)
                free(canvas->_private.ctx->_private.globalCompositeOperation);
            free(canvas->_private.ctx);
        }
        free(canvas);
    }
}