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

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

#include "rasterbuffer.h"
#include "solidcolorfill.h"

using namespace Quill;
using namespace std;

struct SimpleFill
{
    typedef Varying3D Varyings;

    RasterBuffer buffer;
    void operator()(int x, int y, int length, Varying3D v, Varying3D dv) {
        unsigned int *dst = buffer.scanline(y) + x;
        for (int i=0; i<length; ++i) {
            dst[i] = 0xff000000
                     | (int(v.x * 255) << 16)
                     | (int(v.y * 255) << 8)
                     | (int(v.z * 255));
            v = v + dv;
        }
    }
};


int main(int argc, char **argv)
{
    ClipRaster<LerpRaster<SimpleFill>> clipper;

    clipper.x0 = 150;
    clipper.y0 = 150;
    clipper.x1 = 350;
    clipper.y1 = 350;

    RasterBuffer *buffer = &clipper.raster.fill.buffer;
    buffer->allocate(500, 500);
    buffer->fill(0xff000000);

    Triangle t1(200, 100, 400, 200, 100, 400);

    Varying3D gray(0.2f, 0.2f, 0.2f);
    Varying3D blue(0, 0, 1);
    Varying3D red(1, 0, 0);
    Varying3D green(0, 1, 0);

    // draw the whole triangle
    clipper.raster(t1, gray, gray, gray);

    // the draw the clipped rectangle.
    clipper(t1, blue, red, green);

    stbi_write_png("rectclip.png",
                   buffer->width,
                   buffer->height,
                   4,
                   buffer->data,
                   buffer->width * sizeof(unsigned int));

    cout << " - wrote rectclip.png'" << endl;

    return 0;
}
