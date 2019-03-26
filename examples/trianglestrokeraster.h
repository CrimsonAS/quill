/*
    Copyright (c) 2019, reMarkable AS <technology@remarkable.no>
    Copyright (c) 2019, Gunnar Sletta <gunnar@crimson.no>
    All rights reserved.


    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

    1. Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.

    3. Neither the name of the copyright holder nor the names of its
       contributors may be used to endorse or promote products derived from
       this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
    THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
    PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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


