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

#include "quill.h"

#include <cmath>

#include <chrono>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

#include "rasterbuffer.h"
#include "solidcolorfill.h"

using namespace std;
using namespace std::chrono;
using namespace Quill;

struct StrokedTriangleFiller : public MonoRaster<SolidColorFill>
{
    Stroker<MonoRaster<SolidColorFill>> stroker;

    StrokedTriangleFiller()
    {
        fill.buffer.allocate(800, 800);
        fill.buffer.fill(0xff000000);
        fill.value = 0xff808080;

        stroker.raster.fill.buffer.reference(fill.buffer);
        stroker.raster.fill.value = 0xffffa0a0;
        stroker.width = 1;
    }

    void operator()(Triangle t, Varyings a, Varyings b, Varyings c)
    {
        // MonoRaster<SolidColorFill>::operator()(t, a, b, c);

        stroker.moveTo(t.a.x, t.a.y);
        stroker.lineTo(t.b.x, t.b.y);
        stroker.lineTo(t.c.x, t.c.y);
        stroker.close();
    }
};

int main(int argc, char **argv)
{
    Stroker<StrokedTriangleFiller> stroker;
    stroker.capStyle = RoundCap;
    stroker.joinStyle = RoundJoin;

    RasterBuffer *buffer = &stroker.raster.fill.buffer;



    const int COUNT = 10;

    time_point<std::chrono::system_clock> timeStart = system_clock::now();

    for (int i=0; i<COUNT; ++i) {

        float t = i / float(COUNT);

        float x = 100 + t * 600;
        float y = 600 + 100 * cos(4 * t * M_PI) - t * 400;

        stroker.width = 20 + 30 * std::abs(cos(4 * t * M_PI));

        if (i == 0) {
            stroker.moveTo(x, y);
        } else {
            stroker.lineTo(x, y);
        }
    }

    stroker.finish();

    time_point<std::chrono::system_clock> timeEnd = system_clock::now();
    cout << "Simple stroke completed in: " << duration<double>(timeEnd - timeStart).count() << " seconds" << endl
         << " - length: " << stroker.length << " pixels.." << endl
         << " - triangles: " << stroker.triangleCount << endl
         << " - segments: " << COUNT << endl;


    stbi_write_png("triangles_in_line.png",
                   buffer->width,
                   buffer->height,
                   4,
                   buffer->data,
                   buffer->width * sizeof(unsigned int));

    cout << " - wrote 'triangles_in_line.png'" << endl;

    return 0;
}

