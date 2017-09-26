#include "quill.h"

#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace Quill;
using namespace std;

#include "rasterbuffer.h"
#include "solidcolorfill.h"

int main(int argc, char **argv)
{
    Stroker<LerpRaster<SolidColorFill>> stroker;

    SolidColorFill *fill = &stroker.rasterizer.fill;
    fill->value = 0xffe0b0a0;

    RasterBuffer *buffer = &fill->buffer;
    buffer->allocate(400, 400);
    buffer->fill(0xff000000);

    stroker.width = 16;
    stroker.moveTo(10, 10);
    stroker.lineTo(90, 10);
    stroker.width = 8;
    stroker.lineTo(90, 90);
    stroker.lineTo(10, 90);
    stroker.close();

    stbi_write_png("hello.png",
                   buffer->width,
                   buffer->height,
                   4,
                   buffer->data,
                   buffer->width * sizeof(unsigned int));


    return 0;
}
