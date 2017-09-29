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
    stroker.rasterizer.fill.value = 0xffffffff;

    RasterBuffer *buffer = &stroker.rasterizer.fill.buffer;
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
    stroker.rasterizer.fill.value = 0xffffffff;

    RasterBuffer *buffer = &stroker.rasterizer.fill.buffer;
    buffer->allocate(1000, 1000);
    buffer->fill(0xff000000);

    double opsPerMSec = stroke_continuous(&stroker, buffer->width, buffer->height, segments);

    printf("LerpRaster - %6d segments, continuous path: %f ops / msec\n",
           segments,
           opsPerMSec);
}

void testMonoRaster(int segments)
{
    Stroker<MonoRasterizer<SolidColorFill>> stroker;
    stroker.rasterizer.fill.value = 0xffffffff;

    RasterBuffer *buffer = &stroker.rasterizer.fill.buffer;
    buffer->allocate(1000, 1000);
    buffer->fill(0xff000000);

    double opsPerMSec = stroke_continuous(&stroker, buffer->width, buffer->height, segments);

    printf("MonoRaster - %6d segments, continuous path: %f ops / msec\n",
           segments,
           opsPerMSec);
}

void testClippedMonoRaster(int segments)
{
    Stroker<Clipper<MonoRasterizer<SolidColorFill>>> stroker;
    stroker.rasterizer.raster.fill.value = 0xffffffff;
    stroker.rasterizer.x0 = 0;
    stroker.rasterizer.x1 = 1000;
    stroker.rasterizer.y0 = 0;
    stroker.rasterizer.y1 = 1000;

    RasterBuffer *buffer = &stroker.rasterizer.raster.fill.buffer;
    buffer->allocate(1000, 1000);
    buffer->fill(0xff000000);

    double opsPerMSec = stroke_continuous(&stroker, buffer->width, buffer->height, segments);

    printf("ClippedMono - %6d segments, continuous path: %f ops / msec\n",
           segments,
           opsPerMSec);
}

void testBarycentricRaster(int segments)
{
    Stroker<BarycentricRaster<SolidColorFill>> stroker;
    stroker.rasterizer.fill.value = 0xffffffff;
    stroker.rasterizer.x0 = 0;
    stroker.rasterizer.x1 = 1000;
    stroker.rasterizer.y0 = 0;
    stroker.rasterizer.y1 = 1000;

    RasterBuffer *buffer = &stroker.rasterizer.fill.buffer;
    buffer->allocate(1000, 1000);
    buffer->fill(0xff000000);

    double opsPerMSec = stroke_continuous(&stroker, buffer->width, buffer->height, segments);

    printf("Barycentric - %6d segments, continuous path: %f ops / msec\n",
           segments,
           opsPerMSec);
}

int main(int argc, char **argv)
{

    testBarycentricRaster(100);
    testBarycentricRaster(1000);
    testBarycentricRaster(10000);

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

