#include "quill.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "rasterbuffer.h"
#include "solidcolorfiller.h"

using namespace Quill;
using namespace std;

struct Consumer
{
    typedef Float2D VertexData;

    RasterBuffer buffer;

    void operator()(Vertex<Float2D>, int length, Float2D dx);
};


void Consumer::operator()(Vertex<Float2D> left, int length, Float2D dx)
{
    unsigned int *dst = buffer.scanline((int) left.y) + (int) left.x;

    VertexData vd = left.data;

    for (int i=0; i<length; ++i) {
        int r = int(255 * vd.x);
        int b = int(255 * vd.y);
        dst[i] += 0xff000000 | (r) | (b << 16);
        dst[i] += 0x00007f00;
        vd = vd + dx;
    }
}


int main(int argc, char **argv)
{
    LerpRaster<SolidColorFiller> raster;

    RasterBuffer *buffer = &raster.fill.buffer;
    buffer->allocate(100, 100);
    buffer->fill(0xff000000);

    raster.fill.value = 0xffff0000;

    // raster(Triangle<Vertex<Float2D>>(Vertex<Float2D>(10, 10, Float2D(1, 1)),
    //                                  Vertex<Float2D>(90, 50, Float2D(1, 0)),
    //                                  Vertex<Float2D>(40, 90, Float2D(0, 1))));

    // raster(Triangle<Vertex<Float2D>>(Vertex<Float2D>(10, 10, Float2D(1, 1)),
    //                                  Vertex<Float2D>(90, 50, Float2D(1, 0)),
    //                                  Vertex<Float2D>(95, 5, Float2D(0, 1))));

    // raster(Triangle<Vertex<Float2D>>(Vertex<Float2D>(10, 10, Float2D(1, 1)),
    //                                  Vertex<Float2D>(8, 92, Float2D(1, 0)),
    //                                  Vertex<Float2D>(40, 90, Float2D(0, 1))));

    // raster(Triangle<Vertex<Float2D>>(Vertex<Float2D>(93, 93, Float2D(1, 1)),
    //                                  Vertex<Float2D>(90, 50, Float2D(1, 0)),
    //                                  Vertex<Float2D>(40, 90, Float2D(0, 1))));

    raster(Triangle<Vertex2D>(Vertex2D(10, 10),
                              Vertex2D(90, 50),
                              Vertex2D(40, 90)));

    raster(Triangle<Vertex2D>(Vertex2D(10, 10),
                              Vertex2D(90, 50),
                              Vertex2D(95, 5)));

    raster(Triangle<Vertex2D>(Vertex2D(10, 10),
                              Vertex2D(8, 92),
                              Vertex2D(40, 90)));

    raster(Triangle<Vertex2D>(Vertex2D(93, 93),
                              Vertex2D(90, 50),
                              Vertex2D(40, 90)));

    stbi_write_png("strokelength.png",
                   buffer->width,
                   buffer->height,
                   4,
                   buffer->data,
                   buffer->width * sizeof(unsigned int));
}
