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

#include <cmath>

#include <chrono>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

#include "rasterbuffer.h"
#include "solidcolorfill.h"

using namespace Quill;
using namespace std;
using namespace std::chrono;

struct FbmFill
{
    typedef VaryingUV Varyings;
    RasterBuffer buffer;
    void operator()(Vertex pos, int length, VaryingUV v, VaryingUV dx);
};



void FbmFill::operator()(Vertex pos, int length, Varyings v, VaryingUV dx)
{
    unsigned int *dst = buffer.scanline((int) pos.y) + (int) pos.x;
    for (int i=0; i<length; ++i) {
        float n = stb_perlin_fbm_noise3(v.u, v.v, 0, // x/y/z
                                        2.0,         // lacunarity
                                        0.5,         // gain
                                        6,           // octaves
                                        0, 0, 0);    // x/y/z wrap
        if (n > 0) {
            int x = n * 255;
            dst[i] = 0xff000000 | x | (x << 8) | (x << 16);
        }

        v = v + dx;
    }
}



void doFbmStroke()
{
    Stroker<LerpRaster<FbmFill>, VaryingGeneratorLengthWidth> stroker;

    RasterBuffer *buffer = &stroker.rasterizer.fill.buffer;
    buffer->allocate(800, 800);
    buffer->fill(0xff000000);

    const int COUNT = 100;

    time_point<std::chrono::system_clock> timeStart = system_clock::now();

    for (int i=0; i<COUNT; ++i) {

        float t = i / float(COUNT);

        float x = 100 + t * 600;
        float y = 600 + 100 * cos(4 * t * M_PI) - t * 400;

        stroker.width = 10 + 10 * std::abs(cos(4 * t * M_PI));

        if (i == 0) {
            stroker.moveTo(x, y);
        } else {
            stroker.lineTo(x, y);
        }
    }

    time_point<std::chrono::system_clock> timeEnd = system_clock::now();
    cout << "Fancy noise fill stroke completed in: " << duration<double>(timeEnd - timeStart).count() << " seconds" << endl
         << " - length: " << stroker.length << " pixels.." << endl
         << " - triangles: " << stroker.triangleCount << endl
         << " - segments: " << COUNT << endl;


    stbi_write_png("strokelength_fbm.png",
                   buffer->width,
                   buffer->height,
                   4,
                   buffer->data,
                   buffer->width * sizeof(unsigned int));
}

struct SimpleFill
{
    typedef VaryingUV Varyings;
    RasterBuffer buffer;
    void operator()(Vertex pos, int length, VaryingUV v, VaryingUV dx) {
        unsigned int *dst = buffer.scanline((int) pos.y) + (int) pos.x;
        for (int i=0; i<length; ++i) {
            float n = stb_perlin_noise3(v.u, v.v, 0.0f, 0, 0, 0);
            if (n >= 0)
                dst[i] = 0xff000000;
            v = v + dx;
        }
    }
};

void doSimpleStroke()
{
    Stroker<LerpRaster<SimpleFill>, VaryingGeneratorLengthWidth> stroker;

    RasterBuffer *buffer = &stroker.rasterizer.fill.buffer;
    buffer->allocate(800, 800);
    buffer->fill(0xffffffff);

    const int COUNT = 100;

    time_point<std::chrono::system_clock> timeStart = system_clock::now();

    for (int i=0; i<COUNT; ++i) {

        float t = i / float(COUNT);

        float x = 100 + t * 600;
        float y = 600 + 100 * cos(4 * t * M_PI) - t * 400;

        stroker.width = 5 + 15 * std::abs(cos(4 * t * M_PI));

        if (i == 0) {
            stroker.moveTo(x, y);
        } else {
            stroker.lineTo(x, y);
        }
    }

    time_point<std::chrono::system_clock> timeEnd = system_clock::now();
    cout << "Simple stroke completed in: " << duration<double>(timeEnd - timeStart).count() << " seconds" << endl
         << " - length: " << stroker.length << " pixels.." << endl
         << " - triangles: " << stroker.triangleCount << endl
         << " - segments: " << COUNT << endl;


    stbi_write_png("strokelength_simple.png",
                   buffer->width,
                   buffer->height,
                   4,
                   buffer->data,
                   buffer->width * sizeof(unsigned int));
}

int main(int argc, char **argv)
{
    doFbmStroke();

    doSimpleStroke();
}
