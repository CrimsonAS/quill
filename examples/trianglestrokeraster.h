#pragma once

#include "rasterbuffer.h"
#include "solidcolorfill.h"

struct TriangleStrokeRaster
{
    Quill::Stroker<Quill::ClipRaster<Quill::MonoRaster<SolidColorFill>>> stroker;

    typedef Quill::VaryingNoop Varyings;

    TriangleStrokeRaster()
    {
        stroker.raster.raster.fill.value = 0xffffa0a0;
        stroker.width = 1;
    }

    void allocate(int width, int height, unsigned int fill)
    {
        stroker.raster.raster.fill.buffer.allocate(width, height);
        stroker.raster.raster.fill.buffer.fill(fill);
        stroker.raster.x0 = 0;
        stroker.raster.x1 = width - 1;
        stroker.raster.y0 = 0;
        stroker.raster.y1 = height - 1;
    }

    void operator()(Triangle t, Varyings, Varyings, Varyings)
    {
        stroker.moveTo(t.a.x * sx + dx, t.a.y * sy + dy);
        stroker.lineTo(t.b.x * sx + dx, t.b.y * sy + dy);
        stroker.lineTo(t.c.x * sx + dx, t.c.y * sy + dy);
        stroker.close();
    }

    unsigned int *buffer() { return stroker.raster.raster.fill.buffer.data; }
    unsigned int width() { return stroker.raster.raster.fill.buffer.width; }
    unsigned int height() { return stroker.raster.raster.fill.buffer.height; }


    float dx = 0.0f;
    float dy = 0.0f;
    float sx = 1.0f;
    float sy = 1.0f;
};


