/*
    Copyright (c) 2018, Gunnar Sletta <gunnar@crimson.no>
    Copyright (c) 2018, reMarkable AS <technology@remarkable.no>
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
    ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include "rasterbuffer.h"
#include "solidcolorfill.h"
#include "xformraster.h"

#include "quill.h"

struct SolidFillStroker
    : public Quill::Stroker<XformRaster<Quill::ClipRaster<Quill::MonoRaster<SolidColorFill>>>>
{

    // Pass in an allocated raster buffer to initialize the
    void initialize(const RasterBuffer &buffer) {
        raster.raster.raster.fill.buffer.reference(buffer);

        raster.raster.x0 = 0;
        raster.raster.y0 = 0;
        raster.raster.x1 = buffer.width - 1;
        raster.raster.y1 = buffer.height - 1;
    }

    void setFillValue(unsigned int value) {
        raster.raster.raster.fill.value = value;
    }

    void setScale(float sx, float sy) {
        raster.sx = sx;
        raster.sy = sy;
    }

    void setTranslate(float dx, float dy) {
        raster.dx = dx;
        raster.dy = dy;
    }


};
