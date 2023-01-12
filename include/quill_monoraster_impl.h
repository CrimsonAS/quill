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

#pragma once

template <typename SpansConsumer>
void MonoRaster<SpansConsumer>::operator()(Triangle t, Varyings, Varyings, Varyings)
{
    t.sort();

    float y0Floored = std::floor(t.a.y);
    float y;

    if (t.a.y - y0Floored <= 0.5f) {
        y = y0Floored + 0.5f;
    } else {
        y = y0Floored + 1.5f;
    }

    // std::cout << "rasterizing: " << t << " " << a << " " << b << " " << c << std::endl;
    // std::cout << " - y=" << y << ", y0Floored=" << y0Floored << std::endl;

    if (t.a.y != t.b.y) {

        float dy01 = (t.b.y - t.a.y);
        float dy02 = (t.c.y - t.a.y);
        float dxl = (t.b.x - t.a.x) / dy01;
        float dxr = (t.c.x - t.a.x) / dy02;
        float yoffset = y - t.a.y;
        float left = t.a.x + dxl * yoffset + 0.5f;
        float right = t.a.x + dxr * yoffset + 0.5f;

        // std::cout << " - "
        //     << "dy01=" << dy01
        //     << ", dy02=" << dy02
        //     << ", dxl=" << dxl
        //     << ", dxr=" << dxr
        //     << ", yoffset=" << yoffset
        //     << ", left=" << left
        //     << ", right=" << right
        //     << std::endl;

        if (dxr < dxl) {
            std::swap(left, right);
            std::swap(dxl, dxr);
        }
        iterate(y, t.b.y, left, right, dxl, dxr);
    }

    if (t.b.y != t.c.y) {
        float dy02 = (t.c.y - t.a.y);
        float dy12 = (t.c.y - t.b.y);
        float dxl = (t.c.x - t.b.x) / dy12;
        float dxr = (t.c.x - t.a.x) / dy02;
        float left = t.c.x - (t.c.y - y) * dxl + 0.5f;
        float right = t.c.x - (t.c.y - y) * dxr + 0.5f;
        if (left > right) {
            std::swap(left, right);
            std::swap(dxl, dxr);
        }
        iterate(y, t.c.y, left, right, dxl, dxr);
     }
}

template <typename SpanConsumer>
void MonoRaster<SpanConsumer>::iterate(float &y, float ymax, float left, float right, float leftIncr, float rightIncr)
{
     while (y < ymax) {
         int l = (int) (left);
         int r = (int) (right);
         int len = r - l;
         if (len > 0) {
             fill(l, int(y), len, Varyings(), Varyings());
         }
         y += 1.0f;
         left += leftIncr;
         right += rightIncr;
     }
}
