#include "quill.h"

#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


using namespace Quill;
using namespace std;

struct RasterBuffer
{

    int width = 0;
    int height = 0;

    unsigned int *data = nullptr;

    ~RasterBuffer();

    void allocate(int w, int h);
    void release();

    unsigned int *scanline(int y);
    void fill(unsigned int value);
};

RasterBuffer::~RasterBuffer()
{
    release();
}

void RasterBuffer::allocate(int width, int height)
{
    assert(!data);

    this->width = width;
    this->height = height;
    data = new unsigned int[width * height];
}

void RasterBuffer::release()
{
    delete [] data;
}

unsigned int *RasterBuffer::scanline(int y)
{
    assert(y >= 0);
    assert(y < height);
    assert(data);

    return data + y * width;
}

void RasterBuffer::fill(unsigned int value)
{
    for (int y=0; y<height; ++y) {
        unsigned int *line = scanline(y);
        for (int x=0; x<width; ++x) {
            line[x] = value;
        }
    }
}

struct AccumulateInRaster
{
    RasterBuffer buffer;
    unsigned int value;

    void operator()(Span span);
};


void AccumulateInRaster::operator()(Span span)
{
    if (span.y < 0 || span.y >= buffer.height) {
        return;
    }

    if (span.x < 0) {
        span.length += span.x;
        span.x = 0;
    }

    if (span.x + span.length >= buffer.width) {
        span.length = buffer.width - span.x;
    }

    if (span.length > 0) {
        unsigned int *sline = buffer.scanline(span.y) + span.x;
        for (int x=0; x<span.length; ++x) {
            sline[x] += value;
        }
    }
}



int main(int argc, char **argv)
{
    Stroker<MonoRasterizer<AccumulateInRaster>> stroker;

    AccumulateInRaster *accumulator = &stroker.consumer.consumer;
    accumulator->value = 0x1f3f7f;

    RasterBuffer *buffer = &accumulator->buffer;
    buffer->allocate(100, 100);
    buffer->fill(0xff000000);

    stroker.width = 10;
    stroker.moveTo(10, 10);
    stroker.lineTo(90, 10);
    stroker.lineTo(50, 50);

    stroker.width = 1;
    stroker.lineTo(90, 90.5);
    stroker.lineTo(50, 90.5);

    stroker.width = 10;
    stroker.lineTo(10, 90);

    stroker.close();

    stbi_write_png("output.png",
                   buffer->width,
                   buffer->height,
                   4,
                   buffer->data,
                   buffer->width * sizeof(unsigned int));


    return 0;
}