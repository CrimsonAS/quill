/*
    Copyright (c) 2017, Gunnar Sletta <gunnar@crimson.no>
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

#pragma once

template <typename SpansConsumer>
void MonoRasterizer<SpansConsumer>::operator()(Triangle t)
{
    t.sort();

    float y0Floored = std::floor(t.y0);
    float y;

    if (t.y0 - y0Floored <= 0.5) {
        y = y0Floored + 0.5;
    } else {
        y = y0Floored + 1.5;
    }

    // std::cout << t << std::endl;
    // std::cout << " - y=" << y << ", y0Floored=" << y0Floored << std::endl;

    if (t.y0 != t.y1) {
        float dy01 = (t.y1 - t.y0);
        float dy02 = (t.y2 - t.y0);
        float dxl = (t.x1 - t.x0) / dy01;
        float dxr = (t.x2 - t.x0) / dy02;
        float yoffset = y - t.y0;
        float left = t.x0 + dxl * yoffset + 0.5;
        float right = t.x0 + dxr * yoffset + 0.5;

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
        iterate(y, t.y1, left, right, dxl, dxr);
    }

    if (t.y1 != t.y2) {
        float dy02 = (t.y2 - t.y0);
        float dy12 = (t.y2 - t.y1);
        float dxl = (t.x2 - t.x1) / dy12;
        float dxr = (t.x2 - t.x0) / dy02;
        float left = t.x2 - (t.y2 - y) * dxl + 0.5;
        float right = t.x2 - (t.y2 - y) * dxr + 0.5;
        if (left > right) {
            std::swap(left, right);
            std::swap(dxl, dxr);
        }
        iterate(y, t.y2, left, right, dxl, dxr);
     }
}

template <typename SpanConsumer>
void MonoRasterizer<SpanConsumer>::iterate(float &y, float ymax, float left, float right, float leftIncr, float rightIncr)
{
     while (y < ymax) {
         int l = (int) (left);
         int r = (int) (right);
         int len = r - l;
         assert(len >= 0);
         if (len > 0) {
             fill(Span(int(y), l, len));
         }
         y += 1.0f;
         left += leftIncr;
         right += rightIncr;
     }
}