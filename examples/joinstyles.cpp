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

#include "rasterbuffer.h"
#include "solidcolorfill.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace Quill;

template <typename S>
void draw(float width, unsigned int color, S *stroker, float dx, float dy)
{
    stroker->width = width;
    stroker->raster.fill.value = color;

    stroker->moveTo(0 + dx, 0 + dy);
    stroker->lineTo(20 + dx, 0 + dy);
    stroker->moveTo(50 + dx, 0 + dy);
    stroker->lineTo(100 + dx, 0 + dy);
    stroker->lineTo(100 + dx, 100 + dy);
    stroker->lineTo(00 + dx, 70 + dy);
    stroker->lineTo(50 + dx, 50 + dy);
    stroker->finish();
}

int main(int argc, char *argv[])
{
    Stroker<MonoRaster<SolidColorFill>> stroker;

    SolidColorFill *fill = &stroker.raster.fill;

    RasterBuffer *buffer = &fill->buffer;
    buffer->allocate(500, 500);
    buffer->fill(0xff000000);

    float x = 50;
    for (int cap=0; cap<3; ++cap) {
        float y = 50;
        for (int join=0; join<3; ++join) {
            stroker.capStyle = CapStyle(cap);
            stroker.joinStyle = JoinStyle(join);
            draw(20, 0xffe0b0a0, &stroker, x, y);
            draw( 1, 0xffffffff, &stroker, x, y);
            y += 150;
        }
        x += 150;
    }

    stbi_write_png("joinstyles.png",
                   buffer->width,
                   buffer->height,
                   4,
                   buffer->data,
                   buffer->width * sizeof(unsigned int));

    std::cout << "wrote 'joinstyles.png'..." << std::endl;

    return 0;

}
