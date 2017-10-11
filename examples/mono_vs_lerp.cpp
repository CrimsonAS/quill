/*
    Copyright (c) 2017, Gunnar Sletta <gunnar@crimson.no>
    Copyright (c) 2017, reMarkable AS <technology@remarkable.no>
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

#include "quill.h"
#include "rasterbuffer.h"
#include "solidcolorfill.h"

#include <chrono>

static const double RUNNING_TIME = 2;
static const float PEN_WIDTH = 10;

using namespace Quill;

template <typename S>
double stroke_continuous(S *stroker, int width, int height, int segments)
{
    std::chrono::time_point<std::chrono::system_clock> timeStart = std::chrono::system_clock::now();

    int counter = 0;

    while (true) {

        float cx = width / 2;
        float cy = height / 2;

        stroker->reset();
        stroker->width = PEN_WIDTH;
        stroker->moveTo(cx, cy);

        for (int i=0; i<segments; ++i) {
            float t = (i / float(segments - 1));
            float x = sin(t * M_PI * 2 * 8) * t * cx * 0.8 + cx;
            float y = cos(t * M_PI * 2 * 8) * t * cy * 0.8 + cy;
            stroker->lineTo(x, y);
        }

        ++counter;

        if (counter % 100 == 0) {
            std::chrono::duration<double> elapsed = std::chrono::system_clock::now() - timeStart;
            if (elapsed.count() > RUNNING_TIME)
                break;
        }
    }

    std::chrono::time_point<std::chrono::system_clock> timeEnd = std::chrono::system_clock::now();

    double totalTime = std::chrono::duration<double>(timeEnd - timeStart).count();
    return counter / (totalTime * 1000);
}

struct LerpFiller
{
    typedef Quill::VaryingUV Varyings;

    RasterBuffer buffer;
    unsigned int value = 0xff000000;

    void operator()(Vertex pos, unsigned int length, VaryingUV v, VaryingUV dx);
};

inline void LerpFiller::operator()(Vertex pos, unsigned int length, VaryingUV v, VaryingUV dx)
{
    assert(pos.y >= 0);
    assert(pos.y < int(buffer.height));
    assert(pos.x >= 0);
    assert(pos.x + length < buffer.width);

    unsigned int *sline = buffer.scanline((int) pos.y) + (int) pos.x;
    for (unsigned int x=0; x<length; ++x) {
        sline[x] = value + v.u + v.v;
        v = v + dx;
    }
}



void testLerpRasterInterp(int segments)
{
    Stroker<LerpRaster<LerpFiller>, VaryingGeneratorLengthWidth> stroker;
    stroker.raster.fill.value = 0xffffffff;

    RasterBuffer *buffer = &stroker.raster.fill.buffer;
    buffer->allocate(1000, 1000);
    buffer->fill(0xff000000);

    double opsPerMSec = stroke_continuous(&stroker, buffer->width, buffer->height, segments);

    printf("LerpRaster (Float2D) - %6d segments, continuous path: %f ops / msec\n",
           segments,
           opsPerMSec);
}

void testLerpRaster(int segments)
{
    Stroker<LerpRaster<SolidColorFill>> stroker;
    stroker.raster.fill.value = 0xffffffff;

    RasterBuffer *buffer = &stroker.raster.fill.buffer;
    buffer->allocate(1000, 1000);
    buffer->fill(0xff000000);

    double opsPerMSec = stroke_continuous(&stroker, buffer->width, buffer->height, segments);

    printf("LerpRaster - %6d segments, continuous path: %f ops / msec\n",
           segments,
           opsPerMSec);
}

void testMonoRaster(int segments)
{
    Stroker<MonoRaster<SolidColorFill>> stroker;
    stroker.raster.fill.value = 0xffffffff;

    RasterBuffer *buffer = &stroker.raster.fill.buffer;
    buffer->allocate(1000, 1000);
    buffer->fill(0xff000000);

    double opsPerMSec = stroke_continuous(&stroker, buffer->width, buffer->height, segments);

    printf("MonoRaster - %6d segments, continuous path: %f ops / msec\n",
           segments,
           opsPerMSec);
}

void testClippedMonoRaster(int segments)
{
    Stroker<ClipRaster<MonoRaster<SolidColorFill>>> stroker;
    stroker.raster.raster.fill.value = 0xffffffff;
    stroker.raster.x0 = 0;
    stroker.raster.x1 = 1000;
    stroker.raster.y0 = 0;
    stroker.raster.y1 = 1000;

    RasterBuffer *buffer = &stroker.raster.raster.fill.buffer;
    buffer->allocate(1000, 1000);
    buffer->fill(0xff000000);

    double opsPerMSec = stroke_continuous(&stroker, buffer->width, buffer->height, segments);

    printf("ClippedMono - %6d segments, continuous path: %f ops / msec\n",
           segments,
           opsPerMSec);
}

int main(int argc, char **argv)
{
    testLerpRaster(100);
    testLerpRaster(1000);
    testLerpRaster(10000);

    testMonoRaster(100);
    testMonoRaster(1000);
    testMonoRaster(10000);

    testClippedMonoRaster(100);
    testClippedMonoRaster(1000);
    testClippedMonoRaster(10000);

    testLerpRasterInterp(100);
    testLerpRasterInterp(1000);
    testLerpRasterInterp(10000);

    return 0;
}

// c++ -DNDEBUG --std=c++11 -I../include -O3 mono_vs_lerp.cpp -o mono_vs_lerp && ./mono_vs_lerp

